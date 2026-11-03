# Chunk 719 — CWorldMap + CUserProfileManager (agent719)

Grade all payload rows with `FABLE_HDR_OVERLAY=$S/w22/agent719/hdr`.
The parked 0x005029A0 also needs `differ/hdr/stlport/fable_containers.h`, which adds STLport list::size() as the distance loop. It is kept out of hdr/ because its default path differs; it is an inert template addition.

## Matched (7)
| addr | symbol | grade | lever |
|---|---|---|---|
| 004fc6b0 | CWorldMap::DrawIsPosInMap | RELOC | secondary-base override: `add ecx,-4; jmp IsPosInMap` |
| 0051cdf0 | ??_ECWorldMap@@W3AEPAXI@Z | RELOC | the adjustor thunk is emitted with the vtable, so the source is the landed ctor TU verbatim (see below) |
| 004084b0 | CUserProfileManager::GetMaxEffectsDetail | RELOC | the same-TU body of 0x004083E0 (see below) |
| 00408500 | GetMaxTextureDetail | RELOC | same TU |
| 00408550 | GetMaxShadowDetail | RELOC | same TU |
| 004085a0 | GetMaxMeshDetail | RELOC | same TU |
| 004ff750 | CreateRelevantPersonAndUpdateStats | RELOC | same-TU `CDefClassBase::GetSubDef<T>` body |

- **0x0051CDF0 source.** It is `rebuild/src/compiled/00/50/CWorldMap_CWorldMap_005066e0.cpp` verbatim.
  - Its pragmas (auto_inline/pack) and the SHARED_HEADER_AVAILABLE warnings are inherited from that landed file.
- **0x004084B0..0x004085A0 lever.** The TU defines the retail TU's own out-of-line 0x004083E0 `GetConfigOptionsDefaultsDef` and the `GetDef<T>` template body. The same file grades RELOCATION_MATCH at 0x004083E0.
  - With 0x004083E0 only declared, 14 attempts were stuck at 84v67.
  - With its body visible, VC7.1 sees that the returned CDefPointer slot is not captured and reuses it, which is retail's shape.
- **0x004FF750 shape.** `creature = 0` init. Retail compares `noMales*femaleWeight < noFemales*maleWeight`, left side first.

## Parked: 0x005029A0 GetRelevantBuilding (DIFFER 334v337, 13 attempts)
- **Residue.** Register colouring only. Retail keeps the map reference in edi (spilled once) and the def in esi. Ours keeps the map in the frame and the def in edx.
- **What moved it.**
  - Cached end, the spaces local and the if/else-if arms took it from 372 to 334.
  - A same-TU STLport map::operator[] body halved the residue lines (67 to 41).
- **Next idea.** A same-TU body for PeekPSimDefinition 0x0082D2D0, which receives &def.
- The source is `differ/005029a0.cpp`. The oracle name `CBuilding_FindMostSpaceForNPCs` is wrong (see NAME_FIXES).

## Header overlays (hdrcheck2: the default path is the same for every file in hdr/)
- `fable_user_profile.h` (FABLE_RETAIL_CUSERPROFILEMANAGER) declares GetConfigOptionsDefaultsDef and GetMax{Effects,Texture,Shadow,Mesh}Detail.
- `engine/CStartupDefinitionManager.h` gets a new `FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER` branch: `: public CDefinitionManager` (both 0xd0) plus `static Get()` __fastcall.
- `engine/CTCVillage.h` gets a new `FABLE_RETAIL_CTCVILLAGE` branch: `: public CTCBase` (0xc) plus the generated members at the same offsets (sizeof 0xe8) plus `AddThingToVillage(CThing&)`.
  - Chunk 720 works CTCVillage rows and may add its own branch. Merge them.
- `engine/CThingCreatureBase.h` (FABLE_RETAIL_CTHINGCREATUREBASE): the flag bitfields become public (FollowHeroThroughTeleporters).
- `engine/CThingAICreature.h` (FABLE_RETAIL_CTHINGAICREATURE):
  - Adds static Create(long, const C3DVector&, long, const CThingAICreatureInit&), AutoSetupSexuality(float) and inline SetAsAllowedToFollowHero.
  - The bitfield bytes are +0x1C8/+0x1C9. sizeof stays 0x1d8.

## Header debt left in sources
- 004084b0..004085a0 define CConfigOptionsDefaultsDef (Ego_r members at retail offsets, sizeof 0x74). No engine header exists for it.
  - They also define CNewFrontendGameComponent with its inline Get over 0x013B871C, using the bound spelling `m_pNewFrontendGameComponent`. Its PDB data name is unproven.
