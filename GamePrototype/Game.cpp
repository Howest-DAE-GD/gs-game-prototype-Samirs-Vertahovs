#include "pch.h"
#include "Game.h"
#include <cmath>
#include <sstream>
#include <iostream>
#include <iomanip>

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
	
	for (int index{}; index < m_VectorNPCs.size(); ++index)
	{
		if (m_NumberOfCivs < 2)
		{
			int indexToChange{ rand() % 5 };

			if (m_VectorNPCs[indexToChange]->IsEnemy())
			{
				m_NumberOfCivs++;
				m_VectorNPCs[indexToChange]->SetHostility(false);
			}
			else
			{
				index -= 1;
			}
		}
		else
		{
			break;
		}
	}
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_ptrCamera = new Camera(850.0f, 500.0f);
	m_ptrPlayer = new Player(Point2f{ 2000.0f, 1500.0f }, 35.0f, 35.0f, Rectf{ 0.0f, 0.0f, 3000.0f, 2000.0f });
	m_ptrMap = new Texture("Map.png");
	m_StringTexture = new Texture("Kill or be Killed:", "Montserrat-Light.ttf", 40, Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });
	m_StringTexture2 = new Texture("Do not stop:", "Montserrat-Light.ttf", 40, Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });
	m_StringTexture3 = new Texture("Better luck next time.", "Montserrat-Light.ttf", 65, Color4f{ 0.7f, 0.0f, 0.0f, 1.0f });
	m_StringTexture4 = new Texture("Press R to restart.", "Montserrat-Light.ttf", 45, Color4f{ 0.3f, 0.0f, 0.0f, 1.0f });
	m_StringTexture5 = new Texture("Next stage, then.", "Montserrat-Light.ttf", 45, Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });
	m_StringTexture6 = new Texture("Final stage.", "Montserrat-Light.ttf", 45, Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });

	m_One = new Texture("1", "Montserrat-Light.ttf", 40, Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });
	m_Two = new Texture("2", "Montserrat-Light.ttf", 40, Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });
	m_Three = new Texture("3", "Montserrat-Light.ttf", 40, Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });
	m_Four = new Texture("4", "Montserrat-Light.ttf", 40, Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });
	m_Five = new Texture("5", "Montserrat-Light.ttf", 40, Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });
	m_Six = new Texture("6", "Montserrat-Light.ttf", 40, Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });
	m_Seven = new Texture("7", "Montserrat-Light.ttf", 40, Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });
	m_Eight = new Texture("8", "Montserrat-Light.ttf", 40, Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });
	m_Nine = new Texture("9", "Montserrat-Light.ttf", 40, Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });
	m_Zero = new Texture("0", "Montserrat-Light.ttf", 40, Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });
	m_Point = new Texture(".", "Montserrat-Light.ttf", 40, Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });

	m_GameState = GameState::level1play;

	m_NPCwidth = 30.0f;

	m_NumberOfCivs = 0;

	m_KilledEnemiesCurrentLvl = 0;
	m_KilledCivsCurrentLvl = 0;
	m_KilledEnemiesTotal = 0;
	m_KilledCivsTotal = 0;

	for (int index{}; index < 5; ++index)
	{
		m_VectorNPCs.push_back(new NPC(Point2f{ GenerateX(), GenerateY() }, m_NPCwidth, m_NPCwidth, true, m_MapBounds, m_ptrPlayer));
	}

	m_Timer = 35.0f;
	m_DeathTimer = 5.0f;

	m_StringTimer = std::to_string(m_Timer);
}

void Game::Cleanup( )
{
	delete m_ptrCamera;
	delete m_ptrPlayer;
	delete m_ptrMap;

	for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
	{
		delete m_VectorNPCs[index];
	}

	delete m_StringTexture;
	delete m_StringTexture2;
	delete m_StringTexture3;
	delete m_StringTexture4;
	delete m_StringTexture5;
	delete m_StringTexture6;
	delete m_One;
	delete m_Two;
	delete m_Three;
	delete m_Four;
	delete m_Five;
	delete m_Six;
	delete m_Seven;
	delete m_Eight;
	delete m_Nine;
	delete m_Zero;
	delete m_Point;
}

