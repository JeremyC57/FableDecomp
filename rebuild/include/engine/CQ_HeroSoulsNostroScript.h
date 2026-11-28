#ifndef FABLE_ENGINE_CQ_HEROSOULSNOSTROSCRIPT_H
#define FABLE_ENGINE_CQ_HEROSOULSNOSTROSCRIPT_H
#include "engine/CLevelScriptBase.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HeroSoulsNostroScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_HeroSoulsNostroScript(CScriptDataBase*, CGameScriptInterfaceBase&);
    virtual ~CQ_HeroSoulsNostroScript();
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&);
    virtual void Init();
    virtual void RegisterMain();
    virtual void OnPersist(CPersistContext&);
    virtual void Main();
    void WatchForNostroHit();
    void DealWithCreatureGeneration();
    void PlayCutscene(CCharString);
    long SummonerCount;
    bool PlayedNostroCS;
    bool KickedOffQuestStartScreen;
    long UndeadAlive;
    CTimer NostroVulnerableTimer;
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HeroSoulsNostroScript) == 0x58);
FABLE_STATIC_ASSERT(offsetof(NScript::CQ_HeroSoulsNostroScript,KickedOffQuestStartScreen)==0x4d);
FABLE_STATIC_ASSERT(offsetof(NScript::CQ_HeroSoulsNostroScript,UndeadAlive)==0x50);
#endif
