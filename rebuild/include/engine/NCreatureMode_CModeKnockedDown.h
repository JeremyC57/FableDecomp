#ifndef FABLE_ENGINE_MODE_KNOCKED_DOWN_H
#define FABLE_ENGINE_MODE_KNOCKED_DOWN_H
#ifdef FABLE_RETAIL_KNOCKDOWN_INITIALISE_PASS28
#include "engine/CTCCreatureModeManager.h"
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
// FableWin 0281c1d0, Ego_r class layout and native 0x28 allocation.
class CModeKnockedDown : public CTCCreatureModeManager::CModeBase {
public:
    CModeKnockedDown(CThing& owner,const CCharString& name): CModeBase(owner), Filter(name) {}
    virtual unsigned long GetType() const;
    virtual void OnActivate();
    virtual void OnDeactivate();
private:
    CAnimFilterOfGroup Filter;
};
}
#endif
#endif
