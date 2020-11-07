#include "Entity.h"

Entity::Entity() {}
Entity::Entity(int f_x, int f_y, int f_w, int f_h, char f_ch, int f_lf) {
    m_x = f_x; m_y = f_y; m_w = f_w; m_h = f_h; m_char = f_ch; m_life = f_lf;
}

void Entity::SetX(int f_x) { m_x = f_x; }
int  Entity::GetX() const  { return m_x; }

void Entity::SetY(int f_y) { m_y = f_y; }
int  Entity::GetY() const  { return m_y; }

void Entity::SetW(int f_w) { m_w = f_w; }
int  Entity::GetW() const  { return m_w; }

void Entity::SetH(int f_h) { m_h = f_h; }
int  Entity::GetH() const  { return m_h; }

char Entity::GetChar() const { return m_char; }

void Entity::SetLife(int f_lf) { m_life = f_lf; }
int  Entity::GetLife() const   { return m_life; }
