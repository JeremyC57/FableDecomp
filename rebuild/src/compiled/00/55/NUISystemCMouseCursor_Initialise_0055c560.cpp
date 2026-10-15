// NUISystem::CMouseCursor::Initialise() -- the CSprite initialisation (0x0041A8D0), then the cursor position (+0x34, +0x38) taken from the global mouse position (0x013B8AD8 / 0x013B8ADC)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055C560, length 27 bytes.
// Callees: CSprite::Initialise @ 0x0041A8D0. Shape: `this` in esi, the two words copied through eax / ecx, `ret`.
struct CSprite {
    virtual void v00();
    unsigned char _pad04[0x30];                    // +0x04 .. +0x34
    void Initialise();                             // 0x0041A8D0
};

extern float g_mousePositionX;                     // 0x013B8AD8
extern float g_mousePositionY;                     // 0x013B8ADC

namespace NUISystem {
struct CMouseCursor : CSprite {
    float m_positionX;                             // +0x34
    float m_positionY;                             // +0x38
    void Initialise();
};

void CMouseCursor::Initialise()
{
    CSprite::Initialise();
    m_positionX = g_mousePositionX;
    m_positionY = g_mousePositionY;
}
}