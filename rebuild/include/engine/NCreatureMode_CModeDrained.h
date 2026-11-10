#ifndef FABLE_ENGINE_NCREATUREMODE_CMODEDRAINED_H
#define FABLE_ENGINE_NCREATUREMODE_CMODEDRAINED_H
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CIntelligentPointer.h"
namespace NCreatureMode {
class CModeDrained:public CModeStandBase {
public:
 virtual ~CModeDrained();
 virtual unsigned long GetType()const;
 virtual const CAnimFilterBase& GetAnimFilter()const;
 virtual void OnActivate();
 virtual void FrameUpdate();
 virtual void OnDeactivate();
 virtual bool IsAllowingPlayerSpecialMovementMoves()const;
private:
 CAnimFilterOfGroup Filter;
 CIntelligentPointer<CThing> PDrainer;
 unsigned long DrainerMode;
};
}
FABLE_STATIC_ASSERT(sizeof(NCreatureMode::CModeDrained)==128);
#endif