void Game::Update( float elapsedSec )
{
	// Check keyboard state
	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );

	m_ptrPlayer->Update(elapsedSec, m_VectorNPCs, pStates, m_KilledEnemiesCurrentLvl, m_KilledCivsCurrentLvl);

	if (m_Timer > 0)
	{
		m_Timer -= elapsedSec;
	}

	if (m_ptrPlayer->GetVelocity().x == 0 and m_ptrPlayer->GetVelocity().y == 0)
	{
		m_DeathTimer -= elapsedSec;
	}

	if (pStates[SDL_SCANCODE_W] and pStates[SDL_SCANCODE_S] and !pStates[SDL_SCANCODE_A] and !pStates[SDL_SCANCODE_D])
	{
		m_DeathTimer -= elapsedSec;
	}

	if (pStates[SDL_SCANCODE_A] and pStates[SDL_SCANCODE_D] and !pStates[SDL_SCANCODE_W] and !pStates[SDL_SCANCODE_S])
	{
		m_DeathTimer -= elapsedSec;
	}

	if (pStates[SDL_SCANCODE_A] and pStates[SDL_SCANCODE_D] and pStates[SDL_SCANCODE_W] and pStates[SDL_SCANCODE_S])
	{
		m_DeathTimer -= elapsedSec;
	}

	/*if (pStates[SDL_SCANCODE_P])
	{
		m_GameState = GameState::level2play;
		m_KilledEnemiesCurrentLvl = 5;
		m_Timer = 2.0f;
	}*/


	if (m_DeathTimer <= 0)
	{
		m_ptrPlayer->Die();
	}

	if (m_Timer <= 0)
	{
		m_ptrPlayer->Die();
	}

	if (m_Timer < 0 or m_DeathTimer < 0 or !m_ptrPlayer->IsAlive())
	{
		m_Timer = 0;
		m_DeathTimer = 0;
	}


	if (m_ptrPlayer->GetPosition().x < 0.0f or m_ptrPlayer->GetPosition().x > 3000.0f)
	{
		m_ptrPlayer->Die();
	}

	if (m_ptrPlayer->GetPosition().y < 0.0f or m_ptrPlayer->GetPosition().y > 2000.0f)
	{
		m_ptrPlayer->Die();
	}

	for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
	{
		m_VectorNPCs[index]->Draw();
	}

	for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
	{
		m_VectorNPCs[index]->Update(elapsedSec);
	}

	for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
	{
		m_VectorNPCs[index]->Shoot(m_ptrPlayer->GetPosition());
	}

	float scale = std::pow(10.0f, 2);
	m_DeathTimer = std::round(m_DeathTimer * scale) / scale;

	std::ostringstream out;
	out << std::fixed << std::setprecision(2) << m_DeathTimer;
	std::string str = out.str();
	
	m_StringDeathTimer = str;

	m_StringTimer = std::to_string(int(m_Timer));


	std::cout << m_KilledCivsCurrentLvl << std::endl;
	std::cout << m_KilledEnemiesCurrentLvl << std::endl;
	std::cout << m_KilledCivsTotal << std::endl;
	std::cout << m_KilledEnemiesTotal << std::endl;

	// Level switch

	if (m_GameState == GameState::level1play and m_KilledEnemiesCurrentLvl == 3 and m_Timer < 1.25f)
	{
		m_GameState = GameState::level2play;
		m_KilledCivsTotal += m_KilledCivsCurrentLvl;
		m_KilledEnemiesTotal += m_KilledEnemiesCurrentLvl;

		m_KilledCivsCurrentLvl = 0;
		m_KilledEnemiesCurrentLvl = 0;

		m_Timer = 35.0;
		m_DeathTimer = 5.0f;

		m_NumberOfCivs = 0;

		m_ptrPlayer->SetAlive();

		m_ptrPlayer->SetPos(Point2f{ 2000.0f, 1500.0f });

		for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
		{
			delete m_VectorNPCs[index];
		}

		m_VectorNPCs.clear();

		if (m_KilledCivsTotal > 0)
		{
			for (int index{}; index < 8; ++index)
			{
				m_VectorNPCs.push_back(new NPC(Point2f{ GenerateX(), GenerateY() }, m_NPCwidth, m_NPCwidth, true, m_MapBounds, m_ptrPlayer));
			}
		}
		else
		{
			for (int index{}; index < 7; ++index)
			{
				m_VectorNPCs.push_back(new NPC(Point2f{ GenerateX(), GenerateY() }, m_NPCwidth, m_NPCwidth, true, m_MapBounds, m_ptrPlayer));
			}
		}

		for (int index{}; index < m_VectorNPCs.size(); ++index)
		{
			if (m_NumberOfCivs < 3)
			{
				int indexToChange{ int(rand() % m_VectorNPCs.size()) };

				if (m_VectorNPCs[indexToChange]->IsEnemy())
				{
					m_NumberOfCivs++;
					m_VectorNPCs[indexToChange]->SetHostility(false);
				}
				else
				{
					index -= 1;
				}
			}
			else
			{
				break;
			}
		}

	}

	if (m_GameState == GameState::level2play and m_KilledEnemiesCurrentLvl == 5 and m_Timer < 1.5f)
	{
		m_GameState = GameState::level3play;
		m_KilledCivsTotal += m_KilledCivsCurrentLvl;
		m_KilledEnemiesTotal += m_KilledEnemiesCurrentLvl;

		m_KilledCivsCurrentLvl = 0;
		m_KilledEnemiesCurrentLvl = 0;

		m_Timer = 45.0;
		m_DeathTimer = 5.0f;

		m_NumberOfCivs = 0;

		m_ptrPlayer->SetAlive();

		m_ptrPlayer->SetPos(Point2f{ 2000.0f, 1500.0f });

		for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
		{
			delete m_VectorNPCs[index];
		}

		m_VectorNPCs.clear();

		if (m_KilledCivsTotal > 5)
		{
			for (int index{}; index < 13; ++index)
			{
				m_VectorNPCs.push_back(new NPC(Point2f{ GenerateX(), GenerateY() }, m_NPCwidth, m_NPCwidth, true, m_MapBounds, m_ptrPlayer));
			}
		}
		else
		{
			for (int index{}; index < 10; ++index)
			{
				m_VectorNPCs.push_back(new NPC(Point2f{ GenerateX(), GenerateY() }, m_NPCwidth, m_NPCwidth, true, m_MapBounds, m_ptrPlayer));
			}
		}

		for (int index{}; index < m_VectorNPCs.size(); ++index)
		{
			if (m_NumberOfCivs < 5)
			{
				int indexToChange{ int(rand() % m_VectorNPCs.size()) };

				if (m_VectorNPCs[indexToChange]->IsEnemy())
				{
					m_NumberOfCivs++;
					m_VectorNPCs[indexToChange]->SetHostility(false);
				}
				else
				{
					index -= 1;
				}
			}
			else
			{
				break;
			}
		}
	}
	else if (m_GameState == GameState::level2play and m_KilledEnemiesCurrentLvl == 4 and m_KilledCivsTotal < 2 and m_Timer < 1.5f)
	{
		m_GameState = GameState::level3play;
		m_KilledCivsTotal += m_KilledCivsCurrentLvl;
		m_KilledEnemiesTotal += m_KilledEnemiesCurrentLvl;

		m_KilledCivsCurrentLvl = 0;
		m_KilledEnemiesCurrentLvl = 0;

		m_Timer = 40.0;
		m_DeathTimer = 5.0f;

		m_NumberOfCivs = 0;

		m_ptrPlayer->SetAlive();

		m_ptrPlayer->SetPos(Point2f{ 2000.0f, 1500.0f });

		for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
		{
			delete m_VectorNPCs[index];
		}

		m_VectorNPCs.clear();

		if (m_KilledCivsTotal > 4)
		{
			for (int index{}; index < 11; ++index)
			{
				m_VectorNPCs.push_back(new NPC(Point2f{ GenerateX(), GenerateY() }, m_NPCwidth, m_NPCwidth, true, m_MapBounds, m_ptrPlayer));
			}
		}
		else
		{
			for (int index{}; index < 9; ++index)
			{
				m_VectorNPCs.push_back(new NPC(Point2f{ GenerateX(), GenerateY() }, m_NPCwidth, m_NPCwidth, true, m_MapBounds, m_ptrPlayer));
			}
		}
		
		for (int index{}; index < m_VectorNPCs.size(); ++index)	
		{
			if (m_NumberOfCivs < 4)
			{
				int indexToChange{ int(rand() % m_VectorNPCs.size()) };

				if (m_VectorNPCs[indexToChange]->IsEnemy())
				{
					m_NumberOfCivs++;
					m_VectorNPCs[indexToChange]->SetHostility(false);
				}
				else
				{
					index -= 1;
				}
			}
			else
			{
				break;
			}
		}
	}

	if (m_GameState == GameState::level3play and m_KilledEnemiesCurrentLvl == 7 and m_KilledCivsTotal > 4 and m_Timer < 2.0f)
	{
		m_GameState = GameState::finale;
		m_KilledCivsTotal += m_KilledCivsCurrentLvl;
		m_KilledEnemiesTotal += m_KilledEnemiesCurrentLvl;

		m_KilledCivsCurrentLvl = 0;
		m_KilledEnemiesCurrentLvl = 0;
	}
	else if (m_GameState == GameState::level3play and m_KilledEnemiesCurrentLvl == 5 and m_Timer < 2.0f)
	{
		m_GameState = GameState::finale;
		m_KilledCivsTotal += m_KilledCivsCurrentLvl;
		m_KilledEnemiesTotal += m_KilledEnemiesCurrentLvl;

		m_KilledCivsCurrentLvl = 0;
		m_KilledEnemiesCurrentLvl = 0;
	}

}

