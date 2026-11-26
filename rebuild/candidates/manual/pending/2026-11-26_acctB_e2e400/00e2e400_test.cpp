// NScript::CV_AmbushScamScript::ResetRandomSpeechTime() -- Set the speech timer to maximum minus a random variation.
// Leaf: ?ResetRandomSpeechTime@CV_AmbushScamScript@NScript@@QAEXXZ
// Identity: Ego_r.pdb 0x00E77A70 and FableWin.pdb 0x0392E420, both
//   ?ResetRandomSpeechTime@CV_AmbushScamScript@NScript@@QAEXXZ. Retail Fable.exe 0x00E2E400 is 57 bytes
//   (push esi/edi through ret; int3 at 0x00E2E439). pGlobalScriptDef is 0x0143E90C, pDSTGame is 0x0143E8F8,
//   MSVCR71 rand thunk is 0x00BFEB16, SetTimer is vtable slot 89 (call [edx+164h]).
//   Timer index is [esi+7Ch]. Def Max/Var are AmbushScamRandomSpeechTimerMax/Var at +2B8h/+2BCh
//   (CScriptDef.h). The 2026-10-28 draft is unchanged; that spelling loaded the vtable into ebx.
#include "fable_compiler.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
typedef unsigned long RetailUID;
#else
typedef unsigned __int64 RetailUID;
#endif
#include "engine/CBaseClass.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
class CEntityScriptBindingBase; class CActiveEntityScriptBase; class CSpawnedFuncBase;
#include "engine/CArray.h"
#include "engine/CVectorMap.h"
template<class V> class CCompareString { public: bool operator()(const std::pair<CCharString,V>& a, const std::pair<CCharString,V>& b) const { return a.first.FastCompare(b.first); } };
class CPersistContext;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CScriptGameResourceObjectBase.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace NScript { class CRuntimeScriptedEntityPersist { public: class CPersistInfo; CVectorMap<RetailUID, CPersistInfo> PersistScripts; }; class CQ_SunnyvaleMasterData; }
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_def_global.h"
class CGameScriptInterfaceBase : public CBaseClassNonCopyable {
public:
    virtual void UnknownSlot1() {} virtual void UnknownSlot2() {} virtual void UnknownSlot3() {} virtual void UnknownSlot4() {}
    virtual void UnknownSlot5() {} virtual void UnknownSlot6() {} virtual void UnknownSlot7() {} virtual void UnknownSlot8() {}
    virtual void UnknownSlot9() {} virtual void UnknownSlot10() {} virtual void UnknownSlot11() {} virtual void UnknownSlot12() {}
    virtual void UnknownSlot13() {} virtual void UnknownSlot14() {} virtual void UnknownSlot15() {} virtual void UnknownSlot16() {}
    virtual void UnknownSlot17() {} virtual void UnknownSlot18() {} virtual void UnknownSlot19() {} virtual void UnknownSlot20() {}
    virtual void UnknownSlot21() {} virtual void UnknownSlot22() {} virtual void UnknownSlot23() {} virtual void UnknownSlot24() {}
    virtual void UnknownSlot25() {} virtual void UnknownSlot26() {} virtual void UnknownSlot27() {} virtual void UnknownSlot28() {}
    virtual void UnknownSlot29() {} virtual void UnknownSlot30() {} virtual void UnknownSlot31() {} virtual void UnknownSlot32() {}
    virtual void UnknownSlot33() {} virtual void UnknownSlot34() {} virtual void UnknownSlot35() {} virtual void UnknownSlot36() {}
    virtual void UnknownSlot37() {} virtual void UnknownSlot38() {} virtual void UnknownSlot39() {} virtual void UnknownSlot40() {}
    virtual void UnknownSlot41() {} virtual void UnknownSlot42() {} virtual void UnknownSlot43() {} virtual void UnknownSlot44() {}
    virtual void UnknownSlot45() {} virtual void UnknownSlot46() {} virtual void UnknownSlot47() {} virtual void UnknownSlot48() {}
    virtual void UnknownSlot49() {} virtual void UnknownSlot50() {} virtual void UnknownSlot51() {} virtual void UnknownSlot52() {}
    virtual void UnknownSlot53() {} virtual void UnknownSlot54() {} virtual void UnknownSlot55() {} virtual void UnknownSlot56() {}
    virtual void UnknownSlot57() {} virtual void UnknownSlot58() {} virtual void UnknownSlot59() {} virtual void UnknownSlot60() {}
    virtual void UnknownSlot61() {} virtual void UnknownSlot62() {} virtual void UnknownSlot63() {} virtual void UnknownSlot64() {}
    virtual void UnknownSlot65() {} virtual void UnknownSlot66() {} virtual void UnknownSlot67() {} virtual void UnknownSlot68() {}
    virtual void UnknownSlot69() {} virtual void UnknownSlot70() {} virtual void UnknownSlot71() {} virtual void UnknownSlot72() {}
    virtual void UnknownSlot73() {} virtual void UnknownSlot74() {} virtual void UnknownSlot75() {} virtual void UnknownSlot76() {}
    virtual void UnknownSlot77() {} virtual void UnknownSlot78() {} virtual void UnknownSlot79() {} virtual void UnknownSlot80() {}
    virtual void UnknownSlot81() {} virtual void UnknownSlot82() {} virtual void UnknownSlot83() {} virtual void UnknownSlot84() {}
    virtual void UnknownSlot85() {} virtual void UnknownSlot86() {} virtual void UnknownSlot87() {} virtual void UnknownSlot88() {}
    virtual void SetTimer(long, long) = 0;
};
extern CGameScriptInterfaceBase* pDSTGame;
class CTimer {
public:
    long TimerIndex;
    FABLE_FORCEINLINE CTimer& operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
};
namespace NScript {
class CV_AmbushScamScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool QuestFinished;
    bool QuestComplete;
    bool QuestActivated;
    bool LeftRegion;
    bool MoralitySwitch;
    bool BuddyIntroComplete;
    bool DoTricksterIntro;
    bool DoAmbush;
    bool TricksterApproachComplete;
    bool TricksterSpeechComplete;
    bool TricksterRetreatComplete;
    bool TricksterQuestComplete;
    bool TricksterWhiningComplete;
    bool TricksterDeadOrLeft;
    bool TricksterComplete;
    bool TricksterKilledBeforeAmbush;
    bool TricksterKilledAfterAmbush;
    bool TricksterKilledAfterBeingSpared;
    bool TricksterResetAllegience;
    bool TricksterWaitHere;
    float TricksterApproachProximity;
    bool AmbushSpringComplete;
    bool AllAmbushersDead;
    bool ExpressionDetectedFollow;
    bool ExpressionDetectedWait;
    bool HeroAttackOnBuddy;
    bool HeroAttackOnTricksterBeforeAmbush;
    bool HeroAttackOnTricksterAfterAmbush;
    bool HeroAttackOnTricksterKillingBlow;
    bool HeroTalkToTrickster;
    bool HeroAttachOnTricksterWarning;
    bool BuddyHelped;
    bool SpeakTime1;
    bool SpeakTime2;
    bool AllDone;
    bool BanditSpoken;
    bool AllBanditsDead;
    bool LeaderDead;
    long HitCount;
    long BuddyCounter;
    CTimer RandomSpeechTimer;
    void ResetRandomSpeechTime();
};
}
#ifdef _MSC_VER
typedef char ExactTimerOffset[offsetof(NScript::CV_AmbushScamScript, RandomSpeechTimer) == 0x7c ? 1 : -1];
typedef char ExactDefOffset[offsetof(CScriptDef, AmbushScamRandomSpeechTimerMax) == 0x2b8 ? 1 : -1];
#endif

