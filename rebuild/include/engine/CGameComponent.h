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
    virtual ~CGameComponent();
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
