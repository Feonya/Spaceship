#include <clocale>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "constants.h"
#include "Game.h"

Game* Game::m_instance = nullptr;

Game::Game() {
    std::setlocale(LC_ALL, "");  // Set locale info to be used, the empty string  argument means use default locale
    std::srand(std::time(nullptr));  // Initialize pseudo random number generator with the argument pass as seed

    /* Setup ncurses library */

    initscr();  // initsrc function is normally the first call when initializing a ncurses program
    cbreak();   // Don't use line buffering and making user typed characters immediately available
    noecho();   // Don't echo user typed characters on cursor position
    start_color();  // Use color for our terminal, make sure your terminal has color support
    curs_set(0);    // Make curso invisible
    // Create a ncurses window
    m_window = newwin(WINDOW_HEIGHT + WINDOW_POS_Y,  // End row
		      WINDOW_WIDTH  + WINDOW_POS_X,  // End column
		      WINDOW_POS_Y,   // Begin x
		      WINDOW_POS_X);  // Begin y
    nodelay(m_window, true);  // This causes functions like getch() or wgetch() to be a non-blocking call
    keypad(m_window,  true);  // Enable the keypad
    /* Assign some colors in pair to their index number */
    init_pair(0, COLOR_WHITE,  COLOR_BLACK);
    init_pair(1, COLOR_GREEN,  COLOR_BLACK);
    init_pair(2, COLOR_BLUE,   COLOR_BLACK);
    init_pair(3, COLOR_RED,    COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);

    /* Setup game class members */

    m_ticks = 0;  // Ticks start at 0
    // Create the player with specifed arguments 
    m_spaceship = Entity(PLAYGROUND_WIDTH  / 2 + PLAYGROUND_POS_X,  // Centered vertical
			 PLAYGROUND_HEIGHT / 2 + PLAYGROUND_POS_Y,  // Centered horizontal
			 3, 1, 'H', 10);  // The spaceship is a 3x1 size 10 life entity with the body represented by 'H'
    m_rocks   = {};
    m_running = true;
}

Game* Game::GetInstance() {
    if (m_instance == nullptr) m_instance = new Game();
    return m_instance;
}

bool Game::IsRunning() const { return m_running; }

void Game::TickTock() { m_ticks++; }

void Game::Input() {
    // Get a user input character, wgetch() will not block the program, cause we are in the nodelay mode
    int ch = tolower(wgetch(m_window));
    switch (ch) {
	case 'q':
	    m_running = false;
	    break;

	case KEY_UP:
	case 'w':
	    if (m_spaceship.GetY() > PLAYGROUND_POS_Y + 1)
		m_spaceship.SetY(m_spaceship.GetY() - 1);
	    break;

	case KEY_DOWN:
	case 's':
	    if (m_spaceship.GetY() < PLAYGROUND_HEIGHT - 1)
		m_spaceship.SetY(m_spaceship.GetY() + 1);
	    break;

	case KEY_LEFT:
	case 'a':
	    if (m_spaceship.GetX() > PLAYGROUND_POS_X + 1)
		m_spaceship.SetX(m_spaceship.GetX() - 1);
	    break;

	case KEY_RIGHT:
	case 'd':
	    if (m_spaceship.GetX() < PLAYGROUND_WIDTH - 2)
		m_spaceship.SetX(m_spaceship.GetX() + 1);
	    break;

	default: break;
    }
}

void Game::Update() {
    // Random generate three kinds of rocks on random positions
    int randNum = std::rand();
    Entity rock = Entity(randNum % PLAYGROUND_WIDTH + PLAYGROUND_POS_X,  // Random position x
			 PLAYGROUND_POS_Y,  // Fixed position y
			 1, 1,  // Width and height
			 randNum % 15 == 0 ? 'O' : randNum % 15 < 6 ? '*' : '.',
			 1);    // Life

    // Update rocks data
    for (std::size_t i = 0; i < m_rocks.size(); ++i) {
	// Remove rocks from container if anyone of them are moved to the bottom of playground
	if (m_rocks[i].GetY() >= PLAYGROUND_HEIGHT + PLAYGROUND_POS_Y - 1)
	    m_rocks.erase(m_rocks.begin() + i);

	/* Move 3 kinds of rocks with different speed */

	// Tiny rocks
	if (m_rocks[i].GetChar() == '.') {
	    m_rocks[i].SetY(m_rocks[i].GetY() + 1);
	// Normal rocks
	} else if (m_rocks[i].GetChar() == '*' && m_ticks % 2 == 0) {
	    m_rocks[i].SetY(m_rocks[i].GetY() + 1);
	    // Check normal rocks collision
	    if (m_rocks[i].GetX() >= m_spaceship.GetX() - 1 && m_rocks[i].GetX() <= m_spaceship.GetX() + 1 &&
		m_rocks[i].GetY() >= m_spaceship.GetY()     && m_rocks[i].GetY() <= m_spaceship.GetY() + m_spaceship.GetH() - 1) {
		m_spaceship.SetLife(m_spaceship.GetLife() - 1);  // Spaceship life minus 1
		m_rocks.erase(m_rocks.begin() + i);  // Remove this rock from container
		if (m_spaceship.GetLife() <= 0) m_running = false;
	    }
	// Huge rocks
	} else if (m_rocks[i].GetChar() == 'O' && m_ticks % 3 == 0) {
	    m_rocks[i].SetY(m_rocks[i].GetY() + 1);
	    // Check huge rocks collision
	    if (m_rocks[i].GetX() >= m_spaceship.GetX() - 1 && m_rocks[i].GetX() <= m_spaceship.GetX() + 1 &&
		m_rocks[i].GetY() >= m_spaceship.GetY()     && m_rocks[i].GetY() <= m_spaceship.GetY() + m_spaceship.GetH() - 1) {
		m_running = false;
	    }
	}
    }

    m_rocks.emplace_back(rock);  // Add the new rock into rocks container
}

