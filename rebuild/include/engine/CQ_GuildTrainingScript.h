#ifndef FABLE_ENGINE_CQ_GUILDTRAININGSCRIPT_H
#define FABLE_ENGINE_CQ_GUILDTRAININGSCRIPT_H
// Original Ego_r TPI110ab/110ca. Native CWillDummy factory00d43360/vtable012ce364.
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain(); virtual void Main(); virtual void Init(); virtual void OnPersist(CPersistContext&);
    long GameState;
    bool SleepNeeded;
    bool ReachedPlatform;
    bool DomeCutsceneStart;
    long HeroWarnings;
    long CurrentBirdsKilled;
    long StartedTesting;
    bool StartedMeleeTesting;
    CTimer WillHelpTimer;
    bool FightFinished;
    bool DisplayBirdKilledMessage;
    class CWillDummy : public CEntityScriptBase {
    public: virtual CScriptBase* GetParentScript() const;
    protected: CQ_GuildTrainingScript& ParentClass; CQ_SunnyvaleMasterData* PMasterData;
    private: virtual void Main(); virtual void Init(); virtual void OnPredicateFail();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingScript)==0x68);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingScript::CWillDummy)==0x1c);
#endif
