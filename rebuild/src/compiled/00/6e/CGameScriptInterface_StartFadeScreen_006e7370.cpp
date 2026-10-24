// CGameScriptInterface::StartFadeScreen(float fadeInTime, float fadeOutTime, CRGBColour colour) -- start a held screen fade on the display engine (its two times passed in the engine's order, hold time -1)
// Leaf: ?StartFadeScreen@CGameScriptInterface@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E7370, 53 bytes (64 with int3 padding), thiscall, `ret 0Ch`.
// Identity: Ego_r.pdb name set `?StartFadeScreen@CGameScriptInterface@@IAEXMMVCRGBColour@@@Z` (protected); its one caller
//   is CGameScriptInterface::FadeScreenOut 0x00890820 (vtable slot 373), which passes its own `this`, its two floats
//   in order and a copy of its colour. The catalog's "ConstructString" (and the forge type `bool __fastcall
//   NGameText::ConstructString(CWideString&, long, long)`) is a wrong propagated label.
// Callees: CDisplayEngine::StartScreenFadeOut(float, float, CRGBColour, float) @ 0x00434C00 (as the landed
//   CTCWife::HaveSex 0x006CF920 calls it).
// Layout: CGameScriptInterface (Ego_r.pdb = engine/CGameScriptInterface.h): DisplayEngine +0x0C. Modelled locally
//   because the generated header is a method-less struct and the leaf must be a (protected) member of the class.
// Shape: the outgoing colour is copy-constructed byte by byte in its argument slot (`push -1.0f ; push ecx ;
//   mov eax,esp`, then the R/A/G/B byte stores) = CRGBColour's user copy constructor through Set(R, G, B, A), as the
//   landed engine models; the two float arguments are passed swapped (second first).
struct CRGBColour {                                         // the union's channel view: D3DCOLOR byte order
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

class CDisplayEngine {
public:
    void StartScreenFadeOut(float fadeOutTime, float fadeInTime, CRGBColour colour, float holdTime);   // 0x00434C00
};

class CWorld;
class CMainGameComponent;

class CGameScriptInterface {
public:
    virtual ~CGameScriptInterface();
    CWorld*             World;                              // +0x04
    CMainGameComponent* Component;                          // +0x08
    CDisplayEngine*     DisplayEngine;                      // +0x0C
protected:
    void StartFadeScreen(float fadeInTime, float fadeOutTime, CRGBColour colour);
};

void CGameScriptInterface::StartFadeScreen(float fadeInTime, float fadeOutTime, CRGBColour colour)
{
    DisplayEngine->StartScreenFadeOut(fadeOutTime, fadeInTime, colour, -1.0f);
}