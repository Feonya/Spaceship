#ifndef _GAME_H_
#define _GAME_H_

#include <vector>
#include <ncurses.h>
#include "Entity.h"

class Game {
    public:
	Game();

	static Game* GetInstance();

	bool IsRunning() const;
	void TickTock();

	void Input();
	void Update();
	void Render();

	void Quit();

    private:
	static Game* m_instance;  // The singleton

	unsigned m_ticks;  // Frame number from game started

	WINDOW* m_window;    // The main ncurses window
	bool    m_running;   // Game is running or not
	std::vector<Entity> m_rocks;  // Store entities as rocks
	Entity m_spaceship;  // The spaceship entity
};

#endif
