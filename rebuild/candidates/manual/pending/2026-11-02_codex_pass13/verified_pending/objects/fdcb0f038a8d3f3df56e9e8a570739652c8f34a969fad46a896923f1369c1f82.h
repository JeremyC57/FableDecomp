#ifndef FABLE_ENGINE_CQ_HOBBETOOTHCONTESTSCRIPT_H
#define FABLE_ENGINE_CQ_HOBBETOOTHCONTESTSCRIPT_H
// Original Ego_r TPI11cc8; native factory00d9e190 allocates0x68 and constructs timer+0x60.
// Layout: complete original data; native script base0x48 is0x14 smaller than donor.
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HobbeToothContestScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    long NumberOfHobbesInTotal; // +0x48
    long NumberOfHobbeTeethCollectedByHero; // +0x4c
    long NumberOfHobbeTeethCollectedByWhisper; // +0x50
    long NumberOfHobbeTeethNeeded; // +0x54
    bool CS_IntroDone; // +0x58
    bool CS_OutroBegin; // +0x59
    bool DrinkingHobbesNotHit; // +0x5a
    bool MissionSucceeded; // +0x5b
    bool MissionFailed; // +0x5c
    bool WhisperOrFarmerHit; // +0x5d
    CTimer WhisperCommentTimer; // +0x60
    bool QuestStartScreenShown; // +0x64
    class CQ_HobbeToothWhisper : public CEntityScriptBase {
    public:
        virtual CScriptBase* GetParentScript() const;
    protected:
        NScript::CQ_HobbeToothContestScript & ParentClass; // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData; // +0x18
    private:
        C3DVector initial_position; // +0x1c
    private:
        virtual void Init();
    private:
        virtual void Main();
    };
    class CSpawnNewHobbe : public CEntityScriptBase {
    public:
        virtual CScriptBase* GetParentScript() const;
    protected:
        NScript::CQ_HobbeToothContestScript & ParentClass; // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData; // +0x18
    private:
        virtual void Init();
    private:
        virtual void Main();
    };
    class COrchardFarmFarmer : public CEntityScriptBase {
    public:
        virtual CScriptBase* GetParentScript() const;
    protected:
        NScript::CQ_HobbeToothContestScript & ParentClass; // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData; // +0x18
    private:
        virtual void Init();
    private:
        virtual void Main();
    };
    class CHobbeBarrel : public CEntityScriptBase {
    public:
        virtual CScriptBase* GetParentScript() const;
    protected:
        NScript::CQ_HobbeToothContestScript & ParentClass; // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData; // +0x18
    private:
        virtual void Init();
    private:
        virtual void Main();
    };
    class CDrinkersReactMarker : public CEntityScriptBase {
    public:
        virtual CScriptBase* GetParentScript() const;
    protected:
        NScript::CQ_HobbeToothContestScript & ParentClass; // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData; // +0x18
    private:
        virtual void Init();
    private:
        virtual void Main();
    };
    class CDrinkingHobbe : public CEntityScriptBase {
    public:
        virtual CScriptBase* GetParentScript() const;
    protected:
        NScript::CQ_HobbeToothContestScript & ParentClass; // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData; // +0x18
    private:
        virtual void Init();
    private:
        virtual void OnPredicateFail();
    private:
        virtual void Main();
    };
    class CQ_HobbeToothHobbe : public CEntityScriptBase {
    public:
        virtual CScriptBase* GetParentScript() const;
    protected:
        NScript::CQ_HobbeToothContestScript & ParentClass; // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData; // +0x18
    private:
        virtual void Init();
    private:
        virtual void OnPredicateFail();
    private:
        virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HobbeToothContestScript) == 0x68);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HobbeToothContestScript::CSpawnNewHobbe) == 0x1c);
#endif
