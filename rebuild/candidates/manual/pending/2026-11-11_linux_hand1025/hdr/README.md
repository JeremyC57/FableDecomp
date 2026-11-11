# Chunk 1025 header overlay (`FABLE_HDR_OVERLAY=$A/hdr`)

Every grade of this hand-back ran with `export FABLE_HDR_OVERLAY=$A/hdr`. Existing headers are copies of `rebuild/include/engine/`
with additions ONLY under new `#ifdef FABLE_RETAIL_*` opt-ins (default path byte-identical); new headers are new files.

| Header | Kind | Opt-in / content | Used by | Evidence |
|---|---|---|---|---|
| engine/CTargetingDef.h | NEW | class CTargetingDef : CSubDefClassBase (Ego_r members +0x28..+0x8c, TargetingRanges std::map +0x74, sizeof 0x90); inline GetTargetingRange(ETargetingType) (find / end / 0.0f) | 0x008028D0, 0x00802B60 | struct_layouts_egor.tsv; retail 0x00802890 body; RTTI name string as in landed 0x00802D10 |
| engine/CTCTargeted.h | existing | `FABLE_RETAIL_CTCTARGETED_PTARGETINGDEF`: inline GetPTargetingDef() | 0x00802B60 | FableWin ?GetPTargetingDef@CTCTargeted@@QBE?AV?$CDefPointer@$$CBVCTargetingDef@@@@XZ |
| engine/CTCAIScratchpad.h | existing | `FABLE_RETAIL_CTCAISCRATCHPAD_TARGETINGOFFSET`: inline GetTargetingDistanceOffset() (+0x7C) | 0x00802B60 | FableWin ?GetTargetingDistanceOffset@CTCAIScratchpad@@QBEMXZ |
| engine/CPlayer.h | existing | `FABLE_RETAIL_PLAYER_ISSPELLMODE`: inline IsSpellMode() (+0x218); `FABLE_RETAIL_PLAYER_GETNUMBER`: inline GetNumber() (+0x28) | 0x008028D0 / 0x007C0AD0 | FableWin ?IsSpellMode@CPlayer@@QBE_NXZ; Ego_r/FableWin ?GetNumber@CPlayer@@QBEJXZ |
| engine/CTCWeapon.h | existing | `FABLE_RETAIL_CTCWEAPON_TARGETINGRANGE`: GetTargetingRange() const declaration (0x005DA890) | 0x008028D0 | Ego_r ?GetTargetingRange@CTCWeapon@@QBEJXZ + donor order |
| engine/CAtmosProcess.h | NEW | class CAtmosProcess (natural alignment, sizeof 0x20, nested CMyAtmosMap +0x04); `FABLE_RETAIL_CATMOSPROCESS_MIXATMOS_BODY` (inline MixAtmos), `FABLE_RETAIL_CATMOSPROCESS_RAINPLAYER` (typed PRainPlayer) | 0x006B1D30 | Ego_r members; landed ctor 0x006B1960 / Update 0x006B2260 offsets; vtable 0x0125D47C |
| engine/CDefinitionManager.h | existing | `FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY`: noinline by-name GetDef<T> body; `FABLE_RETAIL_CDEFINITIONMANAGER_CLASSTYPENAME`: GetDefClassTypeNameFromGlobalIndex declaration (0x009ACCE0) | 0x006B1D30 / 0x007C0AD0 | FableWin 0x02247E20 (GetDef<CSoundThemeDef> by name); PDB symbol of 0x009ACCE0 |
| engine/CThing.h | existing | `FABLE_RETAIL_CTHING_GETSCRIPTNAME`: inline GetScriptName() (+0x74), after the BULLETTIMEOVERRIDE block (needs FABLE_RETAIL_CTHING_STATEACCESS) | 0x007C0AD0 (differ) | Ego_r/FableWin ?GetScriptName@CThing@@QBEABVCCharString@@XZ |
| engine/NThingShot_CTCShotDefinitionBase.h | existing | `FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT`: Init(CDefPointer<const CThingShotDef>, CThing*, CThing*, const CThing*, float x3, const CDefPointer<const CMeleeCombatAbilityDef>&) declaration (0x007C0690) | 0x007C0AD0 (differ) | Ego_r/FableWin ?Init@CTCShotDefinitionBase@NThingShot@@QAEXV?$CDefPointer@$$CBVCThingShotDef@@@@PAVCThing@@1PBV4@MMM... |
| engine/CThingAICreatureInit.h | NEW | the Ego_r 28-byte record; ctor declared out of line (0x004FACD0), body under `FABLE_RETAIL_CTHINGAICREATUREINIT_CTOR_BODY` (the landed CreateMinion 0x0079ACC0 expands it) | 0x007C0AD0 (differ) | struct_layouts_egor.tsv; ??0CThingAICreatureInit@@QAE@PAVCThingPhysical@@0_NABVCCharString@@21@Z |
| engine/CDefString.h | existing | `FABLE_RETAIL_CDEFSTRING_SETDEFAULTSTRING`: SetDefaultString(const CCharString&) declaration (0x009D5080) | 0x0062FE60 (differ) | PDB symbol ?SetDefaultString@CDefString@@QAEXABVCCharString@@@Z |

Debt for landing: CTargetingDef.h / CAtmosProcess.h / CThingAICreatureInit.h go to rebuild/include/engine/; the landed
CTCSummonSpell::CreateMinion 0x0079ACC0 local CThingAICreatureInit model can move to the new header with the CTOR_BODY opt-in.
