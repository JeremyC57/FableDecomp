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
#ifdef FABLE_RETAIL_NCREATUREMODE_ALLOC
    // Opt-in (row 0x0073B130 CTCCreatureModeManager::RegisterModes): FableWin.pdb ?Alloc@CModeBriarRoseConcentrate@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z.
    static CTCCreatureModeManager::CModeBase* __fastcall Alloc(CThing& owner);
#endif
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
