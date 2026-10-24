#include <cstdio>
struct C2DVector { float x, y; };
struct CRGBFloatColour { float r, g, b, a; };
struct CRGBColour { unsigned char B, G, R, A; CRGBColour() { } CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
struct CUIStateDef { unsigned char _pad00[0x40]; unsigned long StateChangeFlag; long StateChangeType; C2DVector Position; C2DVector Zoom; CRGBFloatColour Colour; float UpdateTime; unsigned char _pad6c[0x0c]; bool LinearChange; };
__forceinline long ROUND(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
struct CUIState { unsigned long StateChangeFlag; long StateChangeType; C2DVector Position; C2DVector Zoom; CRGBColour Colour; float UpdateTime; bool LinearChange; CUIState(const CUIStateDef& def); };
CUIState::CUIState(const CUIStateDef& def)
{
    Position = def.Position; Zoom = def.Zoom;
    long r = ROUND(def.Colour.r * 255.0f); long g = ROUND(def.Colour.g * 255.0f); long b = ROUND(def.Colour.b * 255.0f); long a = ROUND(def.Colour.a * 255.0f);
    Colour = CRGBColour((unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a);
    UpdateTime = def.UpdateTime; StateChangeType = def.StateChangeType; StateChangeFlag = def.StateChangeFlag; LinearChange = def.LinearChange;
}
int main()
{
    CUIStateDef d = {}; d.StateChangeFlag = 7; d.StateChangeType = 3; d.Position.x = 1.5f; d.Position.y = 2.5f; d.Zoom.x = 0.5f; d.Zoom.y = 0.25f;
    d.Colour.r = 1.0f; d.Colour.g = 0.5f; d.Colour.b = 0.2f; d.Colour.a = 0.998f; d.UpdateTime = 4.0f; d.LinearChange = true;
    CUIState s(d);
    bool ok = s.Colour.R == 255 && s.Colour.G == 128 && s.Colour.B == 51 && s.Colour.A == 254   // 127.5 -> 128 (ties to even), 254.49 -> 254
           && s.StateChangeFlag == 7 && s.StateChangeType == 3 && s.Position.y == 2.5f && s.Zoom.y == 0.25f && s.UpdateTime == 4.0f && s.LinearChange
           && *(unsigned long*)&s.Colour == 0xfeff8033u;
    std::printf(ok ? "0052f440_TEST PASS\n" : "0052f440_TEST FAIL\n"); return ok ? 0 : 1;
}