- 004ff750 defines CVillagePeopleDef (Enable +0x25 … ProbabilityOfHomsexualityPerDialogueEnabledMan +0x34). No header exists for it.
- 005029a0 defines CSimBuildingDef (Property +0x3c, WorkSpaces +0x40, LiveSpaces +0x44). No header exists for it.

## Not attempted (budget)
| addr | function | size |
|---|---|---|
| 0x00501150 | CWorldMap::LoadLevelFromFiles | 756 |
| 0x00409730 | CUserProfileManager::VerifySignature | 1078 |
| 0x0040CEC0 | CUserProfileManager ctor | 977 |
| 0x00509A20 | InitializeMapNavigationHeightmap | 3120 |

## Side findings
- 0x00410890 is `CDefinitionManager::GetDef<CConfigOptionsDefaultsDef>`. From the generic template text it grades DIFFER 116v108. It is not a chunk row.
  - reloc_bindings has it as non-template QAE. The retail spelling is the const template member (NAME_FIXES).
- 0x00512FE0 `GetSubDef<CVillagePeopleDef>` grades RELOCATION_MATCH from src/004ff750.cpp.

## Round 2 (coordinator follow-up, 2026-11-03)
- **0x00501150 CWorldMap::LoadLevelFromFiles: RELOCATION_MATCH after 4 attempts.** Added to payload.json, oracle.tsv and test/.
  - Levers:
    - (a) A new CLevelLoader retail branch with nested CObject / CLevelFiles (Ego_r members at STLport sizes).
    - (b) `FABLE_STLP_OUTOFLINE_UNINIT_FILL_N`: a same-TU noinline `__fastcall __uninitialized_fill_n`. Retail keeps that instantiation out of line (0x00513570). Inlined, it makes VC7.1 call the whole vector(n) ctor instead.
    - (c) `while (loadType == 0 && PLevelLoader->IsBusy()) RunSuspendableProcess();`. Retail's loop is not rotated: the load-type test is jump-threaded out of the back edge. `if (type==0) while(...)` and `for(;;){if(!..)break;}` both rotate.
  - New CWorldMap.h retail-branch additions: the nested `ELoadType` enum and the protected LoadLevelFromFiles declaration.
- **0x0040CEC0 CUserProfileManager ctor: parked at DIFFER(1015v977) after 10 attempts.** The source is `differ/0040cec0.cpp`.
  - Residue: retail calls `__uninitialized_copy<CDisplayModeDesc*>` (0x0040DDC0) out of line inside the inlined first push_back growth, and calls the second push_back's `_M_insert_overflow` (0x004124C0) out of line.
  - With the copy inline, the second growth stays out of line as in retail, but the temporaries sit one slot off (122 lines). With the copy out of line (`FABLE_STLP_OUTOFLINE_UNINIT_COPY`), VC7.1 inlines both growths (1164 bytes).
  - What worked:
    - The same-TU 0x004083E0 body: the def is released in the `dec [ecx+4]` form with no zero store.
    - The FableWin C2DExtentsI user copy ctor (`FABLE_RETAIL_C2DEXTENTSI`): by-value arguments are built in place.
    - A scoped, named "Default" CWideString.
    - modes declared before def, and a for-scoped iterator.
  - Header opt-ins: `FABLE_RETAIL_CUSERPROFILEMANAGER_MEMBERS` (the opaque +0x54..+0xCB and +0x100..+0x13F blocks as Ego_r members) and `FABLE_STLP_DEQUE` (STLport deque/queue).
  - CDisplayManager.h gains the EnumerateDisplayModes and AddDeviceResetCallback declarations.
- **0x005029A0: the proposed PeekPSimDefinition same-TU stand-in made it worse** (315 bytes, 63 lines). 0x0082D2D0 lives in CThingBuilding's TU (landed `CThingBuilding_PeekPSimDefinition_0082d2d0.cpp`), so retail never saw that body. Re-parked after 16 attempts.
- **0x00509A20 is identified but not drafted.** It is `CWorldMap::GetMapNavigationAreaUpdateInfo(long, const C2DBoxI&, const CCharString&, CNavigationAreaUpdate&) const` (see NAME_FIXES).
- **0x00409730 VerifySignature was analysed but not drafted.** It has a /GS frame, CMemoryDataInputStream reads with inlined 64-bit guards, and a chunked compare.
- **Include-order gotcha.** `#include "stlport/fable_containers.h"` must come before `fable_string.h` when the overlay changes fable_containers.h. Otherwise rebuild/include/stlport/fable_string.h includes "fable_containers.h" relative to its own directory, the original wins the include guard, and the overlay is ignored.
