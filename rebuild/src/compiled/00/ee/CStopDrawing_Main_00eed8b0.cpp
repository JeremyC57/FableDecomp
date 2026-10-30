// CStopDrawing::Main() -- alextest script: until done, hide Me (EntitySetAsDrawable false) once the hero talks to it, hits it, or hits it with any special ability other than ability 14
// Leaf: Main
// Retail 0x00EED8B0, 391 bytes. Callees: CScriptThing(const CScriptThing&) 0x004ABE90 (filter member), RegisterFilter
// 0x00F35B10, ~CIsThingAlive 0x00E241B0 (out of line in this module), IsDone 0x00F35B30, CCharString(const char*, long)
// 0x0099EBF0 / ~CCharString 0x0099EAE0 ("SCRIPT_NAME_HERO" 0x0125D1C8); CScriptThing (vtable 0x01238C8C) slots 21
// MsgIsHitBy, 27 MsgIsTalkedToBy, 41 MsgIsHitBySpecialAbilityFrom(long, name), 42 MsgIsHitByAnySpecialAbilityFrom
// (names from the slot targets 0x004AAB30/0x004AABD0/0x004AAD20/0x004AAD40); CGameScriptInterface slots 7
// NewScriptFrame and 511 EntitySetAsDrawable (rtti-vtables.tsv).
// Layout: Ego_r.pdb CStopDrawing (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14,
// PMasterData +0x18. Filter model as the landed CTH_Trigger_Oakvale::Main 0x00EE0A20.
// Shape: the four hero-name strings are implicit-conversion temporaries of one `||`/`&&` chain (VC's construction
// flags in ebx, `or ebx,N` per temporary, destroyed in reverse after the chain lands in a frame bool); a plain `while`
// after the first IsDone return gives retail's rotated loop (test duplicated at entry and bottom).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CScriptThing { public:                              // vtable 0x01238C8C, 12 bytes
    virtual void s000(); virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005(); virtual void s006(); virtual void s007();
    virtual void s008(); virtual void s009(); virtual void s010(); virtual void s011(); virtual void s012(); virtual void s013(); virtual void s014(); virtual void s015();
    virtual void s016(); virtual void s017(); virtual void s018(); virtual void s019(); virtual void s020();
    virtual bool MsgIsHitBy(const CCharString& hitter) const;                       // slot 21 (+0x54)
    virtual void s022(); virtual void s023(); virtual void s024(); virtual void s025(); virtual void s026();
    virtual bool MsgIsTalkedToBy(const CCharString& talker) const;                  // slot 27 (+0x6c)
    virtual void s028(); virtual void s029(); virtual void s030(); virtual void s031(); virtual void s032(); virtual void s033(); virtual void s034(); virtual void s035();
    virtual void s036(); virtual void s037(); virtual void s038(); virtual void s039(); virtual void s040();
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& from) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& from) const;    // slot 42 (+0xa8)
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
    virtual bool operator()(const CScriptThing& thing) const;
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
extern const char g_scriptNameHero[];              // 0x0125D1C8 "SCRIPT_NAME_HERO"
class CPersonalScript_alextestScript;
class CalextestData;

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CStopDrawing : public CEntityScriptBase {
public:
    void*                  __vftable;              // +0x00
    CGameScriptInterface*  Game;                   // +0x04
    CScriptThing           Me;                     // +0x08
    CPersonalScript_alextestScript* ParentClass;   // +0x14
    CalextestData*         PMasterData;            // +0x18
    void Main();
};

void CStopDrawing::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        bool touched = Me.MsgIsTalkedToBy(g_scriptNameHero) || Me.MsgIsHitBy(g_scriptNameHero)
                    || (Me.MsgIsHitByAnySpecialAbilityFrom(g_scriptNameHero)
                        && !Me.MsgIsHitBySpecialAbilityFrom(0xe, g_scriptNameHero));
        if (touched) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetAsDrawable(Me, false);
        }
        Game->NewScriptFrame();
    }
}