// CV_FisticuffsClubScript::FisticuffsChallenge() -- with both fighters under an is-alive predicate, set them on each other, wait until either drops below the minimum fighter health (or the fight is otherwise called), then call them off and make them allies again
// Leaf: ?FisticuffsChallenge@CV_FisticuffsClubScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E86E60, 449 bytes (464 with padding), plain `ret`.
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90, CPredicate_And<CIsThingAlive, CIsThingAlive>(const A&, const B&)
//   0x00E24760, CScriptBase::SetPredicate 0x00CB7920, ~CIsThingAlive 0x00E241B0, CScriptBase::IsActiveThreadTerminating
//   0x00CB7940; through the interface (+0x40) NewScriptFrame (slot 7), GetHealth (slot 264), GiveThingBestEnemyTarget
//   (slot 451), ClearThingBestEnemyTarget (slot 452), EntitySetThingAsAllyOfThing (slot 599),
//   EntityUnsetThingAsAllyOfThing (slot 600); the script def global 0x0143E90C.
// Shape: `SetPredicate(CIsThingAlive(a) && CIsThingAlive(b))` -- the inline operator&& builds the CPredicate_And in place
//   (operands built right to left), whose user-written empty destructor stores both vtables (0x012E1B70, base 0x012C2FF4).
//   The script-def global is read into a named local ahead of each GetHealth call (retail keeps it in ebx).
// Layout (retail = Ego_r.pdb - 0x14): FightFinished +0x4f, HeroPaidEntryFee +0x52, SFighterOne +0x64, SFighterTwo +0x70.
//   CScriptDef: FC_MinimumFighterHealth at retail +0x5e0 (PDB +0x63c; the retail def is smaller ahead of it).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

struct CScriptThingImpl;
class CScriptThing : public CBaseClass { public:                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    ~CScriptThing();                                        // 0x004AA840
};
namespace NScript {
class CPredicateBase { public:                                     // vtable 0x012C2FF4
    virtual bool IsTrue() = 0;
    ~CPredicateBase() { }
};
class CIsThingAlive : public CPredicateBase { public:                     // vtable 0x012C32F0, 16 bytes
    CScriptThing Thing;                                     // +0x04
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
    virtual ~CIsThingAlive();                                       // 0x00E241B0
    virtual bool IsTrue();
};
template <class A, class B> class CPredicate_And : public CPredicateBase { public:   // vtable 0x012E1B70 for <CIsThingAlive, CIsThingAlive>
    A First;                                                // +0x04
    B Second;                                               // +0x14
    CPredicate_And(const A& first, const B& second);        // 0x00E24760
    ~CPredicate_And() { }
    virtual bool IsTrue();
};
template <class A, class B> inline CPredicate_And<A, B> operator&&(const A& first, const B& second)
{
    return CPredicate_And<A, B>(first, second);
}
}
using namespace NScript;
class CScriptDef { public:
    unsigned char _pad[0x5e0];
    long          FC_MinimumFighterHealth;                  // +0x5e0
};
extern CScriptDef* pGlobalScriptDef;                            // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptBase { public:                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    void SetPredicate(const NScript::CPredicateBase& predicate);   // 0x00CB7920
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
namespace NScript {
class CV_FisticuffsClubScript : public CQuestScriptBase {
public:
    long         RelevantRegion;                            // +0x48
    bool         FighterOneInRing;                          // +0x4c
    bool         FighterTwoInRing;                          // +0x4d
    bool         ChallengeAccepted;                         // +0x4e
    bool         FightFinished;                             // +0x4f
    bool         HeroInsideRing;                            // +0x50
    bool         HeroOfferedFisticuffsClubEntry;            // +0x51
    bool         HeroPaidEntryFee;                          // +0x52
    bool         HeroInFisticuffsClub;                      // +0x53
    bool         TylerInFight;                              // +0x54
    bool         HeroBeatTyler;                             // +0x55
    bool         TimeOut;                                   // +0x56
    bool         RingCleared;                               // +0x57
    bool         RoundStarted;                              // +0x58
    long         Fighter1Timer;                             // +0x5c CTimer
    long         Fighter2Timer;                             // +0x60 CTimer
    CScriptThing SFighterOne;                               // +0x64
    CScriptThing SFighterTwo;                               // +0x70
    void FisticuffsChallenge();
};
} // namespace NScript
using NScript::CV_FisticuffsClubScript;

void NScript::CV_FisticuffsClubScript::FisticuffsChallenge()
{
    SetPredicate(CIsThingAlive(SFighterOne) && CIsThingAlive(SFighterTwo));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(SFighterOne, SFighterTwo);
    Game->GiveThingBestEnemyTarget(SFighterTwo, SFighterOne);
    Game->EntityUnsetThingAsAllyOfThing(SFighterOne, SFighterTwo);
    Game->EntityUnsetThingAsAllyOfThing(SFighterTwo, SFighterOne);
    while (!FightFinished) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        CScriptDef* def = pGlobalScriptDef;
        if (Game->GetHealth(SFighterOne) < def->FC_MinimumFighterHealth) {
            if (IsActiveThreadTerminating())
                return;
            FightFinished = true;
        }
        def = pGlobalScriptDef;
        if (Game->GetHealth(SFighterTwo) < def->FC_MinimumFighterHealth) {
            if (IsActiveThreadTerminating())
                return;
            FightFinished = true;
        }
        if (HeroPaidEntryFee) {
            if (IsActiveThreadTerminating())
                return;
            FightFinished = true;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->ClearThingBestEnemyTarget(SFighterOne);
    Game->ClearThingBestEnemyTarget(SFighterTwo);
    Game->EntitySetThingAsAllyOfThing(SFighterOne, SFighterTwo);
    Game->EntitySetThingAsAllyOfThing(SFighterTwo, SFighterOne);
}