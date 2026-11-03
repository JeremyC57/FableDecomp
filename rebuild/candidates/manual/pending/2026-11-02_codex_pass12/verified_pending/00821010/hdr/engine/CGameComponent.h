#ifndef FABLE_ENGINE_CGAMECOMPONENT_H
#define FABLE_ENGINE_CGAMECOMPONENT_H
// Original PDB bases at0/4, Quit+8, Running+9, Game+c; native size16.
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
FABLE_STATIC_ASSERT(sizeof(CGameComponent)==16);
#endif