void Game::Draw() const
{
	ClearBackground();

	m_ptrCamera->Aim(3000.0f, 2000.0f, m_ptrPlayer->GetPosition());

	for (int index{}; index < 8; ++index)
	{
		for (int index2{}; index2 < 6; ++index2)
		{
			m_ptrMap->Draw(Point2f{ 0.0f + m_ptrMap->GetWidth() * index, 0.0f + m_ptrMap->GetHeight() * index2 });
		}
	}

	SetColor(Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
	DrawRect(0.0f, 0.0f, 3000.0f, 2000.0f, 20.0f);

	m_StringTexture->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 10.0f, m_ptrCamera->GetTrackPos().y + 440.0f });
	m_StringTexture2->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 10.0f, m_ptrCamera->GetTrackPos().y + 400.0f });

	for (int i = 0; i < m_StringTimer.length(); ++i)
	{
		switch (m_StringTimer[i]) {
		case '1':
			m_One->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 320.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 440.0f });
			break;
		case '2':
			m_Two->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 320.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 440.0f });
			break;
		case '3':
			m_Three->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 320.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 440.0f });
			break;
		case '4':
			m_Four->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 320.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 440.0f });
			break;
		case '5':
			m_Five->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 320.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 440.0f });
			break;
		case '6':
			m_Six->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 320.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 440.0f });
			break;
		case '7':
			m_Seven->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 320.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 440.0f });
			break;
		case '8':
			m_Eight->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 320.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 440.0f });
			break;
		case '9':
			m_Nine->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 320.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 440.0f });
			break;
		case '0':
			m_Zero->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 320.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 440.0f });
			break;
		case '.':
			m_Point->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 320.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 440.0f });
			break;
		}
	}

	for (int i = 0; i < m_StringDeathTimer.length(); ++i)
	{
		switch (m_StringDeathTimer[i]) {
		case '1':
			m_One->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 270.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 400.0f });
			break;
		case '2':
			m_Two->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 270.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 400.0f });
			break;
		case '3':
			m_Three->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 270.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 400.0f });
			break;
		case '4':
			m_Four->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 270.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 400.0f });
			break;
		case '5':
			m_Five->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 270.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 400.0f });
			break;
		case '6':
			m_Six->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 270.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 400.0f });
			break;
		case '7':
			m_Seven->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 270.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 400.0f });
			break;
		case '8':
			m_Eight->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 270.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 400.0f });
			break;
		case '9':
			m_Nine->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 270.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 400.0f });
			break;
		case '0':
			m_Zero->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 270.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 400.0f });
			break;
		case '.':
			m_Point->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 270.0f + i * 30.0f, m_ptrCamera->GetTrackPos().y + 400.0f });
			break;
		}
	}

	m_ptrPlayer->Draw();

	for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
	{
		m_VectorNPCs[index]->Draw();
	}
	// The offset on the boarders is not tied to the half of the screen anymore!
	if (m_ptrPlayer->IsAlive() and m_GameState != GameState::finale)
	{
		SetColor(Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });

		Point2f playerPos = m_ptrPlayer->GetPosition();
		Point2f start{ playerPos.x + 17.5f, playerPos.y + 17.5f };
		Point2f end;

		if (playerPos.x > 425.0f && playerPos.x < 2575.0f)
		{
			if (playerPos.y > 250.0f && playerPos.y < 1750.0f)
			{
				end = Point2f{ m_MousePointer.x + playerPos.x - 850.0f * 0.5f, m_MousePointer.y + playerPos.y - 500.0f * 0.5f };
			}
			else if (playerPos.y < 250.0f)
			{
				end = Point2f{ m_MousePointer.x + playerPos.x - 850.0f * 0.5f, m_MousePointer.y };
			}
			else if (playerPos.y > 1750.0f)
			{
				end = Point2f{ m_MousePointer.x + playerPos.x - 850.0f * 0.5f, m_MousePointer.y + (2000.0f - 500.0f) };
			}
		}
		else if (playerPos.x < 425.0f)
		{
			if (playerPos.y > 250.0f && playerPos.y < 1750.0f)
			{
				end = Point2f{ m_MousePointer.x, m_MousePointer.y + playerPos.y - 500.0f * 0.5f };
			}
			else if (playerPos.y < 250.0f)
			{
				end = Point2f{ m_MousePointer.x, m_MousePointer.y };
			}
			else if (playerPos.y > 1750.0f)
			{
				end = Point2f{ m_MousePointer.x, m_MousePointer.y + (2000.0f - 500.0f) };
			}
		}
		else if (playerPos.x > 2575.0f)
		{
			if (playerPos.y > 250.0f && playerPos.y < 1750.0f)
			{
				end = Point2f{ m_MousePointer.x + (3000.0f - 850.0f), m_MousePointer.y + playerPos.y - 500.0f * 0.5f };
			}
			else if (playerPos.y < 250.0f)
			{
				end = Point2f{ m_MousePointer.x + (3000.0f - 850.0f), m_MousePointer.y };
			}
			else if (playerPos.y > 1750.0f)
			{
				end = Point2f{ m_MousePointer.x + (3000.0f - 850.0f), m_MousePointer.y + (2000.0f - 500.0f) };
			}
		}

		utils::DrawLine(start, end);
	}

	if (!m_ptrPlayer->IsAlive() and m_GameState != GameState::finale)
	{
		m_StringTexture3->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 95.0f, m_ptrCamera->GetTrackPos().y + 250.0f });
		m_StringTexture4->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 240.0f, m_ptrCamera->GetTrackPos().y + 200.0f });
	}

	if (m_GameState == GameState::level2play and m_Timer > 35.0f)
	{
		m_StringTexture5->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 240.0f, m_ptrCamera->GetTrackPos().y + 200.0f });
	}

	if (m_GameState == GameState::level3play and m_Timer > 40.0f)
	{
		m_StringTexture6->Draw(Point2f{ m_ptrCamera->GetTrackPos().x + 250.0f, m_ptrCamera->GetTrackPos().y + 200.0f });
	}

	m_ptrCamera->Reset();

}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch (e.keysym.sym)
	{
	case SDLK_r:
		
		if (m_GameState == GameState::level1play)
		{
			m_Timer = 35.0;
			m_DeathTimer = 5.0f;

			m_NumberOfCivs = 0;

			m_KilledEnemiesTotal = 0;
			m_KilledCivsTotal = 0;
			m_KilledEnemiesCurrentLvl = 0;
			m_KilledCivsCurrentLvl = 0;

			for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
			{
				m_VectorNPCs[index]->SetAlive();
				m_VectorNPCs[index]->SetPos(Point2f{ GenerateX(), GenerateY() });
			}

			m_ptrPlayer->SetAlive();

			m_ptrPlayer->SetPos(Point2f{ 2000.0f, 1500.0f });

			for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
			{
				m_VectorNPCs[index]->SetHostility(true);
			}

			for (int index{}; index < m_VectorNPCs.size(); ++index)
			{
				if (m_NumberOfCivs < 2)
				{
					int indexToChange{ int(rand() % m_VectorNPCs.size()) };

					if (m_VectorNPCs[indexToChange]->IsEnemy())
					{
						m_NumberOfCivs++;
						m_VectorNPCs[indexToChange]->SetHostility(false);
					}
					else
					{
						index -= 1;
					}
				}
				else
				{
					break;
				}
			}
		} 
		else if (m_GameState == GameState::level2play)
		{
			m_Timer = 40.0;
			m_DeathTimer = 5.0f;

			m_NumberOfCivs = 0;

			m_KilledCivsCurrentLvl = 0;
			m_KilledEnemiesCurrentLvl = 0;

			for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
			{
				m_VectorNPCs[index]->SetAlive();
				m_VectorNPCs[index]->SetPos(Point2f{ GenerateX(), GenerateY() });
			}

			m_ptrPlayer->SetAlive();

			m_ptrPlayer->SetPos(Point2f{ 2000.0f, 1500.0f });


			for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
			{
				m_VectorNPCs[index]->SetHostility(true);
			}

			
			for (int index{}; index < m_VectorNPCs.size(); ++index)
			{
				if (m_NumberOfCivs < 3)
				{
					int indexToChange{ int(rand() % m_VectorNPCs.size()) };

					if (m_VectorNPCs[indexToChange]->IsEnemy())
					{
						m_NumberOfCivs++;
						m_VectorNPCs[indexToChange]->SetHostility(false);
					}
					else
					{
						index -= 1;
					}
				}
				else
				{
					break;
				}
			}
		}
		else if (m_GameState == GameState::level3play)
		{
			m_Timer = 45.0;
			m_DeathTimer = 5.0f;

			m_NumberOfCivs = 0;

			m_KilledCivsCurrentLvl = 0;
			m_KilledEnemiesCurrentLvl = 0;

			for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
			{
				m_VectorNPCs[index]->SetAlive();
				m_VectorNPCs[index]->SetPos(Point2f{ GenerateX(), GenerateY() });
			}

			m_ptrPlayer->SetAlive();

			m_ptrPlayer->SetPos(Point2f{ 2000.0f, 1500.0f });


			for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
			{
				m_VectorNPCs[index]->SetHostility(true);
			}

			for (int index{}; index < m_VectorNPCs.size(); ++index)
			{
				if (m_NumberOfCivs < 5)
				{
					int indexToChange{ int(rand() % m_VectorNPCs.size()) };

					if (m_VectorNPCs[indexToChange]->IsEnemy())
					{
						m_NumberOfCivs++;
						m_VectorNPCs[indexToChange]->SetHostility(false);
					}
					else
					{
						index -= 1;
					}
				}
				else
				{
					break;
				}
			}
		}

		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;

	m_MousePointer.x = e.x;
	m_MousePointer.y = e.y;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	Point2f MousePos{ float(e.x), float(e.y) };

	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		m_ptrPlayer->Shoot(MousePos);
		break;
	}
	
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.7f, 0.7f, 0.7f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

float Game::GenerateX()
{
	int x = rand() % 3001;
	if (x >= 1800 && x <= 2200) {
		if (rand() % 2 == 0) {
			x = rand() % 1800;
		}
		else {
			x = 2201 + rand() % (3001 - 2201);
		}
	}
	return float(x);
}

float Game::GenerateY()
{
	int y = rand() % 2001;
	if (y >= 1300 && y <= 1700) {
		if (rand() % 2 == 0) {
			y = rand() % 1300;
		}
		else {
			y = 1701 + rand() % (2001 - 1701);
		}
	}
	return float(y);
}
