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
	m_ptrPlayer = new Player(Point2f{ 2000.0f, 1500.0f }, 35.0f, 35.0f, Rectf{ 0.0f, 0.0f, 4000.0f, 3000.0f });
	m_ptrMap = new Texture("Map.png");

	m_NPCwidth = 30.0f;

	m_NumberOfCivs = 0;

	m_VectorNPCs.push_back(new NPC(Point2f{ 2300.0f, 1750.0f }, m_NPCwidth, m_NPCwidth, true, m_MapBounds, m_ptrPlayer));
	m_VectorNPCs.push_back(new NPC(Point2f{ 1400.0f, 1400.0f }, m_NPCwidth, m_NPCwidth, true, m_MapBounds, m_ptrPlayer));
	m_VectorNPCs.push_back(new NPC(Point2f{ 1854.0f, 1558.0f }, m_NPCwidth, m_NPCwidth, true, m_MapBounds, m_ptrPlayer));
	m_VectorNPCs.push_back(new NPC(Point2f{ 2100.0f, 1840.0f }, m_NPCwidth, m_NPCwidth, true, m_MapBounds, m_ptrPlayer));
	m_VectorNPCs.push_back(new NPC(Point2f{ 2600.0f,  700.0f }, m_NPCwidth, m_NPCwidth, true, m_MapBounds, m_ptrPlayer));

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

	m_ptrCamera->Aim(4000.0f, 3000.0f, m_ptrPlayer->GetPosition());

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

	utils::DrawLine(Point2f{ m_ptrPlayer->GetPosition().x + 17.5f, m_ptrPlayer->GetPosition().y + 17.5f}, Point2f{(m_MousePointer.x + m_ptrPlayer->GetPosition().x - 850.0f * 0.5f), (m_MousePointer.y + m_ptrPlayer->GetPosition().y - 500.0f * 0.5f)});

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

		for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
		{
			m_VectorNPCs[index]->SetAlive();
		}

		m_ptrPlayer->SetAlive();

		m_ptrPlayer->SetPos(Point2f{ 2000.0f, 1500.0f });

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