void Game::Render() {
    werase(m_window);  // Clear screen immediately

    /* Render rocks */

    // Render tiny rocks
    for (Entity& r: m_rocks) {
	if (r.GetChar() == '.') mvwaddch(m_window, r.GetY(), r.GetX(), r.GetChar());
    }
    // Render normal rocks
    for (Entity& r: m_rocks) {
	if (r.GetChar() == '*') {
	    wattron(m_window, A_BOLD);
	    mvwaddch(m_window, r.GetY(), r.GetX(), r.GetChar());
	    wattroff(m_window, A_BOLD);
	}
    }
    // Render huge rocks
    for (Entity& r: m_rocks) {
	if (r.GetChar() == 'O') {
	    wattron(m_window, A_BOLD);
	    wattron(m_window, COLOR_PAIR(4));  // Yellow
	    mvwaddch(m_window, r.GetY(), r.GetX(), r.GetChar());
	    wattroff(m_window, COLOR_PAIR(4));
	    wattroff(m_window, A_BOLD);
	}
    }

    /* Render spaceship */

    /* Render spaceship body */
    wattron(m_window, A_BOLD);
    wattron(m_window, COLOR_PAIR(1));  // Green
    mvwaddch(m_window, m_spaceship.GetY(), m_spaceship.GetX(), m_spaceship.GetChar());
    wattroff(m_window, COLOR_PAIR(1));
    wattroff(m_window, A_BOLD);
    /* Render spaceship engine flame */
    if (m_ticks % 10 > 0) {
	wattron(m_window, A_BOLD);
	wattron(m_window, COLOR_PAIR(m_ticks % 2 ? 3 : 4));  // Red or yellow
	mvwaddch(m_window, m_spaceship.GetY() + 1, m_spaceship.GetX(), 'W');
	wattroff(m_window, COLOR_PAIR(m_ticks % 2 ? 3 : 4));
	wattroff(m_window, A_BOLD);
    }


    /* Render spaceship wings and head */
    wattron(m_window, COLOR_PAIR(2));  // Blue
    mvwaddch(m_window, m_spaceship.GetY(), m_spaceship.GetX() - 1, '[');
    mvwaddch(m_window, m_spaceship.GetY(), m_spaceship.GetX() + 1, ']');
    mvwaddch(m_window, m_spaceship.GetY(), m_spaceship.GetX() - 2, '<');
    mvwaddch(m_window, m_spaceship.GetY(), m_spaceship.GetX() + 2, '>');
    mvwaddch(m_window, m_spaceship.GetY() - 1, m_spaceship.GetX(), 'A');
    wattroff(m_window, COLOR_PAIR(2));

    /* Render UI */
    wattron(m_window, A_BOLD);
    box(m_window, 0, 0);  // Draw a box around the edge of the window
    wattron(m_window, COLOR_PAIR(2));  // Blue
    mvwaddstr(m_window, BORDER_POS_Y, 20, "[ A Space game developed by 2D_Cat ]");  // Draw title
    wattroff(m_window, COLOR_PAIR(2));
    wattron(m_window, COLOR_PAIR(m_spaceship.GetLife() < 3 ? 3 : m_spaceship.GetLife() < 6 ? 4 : 1));  // Green->Yellow->Red
    mvwprintw(m_window, BORDER_HEIGHT - BORDER_POS_Y, 60, "[ LIFE: %d ]", m_spaceship.GetLife());      // Draw life
    wattroff(m_window, COLOR_PAIR(m_spaceship.GetLife() < 3 ? 3 : m_spaceship.GetLife() < 6 ? 4 : 1));
    wattroff(m_window, A_BOLD);

    wrefresh(m_window);  // Get actual output to the terminal
}

void Game::Quit() {
    delete m_instance;

    delwin(m_window);
    endwin();

    std::cout << "\n";
    std::cout << "****************************\n";
    std::cout << "          Game Over         \n";
    std::cout << "****************************\n";
    std::cout << "\n";
}
