#include "pch.h"
#include "Obstacle.h"

Obstacle::Obstacle()
{
	obstacle = nullptr;
	speed = 1.0f;
}

Obstacle::~Obstacle()
{

}

void Obstacle::Create(cpu_mesh* mesh, float x, float y, float z)
{
	obstacle = cpuEngine.CreateEntity();
	obstacle->pMesh = mesh;
	obstacle->transform.pos.x = x;
	obstacle->transform.pos.y = y;
	obstacle->transform.pos.z = z;
}

void Obstacle::Update(float dt)
{
	obstacle->transform.pos.y -= speed * dt;
}

void Obstacle::Destroy()
{
	cpuEngine.Release(obstacle);
}
