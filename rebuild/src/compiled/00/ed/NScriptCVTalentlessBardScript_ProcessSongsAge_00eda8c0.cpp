// NScript::CV_TalentlessBardScript::ProcessSongsAge() -- adds the age verse to the bard's song list: OLD above BardHeroAgeHigh, MEDIUM above BardHeroAgeMedium, YOUNG otherwise
// Leaf: ?ProcessSongsAge@CV_TalentlessBardScript@NScript@@QAEXXZ
// Flags: /d2inlT59
// Inline-budget: /d2inlT59 -- STLport vector<CCharString>::push_back (plain inline) is expanded at the first (OLD) site and called out of
//   line (COMDAT 0x0044BFF0) at the MEDIUM / YOUNG sites in retail; with the default budget VC7.1 lays the three sites out differently
//   (DIFFER 238v236, NF60/NF68); /d2inlT40, /d2inlT55, /d2inlT57 and /d2inlT58 call it at every site (160 bytes), /d2inlT59 and
//   /d2inlT62 match, /d2inlT150 keeps the default layout: 59 is the smallest matching value (58 graded, DIFFER 160v236).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDA8C0, 236 bytes (padded to 240), plain `ret`.
//   PDB name ?ProcessSongsAge@CV_TalentlessBardScript@NScript@@QAEXXZ.
// Callees: CGameScriptInterface GetHeroAge() (virtual), CCharString(const char*, long) 0x0099EBF0, copy ctor 0x0099EC30, ~CCharString
//   0x0099EAE0; std::vector<CCharString>::push_back 0x0044BFF0 (out of line at two sites) and its overflow helper _M_insert_overflow
//   0x00433530.
// Data: the script def 0x0143E90C pGlobalScriptDef (BardHeroAgeHigh / BardHeroAgeMedium).
// Strings: "TEXT_QST_163_AGE_OLD" 0x012F3B14, "TEXT_QST_163_AGE_MEDIUM" 0x012F3AFC, "TEXT_QST_163_AGE_YOUNG" 0x012F3AE4.
// Port (NF68, round 7, from NF60's draft): the local NScript::CV_TalentlessBardScript model is replaced by
//   engine/NScript_CV_TalentlessBardScript.h on the macro block of the landed sibling WatchForResetWanderCentre 0x00ED94D0 (Game reference,
//   SongItems +0x68); CCharString, the STLport vector and pGlobalScriptDef come from the shared headers (NF60).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/EScriptingStateGroups.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CTimer.h"
#include "engine/NScript_CV_TalentlessBardScript.h"
#include "engine/fable_script_def_global.h"

void NScript::CV_TalentlessBardScript::ProcessSongsAge()
{
    if (Game.GetHeroAge() > pGlobalScriptDef->BardHeroAgeHigh) {
        CCharString verse("TEXT_QST_163_AGE_OLD", -1);
        SongItems.push_back(verse);
    }
    else if (Game.GetHeroAge() > pGlobalScriptDef->BardHeroAgeMedium) {
        CCharString verse("TEXT_QST_163_AGE_MEDIUM", -1);
        SongItems.push_back(verse);
    }
    else {
        CCharString verse("TEXT_QST_163_AGE_YOUNG", -1);
        SongItems.push_back(verse);
    }
}
