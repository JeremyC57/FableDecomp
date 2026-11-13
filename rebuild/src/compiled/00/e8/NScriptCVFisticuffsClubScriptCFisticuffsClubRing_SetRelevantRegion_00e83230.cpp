// NScript::CV_FisticuffsClubScript::CFisticuffsClubRing::SetRelevantRegion() -- the Fisticuffs Club ring picks the club's region
//   from its own thing's data string (BowerstoneSlums -> REGION_BOWERSTONE, KnotholeGlade -> REGION_KNOTHOLE, Oakvale ->
//   REGION_OAKVALE, BanditCamp -> REGION_BANDITCAMP, anything else -> REGION_KNOTHOLE) and stores the village thing nearest the
//   hero with the matching VILLAGE_* def name in the parent script's Village; every arm first checks IsActiveThreadTerminating.
// Leaf: ?SetRelevantRegion@CFisticuffsClubRing@CV_FisticuffsClubScript@NScript@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E83230, 820 bytes, __fastcall, plain `ret`; called only from CFisticuffsClubRing::Main
//   0x00E82950 (call 0x00E8299C). Identity: Ego_r.pdb / FableWin.pdb CFisticuffsClubRing member list (Alloc, ctor,
//   GetParentScript, OnPredicateFail, Main, SetRelevantRegion) in retail order; manifest label "HandleVillageSelection" and
//   Main's "SetUpRing" spelling are made-up.
// Callees: IsActiveThreadTerminating 0x00F35B30, CScriptThing::GetDataString (vslot 3), CBasicString<char>::Compare 0x00411570
//   (inline CCharString text ==), CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0, GetHero (Game vslot 70) /
//   GetNearestWithDefName (vslot 81), implicit CScriptThing::operator= 0x008AB980 (expanded at the first arm, called at the
//   others), ~CScriptThing 0x004AA840, operator delete 0x00BFE9BC (inside the expanded operator=).
// Inferred: (inline split; behaviour identical to retail) the first arm's assignment sits in a single-site do { } while (0) so VC7.1 expands that one implicit
//   CScriptThing operator= as retail does (same lever as landed 0x00DA91A0; plain form matches only under /d2inlT100).
//   Retail's construct is unproven.
// Layout: CFisticuffsClubRing (Ego_r.pdb, no shared header; nested here as engine/CV_FisticuffsClubScript.h nests
//   CFisticuffsMember): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18.
//   CV_FisticuffsClubScript (retail = PDB - 0x14): RelevantRegion +0x48, Village +0x9C.
#include "fable_compiler.h"
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_STLP_MAP
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CV_FisticuffsClubScript.h"
#include "engine/CGameScriptInterfaceBase.h"
class CScriptDataBase;
namespace NScript {
class CV_FisticuffsClubScript::CFisticuffsClubRing : public CEntityScriptBase {
protected:
    CV_FisticuffsClubScript* ParentClass;                                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
private:
    void SetRelevantRegion();
};
void CV_FisticuffsClubScript::CFisticuffsClubRing::SetRelevantRegion()
{
    if (Me.GetDataString() == "BowerstoneSlums")
    {
        if (IsActiveThreadTerminating()) return;
        ParentClass->RelevantRegion = REGION_BOWERSTONE;
        do {
        ParentClass->Village = Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_BOWERSTONE_SLUMS");
        } while (0);
    }
    else if (Me.GetDataString() == "KnotholeGlade")
    {
        if (IsActiveThreadTerminating()) return;
        ParentClass->RelevantRegion = REGION_KNOTHOLE;
        ParentClass->Village = Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_KNOTHOLE_GLADE");
    }
    else if (Me.GetDataString() == "Oakvale")
    {
        if (IsActiveThreadTerminating()) return;
        ParentClass->RelevantRegion = REGION_OAKVALE;
        ParentClass->Village = Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_OAKVALE");
    }
    else if (Me.GetDataString() == "BanditCamp")
    {
        if (IsActiveThreadTerminating()) return;
        ParentClass->RelevantRegion = REGION_BANDITCAMP;
        ParentClass->Village = Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_BANDIT_CAMP_RESIDENTIAL");
    }
    else
    {
        if (IsActiveThreadTerminating()) return;
        ParentClass->RelevantRegion = REGION_KNOTHOLE;
    }
}
}