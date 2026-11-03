# Landing: Linux hand chunks 718-725 (2026-11-03)

**Coordinator:** one landing round for every matched row of
`rebuild/candidates/manual/pending/2026-11-03_linux_hand718` … `hand725`.

**Payloads:**

| Chunk | Payload |
|---|---|
| 718 | `payload.json` + `round3` |
| 719 | `payload.json` (round4 sources) |
| 720 | `round3` + `round5` |
| 721 | `payload.json`, with the `retail_asm/` sources |
| 722-725 | `payload.json` |

**Headers:** one merged overlay; see `HEADER_NEEDS.md`.

**Result:** 91 rows landed and 0 held. `bindings_multi_check` printed 0 after every chunk.

## Rows per chunk
| Chunk | Landed | MATCH | RELOCATION_MATCH | Notes |
|---|---|---|---|---|
| 718 | 11 | 1 | 10 | CGameScriptInterface quest and camera members; CPlayer accessors. `CRenderManager2D::Draw2DText` and `CWorld` game-time accessors folded into the class branches |
| 719 | 8 | 0 | 8 | CWorldMap (DrawIsPosInMap, vector deleting dtor, LoadLevelFromFiles with the `FABLE_STLP_OUTOFLINE_UNINIT_FILL_N` opt-in); CUserProfileManager GetMax*Detail; CreateRelevantPersonAndUpdateStats |
| 720 | 3 | 0 | 3 | 0x005C552C, 0x005C36C2, 0x0065B854 (CTCVillage::AddThingToVillage) |
| 721 | 6 | 2 | 4 | CEngineLightingManager. 0x00B44820 and 0x00B44A60 are `retail_asm`. 0x00B46280 now uses `PeekMinCoord()`/`PeekMaxCoord()`. CNavigatorAStar::UpdateGoal |
| 722 | 31 | 0 | 31 | CEngine forwarders 0x00B23B80..0x00B24020; chunk-local global spellings respelled to the tree's `g_pEngine*` names |
| 723 | 6 | 0 | 6 | Engine renderer dtors, CEngineWaterRenderer::WorldFrameUpdate, CEnginePrimitiveManagerParticleSpriteGroup ctor, CNavigatorSwitch::SendStateAndReset |
| 724 | 3 | 0 | 3 | 0x00B1BDA0 gained its own `// Retail-asm: 0x00B1BEAB fistp dword ptr [esp+14h]` line (purity: callers of the shared fistp helper need one). 0x0087AA10 and 0x0087AFF0 use namespace-scope `const double TWO_PI` |
| 725 | 23 | 12 | 11 | CDisplayEngine Pre/PostChangeResolution, CGameCameraManager, CTCCreatureNavigation (8), CThingSoundEmitter, CXMVPlayerBuffers dtor, CChunkedFileChunk dtor, nine placeholder-vtable dtors |

Per-row grades are in `STATUS.tsv`, with the read-only regrade and the land grade. The logs are in `logs/`:
`grade_*`, `land_*`, `multi_*` and `spell_*`.

## Held
None. Every matched row regraded PASS on the merged overlay.

Rows that are still DIFFER, PARKED or NOT_ATTEMPTED, and chunk 725 round2's later match 0x00AA5C80 (not in this landing),
are listed in `PARKED.tsv` with the best grade and residue. Their `differ/` sources stay in the pending directories.

## Includer recheck
- **2,824** landed sources and tests include a changed header.
- **2,792** of them preprocess token-identically.
- **32** differ, and all 32 are `RELOCATION_MATCH PASS` on the final headers (`includer_recheck_grades.tsv`).

The repo's `recheck.py` and `vland --recheck-landed` ran in 4 splits, after one warm-up grade.

## Landed-row change: 0x00938DB0
`CAddFreeSeatToList::operator()` was respelled from `vector<CThing*>` to `vector<CThingObject*>`. Evidence: Ego_r
`??0CAddFreeSeatToList@@QAE@AAV?$vector@PAVCThingObject@@...`.

