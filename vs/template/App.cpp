#include "pch.h"

App::App()
{
	s_pApp = this;
	state = PLAYING;
	pstate = STOP;
	CPU_CALLBACK_START(OnStart);
	CPU_CALLBACK_UPDATE(OnUpdate);
	CPU_CALLBACK_EXIT(OnExit);
	CPU_CALLBACK_RENDER(OnRender);
}

App::~App()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void App::OnStart()
{
	// YOUR CODE HERE

	m_font.Create(cpuDevice.GetHeight() <= 512 ? 14 : 28);
	m_obs.CreateSphere(0.4f, 5, 5, cpu::ToColor(20, 210, 0));

	cpuEngine.GetParticleData()->Create(20000);
	cpuEngine.GetParticlePhysics()->gz = -0.5f;

	m_pEmitter = cpuEngine.CreateParticleEmitter();
	m_pEmitter->rate = 1.0f;
	m_pEmitter->colorMin = cpu::ToColor(255, 0, 0);
	m_pEmitter->colorMax = cpu::ToColor(255, 128, 0);
	//m_pEmitter->active

	CreateRail();
	CreatePlayer();
	state = PLAYING;
	pstate = STOP;

	cpuEngine.GetCamera()->transform.pos.y = 1.5f;
	cpuEngine.GetCamera()->transform.pos.z = -10.0f;
	cpuEngine.GetCamera()->transform.AddYPR(0.0f, 0.2f, 0.0f);
}

void App::OnUpdate()
{

	if (state == LOOSE) {
		if (cpuInput.IsBackPressed()) {
			// quit the game
			cpuEngine.Quit();
		}
		// do something
		// dispaly your score;
		// Press echp to quit game
	}

	// YOUR CODE HERE
	if (state == PLAYING) {
		Game();
	}

	
	if (state == PLAYING && cpuInput.IsBackPressed()) {
		state = PAUSED;
	} else if (state == PAUSED && cpuInput.IsBackPressed())	{
		state = PLAYING;
	}
}

void App::OnExit()
{
	// YOUR CODE HERE
	player.Destroy();
	DestroyObstacles();
}

void App::OnRender(int pass)
{
	// YOUR CODE HERE
	Draw_UI();
}

void App::MyPixelShader(cpu_ps_io& io)
{
	// YOUR CODE HERE
	io.color = io.p.color;
}

void App::CreateRail()
{
	rail = cpuEngine.CreateEntity();
	second_rail = cpuEngine.CreateEntity();

	m_rail.CreateCircle(5.0f, 100, cpu::ToColor(255, 255, 0));
	m_srail.CreateCircle(3.5f, 100, cpu::ToColor(0, 0, 0));

	rail->pMesh = &m_rail;
	second_rail->pMesh = &m_srail;

	rail->transform.pos.x = 0.0f;
	rail->transform.pos.y = -1.0f;

	second_rail->transform.pos.x = 0.0f;
	second_rail->transform.pos.y = -0.9f;
}

void App::CreatePlayer()
{
	m_player.CreateCube(0.4f, cpu::ToColor(255, 255, 255));

	player.Create(&m_player, 0.0f, -0.4f, 0.0f);
	m_pEmitter->pos = player.GetEntity()->transform.pos;
}

void App::CreateObstacles()
{
	float dt = cpuTime.delta;
	Obstacle obs;
	
	//// random spawn pose
	int rand_x = rand() % 360;
	int rand_z = rand() % 360;
	
	//Obstacle 
	float obs_x = second_rail->transform.pos.x + cos(rand_x) * 3.40f;
	float obs_y = 1.9f;
	float obs_z = second_rail->transform.pos.z + sin(rand_z) * 3.40f;
	
	obs.Create(&m_obs, obs_x, obs_y, obs_z);
	obstacles.push_back(obs);
}

void App::UpdateObstacles()
{
	float dt = cpuTime.delta;

	// Move obstacles
	for (auto it= obstacles.begin(); it != obstacles.end();)
	{
		Obstacle pobs = *it;

		pobs.Update(dt);

		bool hit_player = cpu::SphereSphere(player.GetEntity()->transform.pos, player.GetEntity()->sphere.radius,
			pobs.GetEntity()->transform.pos, pobs.GetEntity()->sphere.radius);

		if (hit_player) {
			pobs.Destroy();
			score += 1;
			it = obstacles.erase(it);
		}
		else if (pobs.GetEntity()->transform.pos.y <= -1) {
			if (player.GetLife() > 0) {
				player.GetLife() -= 1;
			}
			pobs.Destroy();
			it = obstacles.erase(it);
		}
		else {
			it++;
		}
	}
}

void App::Game()
{
	float dt = cpuTime.delta;
	float time = cpuTime.total;
	timer += dt;

	Input(dt);
	UpdatePlayer();
	SpwanObstacles();
	UpdateObstacles();
}

void App::Draw_UI()
{
	std::string life_n_score = "Life: " + CPU_STR(player.GetLife()) + "\n";
	life_n_score += "Score: " + CPU_STR(score);
	XMFLOAT3 tint = { 1.0f, 1.0f, 0.8f };
	cpuDevice.DrawText(&m_font, life_n_score.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), 10, CPU_TEXT_CENTER, &tint);
}

void App::UpdatePlayer()
{
	// x = Cx + Cos(Angle) x Rayon) (-1 - 1)
	// z = Cz + Sin(Angle) x Rayon)

	if (player.GetLife() <= 0) {
		state = LOOSE;
	}
	player.Update(second_rail->transform.pos.x, 0.0f, second_rail->transform.pos.z, 3.50f);

	if (pstate == MOVING) {
		m_pEmitter->active = true;
		m_pEmitter->pos = player.GetEntity()->transform.pos;
	}
	if (pstate == STOP) {
		m_pEmitter->active = false;
	}
}

void App::Input(float dt)
{
	// Camera Input
	if (cpuInput.IsUp())
		cpuEngine.GetCamera()->transform.Move(dt * 5.0f);
	if (cpuInput.IsDown())
		cpuEngine.GetCamera()->transform.Move(-dt * 5.0f);

	// Player Input
	if (cpuInput.IsLeft()) {
		pstate = MOVING;
		player.GetRotationAngle() += player.GetSpeed() * dt;
	} else if (cpuInput.IsRight()) {
		pstate = MOVING;
		player.GetRotationAngle() -= player.GetSpeed() * dt;
	}
	else {
		pstate = STOP;
	}
}

void App::SpwanObstacles()
{
	if (timer >= spwan_time) {
		CreateObstacles();
		timer = 0.0f;
	}
}

void App::DestroyObstacles()
{
	for (auto it = obstacles.begin(); it != obstacles.end(); it++)
	{
		Obstacle pobs = *it;
		pobs.Destroy();
	}
	obstacles.clear();
}
