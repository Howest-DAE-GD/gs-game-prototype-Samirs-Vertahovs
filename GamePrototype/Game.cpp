#include "pch.h"
#include "Game.h"

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_ptrCamera = new Camera(850.0f, 500.0f);
	m_ptrPlayer = new Player(Point2f{ 2000.0f, 1500.0f }, 35.0f, 35.0f, Rectf{ 0.0f, 0.0f, 4000.0f, 3000.0f });

	m_VectorNPCs.push_back(new NPC(Point2f{ 2300.0f, 1750.0f }, 30.0f, 30.0f, true));
	m_VectorNPCs.push_back(new NPC(Point2f{ 1400.0f, 1400.0f }, 30.0f, 30.0f, true));
	m_VectorNPCs.push_back(new NPC(Point2f{ 1854.0f, 1558.0f }, 30.0f, 30.0f, true));
	m_VectorNPCs.push_back(new NPC(Point2f{ 2100.0f, 2140.0f }, 30.0f, 30.0f, true));
	m_VectorNPCs.push_back(new NPC(Point2f{ 2600.0f,  700.0f }, 30.0f, 30.0f, true));


	m_Timer = 30.0;
	m_DeathTimer = 5.0f;
}

void Game::Cleanup( )
{
	delete m_ptrCamera;
	delete m_ptrPlayer;

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
}

void Game::Draw( ) const
{
	ClearBackground( );

	m_ptrCamera->Aim(4000.0f, 3000.0f, m_ptrPlayer->GetPosition());

	SetColor(Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
	DrawRect(0.0f, 0.0f, 3000.0f, 2000.0f, 20.0f);

	SetColor(Color4f{ 0.0f, 1.0f, 0.0f, 1.0f });
	FillRect(2000.0f, 1450.0f, 40.0f, 40.0f);

	m_ptrPlayer->Draw();

	for (int index{ 0 }; index < m_VectorNPCs.size(); ++index)
	{
		m_VectorNPCs[index]->Draw();
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
	case SDLK_UP:
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
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
