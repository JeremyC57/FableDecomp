// CAttackingAssassin::Main() -- an attacking assassin: alive filter on Me, take scripted control (priority 3, retried each frame), make Me the hero's enemy and follow the hero; the first time, wait until the hero is within the quest's shout distance, shout a random assassin taunt and stop following; then idle until the script is done
// Leaf: ?Main@CAttackingAssassin@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E383A0, length 541 bytes; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy @ 0x004ABE90, ~CIsThingAlive @ 0x00E241B0 (out of line here), SetPredicate @ 0x00F35B10,
//          IsDone @ 0x00F35B30, CBaseObject ctor @ 0x0099A380, the scripted-thing handle's IsValid @ 0x00CD23B9 /
//          Release @ 0x00CD2770 / FollowThing @ 0x007E7320 / StopFollowingThing @ 0x007E7330 / destructor @ 0x007E74D0,
//          IsDistanceBetweenThingsUnder @ 0x00CBE2FF, rand @ 0x00BFEB16, CDefString::operator CCharString @ 0x00415D70,
//          ~CCharString @ 0x0099EAE0; CGameScriptInterface slots 7, 8, 70 GetHero, 597 EntitySetThingAsEnemyOfThing,
//          690 PlaySoundOnThing. Quest globals 0x0143E90C: shout distance +0x3a0 (long), taunt vector +0x3ac/+0x3b0.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8} + ParentClass +0x14, PMasterData +0x18, a "taunted" flag +0x1c; the
//         model is the landed CChasingScreamer::Main 0x00E1AF30.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void v00();
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};

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
    virtual ~CIsThingAlive();                                  // 0x00E241B0 (out of line here)
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


class CDefString { public:                                    // 4 bytes: a def-side string
    void* PStringData;
    operator CCharString() const;                      // 0x00415D70
};
extern "C" int __cdecl rand();                         // 0x00BFEB16 (MSVCR71)
class CScriptDef { public:
    unsigned char _pad[0x3a0];
    long AssassinShoutDistance;                        // +0x3a0
    unsigned char _pad3a4[0x8];
    CDefString* AssassinTauntsBegin;                   // +0x3ac vector<CDefString> (retail STLport, 12 bytes)
    CDefString* AssassinTauntsEnd;                     // +0x3b0
    CDefString* AssassinTauntsEndOfStorage;            // +0x3b4
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

struct CAttackingAssassin : CEntityScriptBase {
    void* ParentClass;                                 // +0x14
    void* PMasterData;                                 // +0x18
    bool  HasTaunted;                                  // +0x1c
    void Main();
};

void CAttackingAssassin::Main()
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
    Game->EntitySetThingAsEnemyOfThing(Me, Game->GetHero());
    scripted.FollowThing(Game->GetHero(), 1.0f, true);
    if (!HasTaunted) {
        if (IsActiveThreadTerminating())
            return;
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)pGlobalScriptDef->AssassinShoutDistance)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        unsigned long count = pGlobalScriptDef->AssassinTauntsEnd - pGlobalScriptDef->AssassinTauntsBegin;
        unsigned long index = rand() % count;
        Game->PlaySoundOnThing(Me, pGlobalScriptDef->AssassinTauntsBegin[index]);
        HasTaunted = true;
        scripted.StopFollowingThing(Game->GetHero());
    }
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}