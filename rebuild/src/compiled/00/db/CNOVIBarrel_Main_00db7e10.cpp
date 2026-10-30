// CNOVI_Barrel::Main() -- tutorial barrel entity script: until the parent's game-info flag is set, wait for the hero within 2m, show the Xbox or PC game-info text, wait for it to be clicked past and set the flag; then idle until the thread terminates
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DB7E10, 0x1DC bytes (padded to 480), `ret`.
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 / ~CScriptThing 0x004AA840 (predicate vtable 0x012C2FE8),
//   CEntityScriptBase::SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, IsDistanceBetweenThingsUnder
//   0x00CBE2FF (__fastcall), CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0. Entity interface (+4) slots 6 IsXbox,
//   7 NewScriptFrame, 40 MsgIsGameInfoClickedPast, 70 GetHero, 115 DisplayGameInfo (cgsi_slots_aligned.tsv).
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 whose game-info-shown bool is at +0x82.
namespace NScript { class CPredicateBase; }
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CCharString { public:
    void* Data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
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
struct CIsThingPredicate : public NScript::CPredicateBase {                         // vtable 0x012C2FE8: a predicate holding a script thing
    CScriptThing Thing;                            // +0x04
    CIsThingPredicate(const CScriptThing& thing) : Thing(thing) { }
    ~CIsThingPredicate() { }
};
struct CNOVIParent {
    unsigned char _pad[0x82];
    bool          GameInfoShown;                   // +0x82
};
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

class CNOVI_Barrel : public CEntityScriptBase {
public:
        CNOVIParent*          ParentClass;             // +0x14
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
void CNOVI_Barrel::Main()
{
    {
        CIsThingPredicate predicate(Me);
        SetPredicate(predicate);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass->GameInfoShown) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 2.0f)) {
            if (IsActiveThreadTerminating())
                return;
            if (Game->IsXbox()) {
                if (IsActiveThreadTerminating())
                    return;
                {
                    CCharString text("XBOX_TEXT");
                    Game->DisplayGameInfo(text);
                }
                while (!Game->MsgIsGameInfoClickedPast()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
            } else {
                if (IsActiveThreadTerminating())
                    return;
                {
                    CCharString text("PC_TEXT");
                    Game->DisplayGameInfo(text);
                }
                while (!Game->MsgIsGameInfoClickedPast()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
            }
            if (IsActiveThreadTerminating())
                return;
            ParentClass->GameInfoShown = true;
        }
    }
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}