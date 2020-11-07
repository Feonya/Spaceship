#ifndef _ENTITY_H_
#define _ENTITY_H_

class Entity {
    public:
	Entity();
	Entity(int f_x, int f_y, int f_w, int f_h, char f_ch, int f_lf);

	void SetX(int f_x);
	int  GetX() const;
	void SetY(int f_y);
	int  GetY() const;

	void SetW(int f_w);
	int  GetW() const;
	void SetH(int f_h);
	int  GetH() const;

	char GetChar() const;

	void SetLife(int f_lf);
	int  GetLife() const;

    private:
	int  m_x, m_y, m_w, m_h;  // Position and size
	char m_char;  // What character represents the entity
	int  m_life;  // Entity's HP
};

#endif
