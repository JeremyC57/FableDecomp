// CTCUnholyPower::GetDamageMultiplier() const -- the unholy-power def's damage multiplier for the current spell level
// Leaf: ?GetDamageMultiplier@CTCUnholyPower@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00793CB0, 16 bytes: mov eax,[ecx+0Ch] ; mov ecx,[ecx+50h] ; mov edx,[eax+0E8h] ; fld dword [edx+ecx*4] ; ret
// Naming (NF446, 2026-11-25): self respelt from the stand-in ?GetDamageMultiplier@@YIMPAUS@@@Z (catalog CTCDivineWrath::GetDamageMultiplier).
//   Evidence: both PDB name sets (Ego_r 0x007996D0, FableWin 0x02693CF0 ?GetDamageMultiplier@CTCUnholyPower@@QBEMXZ); Ego_r donor order
//   GetDamageMultiplier 0x007996D0 -> Attackable 0x007996E0 -> SetPTarget 0x00799740 = retail 0x00793CB0 -> CTCUnholyPower::Attackable
//   0x00793CC0 -> SetPTarget 0x00793D20; the only caller 0x00868B40 reaches it through interface 0x86 (TCI_UNHOLY_POWER); the body reads PDef
//   (+0x0C) and SpellLevel (+0x50) of engine/CTCUnholyPower.h and the def's DamageTakenMultiplier vector<float> (+0xE8,
//   engine/CSpecialAbilitiesUnholyPowerDef.h). The CTCDivineWrath twin is 0x0078E7B0.
// Header: engine/CTCUnholyPower.h with the define set of the landed 0x007952E0 OnKill (FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE for the
//   inline StopSound) plus FABLE_RETAIL_CDEFSTRINGTABLE for the def header.
#define FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_ADDANDCOPYTC
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_DEFAULTTHEME
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CThingCreatureBase.h"
#include "engine/CTCUnholyPower.h"
#include "engine/CSpecialAbilitiesUnholyPowerDef.h"

float CTCUnholyPower::GetDamageMultiplier() const
{
    return PDef->DamageTakenMultiplier[SpellLevel];
}
