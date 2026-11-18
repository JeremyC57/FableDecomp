# 2026-11-18 landing egod -- the other account's Ego_d lane (FullDecomp-B): 68 rows landed (31,896 bytes), reservation released

Owner: "The other account did as much as they could check their work on full decomp b branch and merge it here. And unreserve their
reserved rows since their usage is used up so they can't work anymore for now", then "Also land the rows that it staged since they
should be ready".

## Merge
`FullDecomp-B` (45 commits, b4a14beade) fast-forwarded `FullDecomp`: it already contained FullDecomp's head (44a3be4a8b, merged in
399b3fbbbe), and it changed only `rebuild/candidates/manual/pending/` (the egod lane's staged and open folders), the egod ledger and
lane tools, `docs/pipeline/ASM_RULES.md` and `tools/decomp_pipeline/crawl/purity.py` (below). No header, source or catalog change came with it.

## Landed: all 68 MATCHED_STAGED rows
From `pending/2026-11-17_egod_e01` .. `_e16` (no e09 folder), each already author-matched, independently rule-audited and coordinator-verified
on the other account (`coordverify.log` in each folder). Here: one combined `payload.json` (source / test text from the folders' `src/` /
`test/`), an oracle built from Fable.exe in scratch, true lengths from the folders' `INDEX.tsv`.
- **Dry run** (`logs/vland_dry.log`, `LANDVERIFY_STRICT_HEADERS=1`, the merged headers as overlay): 68 / 68 RELOCATION_MATCH + PASS. 0x00408820
  matched there only in the lander's pragma sweep; alone it matches plainly (`logs/vland_dry_00408820.log`).
- **Landing** (`logs/vland_land.log`, strict headers, repo headers): 68 RELOCATION_MATCH + PASS (`LANDED.tsv`).
- **One coordinator change:** 0x00B81820 CEngineSimulationManager::FrameStart (e03) declared a local `CEngineClothSimulationMeshBase` with
  only ClearStaticParticleSims, written before e08 added that class's shared header; it now includes engine/CEngineClothSimulationMeshBase.h
  (same bytes).
- Purity scan: no `__asm` / `naked` / `_emit` in any row or test (`ptc_emitter` was a false hit); `#pragma intrinsic` / `optimize` and three
  `// Flags:` rows (0x0086BE40 `/d2inlT49`, 0x00954F40 `/GS`, 0x00B3FC40 `/d2inlT444`, the last decided on FullDecomp-B) carry their notes.
- Lander warnings, none blocking: 3 RELOC_CONFLICT (queued: 0x00911300 v landed 0x00788D68 on the CIntelligentPointer<const CThing> copy
  ctor, 0x008F0BD0 v 0x007DAEF0 on `_mult_to_stay_off_table`, 0x009FB750 v 0x00996610 on `vector<unsigned char>(size)`), 23 RELOC_NAME,
  3 RELOC_ALIAS, 3 STANDIN_LEAF (catalog placeholder names the landing replaced).
- Funclets: 1,355 checked, 0 broken; bindings_multi_check at its baseline.

## Headers (`HEADERS.txt`: 86 files, 16 new)
Each folder's `hdrdiff/*.diff` was applied to today's header (all applied cleanly: the lane rebased them onto 44a3be4a8b) and the versions
were merged per header with `git merge-file`, base = the repo header. Five headers had two versions: four merged cleanly; in
CEngineLightingManager.h e11's version is e08's plus one opt-in, so e11's was taken. `NEW_*.h` copies became 16 new engine headers (e03 and e08
handed back identical CEngineSimulationBase.h / CParticleContext.h). Checks: no conflict markers, every `#if` balanced, no directive block
duplicated beyond its versions, `lane_merge/assert_names_check.py` 0 mismatches against 49b943df65, and with no opt-in defined every header
preprocesses as before (hdrcheck2: `default` same). One fix: e02's diff added a second identical `multimap::empty()` under
`FABLE_RETAIL_STLP_MULTIMAP_EMPTY`, which NF67 had already landed (used by 0x00869020); the duplicate block was dropped.

## Reservation released
`pending/2026-11-17_egod_parked/RESERVED.tsv` is now `RELEASED.tsv`; UNMATCHED_ROWS.tsv has no `reserved (egod ...)` row left: the 68 landed rows
are gone, the other 706 are `parked` again. The 34 PARKED rows with a draft point `closest_version` at it in `pending/2026-11-17_egod_open/`
with the residue in `best_grade_on_record`. The ledger `STATUS.tsv` marks the 68 rows LANDED.
- **Not landed, back in the pool with notes:** 0x009D8650 MicroThread::MicroThreadSleep (matched through a whole-function `naked` asm helper
  header; see below) and 0x009FDE80 CGraphicInfoFrame::CFrameData::BeginLoading (matched by the author just before the wind-down, never
  verified or audited; `pending/2026-11-17_egod_open/e17/`).

## For the owner: decisions that came with the merge
FullDecomp-B's coordinator recorded four "owner-delegated" decisions (`pending/2026-11-17_egod_parked/README.md`). Three only concern rows
(the X-first GFGetWorld2DDistance opt-in, row-local MIN / MAX macros, `/d2inlT444` on 0x00B3FC40) and are in this landing. The fourth amends
`docs/pipeline/ASM_RULES.md` and `crawl/purity.py` to accept a whole function written `__declspec(naked)` in a shared helper header (first case
0x009D8650, whose portable twin is not yet portable on its own). That is a change to the asm rule, so 0x009D8650 is not landed here.

## Name fixes queued for later (`land1118eg`, 77)
The lane's NAME_FIXES.tsv (74) and the three binding conflicts. No naming agent runs now (owner, 2026-11-17).
