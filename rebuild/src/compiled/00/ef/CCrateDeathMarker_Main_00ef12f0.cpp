// NScript::CPersonalScript_EmptyScript::CCrateDeathMarker::Main() -- death-marker crate: alive filter on Me; each round wait for the living hero to come within 5.5 of the marker, spawn a CREATURE_BS_VILLAGER_MALE at it, fade it in (alpha 0 to 1 in 0.05 steps, one per frame), then wait until the hero is dead or more than 8 away, and end the round with a script frame
// Leaf: Main
// Identity (naming wave 11): ?Main@CCrateDeathMarker@CPersonalScript_EmptyScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012F5FC8 slot 1 is stored by the factory 0x00EF1210 registered as "CrateDeathMarker"
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
template <class T> class CCountedPointer { public:                     // CCountedPointer<CScriptThing>
    CScriptThing*   Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
namespace NScript {
}
namespace NScript {
}
using NScript::CIsThingAlive;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CPersonalScript_EmptyScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CCrateDeathMarker;                        // the entity class below (naming wave 11)
};
}
using NScript::CPersonalScript_EmptyScript;
namespace NScript {
class CPersonalScript_EmptyScript::CCrateDeathMarker : public CEntityScriptBase {
public:
private:
        virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CPersonalScript_EmptyScript::CCrateDeathMarker::Main()
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