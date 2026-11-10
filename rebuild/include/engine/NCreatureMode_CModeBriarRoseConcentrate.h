#ifndef FABLE_ENGINE_NCREATUREMODE_CMODEBRIARROSECONCENTRATE_H
#define FABLE_ENGINE_NCREATUREMODE_CMODEBRIARROSECONCENTRATE_H
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
class CThingCreatureBase;
class CBriarRoseDef;
namespace NCreatureMode {
class CModeBriarRoseConcentrate:public CModeStandBase {
public:
 virtual unsigned long GetType()const;
 virtual const CAnimFilterBase& GetAnimFilter()const;
 virtual void OnActivate();
 virtual void FrameUpdate();
 virtual void OnDeactivate();
 virtual bool IsRemoveOnDeactivate()const;
 virtual ~CModeBriarRoseConcentrate();
 CThing* GetRandomImposter()const;
private:
 CAnimFilterOfGroup Filter;
 long Timer;
 CIntelligentPointer<CThingCreatureBase> PTarget;
 std::vector<CIntelligentPointer<CThing> > Imposters;
 bool Imposter;
 CDefPointer<const CBriarRoseDef> PDef;
 CIntelligentPointer<CThing> PBodyEffect,PSwordEffect;
};
}
FABLE_STATIC_ASSERT(sizeof(NCreatureMode::CModeBriarRoseConcentrate)==164);
#endif
