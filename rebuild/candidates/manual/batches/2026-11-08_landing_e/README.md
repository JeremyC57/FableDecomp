# Landing 2026-11-08e: the four saved chunk 854 matches, and the CMouseDX ctor respelled

Chunk 854 was stopped mid-run (`pending/2026-11-08_linux_hand854_stopped/`, the user: "No more authoring"). Its four agent-graded
RELOCATION_MATCH sources were landed here as the handoff's first resume job; no agent was started and nothing new was authored
beyond the two missing behaviour tests.

| Row | Grade | Note |
|---|---|---|
| 0x00AB58E0 CMouseDX::Update | **RELOCATION_MATCH + PASS, landed** | base flags (the first grade's base compile hit the 180 s Wine timeout and matched only under the `optimize("t")` variant; regraded alone it matches on the base flags, which is what landed) |
| 0x0083EFE0 LHNetworkLib::LHHttp2::ReceiveDocumentAsync | **RELOCATION_MATCH + PASS, landed** | coordinator fix: the source declared only `operator delete[]`, so `new char[n]` called scalar `??2` where retail calls the MSVCR71 `new[]` thunk 0x00BFEB22 (vland RELOC_IDENTITY; the agent's grader masks call targets). Added `void* __cdecl operator new[](unsigned int);`. Test written here. |
| 0x00501D30 CWorldMap::GetLevelFilesFromWorldFile | **RELOCATION_MATCH + PASS, landed** | int3-after-noreturn: graded and landed against the N+1 row (1087 bytes, the 0xCC after the final `call _CxxThrowException`; CreateWorldLevelWad starts at 0x00502170) |
| 0x00924C70 CAIStateGroup_KrakenAttack::Update | **RELOCATION_MATCH + PASS, landed** | test written here (the phase machine) |
| 0x00AB5D00 CMouseDX::CMouseDX | **RELOCATION_MATCH + PASS, re-landed** | respelled `CMouse::ClearStoredEvents` (no such PDB name) to the PDB `protected: CMouse::EmptyEventStore` 0x00A66B10, the spelling 0x00AB58E0 binds; removed with `repair_remove.py`, bytes unchanged |

- **Headers landed** (each an opt-in `#ifdef` branch; `coordinator/hdrcheck2.py` printed `default=same` for all four, and no landed file
  defines the new macros, so every landed includer compiles the same text):
  - `engine/LHHttp2.h` FABLE_RETAIL_LHHTTP2_RECEIVEDOCUMENT (LHHttpDocumentParts' inline zeroing ctor; ReceiveDocumentAsync, GetUntilCRLF,
    IsDataAvailable, ResetDocumentRecv declarations);
  - `engine/CAIStateGroupBase.h` FABLE_RETAIL_CAISTATEGROUPBASE_COMBATPERFORMACTION (the PDB `IAE` / const-reference / EAIActionReturn spelling);
  - `engine/CCombatActionBase.h` FABLE_RETAIL_CCOMBATACTIONBASE_SETCOMBATANIMATION;
  - `engine/CThingAICreature.h` FABLE_RETAIL_CTHINGAICREATURE_IMMUNETOATTACK (the inline FableWin `SetAsImmuneToAttack`).
  - Not landed (serve only the parked 0x00507070): the CWorldMap STATICMAPVISIBILITY and CGameDefinitionManager GLOBALDEF opt-ins stay in the pending dir.
- **Header debt** (vland WARN SHARED_HEADER_AVAILABLE, non-blocking, as in earlier landings): 0x00AB58E0 models C2DExtentsI, C2DVector,
  CDisplayManager, CInputEvent, CMouse, CSurface, CSystemManager locally; 0x00501D30 models CBaseClass, CBaseClassNonCopyable,
  CExceptionBase, CParsedItem, CWorldMap locally; the re-landed 0x00AB5D00 keeps its existing local CMouse / CSystemManager models.
- **Names:** `ghidra_out/labels_landing_2026-11-08e.tsv` (the four rows; CMouse::EmptyEventStore 0x00A66B10; CCombatActionBase::~CCombatActionBase
  0x0062E900, was a propagated CCreatureAction_CastWillFireballSpell label; CTCKraken::IsTargetAlive 0x007FB120, **descriptive**: one caller,
  in neither PDB name set). Queue `land0811e`: 0x00A66B10 (closed here), 0x0097DA40 (PRIORITY: the landed binders use four older spellings of
  AIAction_CombatPerformActionAndWait).
- **Catalog sync:** `tools/sync_catalog_names.py` would also rewrite 4,284 other catalog / manifest names from landed Leaf lines (a backlog,
  some to placeholder stand-ins such as 0x00401377 `T::Get`). Only this landing's addresses were kept; the backlog needs its own review pass.
- `bindings_multi_check.py` prints 1, the pre-existing `?NullCoord@CScriptThing@@1VC3DVector@@A` (0x0143E8E0 / E4 / E8, same at HEAD).
