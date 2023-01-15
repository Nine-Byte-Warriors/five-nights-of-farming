#pragma once
#ifndef BUTTON_WIDGET_H
#define BUTTON_WIDGET_H

#include "Widget.h"
#include "Transform.h"

class Button_Widget : public Widget
{
public:
    enum class ButtonState
    {
        Default = 0,
        Pressed,
        Hover
    };

    Button_Widget();
    Button_Widget( const std::string& texture, XMFLOAT2 pos, XMFLOAT2 size );
    ~Button_Widget();

    void Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat );
    void Update( const float dt );
    void Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho, TextRenderer* textRenderer );
    bool Resolve( const std::string& text, XMVECTORF32 textColour, std::vector<std::string> textures, MouseData MData, XMFLOAT2 pos, XMFLOAT2 size );
    
    inline bool GetIsPressed() const noexcept { return m_bIsPressed; }
    inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
private:
    std::string m_sText;
    XMVECTORF32 m_vTextColor;
    bool m_bIsPressed = false;
    XMFLOAT2 m_vPosition, m_vSize;

    ButtonState m_buttonState;
    std::string  m_buttonTexture;

    std::shared_ptr<Sprite> m_sprite;
    std::shared_ptr<Transform> m_transform;
};

#endif