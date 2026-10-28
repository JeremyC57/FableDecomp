// CGameScriptInterface::FadeScreenOut(float fadeTime, float holdTime, CRGBColour colour) -- script call: starts a screen fade-out unless one is active and fading over an existing fade is not allowed; true when the fade starts
// Leaf: ?FadeScreenOut@CGameScriptInterface@@UAE_NMMVCRGBColour@@@Z
// Retail: Fable.exe @ 0x00890820, 80 bytes, thiscall `ret 0Ch` (the colour by value).
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 373 holds this body; slot 373 is
//   ?FadeScreenOut@CGameScriptInterface@@UAE_NMMVCRGBColour@@@Z (non-const in both PDBs).
// Body: if DisplayEngine (+0x0c)->IsScreenFadeOut() (inline: ScreenFadeOutInfo.Active, retail +0xbc = Ego_r.pdb
//   CDisplayEngine::ScreenFadeOutInfo +0xbc, CFadeInFadeOutBase::Active +0) and not AllowScreenFadingIfAlreadyFaded
//   (+0x2d, Ego_r.pdb CGameScriptInterface +0x2d): false. Else StartFadeScreen(fadeTime, holdTime, colour) (0x006E7370,
//   FableWin.pdb ?StartFadeScreen@CGameScriptInterface@@IAEXMMVCRGBColour@@@Z, protected; the manifest's
//   "ConstructString" label is wrong; its only caller is this row) with the colour copied byte-wise into its stack slot
//   through the user copy constructor (??0CRGBColour@@QAE@ABV0@@Z), and true.
// Shape: two StartFadeScreen calls (allowed-over-a-fade and no-fade) that VC7.1 tail-merges after their different colour
//   loads (`shr edx,10h` vs `mov dx,[esp+12h]`); the allowed call is the inner `if` body, so `return false` is the else.
class CRGBColour {
public:
    unsigned char B, G, R, A;                            // D3DCOLOR byte order
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
class CFadeInFadeOutBase {
public:
    bool Active;                                         // +0x00
    bool IsActive() const { return Active; }
};
class CDisplayEngine {
public:
    unsigned char _pad00[0xbc];
    CFadeInFadeOutBase ScreenFadeOutInfo;                // +0xbc
    bool IsScreenFadeOut() const { return ScreenFadeOutInfo.IsActive(); }
};
class CGameScriptInterface {
public:
    virtual bool FadeScreenOut(float fadeTime, float holdTime, CRGBColour colour);
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    CDisplayEngine* DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    void*           PlayerManager;                       // +0x14
    void*           ThingSearchTools;                    // +0x18
    long            CurrentPlayer;                       // +0x1c
    long            CurrentLevelID;                      // +0x20
    long            CurrentScriptLevelID;                // +0x24
    long            CurrentScriptID;                     // +0x28
    bool            InMovieSequence;                     // +0x2c
    bool            AllowScreenFadingIfAlreadyFaded;     // +0x2d
protected:
    void StartFadeScreen(float fadeTime, float holdTime, CRGBColour colour);   // 0x006E7370
};
bool CGameScriptInterface::FadeScreenOut(float fadeTime, float holdTime, CRGBColour colour)
{
    if (DisplayEngine->IsScreenFadeOut())
    {
        if (AllowScreenFadingIfAlreadyFaded)
            StartFadeScreen(fadeTime, holdTime, colour);
        else
            return false;
    }
    else
        StartFadeScreen(fadeTime, holdTime, colour);
    return true;
}