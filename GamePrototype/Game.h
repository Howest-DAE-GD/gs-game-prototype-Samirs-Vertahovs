#pragma once
#include "BaseGame.h"
#include "Camera.h"
#include "Player.h"
#include "NPC.h"
#include "Texture.h"
class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

	enum class GameState {
		level1play,
		level2play,
		level3play,
		pause
	};

private:

	Camera* m_ptrCamera;
	Player* m_ptrPlayer;
	std::vector<NPC*> m_VectorNPCs;
	Texture* m_ptrMap;

	double m_Timer;
	float m_DeathTimer;
	Point2f m_MousePointer;

	int m_NumberOfEnemies;
	int m_NumberOfCivs;

	int m_EntityCount;

	float m_NPCwidth;

	Rectf m_MapBounds{ 0.0f, 0.0f, 3000.0f, 2000.0f };

	//Texture m_StringTexture;

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
};