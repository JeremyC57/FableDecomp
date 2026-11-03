#ifndef FABLE_ENGINE_NUISYSTEM_CSCROLLINGVIEWPORT_H
#define FABLE_ENGINE_NUISYSTEM_CSCROLLINGVIEWPORT_H
// NUISystem::CScrollingViewport (new, chunk 726). Ego_r.pdb layout (464 bytes): ScrollingSpeed +424, Window +428 (C2DBoxF),
// PScrollBar +444, PScrollBarOutside +448, Changed +452, OffsetToUseAbove +456, OffsetToUseUnder +460; retail is the PDB
// minus 0x34 (CViewport 0x174, not 0x1A8): GetScrollingSpeed 0x0053FC00 `fld [ecx+174h]`, GetDataFromDef 0x0053ED40 stores
// +0x174 and the 16-byte window at +0x178, the constructor 0x0053F120 clears +0x188/+0x18C/+0x194/+0x198 and sets the
// byte +0x190, Update 0x0053F1C0 reads +0x178..+0x184 and the scroll bar at +0x188. Name set: Ego_r / FableWin
// (??0..QAE@J@Z, ??1..UAE@XZ, ChangeState, GetDataFromDef, GetPScrollBar, Get/SetPViewportListeningToMouse (static
// __fastcall), GetScrollingSpeed, Initialise, ProcessEvent, ScrollChildren, Update). New virtuals in vtable 0x012468D4
// order: ScrollChildren slot 143 (0x0053EDA0), GetScrollingSpeed slot 144 (0x0053FC00).
// The listening viewport is the global 0x013B8AAC; its data symbol is not in the PDB name lists, so it keeps the
// established spelling NUISystem::g_pActiveScrollingViewport (landed 0x0053FC30); the PDB accessors read it inline.
#include "engine/NUISystem_CViewport.h"
#include "engine/C2DBoxF.h"
namespace NUISystem {
class CScrollingViewport;
extern CScrollingViewport* g_pActiveScrollingViewport;     // 0x013B8AAC
class CScrollingViewport : public CViewport {
public:
    CScrollingViewport(long);
    CScrollingViewport(const CScrollingViewport&);
    virtual ~CScrollingViewport();
    void GetDataFromDef();
    virtual void Initialise();
    virtual void Update(float);
    virtual void ChangeState(unsigned long);
    virtual void ProcessEvent(EEvent);
    virtual void ScrollChildren(bool);                      // slot 143
    virtual float GetScrollingSpeed();                      // slot 144
    CComponent* GetPScrollBar() { return PScrollBar; }
    static CScrollingViewport* __fastcall GetPViewportListeningToMouse() { return g_pActiveScrollingViewport; }
    static void __fastcall SetPViewportListeningToMouse(CScrollingViewport* viewport) { g_pActiveScrollingViewport = viewport; }
protected:
    float ScrollingSpeed;                                   // +0x174
    C2DBoxF Window;                                         // +0x178
    CComponent* PScrollBar;                                 // +0x188
    CComponent* PScrollBarOutside;                          // +0x18c
    bool Changed;                                           // +0x190
    float OffsetToUseAbove;                                 // +0x194
    float OffsetToUseUnder;                                 // +0x198
};
FABLE_STATIC_ASSERT(sizeof(CScrollingViewport)==0x19c);
}
#endif
