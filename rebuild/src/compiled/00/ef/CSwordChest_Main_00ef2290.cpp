// CSwordChest::Main() -- sword-chest script: wait until the hero uses the chest, then, if "LovelyBigSword" is alive and not already carried by the hero, make the hero carry it in hand
// Leaf: Main
// Retail 0x00EF2290, 400 bytes, `ret`.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90, the filter registration 0x00F35B10,
// ~CIsThingAlive 0x00E241B0 (out of line in this TU), IsDone 0x00F35B30, CCharString(const char*, long) 0x0099EBF0
// ("SCRIPT_NAME_HERO" 0x0125D1C8, "LovelyBigSword" 0x012F61DC), ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840;
// CScriptThing slot 20 MsgIsUsedBy (+0x50) on Me, and on the sword (inlined, devirtualised) slot 75 IsAlive (+0x12c) and
// slot 63 IsBeingCarriedBy (+0xfc) forwarded to PImp.Data; CGameScriptInterface slots 7 NewScriptFrame, 72
// GetThingWithScriptName (+0x120, by-value CScriptThing), 142 MakeHeroCarryItemInHand (+0x238).
// Layout = Ego_r.pdb CSwordChest (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14,
// PMasterData +0x18; CScriptThing per the landed script model (CBanditCronies::Main 0x00E03810: vptr, PImp +4/+8).
// Shape: `!Me.MsgIsUsedBy("...")` is a byte computed (`neg ; sbb ; inc`) before the string's dtor, the rotated wait loop
// repeats it; the sword test `IsAlive() && !IsBeingCarriedBy("...")` keeps a conditional-temporary flag zeroed at entry;
// the forwarding IsBeingCarriedBy needs `__forceinline virtual` (a plain inline virtual is called out of line).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    virtual ~CScriptThing();                                    // 0x004AA840
    virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const; virtual void v005() const; virtual void v006() const; virtual void v007() const;
    virtual void v008() const; virtual void v009() const; virtual void v010() const; virtual void v011() const; virtual void v012() const; virtual void v013() const; virtual void v014() const; virtual void v015() const;
    virtual void v016() const; virtual void v017() const; virtual void v018() const; virtual void v019() const;
    virtual bool MsgIsUsedBy(const CCharString& name) const;   // slot 20 (+0x50)
    virtual void v021() const; virtual void v022() const; virtual void v023() const; virtual void v024() const; virtual void v025() const; virtual void v026() const; virtual void v027() const; virtual void v028() const;
    virtual void v029() const; virtual void v030() const; virtual void v031() const; virtual void v032() const; virtual void v033() const; virtual void v034() const; virtual void v035() const; virtual void v036() const;
    virtual void v037() const; virtual void v038() const; virtual void v039() const; virtual void v040() const; virtual void v041() const; virtual void v042() const; virtual void v043() const; virtual void v044() const;
    virtual void v045() const; virtual void v046() const; virtual void v047() const; virtual void v048() const; virtual void v049() const; virtual void v050() const; virtual void v051() const; virtual void v052() const;
    virtual void v053() const; virtual void v054() const; virtual void v055() const; virtual void v056() const; virtual void v057() const; virtual void v058() const; virtual void v059() const; virtual void v060() const;
    virtual void v061() const; virtual void v062() const;
    __forceinline virtual bool IsBeingCarriedBy(const CCharString& name) const { return PImp.Data && PImp.Data->IsBeingCarriedBy(name); }   // slot 63 (+0xfc)
    virtual void v064() const; virtual void v065() const; virtual void v066() const; virtual void v067() const; virtual void v068() const; virtual void v069() const; virtual void v070() const; virtual void v071() const;
    virtual void v072() const; virtual void v073() const; virtual void v074() const;
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c)
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
class CIsThingAlive : public NScript::CPredicateBase { public:                                  // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                               // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive();                                   // 0x00E241B0
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

class CSwordChest : public CEntityScriptBase {
public:
    void*                        __vftable;             // +0x00
    CGameScriptInterface*        Game;                  // +0x04
    CScriptThing                 Me;                    // +0x08
    CPersonalScript_EmptyScript* ParentClass;           // +0x14
    CEmptyData*                  PMasterData;           // +0x18
    void Main();
};

void CSwordChest::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing sword = Game->GetThingWithScriptName("LovelyBigSword");
    if (sword.IsAlive() && !sword.IsBeingCarriedBy("SCRIPT_NAME_HERO"))
        Game->MakeHeroCarryItemInHand(sword, true, false);
}