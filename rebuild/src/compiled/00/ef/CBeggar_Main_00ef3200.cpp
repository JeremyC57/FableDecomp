// CBeggar::Main() -- the beggar entity: hold an alive filter over Me, then loop: wait until the hero talks to the beggar, sit him on the floor, wait until the hero talks to him again, stand him up
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF3200, 443 bytes; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy ctor @ 0x004ABE90, CEntityScriptBase::SetPredicate @ 0x00F35B10, ~CIsThingAlive @ 0x00E241B0
//          (out of line in this module), CEntityScriptBase::IsActiveThreadTerminating @ 0x00F35B30, CCharString(const char*, long)
//          @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0; CScriptThing slot 27 MsgIsTalkedToBy (0x004AABD0); CGameScriptInterface
//          slots 7 NewScriptFrame and 493 EntitySetAsSittingOnFloor (0x008A8C60, ghidra_out/cgsi_slots_aligned.tsv).
//          Literal "SCRIPT_NAME_HERO" @ 0x0125D1C8.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8 (12-byte CScriptThing, vtable 0x01238C8C)} as in the landed entity Mains
//         (CSickChildFishingSpot::Main 0x00ECD530); the CIsThingAlive filter (vtable 0x012C32F0) holds a CScriptThing copy.
// Shape: the landed alive-filter prelude, then `while (!IsActiveThreadTerminating())` around two scripted
//        WAIT_UNTIL(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) loops written as plain `while (!x)` (retail's neg/sbb/inc bool),
//        each followed by a terminating test and the sit/stand call.
// Leaf: Main
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026();
    virtual bool MsgIsTalkedToBy(const CCharString& name) const;   // slot 27 (+0x6c)
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
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
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive();                                  // 0x00E241B0 (out of line in this module)
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

struct CBeggar : CEntityScriptBase {
    void Main();
};

void CBeggar::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetAsSittingOnFloor(Me, true);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetAsSittingOnFloor(Me, false);
        Game->NewScriptFrame();
    }
}