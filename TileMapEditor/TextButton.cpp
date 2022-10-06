#include "pch.h"
#include "TextButton.h"

void TextButton::Draw(const Vector2& mouse, const bool& RecBG, const bool& entireWidth) const
{
	if (CheckCollisionPointRec(mouse, *collider))
	{
		Rectangle source = { -100, collider->y - collider->height * 0.25f, screenWidth * 1.5f, (collider->height * 1.5f) };
		if (!entireWidth)
			source.width = collider->width;

		if (RecBG) 
			DrawRectanglePro(source, { 0,0 }, *rotation, WHITE);

		DrawRectanglePro(source, { 0,0 }, *rotation, { 75,75,75,200 });
		if (secondaryText) DrawTextPro(fonts[*fontKey] , (*secondaryText).c_str(), {50, collider->y + 10}, {0,0}, *rotation, (*fontSize * 0.5f), 2, *color);
	}
	DrawTextPro(fonts[*fontKey], (*text).c_str(), { collider->x,collider->y }, { 0,0 }, *rotation, *fontSize, 2, *color);
}
