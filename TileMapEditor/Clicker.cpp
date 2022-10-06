#include "pch.h"
#include "Clicker.h"

void Clicker::UpdatePostition()
{
	(*this->collider).x = (float)GetMouseX();
	(*this->collider).y = (float)GetMouseY();
}

