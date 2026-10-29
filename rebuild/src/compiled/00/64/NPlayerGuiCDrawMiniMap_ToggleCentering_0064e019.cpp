#pragma optimize("s", on)
// NPlayerGui::CDrawMiniMap::ToggleCentering() -- while the mini-map is active, either start the screen-position fade (after re-applying the hero's current region) or, when that fade is fully open, start closing it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064E019, length 136 bytes.
// Callees: CDrawMiniMap::IsActive() @ 0x00643E09, the world's virtual slot 13 PeekWorldMap (`call [eax+34h]`, twice),
// CWorldMap::GetLoadedRegion() @ 0x004FB150 (`mov eax,[ecx+9Ch]`; manifest "CWorld::DrawGetEnvironment" is an ICF twin),
// CWorldMap::GetRegion(long) const @ 0x004FC180 (0x58-byte regions), CDrawMiniMap::UpdateRegionExitNames(const CRegion&) @ 0x0064DD9A
// (Ego_r.pdb order; the landed SetRegion 0x0064DFDE calls it), CFadeInFadeOutBase::Start(float, float, CRGBColour) @ 0x00434840 (manifest "CEngine::StartNewScene"
// is propagated), CFadeInFadeOutBase::IsOpen() @ 0x0048DA30. Constant 0.5f @ 0x0122F59C.
// Layout: engine/CDrawMiniMap.h is the PDB layout and does not fit the shipped build (see the constructor 0x0064E83F): CDrawBase
// World +0xc; ScreenPosFadeInOut +0x138 (CFadeInFadeOutBase: Active +0, Closing +0xc, Opening +0xd) -- local model.
// Shape: size TU (`fld ; push ecx ; fstp [esp]` float arguments); bl is the shared zero (the Active test, the colour bytes and
// the Opening store); the World member is written twice (no local): VC reads it once straight into ecx for the inner call
// and keeps the copy in edi for the outer one (a named `world` local recolours this/fade/world), the region index evaluated
// first as the inner argument; the colour is built in place in the argument area by a CRGBColour(r, g, b, a) constructor over
// the D3DCOLOR byte order (B, G, R, A), assigning R, G, B, A -- the user copy constructor makes VC build the by-value argument
// in its own stack slot (`push ecx ; mov eax,esp`) instead of a frame temporary pushed as a dword; the fade is reached through a
// pointer local (ebp) in both arms.
class CRegion;

class CRGBColour { public:                                // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
    CRGBColour(const CRGBColour& other) : B(other.B), G(other.G), R(other.R), A(other.A) { }
};

class CFadeInFadeOutBase { public:                        // PDB 0x1c bytes
    bool          Active;                          // +0x00
    float         FadeInTime;                      // +0x04
    float         FadeOutTime;                     // +0x08
    bool          Closing;                         // +0x0c
    bool          Opening;                         // +0x0d
    float         OpenTimer;                       // +0x10
    float         CloseTimer;                      // +0x14
    unsigned long ToColour;                        // +0x18
    void Start(float fadeInTime, float fadeOutTime, CRGBColour colour);   // 0x00434840
    bool IsOpen();                                 // 0x0048DA30
};

class CWorldMap { public:
    long GetLoadedRegion() const;                  // 0x004FB150
    const CRegion& GetRegion(long index) const;    // 0x004FC180
};

class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual const CWorldMap& PeekWorldMap() const; // slot 13 (+0x34)
};

namespace NPlayerGui {

class CDrawMiniMap { public:
    void*              _vtable;                    // +0x00
    void*              PlayerGui;                  // +0x04
    void*              PlayerManager;              // +0x08
    CWorld*            World;                      // +0x0c
    unsigned char      _body10[0x138 - 0x10];
    CFadeInFadeOutBase ScreenPosFadeInOut;         // +0x138
    bool IsActive();                               // 0x00643E09
    void ToggleCentering();
protected:
    void UpdateRegionExitNames(const CRegion& region);   // 0x0064DD9A
};

void CDrawMiniMap::ToggleCentering()
{
    if (IsActive()) {
        CFadeInFadeOutBase* fade = &ScreenPosFadeInOut;
        if (!fade->Active) {
            UpdateRegionExitNames(World->PeekWorldMap().GetRegion(World->PeekWorldMap().GetLoadedRegion()));
            fade->Start(0.5f, 0.5f, CRGBColour(0, 0, 0, 0xff));
        }
        else if (fade->IsOpen()) {
            fade->Opening = false;
            fade->Closing = true;
        }
    }
}

}