CDefPointer<const CScriptDef> pGlobalScriptDef;
CGameScriptInterfaceBase* pDSTGame;
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
class MockGame : public CGameScriptInterfaceBase {
public:
    long index;
    long value;
    int calls;
    virtual void SetTimer(long timer, long time) { index = timer; value = time; ++calls; }
};
static void set_def(unsigned char* buf, long maximum, long variation)
{
    memset(buf, 0, 0x2c0);
    *reinterpret_cast<unsigned long*>(buf + 4) = 8;
    *reinterpret_cast<long*>(buf + 0x2b8) = maximum;
    *reinterpret_cast<long*>(buf + 0x2bc) = variation;
}
int main()
{
    unsigned char buf[0x2c0];
    unsigned char storage[sizeof(NScript::CV_AmbushScamScript)];
    memset(storage, 0, sizeof storage);
    NScript::CV_AmbushScamScript* script = reinterpret_cast<NScript::CV_AmbushScamScript*>(storage);
    MockGame game;
    pDSTGame = &game;
    script->RandomSpeechTimer.TimerIndex = 73;
    bool ok = true;
    long maxima[] = {100, 200};
    long variations[] = {7, 3};
    int seeds[] = {1, 99};
    for (int i = 0; i < 2; ++i) {
        set_def(buf, maxima[i], variations[i]);
        pGlobalScriptDef.Object = reinterpret_cast<CDefPointeeBase*>(buf);
        srand(seeds[i]);
        int random_value = rand();
        srand(seeds[i]);
        game.calls = 0;
        script->ResetRandomSpeechTime();
        long expected = maxima[i] - random_value % variations[i];
        ok = ok && game.calls == 1 && game.index == 73 && game.value == expected;
        ok = ok && script->RandomSpeechTimer.TimerIndex == 73;
        pGlobalScriptDef.Object = 0;
    }
    puts(ok ? "00e2e400_TEST PASS" : "00e2e400_TEST FAIL");
    return ok ? 0 : 1;
}
