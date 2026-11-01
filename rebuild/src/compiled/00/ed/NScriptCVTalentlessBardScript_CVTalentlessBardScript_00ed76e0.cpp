// NScript::CV_TalentlessBardScript::CV_TalentlessBardScript(CScriptDataBase*, CGameScriptInterfaceBase&) -- construct bard script state, strings and timers
// Leaf: ??0CV_TalentlessBardScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Naming wave 16 (c673): the PDB parameter pair (CScriptDataBase*, CGameScriptInterfaceBase&) replaces the landed (CScriptDataBase*, CGameScriptInterface*); the user ~CV_TalentlessBardScript the landed model declared is
//   dropped (retail's 0x00EDAD10 is the implicit destructor, chunk c673);
//   name-only (both are one pushed pointer: the bytes are unchanged).
// Retail0x00ED76E0,164 bytes. Historical grade count uncertain: reserve5, leaving at most5 new grades.
// CScriptBase ctor00CB8110; CBaseClass ctor0099A2D0 (RTTI1231710); CScriptThing vtable1238C8C.
// Shared CScriptThing is12 bytes; this native constructor view preserves its PImp member.
// Donor script base84 bytes is64 in retail; SongItems16->12 and SongThingAvailable20->12 explain total260->228.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
class CScriptDataBase;
class CGameScriptInterfaceBase;
struct CGameScriptTimerMethods {
    void* BeforeRegisterTimer[87];
    long (__fastcall *RegisterTimer)(CGameScriptInterface*); // slot87,0x006E79E0
};
extern CGameScriptInterface* g_pGameScriptInterface; // 0x0143E8F8
struct CScriptBaseNative {
    virtual ~CScriptBaseNative();
    unsigned char RetailMembers[0x3c];
    CScriptBaseNative(); // 0x00CB8110
};
struct __declspec(novtable) CQuestScriptObjectNative : CScriptBaseNative {
    CGameScriptInterfaceBase* Game;                // Ego_r CLevelScriptBase<T>::Game
    CScriptDataBase* Data;
    CQuestScriptObjectNative(CScriptDataBase* data, CGameScriptInterfaceBase& game)
        : CScriptBaseNative(),Game(&game),Data(data) {}
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CScriptThingImplementationPointer {
    CScriptThing* Data;
    CCPPointerInfo* Info;
    CScriptThingImplementationPointer() : Data(0),Info(0) {}
};
struct CScriptThingConstructorView : CBaseClass {
    CScriptThingImplementationPointer PImp;
    virtual ~CScriptThingConstructorView(); // CScriptThing RTTI1238C8C slot0 destructor004AB170
    CScriptThingConstructorView() : CBaseClass(),PImp() {}
};
#ifndef FABLE_WIDE_STRING_HOST_TYPES
#include "fable_string.h"
#endif
struct CQuestInfoStrings { CCharString name; CCharString tag; };
struct CScriptTimer {
    long TimerIndex;
    CScriptTimer() : TimerIndex(((CGameScriptTimerMethods*)g_pGameScriptInterface->__vftable)->RegisterTimer(g_pGameScriptInterface)) {}
};
struct CScriptRetailArray {
    void* Start; void* Finish; void* End;
    CScriptRetailArray() : Start(0),Finish(0),End(0) {}
};
namespace NScript {
class CV_TalentlessBardScript : public CQuestScriptObjectNative {
public:
    bool bHurtBard,bBard_finished;
    unsigned char BeforeSongCount[2];
    unsigned long lNumber_of_things_sung_about;
    bool bRewardGiven;
    unsigned char BeforeBard[3];
    CScriptThingConstructorView stBard;
    bool BardIntroComplete,AttackedByHero,BardFollowing;
    unsigned char BeforeTimer;
    CScriptTimer BardWanderTime;
    CScriptRetailArray SongItems;
    CQuestInfoStrings QuestInfo[12];
    CScriptRetailArray SongThingAvailable;
    CScriptTimer RandomSpeechTimer;
    CV_TalentlessBardScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
} // namespace NScript
using NScript::CV_TalentlessBardScript;
NScript::CV_TalentlessBardScript::CV_TalentlessBardScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CQuestScriptObjectNative(data,game),stBard() {}