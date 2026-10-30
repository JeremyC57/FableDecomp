// CCrateDeathMarker::Main() -- death-marker crate: alive filter on Me; each round wait for the living hero to come within 5.5 of the marker, spawn a CREATURE_BS_VILLAGER_MALE at it, fade it in (alpha 0 to 1 in 0.05 steps, one per frame), then wait until the hero is dead or more than 8 away, and end the round with a script frame
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) CCrateDeathMarker::Main @ 0x00EF12F0, 693 bytes. Strings: "CREATURE_BS_VILLAGER_MALE" @ 0x012E39D8, "" @ 0x0122D70E; 0.05f @ 0x0125A15C.
// Callees: CScriptThing copy ctor 0x004ABE90, ~CScriptThing 0x004AA840, CIsThingAlive dtor 0x00E241B0, RegisterFilter 0x00F35B10, IsDone 0x00F35B30, IsDistanceBetweenThingsUnder 0x00CBE2FF / Over 0x00CBE3EA, CGameScriptInterface slots 7/70/91/510.
// Layout: entity-script base {vptr, Game +4, CScriptThing Me +8} as the landed CTrophiesBriarTrophy::Main 0x00E9D800.
// Shape: the villager is scoped inside the outer while so its destruction precedes the round-closing NewScriptFrame (retail
//   calls 0x004AA840, then slot 7). Retail expands the inline ~CScriptThing only at the fade-loop-top and post-fade IsDone
//   returns and calls 0x004AA840 at the fade-bottom, wait-loop, post-wait and scope-end exits; VC7.1 applies
//   `#pragma inline_depth` at the point it appears inside a body, so inline_depth(0) is toggled around exactly the four
//   out-of-line dtor sites (the call sites then tail-merge as retail does). Without it every site is expanded and the
//   round tail cross-jumps with the 5.5 wait loop.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
void __cdecl operator delete(void* block);              // 0x00BFE9BC
class CCPPointerInfo { public:                                 // counted-pointer block
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                         // +0x08
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing;
struct CCountedPointerScriptThing {                     // CCountedPointer<CScriptThing>
    CScriptThing*   Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    ~CCountedPointerScriptThing()
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
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    CCountedPointerScriptThing PImp;                    // +0x04
    virtual ~CScriptThing() { }                         // out of line: 0x004AA840
    virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const; virtual void v005() const;
    virtual const C3DVector& GetPos() const;          // slot 6 (+0x18)
    virtual void v007() const; virtual void v008() const; virtual void v009() const; virtual void v010() const; virtual void v011() const; virtual void v012() const; virtual void v013() const; virtual void v014() const;
    virtual void v015() const; virtual void v016() const; virtual void v017() const; virtual void v018() const; virtual void v019() const; virtual void v020() const; virtual void v021() const; virtual void v022() const;
    virtual void v023() const; virtual void v024() const; virtual void v025() const; virtual void v026() const; virtual void v027() const; virtual void v028() const; virtual void v029() const; virtual void v030() const;
    virtual void v031() const; virtual void v032() const; virtual void v033() const; virtual void v034() const; virtual void v035() const; virtual void v036() const; virtual void v037() const; virtual void v038() const;
    virtual void v039() const; virtual void v040() const; virtual void v041() const; virtual void v042() const; virtual void v043() const; virtual void v044() const; virtual void v045() const; virtual void v046() const;
    virtual void v047() const; virtual void v048() const; virtual void v049() const; virtual void v050() const; virtual void v051() const; virtual void v052() const; virtual void v053() const; virtual void v054() const;
    virtual void v055() const; virtual void v056() const; virtual void v057() const; virtual void v058() const; virtual void v059() const; virtual void v060() const; virtual void v061() const; virtual void v062() const;
    virtual void v063() const; virtual void v064() const; virtual void v065() const; virtual void v066() const; virtual void v067() const; virtual void v068() const; virtual void v069() const; virtual void v070() const;
    virtual void v071() const; virtual void v072() const; virtual void v073() const; virtual void v074() const;
    virtual bool IsAlive() const;                      // slot 75 (+0x12c), 0x004AB130
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA
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

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CCrateDeathMarker : public CEntityScriptBase {
public:
    void*                 __vftable;                    // +0x00
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    void Main();
};

void CCrateDeathMarker::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        while (!(Game->GetHero().IsAlive() && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 5.5f))) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CScriptThing villager = Game->CreateCreature("CREATURE_BS_VILLAGER_MALE", Me.GetPos(), "", false);
            Game->EntitySetAlpha(villager, 0.0f, true);
            for (float alpha = 0.0f; alpha <= 1.0f; alpha += 0.05f) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetAlpha(villager, alpha, true);
                Game->NewScriptFrame();
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
            }
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth(0)
            while (Game->GetHero().IsAlive() && !IsDistanceBetweenThingsOver(Me, Game->GetHero(), 8.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
    }
}
#pragma inline_depth()