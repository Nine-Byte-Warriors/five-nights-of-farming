#include "stdafx.h"
#include "UIScreen.h"
#include "Graphics.h"
#include <shellapi.h>

void UIScreen::Initialize( const Graphics& gfx, ConstantBuffer<Matrices>* mat )
{
	AddToEvent();	
	UIElement::Initialize( gfx, mat );
	m_titlecard.Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
	m_mainMenuBackground.Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
	for ( unsigned int i = 0; i < 2; i++ )
		m_mainMenuButtons[i].Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
}

void UIScreen::Update( const float dt )
{
	if ( m_bMouseLoad )
	{
		m_bMouseLoad = false;
		m_mouseData.LPress = false;
	}

	XMFLOAT2 size = { m_vScreenSize.x * 0.15f, m_vScreenSize.y * 0.13f };

	// --- Background image ---
	m_mainMenuBackground.Resolve( { 210, 210, 150 }, { 0.0f, 0.0f }, { m_vScreenSize.x, m_vScreenSize.y } );
	m_mainMenuBackground.Update( dt );

	// --- Game title card ---
	m_titlecard.Resolve( "Resources\\Textures\\cauliflower_ss.png", { m_vScreenSize.x * 0.4f, m_vScreenSize.y * 0.12f }, size );
	m_titlecard.Update( dt );

	// --- Menu buttons ---
	float xPos = ( m_vScreenSize.x * 0.5f ) - size.x / 2.0f;
	float yPos = 0.25f;

	// Github link
	if ( m_mainMenuButtons[0].Resolve( "Game Page", Colors::Black, m_buttonTexturesGithub, m_mouseData, { xPos, m_vScreenSize.y * yPos }, size ) )
		if ( !m_bOpenLink && m_bOpen )
			m_bOpenLink = true;
	m_mainMenuButtons[0].Update( dt );

	if ( !m_mainMenuButtons[0].GetIsPressed() )
		m_bOpen = true;

	if ( m_bOpenLink )
	{
		ShellExecute( 0, 0, L"https://github.com/Nine-Byte-Warriors", 0, 0, SW_SHOW );
		m_bOpenLink = false;
		m_bOpen = false;
	}

	yPos += 0.2f;

	// Quit game
	if ( m_mainMenuButtons[1].Resolve( "Exit", Colors::Black, m_buttonTexturesMain, m_mouseData, { xPos, m_vScreenSize.y * yPos }, size ) )
		EventSystem::Instance()->AddEvent( EVENTID::QuitGameEvent );
	m_mainMenuButtons[1].Update( dt );
}

void UIScreen::Draw( VertexShader vtx, PixelShader pix, XMMATRIX worldOrtho, TextRenderer* textRenderer )
{
	Shaders::BindShaders( m_pContext.Get(), vtx, pix );
	m_mainMenuBackground.Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho );
	m_titlecard.Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho );
	for ( unsigned int i = 0; i < 2; i++ )
	{
		m_mainMenuButtons[i].Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer );
		Shaders::BindShaders( m_pContext.Get(), vtx, pix );
	}
}

void UIScreen::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::MousePosition, this );
	EventSystem::Instance()->AddClient( EVENTID::LeftMouseClick, this );
	EventSystem::Instance()->AddClient( EVENTID::LeftMouseRelease, this );
	EventSystem::Instance()->AddClient( EVENTID::RightMouseClick, this );
	EventSystem::Instance()->AddClient( EVENTID::RightMouseRelease, this );
	EventSystem::Instance()->AddClient( EVENTID::MiddleMouseClick, this );
	EventSystem::Instance()->AddClient( EVENTID::MiddleMouseRelease, this );
}

void UIScreen::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::MousePosition, this );
	EventSystem::Instance()->RemoveClient( EVENTID::LeftMouseClick, this );
	EventSystem::Instance()->RemoveClient( EVENTID::LeftMouseRelease, this );
	EventSystem::Instance()->RemoveClient( EVENTID::RightMouseClick, this );
	EventSystem::Instance()->RemoveClient( EVENTID::RightMouseRelease, this );
	EventSystem::Instance()->RemoveClient( EVENTID::MiddleMouseClick, this );
	EventSystem::Instance()->RemoveClient( EVENTID::MiddleMouseRelease, this );
}

void UIScreen::HandleEvent( Event* event )
{
	switch ( event->GetEventID() )
	{
	case EVENTID::MousePosition: { m_mouseData.Pos = *(XMFLOAT2*)event->GetData(); } break;
	case EVENTID::LeftMouseClick: { m_mouseData.LPress = true; } break;
	case EVENTID::LeftMouseRelease: { m_mouseData.LPress = false; } break;
	case EVENTID::RightMouseClick: { m_mouseData.RPress = true; } break;
	case EVENTID::RightMouseRelease: { m_mouseData.RPress = false; } break;
	case EVENTID::MiddleMouseClick: { m_mouseData.MPress = true; } break;
	case EVENTID::MiddleMouseRelease: { m_mouseData.MPress = false; } break;
	case EVENTID::WindowSizeChangeEvent: { m_vScreenSize = *static_cast<XMFLOAT2*>( event->GetData() ); } break;
	}
}