#include "pch.h"
#include "Player.h"

Player::Player()
{
	player = nullptr;
	life = 3;
	speed = 5;
	playerRotationAngle = 0.0f;
}

Player::~Player()
{

}

void Player::Create(cpu_mesh* mesh, float x, float y, float z)
{
	player = cpuEngine.CreateEntity();
	player->pMesh = mesh;
	player->transform.pos.x = x;
	player->transform.pos.y = y;
	player->transform.pos.z = z;
}

void Player::Update(float x, float y, float z, float radius)
{
	// x = Cx + Cos(Angle) x Rayon) (-1 - 1)
	// z = Cz + Sin(Angle) x Rayon)
	player->transform.pos.x = x + cos(playerRotationAngle) * radius;
	player->transform.pos.z = z + sin(playerRotationAngle) * radius;
}

void Player::Destroy()
{
	cpuEngine.Release(player);
}
