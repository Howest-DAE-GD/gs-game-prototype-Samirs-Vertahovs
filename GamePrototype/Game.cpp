#include "pch.h"
#include "Game.h"

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

	m_NPCwidth = 30.0f;

	m_NumberOfCivs = 0;

	for (int index{}; index < 5; ++index)
	{
		m_VectorNPCs.push_back(new NPC(Point2f{ float(rand() % 3001), float(rand() % 2001) }, m_NPCwidth, m_NPCwidth, true, m_MapBounds, m_ptrPlayer));
	}

	m_Timer = 30.0;
	m_DeathTimer = 5.0f;
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
}

void Game::Update( float elapsedSec )
{
	// Check keyboard state
	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );

	m_ptrPlayer->Update(elapsedSec, m_VectorNPCs, pStates);

	m_Timer -= elapsedSec;

	if (m_ptrPlayer->GetVelocity().x == 0 or m_ptrPlayer->GetVelocity().y == 0)
	{
		m_DeathTimer -= elapsedSec;
	}

	if (m_DeathTimer <= 0)
	{
		m_ptrPlayer->Die();
	}

	if (m_Timer <= 0)
	{
		m_ptrPlayer->Die();
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
}

void Game::Draw( ) const
{
	ClearBackground( );

	m_ptrCamera->Aim(3000.0f, 2000.0f, m_ptrPlayer->GetPosition());

	for (int index{}; index < 8; ++index)
	{
		for (int index2{}; index2 < 6; ++index2)
		{
			m_ptrMap->Draw(Point2f{ 0.0f + m_ptrMap->GetWidth() * index, 0.0f + m_ptrMap->GetHeight() * index2});
		}
	}

	SetColor(Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
	DrawRect(0.0f, 0.0f, 3000.0f, 2000.0f, 20.0f);

	m_ptrPlayer->Draw();

	for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
	{
		m_VectorNPCs[index]->Draw();
	}
	// The offset on the boarders is not tied to the half of the screen anymore!
	if (m_ptrPlayer->IsAlive())
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

		m_Timer = 30.0;
		m_DeathTimer = 5.0f;

		m_NumberOfCivs = 0;

		for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
		{
			m_VectorNPCs[index]->SetAlive();
			m_VectorNPCs[index]->SetPos(Point2f{ float(rand() % 3001), float(rand() % 2001) });
		}

		m_ptrPlayer->SetAlive();

		m_ptrPlayer->SetPos(Point2f{ 1500.0f, 1000.0f });

		m_ptrPlayer->SetCounter(0);

		for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
		{
			m_VectorNPCs[index]->SetHostility(true);
		}

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
