# agentFAM09 header overlay

Each file here is a full copy of the current repo header (rebuild/include/...) with one `#ifdef` block added. Nothing else
changed. A row turns a block on by defining the macro before it includes the header. Rows were graded with
FABLE_HDR_OVERLAY pointing at this folder.

| Header | Opt-in macro | Adds | Used by row |
|---|---|---|---|
| engine/CUserProfileManager.h | FABLE_RETAIL_CUSERPROFILEMANAGER_PROFILENAME | `#include "fable_string.h"`; private `static CWideString m_strProfileName` (0x013B7D54) | 0x01219EC0 |
| engine/CLevelLoader.h | FABLE_RETAIL_CLEVELLOADER_GLOBALSECTION (with FABLE_RETAIL_CLEVELLOADER) | public `static CCharString GlobalSectionName` (0x013BAB0C) | 0x0121E160 |
| engine/CTCWife.h | FABLE_RETAIL_CTCWIFE_MALEVOICE (with FABLE_RETAIL_CTCWIFE) | public `static const CCharString MarriageEnabledMaleVoice` (0x013BAB2C) | 0x0121E1F0 |
| engine/CWeaponDef.h | FABLE_RETAIL_CWEAPONDEF_TOOHEAVY (with FABLE_RETAIL_DEF_CLASS_BASE) | public `static const CCharString TooHeavyAnimationGroupName` (0x013BAF5C) | 0x0121F610 |
| engine/CTCCreatureModeManager.h | FABLE_RETAIL_CMODEBASE_NOFRAMES | private `static const long` NO_FRAMES_BEFORE_PLAYING_COMB_IDLE_ANIMS / _IDLE_ANIMS / _WAVE_ANIMS in CModeBase (0x013BAFE4 / E8 / EC) | 0x0121F7D0, 0x0121F800 (0x0121F7E0 could use it too) |
| engine/CDefinitionManager.h | FABLE_RETAIL_CDEFINITIONMANAGER_COMPILEDDEFDIR (with FABLE_RETAIL_CDEFINITIONMANAGER) | protected `static CWideString CompiledDefDirectory` (0x013CA7DC) | 0x01224050 |
| engine/CSymbolMap.h | FABLE_RETAIL_CSYMBOLMAP_CURRENTFILENAME (with FABLE_RETAIL_CSYMBOLMAP) | protected `static CWideString CurrentFilename` (0x013CAA34) | 0x01224190 |

Each declaration is static, so no class layout changes. The access level and const come from the Anniv360 Test PDB (third
source) mangled names. The member names come from the Ego_r.pdb and FableWin.pdb `dynamic initializer for '...'` names.

These rows used opt-ins that already exist in the repo:
- 0x0121E140: FABLE_RETAIL_CLEVELLOADER_LOADALL
- 0x01223DA0: FABLE_RETAIL_CWIDESTRING_NULLSTRING
- 0x01224C50: FABLE_RETAIL_NSCRIPT
