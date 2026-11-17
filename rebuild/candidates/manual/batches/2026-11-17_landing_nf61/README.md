# 2026-11-17 landing nf61: name-fix round 7, NF61 (107 rows: follow-up entries queued at nf58)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF61 took the 85 follow-up entries queued at the nf58 landing (NAME_FIXES_QUEUE.tsv q10552..q10636): the per-instance STLport tree
helpers (`_M_create_node` "buyNNNN" rows, `_M_copy`, iterator `operator++` / `operator--`) whose callers NF58 had moved onto the
STLport spellings, and 35 out-of-slice binders. That is 137 rows. Brief, worklist and records: `naming/2026-11-17_agentNF61/`.

## Rows (137, all terminal; STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 107 |
| not-a-binder | 28 |
| blocked | 2 (unchanged) |

**Ported rows:**
- **76 tree self renames:** 43 `_M_create_node`, 27 `_M_copy` and 6 iterator `operator--` / `operator++`, each now an explicit
  instantiation from stlport/fable_map.h. Their catalog modules use exactly the instance spellings already in the catalog (70 of 76
  match an existing row's module; 6 are instances not catalogued before).
- **31 other rows moved onto shared headers:** 3 `map::operator[]` rows and 11 `GFSerialiseMapBinaryIn` instances on
  engine/CPersistTraits.h (each now catalogued with its template arguments; before, eight were a bare `GFSerialiseMapBinaryIn` and three one malformed
  template name); 0x00851CC0
  `CHeroAbilityStatPackage::AddStatThreshold` (was `CRetailMap_00851cc0::Add`), 0x005812AE `CTCHaste::SetActionSpeedMultiplier` (was
  `CBlockTable_005812ae::GetBlockData`), 0x00CD36C2, the CPersistTraits TransferBinaryIn rows 0x006DC070 / 0x006DBE00,
  `NThingMap::CThingMap::GetAllThings` 0x0085D010, CTCInventoryExperience BuyStat / BuyAbility 0x005786B5 / 0x005785B3, 0x00AD91F0
  and 0x00A7A7F0.
- **7 of those are partial ports** (0x009CDA70, 0x008E6780, 0x0078B410, 0x00694020, 0x005CDCCB, 0x005907AB, 0x0047E07F): their
  container and CTCHaste calls now use the shared headers and bind the right spellings, but they still model other classes locally
  (CBankFile, CTCInventoryStats, CTCInventory, CTCInGameMenu, CCreatureActionBase, CThing, action classes). vland reports 24
  SHARED_HEADER_AVAILABLE warnings on them, all on classes the rows already modelled.

**Blocked rows:**
- **0x00868CA0** (CHitParameters::AddDamageWithAttribute): the re-authored row grades RELOCATION_MATCH (NF61 `work/ready/`) but was
  held back. Its PDB self name would split the binding of the landed caller 0x007E2600, which uses `...QAEXJM@Z`. The two must land
  together (queued).
- **0x0070A570** (CTCTextureDecal::PeekEngineDecals, DIFFER 815 v 518): about a dozen local classes, including a whole STLport
  implementation. It needs a CTCTextureDecal header and per-instance out-of-line control for `vector<CCompositeBlendDesc>` and
  `map<long, CEngineTextureDecal>::find`.

## Entries (APPLIED.tsv, 85)
- **79 done.** REJECTED.tsv is empty: every queued claim passed its two-source check.
- **6 blocked:** q10559 (0x00868CA0), q10560 (binder 0x0046190A outside the slice), q10580 (0x0070A570), q10584 / q10585 (binders
  0x00A84640 / 0x00A84BE0 outside the slice) and q10636 (the coordinator's CWeaponTrailGraphicSet header change).

## Rows and checks
- **Section 1: 96 self renames (168 file moves). Section 2: 126 respelled bindings in 61 caller rows.** NF61 checked each rename
  against landed callers outside the slice: 66 now match, 39 already used another spelling before (all in NAME_FIXES), none is
  newly split.
- **Funclets:** no EH funclet includes a changed row. funclet_includes_check reports 1,355 checked, 0 broken.
- **Recheck (`logs/recheck_part0..2.log`):** all 107 changed rows graded from the repo after landing: 101 RELOCATION_MATCH and 6 MATCH,
  every test PASS. NF61's own final run agrees.
  - The PASS results come from the repo's own tests (NF61 changed none). Several tests embed an older copy of the row source
    (0x005CDCCB, for example), so PASS shows they still build and behave, not that they exercise the new text.
- **Checks:**
  - `naming/rebind.py` removed 155 binding lines and added 101;
  - bindings_multi_check is at its baseline (`NullCoord`).
- **Known, not new:** 0x005907AB's local `CAction()` binds 0x0042BED4 here but 0x0042BE50 in landed 0x0056F615 (a pre-existing
  RELOC_CONFLICT). 0x005CDCCB still defines `?FrameUpdate@CTCInventoryStats@NInventory@@QAEXXZ` where the PDB has the virtual
  `?FrameUpdate@CTCInventoryStats@@UAEXXZ` (queued).

## Headers (11 opt-ins in 8 files; lines added only, default path unchanged)
stlport/fable_map.h (`STLP_TREE_MCOPY_BODY`, `STLP_MAP_CTOR_NOINLINE`), stlport/fable_containers.h
(`STLP_PAIR_DEFAULT_CTOR_NOINLINE`), CEnginePrimitiveHandle.h (`_COPY_DECL`), CWeaponDef.h (`CWEAPONTRAILGRAPHICSET_CTOR`), CTCHaste.h
(3), CTCInventoryExperience.h (`_BUY`), CTCHeroExperience.h (`_SPENDEXPERIENCE`) and CNavQuadTree.h (`_CLEARNODES`). Details:
`naming/2026-11-17_agentNF61/hdr/README.md`. Eight landed rows that include these headers but use no opt-in preprocess to the same
tokens with or without the overlay (`hdr/overlay_token_check.txt`).

## Name fixes queued (`land1117fu`, 59)
From NF61's NAME_FIXES.tsv: the 39 out-of-slice callers that bind another spelling of a row NF61 renamed, the 0x00868CA0 /
0x007E2600 pair, the q10560 / q10584 / q10585 binders, and 0x005CDCCB's FrameUpdate.

## Rule slips (NF61 `slips.txt`)
- Five Bash calls exited non-zero (two read-only checks, a generator KeyError, a shell-parse error that ran nothing, an IndexError).
- One shell-escaped `\&` reached a row and cost one grade.
- NF61 released the locks of its 30 unchanged rows (the brief does not ask for that).
