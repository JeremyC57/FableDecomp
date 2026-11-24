#ifndef FABLE_ENGINE_CGAMECOMPONENT_H
#define FABLE_ENGINE_CGAMECOMPONENT_H
// Retail class (header wave 2026-11-03, from Codex pass 12): bases CBaseClassNonCopyable +0 and CDeviceResetCallback +4
// (Ego_r.pdb CGameComponent: Quit +8, Running +9, Game +0xC; sizeof 0x10, struct_layouts_egor.tsv), Game a CGame& as the
// PDB declares it.
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CDeviceResetCallback.h"
class CGame;
class CGameComponent : public CBaseClassNonCopyable, public CDeviceResetCallback {
public:
    CGameComponent(CGame&);
#ifdef FABLE_RETAIL_CGAMECOMPONENT_DTOR_INLINE
    // NF233 opt-in (row 0x0042F50B ~CNewFrontendGameComponent): ??1CGameComponent@@UAE@XZ is in FableWin.pdb and Ego_d.pdb (debug) but
    // not in Ego_r.pdb (inline in release, no out-of-line copy); retail expands it in 0x0042F50B (0x0042F594..0x0042F5A4: the two
    // CGameComponent vtable stores 0x0122ECB0 / 0x0122ECA4, then the tail jump to ~CBaseClassNonCopyable 0x0099A300) and in the
    // scalar deleting destructor 0x00415940. Empty body (the class has no member with a destructor). Same virtual slot.
    virtual ~CGameComponent() {}
#else
    virtual ~CGameComponent();
#endif
    virtual void Init();
    virtual bool Run(CGameComponent**)=0;
    virtual void ChangeTextureColourDepth(long)=0;
#ifdef FABLE_RETAIL_CGAMECOMPONENT_ISRUNNING
    // NF228 opt-in (2026-11-22, row 0x0041E3F6 NUISystem::CManager::CManager): FableWin.pdb 0x01AE52EA ?IsRunning@CGameComponent@@QAE_NXZ
    // (public, non-const, bool) and the retail body that reads Running +9 in place (0x0041E558 `cmp byte ptr [eax+9],bl` on the
    // front-end component). Inline, as retail expands it.
    bool IsRunning() { return Running; }
#endif
protected:
    virtual void SetQuit();
    bool Quit;
    bool Running;
    CGame& Game;
};
FABLE_STATIC_ASSERT_AT(22, sizeof(CGameComponent)==0x10);
#endif
