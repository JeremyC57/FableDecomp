#ifndef FABLE_ENGINE_NCREATUREMODE_CMODEDRAGONHOVER_H
#define FABLE_ENGINE_NCREATUREMODE_CMODEDRAGONHOVER_H
#ifdef FABLE_RETAIL_JACKDRAGON_HOVER_PASS27
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CIntelligentPointer.h"
namespace NCreatureMode {
// Original RTTI derives from CModeStandBase. FableWin 02864F80 builds Filter and PTarget in sequence.
class CModeDragonHover : public CModeStandBase {
public:
    CModeDragonHover(CThing& parent_thing,CThing& target);
    virtual unsigned long GetType() const;
private:
    CAnimFilterOfGroup Filter;
    CIntelligentPointer<CThing> PTarget;
    bool UseHeadTurning;
};
FABLE_STATIC_ASSERT(sizeof(CModeDragonHover)==0x80);
}
#endif
#endif
