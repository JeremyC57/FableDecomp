// CUndeadSoldier::Main() -- undead soldier entity script: alive filter on Me, then invulnerable and hostile until the hero comes within 15 units, then damageable for the rest of the script
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D88F90, length 265 bytes (+ int3 padding). Entity-script slot 1 (Main).
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 / ~CScriptThing @ 0x004AA840 (the CIsThingAlive filter, vtable
// 0x012C32F0), the filter registration @ 0x00F35B10, IsActiveThreadTerminating @ 0x00F35B30, CGameScriptInterface slots 7
// NewScriptFrame (+0x1c), 516 EntitySetAsDamageable (+0x810), 601 EntitySetInFaction (+0x964), 70 GetHero (+0x118),
// IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float) @ 0x00CBE2FF (`__fastcall`),
// CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0.
// Strings: 0x012D4F1C "FACTION_GUARDS_ENEMY".
// Layout: the entity-script base {vptr, Game +4, Me +8} (the landed CSummonerToKill::Main 0x00D83DC0 model).
// Shape: the scoped filter object of the landed entity-script Mains; the faction name is an implicit conversion; the
// distance test's float is pushed before GetHero() is evaluated (right-to-left); the final wait is an endless frame loop
// that only leaves when the thread terminates.
namespace NScript { class CPredicateBase; }
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CScriptThing { public:                                    // 12 bytes
    unsigned char _body[12 - 4];
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    virtual ~CScriptThing();                                     // 0x004AA840
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                   // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                                // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CUndeadSoldier : public CEntityScriptBase {
public:
    void*                 __vftable;                     // +0x00
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing          Me;                            // +0x08
    void Main();
};

void CUndeadSoldier::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsDamageable(Me, false);
    Game->EntitySetInFaction(Me, "FACTION_GUARDS_ENEMY");
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsDamageable(Me, true);
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}