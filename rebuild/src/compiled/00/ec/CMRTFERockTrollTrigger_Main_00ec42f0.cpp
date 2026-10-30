// CM_RTFERockTrollTrigger::Main() -- rock-troll trigger marker: alive filter on Me; unless the quest already spawned the troll, wait until the hero comes within the script def's trigger distance, then create the standing rock troll ("RTFE_RockTroll") at the "M_RTFERockTrollSpawnPos" marker, flag it spawned and remove this trigger
// Leaf: ?Main@CM_RTFERockTrollTrigger@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EC42F0, length 412 bytes (+ int3 padding).
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 / ~CScriptThing @ 0x004AA840; the CIsThingAlive filter (vtable 0x012C32F0,
//   destructor 0x00E241B0) registered @ 0x00F35B10; IsDone @ 0x00F35B30; IsDistanceBetweenThingsUnder @ 0x00CBE2FF (`__fastcall`);
//   CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("M_RTFERockTrollSpawnPos" 0x012F16AC, "RTFE_RockTroll"
//   0x012F1644, "CREATURE_ROCK_TROLL_START_STANDING" 0x012F1688); CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero,
//   72 GetThingWithScriptName, 91 CreateCreature, 108 RemoveThing. Script def @ 0x0143E90C (trigger distance +0x72c); zero vector
//   @ 0x0143E8E0 (the inline CScriptThing::GetPos).
// Layout: the entity-script base {vptr, Game +4, Me +8} with the quest object at +0x14 (spawned flag +0x4e), as the landed
//   CUndeadSoldier::Main 0x00D88F90 and CHTG_AmbushPointCond::Main 0x00D73970.
// Shape: the scoped filter object of the landed entity-script Mains; the creature's two names are implicit conversion temporaries; the in-loop
//   IsDone exit jumps to one shared `done:` return at the end, which moves the destroy-spawnPos exit block below the inline GetPos
//   ternary as in retail (a plain in-loop `return` keeps it inline after the post-loop IsDone test).
//   built before the position argument; the CreateCreature result is a discarded temporary (it reuses the filter's slot).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector { public: float x, y, z; };
extern C3DVector g_ZeroVector;                     // 0x0143E8E0
class CCPPointerInfo;
class CScriptThing { public:                              // vtable 0x01238C8C, 12 bytes
    virtual void v00() const; virtual void v01() const; virtual void v02() const; virtual void v03() const;
    virtual void v04() const; virtual void v05() const;
    virtual __forceinline const C3DVector& GetPos() const   // slot 6 (+0x18), 0x004AA980
    {
        if (!PImp_Data)
            return g_ZeroVector;
        return PImp_Data->GetPos();
    }
    CScriptThing*   PImp_Data;                     // +0x04
    CCPPointerInfo* PImp_Info;                     // +0x08
    CScriptThing(const CScriptThing& other);       // 0x004ABE90
    virtual ~CScriptThing();                               // 0x004AA840
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
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
class CIsThingAlive : public NScript::CPredicateBase { public:                             // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                          // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive();                              // 0x00E241B0 (out of line in this module)
};
}
using NScript::CIsThingAlive;
class CScriptDef { public:
    unsigned char _pad[0x72c];
    float         RockTrollTriggerDistance;        // +0x72c (descriptive)
};
extern CScriptDef* pGlobalScriptDef;                   // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CQ_RTFEScript {
    unsigned char _pad[0x4e];
    bool          RockTrollSpawned;                // +0x4e (descriptive)
};

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CM_RTFERockTrollTrigger : public CEntityScriptBase {
public:
    void*                 __vftable;               // +0x00
    CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    CQ_RTFEScript*        ParentClass;             // +0x14
    void Main();
};

void CM_RTFERockTrollTrigger::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (ParentClass->RockTrollSpawned)
        return;
    if (IsActiveThreadTerminating())
        return;
    CScriptThing spawnPos = Game->GetThingWithScriptName("M_RTFERockTrollSpawnPos");
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, pGlobalScriptDef->RockTrollTriggerDistance)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            goto done;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->CreateCreature("CREATURE_ROCK_TROLL_START_STANDING", spawnPos.GetPos(), "RTFE_RockTroll", false);
    ParentClass->RockTrollSpawned = true;
    Game->RemoveThing(Me, false, true);
    return;
done:
    return;
}