It used to bind `vector<CThing*>::push_back` to 0x0074FAE0. That spelling belongs to 0x00436C80 (chunk 720 row
0x005C552C). The row now binds `?push_back@?$vector@PAVCThingObject@@...` to 0x0074FAE0. Its source and test were
re-landed: `repair_remove` first, then `vland --land`, giving RELOCATION_MATCH.

## Naming
- **Spellcheck:** `spellcheck.py` ran on every chunk after landing (`logs/spell_*.txt`).
- **Queue:** 86 lines were appended to `naming/NAME_FIXES_QUEUE.tsv` (c718 1, c719 3, c720 8, c722 28, c723 31, c724 4,
  c725 11). Three of them settle addresses that already had open one-name review lines: 0x01436E80 (sec6b),
  0x01436E98 (c600) and 0x01436E9C (c711). No other line repeats a queued address, wave 34's lines included.
  Each line is one of these:
  - a PDB spelling bound by a chunk row while landed binders spell the address differently;
  - a chunk NAME_FIXES row whose landed leaf spelling is proven wrong (CEnginePrimitiveRenderer_Methods → CEnginePrimitiveRenderer, CEngineMapManager → CEngineWorldMap, CWeatherState → CEngineWeatherRenderer, AddExclusiveObserver → CEngineCamera::SetGameWindow, and others);
  - a REVIEW for an unproven placeholder against a landed placeholder (0x00C57310, 0x00C577B0, 0x012B32A0, 0x012B33A4).
- **0x01436E98:** settled on `?g_pResourceManager@@3PAVCEngineResourceManager@@A`. The other landed spellings are queued
  as PRIORITY c723.
- **0x00C426E0:** queued as `?Close@CVtable_012b4f9c@@UAEXXZ`. Chunk 725's NAME_FIXES wrote `QAE`, but slot 2 of the
  vtable is virtual.
- **TWO_PI 0x0124F2B8:** stays an internal-linkage `const double`, which VC7.1 decorates as `_TWO_PI`. An `extern`
  declaration (`?TWO_PI@@3NB`) DIFFERs (1504v1504). The address keeps two spellings in `reloc_bindings`; c636 and
  ESTABLISHED already cover it.
- **Labels:** `ghidra_out/labels_chunks718_725_2026-11-03.tsv` holds 347 labels. `apply_labels.py` renamed 163 catalog
  rows and 187 manifest rows. Then `sync_catalog_names.py` and `sync_manifest_from_catalog.py --labels` ran: 473 landed
  rows were resynced.
- **wchar_t:** the `G` vs `_W` mangling question was left alone. It is an open global decision.

## Gotchas found
- **hdrcheck2 stubs:** `hdrcheck2.py` writes stubs into the tracked `coordinator/stubinc/`, which comes first in `-I`.
  The stubs shadow real headers in later checks. Clean them with `git clean -fq <lane_tools>/coordinator/stubinc`.
- **CDefString:** it needs one model, with `operator CCharString()` forwarding to `GetString()`. A GETSTRING-only model
  breaks 0x00775C60; the old model breaks 0x0065B854.
- **push_back COMDAT identity:** the pointee type picks the COMDAT. Take it from the PDB constructor's parameter types,
  not from what the body passes to `GetAllCarriedThings`.
- **`static const double` at namespace scope:** VC7.1 binds it as `_TWO_PI`. An extern without an initialiser changes
  the codegen.
- **Per-TU inline choices stay separate opt-ins:** `CBASICSTRING_EQ_OUTOFLINE`, `CVECTORMAP_FORCEINLINE_FIND` and
  `STLP_OUTOFLINE_UNINIT_FILL_N`. Folding them changes bytes in landed rows.
- **`repair_remove`:** it takes a bare address (`00938db0`, not `0x00938db0`). `vland --land` refuses a row that is
  still landed.
