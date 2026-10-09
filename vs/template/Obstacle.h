#pragma once
class Obstacle
{
public:
	Obstacle();
	~Obstacle();
	void Create(cpu_mesh* mesh, float x, float y, float z);
	void Update(float dt);
	cpu_entity* GetEntity() { return obstacle; }
	void Destroy();
private:
	cpu_entity* obstacle;
	float speed;
};

