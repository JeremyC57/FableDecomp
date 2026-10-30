// CBRIAR_ROSE_IMPOSTER::Main() -- a Briar Rose imposter in the Hero Souls quest: alive filter on Me, reset the two taunt timers, then every frame: when the hero hits it (or hits it with any special ability but number 14) and the taunt timer has run out, run a one-line taunt conversation and restart the timers; when the "here" timer has run out, the taunt timer is above 7 and the hero is over 13 units away, say "here" and restart that timer
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D81820, length 822 bytes; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy @ 0x004ABE90 / dtor @ 0x004AA840, SetPredicate @ 0x00F35B10, IsDone @ 0x00F35B30,
//          CScriptThing slots 21 MsgIsHitBy (+0x54), 41 MsgIsHitBySpecialAbilityFrom (+0xa4), 42
//          MsgIsHitByAnySpecialAbilityFrom (+0xa8); CGameScriptInterface slots 7, 70 GetHero, 89 SetTimer, 90 GetTimer,
//          364/365/366 AddNewConversation / AddPersonToConversation / AddLineToConversation (Game +4 and the timer
//          interface global 0x0143E8F8); IsDistanceBetweenThingsOver @ 0x00CBE3EA; CCharString @ 0x0099EBF0 / 0x0099EAE0.
//          Strings "SCRIPT_NAME_HERO" 0x0125D1C8, "TEXT_QST_B04_BRIAR_COPY_TAUNT" 0x012D4350,
//          "TEXT_QST_B04_BRIAR_COPY_HERE" 0x012D4330.
// Layout: Ego_r.pdb CBRIAR_ROSE_IMPOSTER (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//         (CQ_HeroSoulsBriarScript: BriarRoseImposterTauntTimer PDB +0x80 / retail +0x68, BriarRoseImposterHereTimer
//         PDB +0x84 / retail +0x6c), PMasterData +0x18.
// Shape: the hit test is one bool whose CCharString temporaries are destroyed under the flag word (spilled to the
//         frame); the timers are CTimer members with inline Get/Set through the interface global.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator+=(const CCharString& other); // 0x0099F0A0
};
CCharString __fastcall GFIntToCharString(long value);  // 0x0099F830

struct CScriptThingImpl;
class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void v00();
    CScriptThingImpl* PImp_Data;                       // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    ~CScriptThing();                                   // 0x004AA840
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20();
    virtual bool MsgIsHitBy(const CCharString& name) const;                                  // slot 21 (+0x54)
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40();
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& name) const; // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;            // slot 42 (+0xa8)
    bool IsAlive() const;                              // slot 75, inlined below
};
struct CScriptThingImpl {
    virtual void s00();
    virtual void s01();
    virtual void s02();
    virtual void s03();
    virtual void s04();
    virtual void s05();
    virtual void s06();
    virtual void s07();
    virtual void s08();
    virtual void s09();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53();
    virtual void s54();
    virtual void s55();
    virtual void s56();
    virtual void s57();
    virtual void s58();
    virtual void s59();
    virtual void s60();
    virtual void s61();
    virtual void s62();
    virtual void s63();
    virtual void s64();
    virtual void s65();
    virtual void s66();
    virtual void s67();
    virtual void s68();
    virtual void s69();
    virtual void s70();
    virtual void s71();
    virtual void s72();
    virtual void s73();
    virtual void s74();
    virtual bool IsAlive() const;                      // slot 75 (+0x12c)
};
inline bool CScriptThing::IsAlive() const { return PImp_Data && PImp_Data->IsAlive(); }

bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CCPPointerInfo { public:                         // CCountedPointer's shared block (Ego_r.pdb CCPPointerInfo)
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4 destroys the pointee
    void* Data;                                        // +8
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
template <class T> class CCountedPointer { public:     // retail CCountedPointer<T>: Data +0, Info +4; the release
    T*              Data;                              // dtor is 0x007E74D0's inline PImp release (wave 6 GRO rule:
    CCPPointerInfo* Info;                              // VC7.1's per-site inlining of the implicit STB dtor follows
    CCountedPointer() : Data(0), Info(0) {}            // from this size)
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
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;

class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};




extern CGameScriptInterface* g_pGameScriptInterface;          // 0x0143E8F8
class CTimer { public:
    long Index;
    long GetTime() const { return g_pGameScriptInterface->GetTimer(Index); }
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
};
struct CQ_HeroSoulsBriarScript {
    unsigned char _pad[0x68];
    CTimer BriarRoseImposterTauntTimer;                // +0x68
    CTimer BriarRoseImposterHereTimer;                 // +0x6c
};

struct CBRIAR_ROSE_IMPOSTER : CEntityScriptBase {
    CQ_HeroSoulsBriarScript* ParentClass;              // +0x14
    void* PMasterData;                                 // +0x18
    void Main();
};

void CBRIAR_ROSE_IMPOSTER::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    ParentClass->BriarRoseImposterTauntTimer.SetTime(0);
    ParentClass->BriarRoseImposterHereTimer.SetTime(4);
    while (!IsActiveThreadTerminating()) {
        bool hit = (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                    || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                        && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO")))
                   && ParentClass->BriarRoseImposterTauntTimer.GetTime() == 0;
        if (hit) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_B04_BRIAR_COPY_TAUNT", false, Me, Game->GetHero());
            ParentClass->BriarRoseImposterTauntTimer.SetTime(15);
            ParentClass->BriarRoseImposterHereTimer.SetTime(4);
        }
        if (ParentClass->BriarRoseImposterHereTimer.GetTime() == 0
            && ParentClass->BriarRoseImposterTauntTimer.GetTime() > 7
            && IsDistanceBetweenThingsOver(Me, Game->GetHero(), 13.0f)) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_B04_BRIAR_COPY_HERE", false, Me, Game->GetHero());
            ParentClass->BriarRoseImposterHereTimer.SetTime(15);
        }
        Game->NewScriptFrame();
    }
}