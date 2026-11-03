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
protected:
    virtual void SetQuit();
    bool Quit;
    bool Running;
    CGame& Game;
};
FABLE_STATIC_ASSERT(sizeof(CGameComponent)==0x10);
#endif
