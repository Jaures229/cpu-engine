#include "pch.h"

App::App()
{
	s_pApp = this;
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
	CreateRail();
	CreatePlayer();
	state = PLAYING;

	cpuEngine.GetCamera()->transform.pos.y = 1.0f;
	cpuEngine.GetCamera()->transform.pos.z = -10.0f;
}

void App::OnUpdate()
{
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
	m_srail.CreateCircle(3.5f, 100, cpu::ToColor(255, 255, 255));

	rail->pMesh = &m_rail;
	second_rail->pMesh = &m_srail;

	rail->transform.pos.x = 0.0f;
	rail->transform.pos.y = -1.0f;

	second_rail->transform.pos.x = 0.0f;
	second_rail->transform.pos.y = -0.9f;
}

void App::CreatePlayer()
{
	player = cpuEngine.CreateEntity();
	m_player.CreateCube(0.4f, cpu::ToColor(255, 255, 255));

	player->pMesh = &m_player;
	player->transform.pos.x = 0.0f;
	player->transform.pos.y = -0.4f;
}

void App::CreateObstacles()
{
	float dt = cpuTime.delta;
		//// random spawn pose
		cpu_entity* obs;

		obs = cpuEngine.CreateEntity();
		obs->pMesh = &m_obs;
		//obs->transform.pos.x =  0.0f;

		//// random spawn pose

		int rand_x = rand() % 360;
		int rand_z = rand() % 360;

		obs->transform.pos.x = second_rail->transform.pos.x + cos(rand_x) * 3.0f;
		obs->transform.pos.z = second_rail->transform.pos.z + sin(rand_z) * 3.0f;
		obs->transform.pos.y = 1.5f;

		obstacles.push_back(obs);



	// Ct = 0, St = 10
	// if Ct >= (Ct + sT) spawn
	// 0
}

void App::UpdateObstacles()
{
	float dt = cpuTime.delta;

	// Move obstacles
	for (auto it= obstacles.begin(); it != obstacles.end();)
	{
		cpu_entity* pobs = *it;
		pobs->transform.pos.y -= 1.0f * dt;

		bool hit_player = cpu::SphereSphere(player->transform.pos, player->sphere.radius, pobs->transform.pos, pobs->sphere.radius);

		if (hit_player) {
			cpuEngine.Release(pobs);
			score += 1;
			it++;
		}
		else if (pobs->transform.pos.y <= -1) {
			cpuEngine.Release(pobs);
			life -= 1;
			it++;
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

	if (cpuInput.IsUp())
		cpuEngine.GetCamera()->transform.Move(dt * 5.0f);
	if (cpuInput.IsDown())
		cpuEngine.GetCamera()->transform.Move(-dt * 5.0f);
	if (cpuInput.IsLeft())
		m_playerRotationAngle += speed * dt;
	if (cpuInput.IsRight())
		m_playerRotationAngle -= speed * dt;

	player->transform.pos.x = second_rail->transform.pos.x + cos(m_playerRotationAngle) * 3.0f;
	player->transform.pos.z = second_rail->transform.pos.z + sin(m_playerRotationAngle) * 3.0f;

	// x = Cx + Cos(Angle) x Rayon) (-1 - 1)
	// z = Cz + Sin(Angle) x Rayon)

	// Obstacles
	if (timer >= spwan_time) {
		CreateObstacles();
		timer = 0.0f;
	}

	UpdateObstacles();
}

void App::Draw_UI()
{
	std::string life_n_score = "Life: " + CPU_STR(life) + "\n";
	life_n_score += "Score: " + CPU_STR(score);
	XMFLOAT3 tint = { 1.0f, 1.0f, 0.8f };
	cpuDevice.DrawText(&m_font, life_n_score.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), 10, CPU_TEXT_CENTER, &tint);

}
