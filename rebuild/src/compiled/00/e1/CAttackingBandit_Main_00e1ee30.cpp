// CAttackingBandit::Main() -- an escort-quest ambush bandit: alive filter on Me, take scripted control (priority 3, retried each frame), find the trader to escort; while it lives make Me hostile to the hero and the trader and follow the trader; the first time, wait for the hero within 10 units and laugh (SND_MAN_05_NONSPEECH_AMBUSHLAUGH_0<n>) then stop following the hero; keep running script frames while the trader is alive
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E1EE30, length 656 bytes; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy @ 0x004ABE90 / dtor @ 0x004AA840, SetPredicate @ 0x00F35B10, IsDone @ 0x00F35B30,
//          CBaseObject ctor @ 0x0099A380, the scripted-thing handle's IsValid @ 0x00CD23B9 / Release @ 0x00CD2770 /
//          FollowThing @ 0x007E7320 / StopFollowingThing @ 0x007E7330 / destructor @ 0x007E74D0,
//          IsDistanceBetweenThingsUnder @ 0x00CBE2FF, CCharString @ 0x0099EBF0 / 0x0099EAE0 / operator+= @ 0x0099F0A0,
//          GFIntToCharString @ 0x0099F830; CGameScriptInterface slots 7, 8, 70 GetHero, 72 GetThingWithScriptName,
//          597 EntitySetThingAsEnemyOfThing, 690 PlaySoundOnThing. Strings "TraderToEscort" 0x012E14E4,
//          "SND_MAN_05_NONSPEECH_AMBUSHLAUGH_0" 0x012E1570.
// Layout: Ego_r.pdb CAttackingBandit (32 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//         (CQR_EscortTraderScript; its laugh index at +0x54), PMasterData +0x18, Screamed +0x1c. Model: the landed
//         CChasingScreamer::Main 0x00E1AF30 (inline alive-filter destructor here).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator+=(const CCharString& other); // 0x0099F0A0
};
CCharString __fastcall GFIntToCharString(long value);  // 0x0099F830

struct CScriptThingImpl;
class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void v00();
    CScriptThingImpl* PImp_Data;                       // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
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

bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

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



struct CQR_EscortTraderScript {
    unsigned char _pad[0x54];
    long LaughIndex;                                   // +0x54
};

struct CAttackingBandit : CEntityScriptBase {
    CQR_EscortTraderScript* ParentClass;               // +0x14
    void* PMasterData;                                 // +0x18
    bool  Screamed;                                    // +0x1c
    void Main();
};

void CAttackingBandit::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    if (scripted.IsValid())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)3)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing trader = Game->GetThingWithScriptName("TraderToEscort");
    if (trader.IsAlive()) {
        Game->EntitySetThingAsEnemyOfThing(Me, Game->GetHero());
        Game->EntitySetThingAsEnemyOfThing(Me, trader);
        scripted.FollowThing(trader, 1.0f, true);
        if (!Screamed) {
            if (IsActiveThreadTerminating())
                return;
            while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 10.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            CCharString sound("SND_MAN_05_NONSPEECH_AMBUSHLAUGH_0");
            sound += GFIntToCharString(ParentClass->LaughIndex);
            Game->PlaySoundOnThing(Me, sound);
            Screamed = true;
            scripted.StopFollowingThing(Game->GetHero());
        }
        while (trader.IsAlive()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}