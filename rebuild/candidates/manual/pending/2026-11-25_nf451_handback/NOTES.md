# NF451 (2026-11-25): landed-row recheck (NF448 DIFFERs) + NF450 rulings r450a / r450b done through header opt-ins

## Task 1: the five rows NF448 saw DIFFER (official grader: recheck.py + vland --recheck-landed, catalog flags; t1/recheck.log)
Note: the numbers are bytes (object text v retail length), not instructions (coverage.tsv sizes 54 / 2983 / 2654 / 365 / 8).
| row | official regrade | cause | fix |
|---|---|---|---|
| 0x0072A680 | RELOCATION_MATCH PASS | NF448 census artefact: the source has no `// Leaf:`; nf.compile_one used the catalog name `~CTCActionUseBuyOrSteal`, which matches no symbol ("WARN leaf ... matches no symbol; graded the largest function", NF448 work log). recheck.py maps it to `??1CTCActionUseBuyOrSteal@`. | none (optional: add `// Leaf: ??1CTCActionUseBuyOrSteal@@...` comment) |
| 0x01214190 | RELOCATION_MATCH PASS | NF448 census artefact: EH unwind funclet row (`#include` of parent 0x00C31F00's file); graded on the parent's funclet block at +0x0. NF448's `Unwind@01214190` leaf matched no symbol, so it graded the parent's largest function (95 v 8). | none |
| 0x00BE7D70 | DIFFER(1437v2983) PASS | pre-existing, never a byte match: | real re-decomp (not mechanical); no batch |
| 0x00BE6F70 | DIFFER(198v2654) PASS | same | same |
| 0x00BE8920 | DIFFER(328v365) PASS | same | same |

Landscape rows evidence (no header / flag / owner change caused it):
- rebuild/compile-gate/retail-parity.tsv (Windows-lane record) already lists all three DIFFER with the same object sizes 328 / 1437 / 198
  and first difference at byte 0-2; parity_audit.tsv the same verdicts; self_symbols.tsv status column = DIFFER(...) for all three.
- Every includer recheck since nf108 shows the same numbers (batches nf127/131/149/151/152/172/181/188/192/269 logs:
  "July candidates-header rows that grade DIFFER at HEAD").
- The sources are July behaviour-only rows over `candidates/landscape_background_patch_*.h`: they call made-up helpers
  (BeginLandscapeBackgroundRenderState, DrawLandscapeBackgroundLayer, LoadLandscapeWaterBackgroundSubPatch...) where retail inlines
  (Render 198 v 2654 bytes). No catalog CompilerFlags, no option S / funclet owner. vc71-compiled.tsv records them PASS (compile +
  behaviour test), retail-parity.tsv DIFFER. Not checked: how purity / the dashboard count them.
- Last change pinned as far as the logs go: nf127 (2026-11-20) logs/differ_head_check.txt regraded them "with HEAD headers and their
  unchanged sources" to the same DIFFER numbers; every recheck since (nf108..nf269, now) gives the same numbers, so neither a header
  nor a flag change since then caused it. Earlier history needs git, which this lane may not run.

## Task 2: r450b (batch b1_hdr, 10 rows ACCEPT) and r450a (batch b2_hdr, 1 row ACCEPT)
- Root cause of both declines: `stlport/fable_containers.h` (and engine/CArray.h, which includes it) emits `/DEFAULTLIB:"libcp"`
  (bisected with one-include TUs, work/bis). CPlayerGui.h (GUIMODE enum-only), CEnginePrimitiveHandle.h, CIntelligentPointer.h,
  CPlayerGuiDef.h, C3DVector.h, CRightHandedSet.h do not.
- engine/NPlayerGui_CStatBase.h did not even compile on its own (needs CPlayerGui / CEnginePrimitiveHandle opt-in macros).
- NF450's four DTORX rows used a bodiless `namespace NPlayerGui { class CStatBase { ~CStatBase(); }; }`: also a local model of a
  headered class (NF450's "(no engine header)" was wrong). All ten now include the header. descriptive_check proves the ten belong
  together: the four alone leave OTHER_SPELLING (1 FAIL at HEAD; 0 FAIL after all 4 deletes, work/sim).
- b1_hdr opt-in `FABLE_RETAIL_NPLAYERGUI_CSTATBASE_NO_CONTAINERS`: whole class body without the two vectors, no STLport, includes
  fable_compiler.h + CPlayerGui.h (row defines FABLE_RETAIL_CPLAYERGUI + FABLE_RETAIL_CPLAYERGUI_GUIMODE); sizeof 0x30; original text
  untouched under #else. Six model rows: `class X : public NPlayerGui::CStatBase { unsigned char _body[0x64 - 0x30]; ...}` (polymorphic
  base adds no vptr store: bytes equal). Extension of the CDecal-style opt-in to CStatBase: say so to the coordinator.
- b2_hdr opt-in `FABLE_RETAIL_CDECAL_NO_CONTAINERS` (define INSTEAD of FABLE_RETAIL_CDECAL): CDecal declarations without the CArray
  members / CArray.h; sizeof 0x5C; pinned FABLE_STATIC_ASSERT_AT(76).
- Header includer checks: CStatBase.h (8 rows + 2 tests via CStatBar / CCrimeStatBar / CMultiStatBar): equiv_check differs only in
  the unpinned FABLE_STATIC_ASSERT line-number typedef name, so real VC7.1 recheck HEAD v new (batch b1inc): objgate ALL_EQ 8/8,
  grades equal (2 MATCH, 6 RELOCATION_MATCH, all PASS). CDecal.h (10 includers): equiv_check 0 differing lines.

## Gates (HEAD v new, one LANDVERIFY_WORK path; n run with FABLE_HDR_OVERLAY=batch hdr/)
| gate | b1_hdr | b2_hdr |
|---|---|---|
| vland normal | 10/10 RELOCATION_MATCH PASS both; only RELOC_NAME warns removed (10) | RELOCATION_MATCH PASS both; 2 RELOC_NAME removed |
| vland strict | identical, all PASS | identical PASS (STANDIN_LEAF warn on self leaf both sides, pre-existing) |
| objgate (incl .drectve, UND old->new) | ALL_EQ 10/10 | ALL_EQ |
| name_gate --test | NO_WORSE 10/10 (LOCAL_CLASS CRecordBase / CBase_00443220 gone) | NO_WORSE |
| pk + reachflip | BAD 0; 10 pairs, 0 NEQ flips | BAD 0; 1 pair, 0 flips |
| /FORCE link | dup 1->1, NEW_UNRESOLVED 0, new names defined by target 1/1, stand-ins 4->0 | dup 0->0, 0 unresolved, 2/2, 2->0 |
| xrcheck | 10 checked, 0 bad | 0 bad |
| reserved_check | 0 | 0 |
| apply_table --dry | delete 4, 0 misses, multi-target 1->1 | delete 2, 0 misses, 1->1 |
| head/payload = repo, orig drift | 0 | 0 |
| descriptive_check --extra | HEAD 1 FAIL (OTHER_SPELLING of the 4 deleted spellings); after deletes 0 FAIL | no guessed name |
| tests | unchanged (vland PASS, PassPattern printed) | unchanged |
- Screen: no Account-B claim. T3: b1 resolves nf303 0x00442280 fully; b2 resolves nf303 0x00AF7C10 / 0x00AF8AD0 partly (0x00AF7CA0's
  ?CDecal_Dtor / ?CDecal_OpDelete stand-ins remain: self-identity work).
- Landing order: b1_hdr and b2_hdr are independent (no shared header, row, or table key). Each is a header batch: apply HEADERS
  (hdr/engine/*) with the rows; base copies in orig/hdr_<name>.

## Findings
- 0x00443190 / 0x004431C0 / 0x004431F0 family: layout = CStatBar (base 72 + six words + FontBank +0x60, list at +0x64; FableWin
  ??1CStatBar@NPlayerGui@@QAE@XZ QAE and list<CStatBar::CChangeText>; entry = CRGBColour(16) + CWideString). Self symbols are
  stand-ins (CRecordWithEntryList): identity work, not done here.
- The landed target row 0x00442280 itself models CStatBase locally ("no matching engine header exists", Unproven slots): header debt.
- 0x00443CF0 (Task 3 first item) is not a header port: its model reads +0x154/+0x158 (CMiniMapDisplay retail sizeof 0xC0) and the self
  is a free __fastcall `?ClearPrimitiveHandles@@YIXPAVCMiniMapDisplay@@@Z`: identity first.

## Locks
NF451 holds 00443190 004431c0 004431f0 00443220 00443bd0 00443c10 00443c50 00443ca0 00444e10 00444e50 (taken over from NF450, which
handed them back held for r450b) and 00af7c80. Release before landing.

## Task 3: b3_hdr (0x004582BA -> 0x004582D6) and b4_hdr (0x00459AA2 -> 0x00459ABE)
- Both caller rows are scalar deleting destructors that called a free `extern "C" __fastcall <Class>_dtor` stand-in. Ported to a
  qualified destructor call through the engine header's new add-only destructor-only opt-in (both full headers pull STLport ->
  libcp): FABLE_RETAIL_CSPECIALABILITIESUNHOLYPOWERDEF_DTOR_DECL_ONLY / FABLE_RETAIL_CBRAINDEF_DTOR_DECL_ONLY (class + retail base
  CParentDefClassBase + `virtual ~X();`, no members). .drectve unchanged (LIBC, OLDNAMES), RELOCATION_MATCH.
- Both destructor names are FableWin.pdb only (egor_pdb_names / egod_pdb_names have neither): BEST_GUESS.tsv one-source:fablewin-pdb in
  each batch; descriptive_check 1 FAIL (OTHER_SPELLING of the stand-in) at HEAD, 0 FAIL after the batch's table changes (sandbox copy
  with FABLE_ROOT).
- 0x00459AA2's HEAD test links the row object and stubbed `CBrainDef_dtor`: new test (batch tests/) names the class through the same
  header view (no local model), stubs ??1CBrainDef@@UAE@XZ with `/alternatename` to a fastcall stub (this in ecx, no stack args) and
  checks the this pointer and the delete flag. The HEAD test also had a local CBrainDef model (gone).
- 0x004582BA's HEAD test is a standalone model (no row link): unchanged.
- Includers: equiv_check copy against copy 0 differing lines (UnholyPowerDef.h 10 rows incl. via CSpecialAbilitiesDivineWrathDef.h;
  CBrainDef.h 359 rows via CAIBrain.h / CAIStateGroupBase.h).

## INCIDENT (fixed): I wrote the repo table once by mistake
While simulating b3/b4 TABLE_CHANGES I ran the repo's apply_table.py without --dry from a sandbox cwd; it writes
FABLE_ROOT (default /home/user/FableDecomp)/rebuild/oracles/reloc_bindings.tsv. It deleted 2 stand-in lines and appended 2 lines.
Restored within minutes from the pristine copy taken just before (work/sim_b3_hdr/...; diff showed exactly those 4 lines; cmp equal
after restore, sha256 4619503830fbb14d). The simulations were then redone with FABLE_ROOT=<sandbox>. No other repo file was touched.

## Resume point
- 0x00739E20 (nested CTCCreatureModeManager::CModeBase dtor, 6 caller rows) not started: needs a _hdr opt-in in
  engine/CTCCreatureModeManager.h declaring the public nested CModeBase destructor (check that header for STLport first: the same libcp
  trap; use a decl-only view if it pulls containers).
- 0x00443CF0: identity first (see Findings), not a header port.
- 0x0045782F (CSpecialAbilitiesPhysicalShieldDef scalar dtor) is the same shape as b3/b4: the same decl-only opt-in recipe applies.

## All four batches together (evidence/ALL_TABLE.tsv, ALL_BEST_GUESS.tsv)
- apply_table in a sandbox (FABLE_ROOT): delete 8, add 2, 0 misses, multi-target 1 -> 1, none new. descriptive_check --extra with the
  three BEST_GUESS files: 190 lines, 0 FAIL. No shared row, header or (symbol, target) key between batches: any landing order.
- b4_hdr strict vland: HEAD HEADER_POLICY (local CBrainDef model) -> new RELOCATION_MATCH PASS (improvement); others identical.
- work/b1inc_batch is a helper (includer recheck of CStatBase.h), not a batch.
- Locks also held: 004582ba, 00459aa2.
- Format: HEADERS.tsv = header, macro, rows (NF448 h1_hdr form); hdr/ holds only engine/<Name>.h; b1_hdr UNITS = one unit (00443190)
  for all ten rows, because the BEST_GUESS line is only consistent when all ten land.
- Lander's equiv_check on CStatBase.h will flag 10 lines (8 rows + 2 tests: 00439070, 00646f67) because the unpinned
  FABLE_STATIC_ASSERT typedef name follows __LINE__; the real VC7.1 recheck is in b1_hdr/evidence/includers (ALL_EQ, grades equal).
- Opt-in headers per batch (one line each): b1 FABLE_RETAIL_NPLAYERGUI_CSTATBASE_NO_CONTAINERS (CStatBase without its vectors, needs
  FABLE_RETAIL_CPLAYERGUI + _GUIMODE); b2 FABLE_RETAIL_CDECAL_NO_CONTAINERS (instead of FABLE_RETAIL_CDECAL); b3
  FABLE_RETAIL_CSPECIALABILITIESUNHOLYPOWERDEF_DTOR_DECL_ONLY; b4 FABLE_RETAIL_CBRAINDEF_DTOR_DECL_ONLY (both need DEF_CLASS_BASE).
