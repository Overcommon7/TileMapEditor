#include "pch.h"
#include "Player.h"

Player::Player(const Player& p)
{
	collider = p.collider;
	direction = p.direction;
	JumpTimer = p.JumpTimer;
	onPlatform = p.onPlatform;
	keyDown = p.keyDown;
	verticalVel = p.verticalVel;
	horizantalVel = p.horizantalVel;
}

Player& Player::operator=(const Player& p)
{
	collider = p.collider;
	direction = p.direction;
	JumpTimer = p.JumpTimer;
	onPlatform = p.onPlatform;
	keyDown = p.keyDown;
	verticalVel = p.verticalVel;
	horizantalVel = p.horizantalVel;

	return *this;
}

void Player::Draw() const
{
	float flip = 1.f;
	if (Direction::Left == direction) flip = -1.f;
	DrawTexturePro(gameTextures[KP_PLAYER], { 0.f, 0.f, collider.width * flip, collider.height }, collider, { 0, 0 }, 0, WHITE);
		
}

void Player::MoveLeft()
{
	float speed = -0.5f;
	if (IsKeyUp(KEY_A)) return;
	if (IsKeyDown(KEY_LEFT_SHIFT)) speed = -1.f;
	direction = Direction::Left;
	keyDown = true;
	if (horizantalVel <= P_MAX_SPEED * speed) return;
	horizantalVel -= P_MAX_SPEED * dt;
}

void Player::MoveRight()
{
	float speed = 0.5f;
	if (IsKeyUp(KEY_D)) return;
	if (IsKeyDown(KEY_LEFT_SHIFT)) speed = 1.f;
	direction = Direction::Right;
	keyDown = true;
	if (horizantalVel >= P_MAX_SPEED * speed) return;
	horizantalVel += P_MAX_SPEED * dt;
}

void Player::Jump()
{
	if (!onPlatform) return;
	if (!IsKeyPressed(KEY_W)) return;

	verticalVel = P_JUMP_VELOCITY;
	onPlatform = false;
}

bool Player::Gravity()
{
	if (verticalVel < GRAVITY * 2.5f)
		verticalVel += GRAVITY * dt;

	collider.y += verticalVel * dt;

	if (collider.y > screenHeight + collider.height)
	{
		verticalVel = 1;
		horizantalVel = 0;
		return false;
	}
	return true;
}

void Player::Friction()
{
	if (!keyDown)
	{
		if (horizantalVel > 0)
		{
			if (onPlatform) horizantalVel -= FRICTION * dt;
			else horizantalVel -= (FRICTION * 0.66666f) * dt;

			if (horizantalVel < 0) horizantalVel = 0;
	    }
		else if (horizantalVel < 0)
		{
			if (onPlatform) horizantalVel += FRICTION * dt;
			else horizantalVel += (FRICTION * 0.66666f) * dt;

			if (horizantalVel > 0) horizantalVel = 0;
		}		
    }

	collider.x += horizantalVel * dt;
}

bool Player::Update()
{
	MoveRight();
	MoveLeft();
	Jump();
	Friction();
	return Gravity();	
}

const vector<Vector2> Player::GetAllPoints() const
{	
	float scrollOffset = ((screenWidth / TILE_DIMENSION) * (page - 1));
	Vector2 tl = { scrollOffset + (collider.x / TILE_DIMENSION), collider.y / TILE_DIMENSION};
	Vector2 tr = { scrollOffset + ((collider.x + collider.width) / TILE_DIMENSION), collider.y / TILE_DIMENSION };
	Vector2 bl = { scrollOffset + (collider.x / TILE_DIMENSION), (collider.y + collider.height) / TILE_DIMENSION };
	Vector2 br = { scrollOffset + ((collider.x + collider.width) / TILE_DIMENSION), (collider.y + collider.height) / TILE_DIMENSION };

	return { tl, tr, bl, br };
}

