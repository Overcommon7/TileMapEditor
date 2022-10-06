#pragma once
#include "Defines.h"

extern float dt;
extern unordered_map<string, Texture2D> gameTextures;
extern short page;

enum class Direction
{
	Right,
	Left
};

class Player
{
private:
	Rectangle collider;
	Direction direction;

	float verticalVel;
	float horizantalVel;

	float JumpTimer = 0;
	bool onPlatform = false;
	bool keyDown = false;

	void MoveRight();
	void MoveLeft();
	void Jump();
	bool Gravity();
	void Friction();


public:

	Player() : collider({0, 0, TILE_DIMENSION, TILE_DIMENSION}), verticalVel(1), horizantalVel(0), direction(Direction::Right) {}
	Player(const Player& p);
	Player& operator=(const Player& p);
	~Player() {}


	bool Update();
	const vector<Vector2> GetAllPoints() const;
	inline const Vector2 GetTopLeftPoint() const { return { collider.x / TILE_DIMENSION, collider.y / TILE_DIMENSION }; }
	inline const Vector2 GetTopRightPoint() const { return { (collider.x + collider.width) / TILE_DIMENSION, collider.y / TILE_DIMENSION };}
	inline const Vector2 GetBottomLeftPoint() const { return { collider.x / TILE_DIMENSION, (collider.y + collider.height) / TILE_DIMENSION };}
	inline const Vector2 GetBottomRightPoint() const { return { (collider.x + collider.width) / TILE_DIMENSION, (collider.y + collider.height) / TILE_DIMENSION };}
	inline const Rectangle& GetCollider() const { return collider; }
	inline const Vector2 GetCoords() const { return { collider.x / TILE_DIMENSION, collider.y / TILE_DIMENSION }; }
	inline const float& GetHorizantalVelocity() const { return horizantalVel; }
	inline const float& GetVerticalVelocity() const { return verticalVel; }
	inline const Vector2 GetCenterPos() const { return { collider.x + (collider.width * 0.5f), collider.y + (collider.y * 0.5f) }; }

	inline void SetOnplatform(const bool& p) { onPlatform = p; }
	inline void SetX(const float& x) { collider.x = x; }
	inline void SetY(const float& y) { collider.y = y; }
	inline void SetPosition(const float& x, const float& y) { collider.x = x, collider.y = y; }
	inline void SetVerticalvelocity(const float& v) { verticalVel = v; }
	inline void SetHorizantalvelocity(const float& v) { horizantalVel = v; }

	void Draw() const;
};

