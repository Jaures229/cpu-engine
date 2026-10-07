#pragma once

enum GameState {
	PLAYING,
	PAUSED,
	WIN,
	LOOSE
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

	// Rail
	cpu_entity* rail;
	cpu_mesh m_rail;
	cpu_material m_ailMaterial;

	cpu_entity* second_rail;
	cpu_mesh m_srail;

	cpu_font m_font;

	// Game stats
	float m_playerRotationAngle = 0.0f;
	int speed = 5;
	float spwan_time = 1.5f;
	float timer = 0.0f;
	int obs_speed = 3;

	int score = 0;
	int life = 3;
	int click_count = 0;
	GameState state;

	cpu_mesh m_obs;

	// player

	cpu_entity* player;
	cpu_mesh m_player;

	// OBSTACLES
	std::list<cpu_entity*> obstacles;

	void CreateRail();

	void CreatePlayer();

	void CreateObstacles();

	void UpdateObstacles();

	void Game();
	void Draw_UI();

};

