#pragma optimize("s", on)
// NPlayerGui::CDrawMiniMap::CDrawMiniMap(CPlayerGui*, std::vector<CDrawBase*>*) -- CDrawBase(gui, list), own vtable, the mini-map display built, enabled/active flags set and alpha opaque, three fades built, empty region-exit vector, the display initialised with the GUI def's world resolution and nine primitive handles reserved
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064E83F, length 157 bytes.
// Callees: NPlayerGui::CDrawBase::CDrawBase @ 0x0064CC65, CMiniMapDisplay::CMiniMapDisplay() @ 0x0082A460 (manifest label
// "CHeroMorphDef" is propagated), CFadeInFadeOutBase::CFadeInFadeOutBase() @ 0x00436560 (x3), PeekGuiDef @ 0x00438570,
// CMiniMapDisplay::Initialise(float) @ 0x0082A310, vector<CEnginePrimitiveHandle>::Resize(long) @ 0x00443F90 (+0x1c, 9).
// Vtable 0x0125A634.
// Layout: engine/CDrawMiniMap.h is the PDB layout (MiniMap +0x2c, Enabled +0x108, fades +0x120/+0x13c/+0x158, RegionExits
// +0x174) and does not fit the shipped build, where everything sits 4 lower up to the display and 0x20 lower after it (the
// CDrawBase vector is 12 bytes and the retail CMiniMapDisplay is 0xc0 bytes, PDB 0xdc): MiniMap +0x28, Enabled +0xe8,
// Active +0xe9, OldActive +0xea, RegionChange +0xeb, Alpha +0xec, ScreenBox +0xf0 (left uninitialised), FadeInOut +0x100,
// TextFadeInOut +0x11c, ScreenPosFadeInOut +0x138, RegionExits +0x154 -- so the model is local.
// The GUI def float at +0x980 (retail offset) is the mini-map world resolution handed to the display.
// Shape: size TU; the display is built through edi (reused as `this` of Initialise), the byte flags are immediate stores, the
// region-exit vector's inline constructor zeroes through eax with `and dword ptr [..],0`; Initialise's float argument is
// copied `fld [eax+980h] ; push ecx ; fstp [esp]`.
class CPlayerGui;

class CPlayerGuiDef { public:
    unsigned char _pad[0x980];
    float         MiniMapWorldResolution;          // +0x980 (retail offset)
};

class CPlayerGui {
public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();   // 0x00438570
};     // 0x00438570

class CMiniMapDisplay { public:                           // retail 0xc0 bytes
    unsigned char _body[0xc0];
    CMiniMapDisplay();                             // 0x0082A460
    void Initialise(float worldResolution);        // 0x0082A310
};

class C2DBoxF { public:
    float Left, Top, Right, Bottom;
};

namespace NPlayerGui {

struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>: three words
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    void Resize(long count);                       // 0x00443F90
};

struct CRetailExitVector {                         // VC6 Dinkumware vector<pair<C3DVector,CCharString>>: three words
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CRetailExitVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CFadeInFadeOutBase {
    unsigned char _body[0x1c];
    CFadeInFadeOutBase();                          // 0x00436560
};

class CDrawBase { public:
    virtual void v00();
    unsigned char       _body[0x1c - 4];           // +0x04 .. +0x1c
    CRetailHandleVector PrimitiveHandles;          // +0x1c
    CDrawBase(CPlayerGui* gui, void* drawList);    // 0x0064CC65
};

class CDrawMiniMap : public CDrawBase { public:                  // vtable 0x0125A634
    virtual void v00();
    CMiniMapDisplay    MiniMap;                    // +0x28
    bool               Enabled;                    // +0xe8
    bool               Active;                     // +0xe9
    bool               OldActive;                  // +0xea
    bool               RegionChange;               // +0xeb
    unsigned char      Alpha;                      // +0xec
    C2DBoxF            ScreenBox;                  // +0xf0
    CFadeInFadeOutBase FadeInOut;                  // +0x100
    CFadeInFadeOutBase TextFadeInOut;              // +0x11c
    CFadeInFadeOutBase ScreenPosFadeInOut;         // +0x138
    CRetailExitVector  RegionExits;                // +0x154
    CDrawMiniMap(CPlayerGui* gui, void* drawList);
};

CDrawMiniMap::CDrawMiniMap(CPlayerGui* gui, void* drawList)
    : CDrawBase(gui, drawList), MiniMap(), Enabled(true), Active(true), OldActive(true), RegionChange(false), Alpha(0xff),
      FadeInOut(), TextFadeInOut(), ScreenPosFadeInOut(), RegionExits()
{
    MiniMap.Initialise(CPlayerGui::PeekGuiDef().MiniMapWorldResolution);
    PrimitiveHandles.Resize(9);
}

}