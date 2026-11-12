# agentNF26 header overlay

This folder starts as FAM09's overlay (hdr_base_FAM09). FAM09's seven headers are now identical to the repo (FAM09 has landed), so
only the five headers below differ from rebuild/include. Each one is a full copy of the current repo header with `#ifdef` blocks
added and nothing else changed. A row turns a block on by defining the macro before it includes the header. Every row was graded
with FABLE_HDR_OVERLAY pointing at this folder.

## Headers changed beyond FAM09 (land these)

| Header | Opt-in macro | Adds | Used by row |
|---|---|---|---|
| engine/CTCWeapon.h | FABLE_RETAIL_CTCWEAPON_AIMING (with FABLE_RETAIL_CTCWEAPON) | `bool IsAimedForLongEnough(float) const` (0x005DA490), `void SetAimingTimeOverride()` (0x005DA4C0) | 0x005DA490, 0x005DA4C0 |
| engine/CTCVillage.h | FABLE_RETAIL_CTCVILLAGE_SCHOOLSTORYTIME (with FABLE_RETAIL_CTCVILLAGE) | public `bool IsSchoolTime() const` (0x00658A35), `bool IsStoryTime() const` (0x00658A80); `IsTimeBetween` (0x00658620) only when FABLE_RETAIL_CTCVILLAGE_ISWORKTIME is not defined. The same macro also adds the two method declarations to the flattened (default) branch | 0x00658A35, 0x00658A80 (retail branch); 0x00916910 (flattened branch) |
| engine/CScriptThing.h | FABLE_RETAIL_CSCRIPTTHING_NULLSTRING (flattened, non-NSCRIPT branch) | forward `class CCharString;` and public `static CCharString NullString` (0x0143E8EC), spelled as the retail NSCRIPT branch declares it | 0x004AA8B0, 0x004AA8D0, 0x004AA900, 0x004AA9E0, 0x00E6A0B0, 0x00E9CB20, 0x00EDBD80, 0x00EEC410 |
| engine/CMainGameComponent.h | FABLE_RETAIL_CMAINGAMECOMPONENT_INITLUA | protected `void InitLUA(const class CCharString&)` (0x004BBC00) | 0x004BBC00 |
| engine/CLevelLoader.h | FABLE_RETAIL_CLEVELLOADER_GETGLOBALSECTION (nested inside FAM09's FABLE_RETAIL_CLEVELLOADER_GLOBALSECTION) | inline `static CCharString __fastcall GetGlobalSectionName()` (Ego_r ?GetGlobalSectionName@CLevelLoader@@SI?AVCCharString@@XZ) | 0x004CA010 |

All additions are declarations of member functions or statics, so no class layout changes. The names come from Ego_r.pdb and
FableWin.pdb, and the access and const from their mangled names. The CScriptThing::NullString access cannot be settled from
the TLC PDBs (see NOTES.md), so the block keeps the access the retail branch and the landed binders already use.

## Existing repo opt-ins used by my rows

- FABLE_RETAIL_CWIDESTRING_NULLSTRING (fable_string.h): 0x00542140, 0x0061A3B0
- FABLE_RETAIL_CLEVELLOADER_GLOBALSECTION (FAM09): 0x004CA010
- FABLE_RETAIL_CTCBULLETTIME / _MEMBERS, FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CINTELLIGENTPOINTER, FABLE_RETAIL_CDEFSTRINGTABLE: 0x0079C740
- FABLE_RETAIL_NSCRIPT, FABLE_RETAIL_CWORLD, FABLE_RETAIL_CWORLDMAP: 0x0088E300
