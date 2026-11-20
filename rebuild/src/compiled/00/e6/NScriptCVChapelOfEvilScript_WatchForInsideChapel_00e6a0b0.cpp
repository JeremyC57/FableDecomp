// NScript::CV_ChapelOfEvilScript::WatchForInsideChapel() -- track whether the (living) hero stands inside the evil chapel building: set InsideChapel on entering BUILDING_CHAPEL_EVIL, clear it once he stands in no building
// Leaf: ?WatchForInsideChapel@CV_ChapelOfEvilScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E6A0B0, length 650 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0 (the inline
//   CScriptThing copies), ~CScriptThing 0x004AA840 (the out-of-line copy at the in-branch returns), operator delete
//   0x00BFE9BC; CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0 and the empty string global 0x0143E8EC;
//   Compare 0x00411570 (the inlined CCharString == const char*); CScriptThing slots 2 GetDefName, 75 IsAlive, 77 IsNull;
//   GameCalls slots 7 NewScriptFrame, 70 GetHero, 504 EntityGetStandingInsideBuilding (by value). "BUILDING_CHAPEL_EVIL".
// Layout: Ego_r.pdb CV_ChapelOfEvilScript InsideChapel +0x6d = retail +0x59 (retail CScriptBase is 0x14 below the PDB).
// Shape: the building is a function-scope CScriptThing reassigned each frame (inline counted-pointer operator=, then the
//   by-value result destroyed inline); IsNull/GetDefName are inline virtuals called on the local, so VC devirtualises and
//   inlines them; the GetDefName temporary is conditional (flag word in the frame). The two in-branch terminate returns use
//   the out-of-line destructor (terminate checks as `do { if (...) return; } while (0);`; VC7.1's per-site frequency rule (docs/pipeline/INLINE_DEPTH_REVISIT.md)).
//   The loop is `for (;;) { if (IsActiveThreadTerminating()) return; ... }`: a `while (!IsActiveThreadTerminating())` loop
//   tail-merges the two inline destructor exits the other way round. ~CScriptThing is plain inline.
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x0143E8EC is CScriptThing::NullString (?NullString@CScriptThing@@2VCCharString@@A, the
//   engine/CScriptThing.h spelling; Ego_r.pdb + FableWin.pdb `dynamic initializer for 'CScriptThing::NullString'', initializer 0x01224C50 landed by agentFAM09); was g_EmptyCharString.
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
// Naming (NF162, 2026-11-20, q6270): was a row-local global `struct CV_ChapelOfEvilScript : CScriptBase` (leaf
//   ?WatchForInsideChapel@CV_ChapelOfEvilScript@@QAEXXZ) with local CBasicString / CCharString / GameCalls models; now on the shared
//   engine/CV_ChapelOfEvilScript.h and fable_string.h (CBasicString<char>::operator==(const char*) expanded per the quest-script
//   opt-in FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE). Identity: Ego_r.pdb ?WatchForInsideChapel@CV_ChapelOfEvilScript@NScript@@QAEXXZ,
//   the thread "WatchForInsideChapel" Main 0x00E693F0 spawns (the landed Main binds this spelling; its only referencer).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#include <string.h>
#pragma intrinsic(strcmp)
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_ChapelOfEvilScript.h"
inline CCharString CScriptThing::GetDefName() const { if (!PImp.Data) return CScriptThing::NullString; return PImp.Data->GetDefName(); }
inline bool CScriptThing::IsNull() const { if (!PImp.Data) return true; return PImp.Data->IsNull(); }
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }

void NScript::CV_ChapelOfEvilScript::WatchForInsideChapel()
{
    CScriptThing building;
    for (;;) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        if (Game->GetHero().IsAlive()) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            building = Game->EntityGetStandingInsideBuilding(Game->GetHero());
            if (!building.IsNull() && building.GetDefName() == "BUILDING_CHAPEL_EVIL" && !InsideChapel) {
                do { if (IsActiveThreadTerminating()) return; } while (0);
                InsideChapel = true;
            } else if (building.IsNull() && InsideChapel) {
                do { if (IsActiveThreadTerminating()) return; } while (0);
                InsideChapel = false;
            }
        }
        Game->NewScriptFrame();
    }
}