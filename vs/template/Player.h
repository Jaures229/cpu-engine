#pragma once

class Player
{
public:
	Player();
	 ~Player();
	 void Create(cpu_mesh *mesh, float x, float y, float z);
	 void Update(float x, float y, float z, float radius);
	 cpu_entity* GetEntity() { return player; }
	 int& GetLife() { return life; }
	 int& GetSpeed() { return speed; }
	 float& GetRotationAngle() { return playerRotationAngle; }
	 void Destroy();
private:
	cpu_entity* player;
	int life;
	int speed;
	float playerRotationAngle;
};
