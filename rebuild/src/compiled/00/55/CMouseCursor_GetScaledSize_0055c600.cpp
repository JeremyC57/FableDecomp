// CMouseCursor::GetScaledSize() -- the sprite size with each axis multiplied by the front-end UI scale
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055C600, length 80 bytes.
// Callees: CSprite::GetSize @ 0x0041A850 (C2DVector by hidden pointer, `this` passed through), CFrontEndManager::GetInstance
// @ 0x0041E5F2 (`__fastcall`, no arguments), CManager::GetUIScale @ 0x0041CF47 (C2DVector by hidden pointer).
// Class attribution: manifest label "CGameScriptInterface::ClearGossip" is propagated; the body sits among the CMouseCursor
// functions (0x0055C5B0 TurnToPointer, 0x0055C650 CMouseCursor::CMouseCursor) and `this` is handed to CSprite::GetSize, so it is
// the sprite-derived cursor's method -- provisional.
// Shape: 16 bytes of frame = the size local and the scale temporary; each axis is `scale * size` (`fld size ; fmul [scale]`),
// the X product stored back to the local and the Y product left on the FPU stack and written straight into the result.
struct C2DVector {
    float X;
    float Y;
    C2DVector() {}
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) {}
};

struct CFrontEndManager {
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
    C2DVector GetUIScale();                         // 0x0041CF47
};

struct CSprite {
    C2DVector GetSize();                            // 0x0041A850
};

struct CMouseCursor : CSprite {
    C2DVector GetScaledSize();
};

C2DVector CMouseCursor::GetScaledSize()
{
    C2DVector size = GetSize();
    size.X = CFrontEndManager::GetInstance()->GetUIScale().X * size.X;
    size.Y = CFrontEndManager::GetInstance()->GetUIScale().Y * size.Y;
    return size;
}