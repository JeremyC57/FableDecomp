// CTrappedMan::Main() -- the trapped man on the hill: register an alive filter over Me, run a frame; then, until the script is done, wait for the hero to hit Me, show "You hit the man on the hill!" and, when his health is below 1e-4, "You hit him to the point of death!", once per script frame
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF1E10, 384 bytes (entity-script vtable slot 1 = Main).
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 (filter member; filter vtable 0x012C32F0), the filter registration
//          @ 0x00F35B10, the filter's out-of-line dtor @ 0x00E241B0, IsDone @ 0x00F35B30, CCharString(const char*, long)
//          @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CScriptThing::MsgIsHitBy (slot 21, +0x54), CGameScriptInterface slots
//          7 NewScriptFrame (+0x1c), 112 AddScreenMessage (+0x1c0) and 264 GetHealth (+0x420); float pool 0x0129BA3C = 1e-4f.
// Layout: Ego_r.pdb CTrappedMan (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14,
//          PMasterData +0x18.
// Shape: the landed CHHUndead::Main filter block; `WAIT_UNTIL(Me.MsgIsHitBy("SCRIPT_NAME_HERO"))` is a `while (!x)` whose
//          neg/sbb/inc bool survives the string temporary's dtor (VC duplicates the test at the loop bottom); the outer
//          loop is a rotated `while (!IsActiveThreadTerminating())`.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CScriptThing { public:   // 12 bytes: vptr + counted implementation (vtable 0x01238C8C)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual bool MsgIsHitBy(const CCharString& hitter) const;   // slot 21 (+0x54)
    unsigned char _body[8];
    CScriptThing(const CScriptThing& other);       // 0x004ABE90
    virtual ~CScriptThing();                               // 0x004AA840
};
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
struct CPersonalScript_EmptyScript;
struct CEmptyData;

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CTrappedMan : public CEntityScriptBase {
public:
    void*                        __vftable;        // +0x00
    CGameScriptInterface*        Game;             // +0x04
    CScriptThing                 Me;               // +0x08
    CPersonalScript_EmptyScript* ParentClass;      // +0x14
    CEmptyData*                  PMasterData;      // +0x18
    void Main();
};

void CTrappedMan::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        while (!Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->AddScreenMessage("You hit the man on the hill!", (ETextGroupSelectionMethod)0);
        if (Game->GetHealth(Me) < 1e-4f) {
            if (IsActiveThreadTerminating())
                return;
            Game->AddScreenMessage("You hit him to the point of death!", (ETextGroupSelectionMethod)0);
        }
        Game->NewScriptFrame();
    }
}