#pragma once
enum  ConsumType
{
	LIFE,
	SCORE,
};

class Consumable
{
public:
	Consumable();
	~Consumable();
	void Create(ConsumType type, cpu_mesh* mesh, float x, float y, float z);
	void Update(float dt);
	cpu_entity* GetEntity() { return csm; }
	void Destroy();
private:
	cpu_entity* csm;
	float speed;
};
