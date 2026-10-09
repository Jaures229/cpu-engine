#pragma once

#include "Player.h"
#include "Obstacle.h"

enum GameState {
	PLAYING,
	PAUSED,
	WIN,
	LOOSE
};

enum PlayerState {
	MOVING,
	STOP
};

class App
{
public:
	App();
	virtual ~App();

	static App& GetInstance() { return *s_pApp; }

	void OnStart();
	void OnUpdate();
	void OnExit();
	void OnRender(int pass);

	static void MyPixelShader(cpu_ps_io& io);

private:
	inline static App* s_pApp = nullptr;

	// Ressources
	cpu_mesh m_obs;
	cpu_mesh m_player;
	// Rail
	cpu_entity* rail = nullptr;
	cpu_mesh m_rail;
	cpu_material m_ailMaterial;

	cpu_entity* second_rail = nullptr;
	cpu_mesh m_srail;
	cpu_font m_font;
	cpu_particle_emitter* m_pEmitter = nullptr;

	// Game stats
	float spwan_time = 1.5f;
	float timer = 0.0f;
	int obs_speed = 3;
	int score = 0;
	GameState state;
	PlayerState pstate;

	// player
	Player player;

	// OBSTACLES
	std::list<Obstacle> obstacles;
	void CreateRail();
	void CreatePlayer();
	void CreateObstacles();
	void UpdateObstacles();
	void Game();
	void Draw_UI();
	void UpdatePlayer();
	void Input(float dt);
	void SpwanObstacles();
	void DestroyObstacles();
};
