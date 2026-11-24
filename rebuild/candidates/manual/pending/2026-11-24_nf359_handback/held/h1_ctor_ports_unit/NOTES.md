# NF359 hand-back: the CThingNoiseDef def-TU follow-ups (T3 nf355 lines), Account A naming queue, 2026-11-24

Folder `$S/w22/agentNF359/` (S = the 40effb6c scratchpad). Lock name NF359. Nothing is ACCEPT and there is NO land_scripted run dir:
`held/h1_ctor_ports_unit/` is an EVIDENCE dir (RESULTS all HOLD, no rows/, no head/, no orig/; never flip it to ACCEPT), with the measured
option texts under `options/` and the evidence under `evidence/`. A ruling needs a fresh run dir built from the chosen option. No header is
handed back. No row file changed. No test changed (all landed tests are standalone and prechecked).
Limits: the option grades are `parity_of` (exp.py) only; no option text went through vland `--recheck-landed` or a behaviour-test run, and
there is no pk_gate / reversed / planted control, because nothing is a candidate.

## Result
| address | what | verdict |
|---|---|---|
| 0x00455EF0 `??0CThingNoiseDef@@QAE@XZ` | ctor port onto engine/CThingNoiseDef.h | HOLD, ruling NF359-Q1 |
| 0x0044C1BB `??0CTCNoiseDef@@QAE@XZ` | ctor port onto engine/CTCNoiseDef.h | HOLD, ruling NF359-Q1 (same unit) |
| 0x0045C5DA self `??_GCThingNoiseDef@@UAEPAXI@Z` | self respell | HOLD, ruling NF359-Q2 |
| 0x0045C5C1 `?Alloc@CThingNoiseDef@@SIPAVCDefClassBase@@XZ` | Alloc coverage | proposal only (no row; see Q1 option S) |

## The structural finding (why every form needs a ruling)
- Retail references 0x00455EF0 exactly once: Alloc 0x0045C5C1's tail `jmp`. It references 0x0044C1BB nowhere: no call, jmp or
  dword in any section of Fable.exe (xscan + a whole-image dword scan).
- VC7.1 emits `??0CTCNoiseDef` unreferenced from the 0x0045C5F6 TU too: the only relocation naming it there is a `.debug$F` record
  (evidence/orphan_ctcnoisedef_ctor.txt); `??0CThingNoiseDef` expands it inline, as retail 0x00455EF0 does. So retail's orphan
  0x0044C1BB is what the inline-ctor model predicts. Retail's ctor/GetSizeofClass pairs (0x0044C1BB / 0x0044C1CD; 0x00455EF0 /
  0x00455F14) sit with no padding before them (0x0044C1A0 + 27 = 0x0044C1BB; 0x00455ECA + 38 = 0x00455EF0), consistent with size-mode
  (alignment 1) COMDATs. 0x00455EF0's 16-byte alignment is a coincidence of packing, not /O2.
- The landed 0x0045C5F6 object already emits EVERY leaf of this TU, all EQ to retail (retcmp, bytes + relocation targets): Alloc
  0x0045C5C1, `??0CThingNoiseDef` 0x00455EF0, `??0CTCNoiseDef` 0x0044C1BB, `??_GCThingNoiseDef` 0x0045C5DA, `??1CThingNoiseDef`
  0x0045C5F6, plus `??1CThingBaseDef` 0x00454B4C, `??_GCTCNoiseDef` 0x0044F71B, `??_GCDefPointeeBase` 0x0042D9A0; both vftables 25 slots,
  slot for slot (slot 0 is `??_E`, VC7.1's alias of `??_G`). Graded as each of 0x00455EF0 / 0x0044C1BB / 0x0045C5DA (Leaf moved): RELOCATION_MATCH.
- The only text that emits the inline ctor (and so `??_G`) without a device is Alloc, which 0x0045C5F6 already defines strongly. One retail
  TU, five catalog addresses: no per-row text avoids either a second strong definition or a device.

## Measurements (held/h1_ctor_ports_unit/evidence)
Probes (exp.py = verify_and_land.parity_of on the catalog oracle; all RELOCATION_MATCH, HEAD too):

| form | 0x00455EF0 ??0 | 0x0044C1BB ??0 | COMDAT set change v HEAD (setdiff_round1.txt) | link of the unit (link_*.txt) |
|---|---|---|---|---|
| HEAD | sel 1, 36 B, align 16; 1-slot (4 B) ??_7CThingNoiseDef / ??_7CTCNoiseDef | sel 1, 18 B, align 16; 1-slot ??_7CTCNoiseDef | - | 2 LNK2005: ??0CThingNoiseDef (00455ef0 v 0045c5f6), ??0CTCNoiseDef (0044c1bb v 0045c5f6) |
| A Alloc form (= the 0x004304E0 precedent) | sel 1 -> 2, align 16 -> 1, reloc ??0CMemberBase_0044c0c0 -> ??0CSubDefClassBase (both 0x0044C0C0); both vftables 4 -> 100 B (align 4 -> 8); +11 COMDATs (??_7CDefPointeeBase, 6 x ??1, 3 x ??_G, strong Alloc) | sel 1 -> 2, align 16 -> 1, reloc ??0CShoveHaPennyRegionDef -> ??0CSubDefClassBase; vftable 4 -> 100 B; +13 COMDATs (also ??0 / ??_7 CThingNoiseDef) | object section-for-section IDENTICAL to 0x0045C5F6's (setdiff_v_0045c5f6.txt) | 3 LNK2005, all on ?Alloc@CThingNoiseDef (4 with a 0x0045C5C1 Alloc row) |
| O out-of-line (overlay CTOR_DECL opt-in for CThingNoiseDef; CTCNoiseDef.h default) | sel stays 1, align 16 -> 1; vftables 4 -> 100 B; +10 COMDATs | sel stays 1; vftable 4 -> 100 B; +6 COMDATs | = 0x0045C5F6 object minus Alloc, ??0 sel 1 | 2 LNK2005 (unchanged pair) |
| N nf86 helper (one row: 0x00455EF0) | sel 1 -> 2; = 0x0045C5F6 object with Alloc renamed to `?ReferenceThingNoiseDefCtor@?A0x81fc743f@@...` (sel 1, per-TU) | not compiled (hash is per path) | +11 COMDATs incl. the helper | 1 LNK2005 left (0x0044C1BB, unported) |

- `.drectve` SAME in every form. The UND additions are the added COMDATs' callees (??1CResource, ??1CParentDefClassBase,
  ??1CThingComponentSet, ??3, ??0CSubDefClassBase; ??2 only with Alloc) and the vftable slots (NF323 Q6 shape: Copy / GetSizeofClass /
  Transfer of both classes and the base virtuals).
- Pick-any (both checkers, `--leaf`, evidence/pk/): every copy with a retail home is EQ in every form (A, O, N, HEAD); no NEQ copy, so no reach flip
  onto a NEQ copy. Copies reached only through vftables (??_G / ??1) were compared with retail directly (retcmp_round1.txt: all EQ).
- Per-symbol definers per option (definers.txt): the duplicate count is not the whole story, pick-any copies with different bodies link silently:
  HEAD keeps two bodies of ??_7CThingNoiseDef / ??_7CTCNoiseDef (4 B and 100 B) and of ??0CTCNoiseDef / ??_GCTCNoiseDef (0x00749810's NEQ
  copies, below). A, O and S leave one body of each ??_7; N (one row) still has 0x0044C1BB's 4 B ??_7CTCNoiseDef. 0x00749810's NEQ
  ??0CTCNoiseDef / ??_GCTCNoiseDef copies stay in every option (debt, not changed here).
- Emitter census (compiled HEAD): 0x00749810 emits a full 25-slot EQ ??_7CTCNoiseDef plus NEQ ??0CTCNoiseDef (29 B) / ??_GCTCNoiseDef (30 B)
  pick-any copies (not leaf-reached; T3_PROPOSED); 0x00749A70 and 0x00455F3C emit no CTCNoiseDef / CThingNoiseDef vftable or ctor.
  0x004537E9 not compiled (T3 nf303 column 2): source-level only, a Copy via operator= with no vptr store. So "a port removes the one-slot
  copies" holds for the compiled set; the other local-model rows (0x00455F14, 0x0044C72B, 0x00463923, 0x007498E0, 0x00C3A7xx, ...) are
  grep-level only: no other landed source defines a NoiseDef ctor body or `new`s one; 0x007498E0, 0x007497C0 and 0x00455F1A hold a
  CTCNoiseDef member but were not compiled, and 0x0044C72B, 0x00452184, 0x004DEF0F, 0x004DEDAC, 0x00C3A7xx were not opened beyond that grep.

## Item 2: Alloc 0x0045C5C1 and the precedent
- 0x00432C40 (CUIMiscThingsDef::Alloc) HAS its own landed row, strong (sel 1). 0x004317D0 and 0x004304E0 also define the same strong Alloc
  (the "Alloc graded on ??1 / ??0" form). Measured: the three Alloc bodies are byte- and relocation-identical and EQ to retail; a link of the
  three HEAD objects gives 2 LNK2005 on that Alloc (evidence/link_prec.txt, precedent_objects.txt). So the precedent's "own Alloc row" form
  is exactly a second strong Alloc, which the brief rules out for 0x0045C5C1.
- Precedent debt found on the way (T3_PROPOSED.tsv): 0x004317D0's object carries a NEQ ??0CUIMiscThingsDef copy (1009 B v 1039) and two
  leaf-reached NEQ STLport _M_erase copies (0x0042FE07, 0x0043020A); none has a T3 / DEBT_BY_COPY line.
- Proposal for 0x0045C5C1: option S below. The 0x0045C5F6 object grades Alloc EQ to retail (retcmp bytes + relocation targets; pk_all EQ via
  table + address file).

## Rulings needed
- **NF359-Q1 (0x00455EF0 / 0x0044C1BB ctor ports).** Choose one:
  - **S, one source covers several addresses (proposed).** Catalog entries 0x00455EF0, 0x0044C1BB, 0x0045C5DA (self) and a new 0x0045C5C1
    graded from the 0x0045C5F6 object with a per-entry leaf, the object linked once. No new text, no device, no duplicate on the NoiseDef
    symbols (from the definer table, not linked; the census objects carry an unrelated ??0CTCNoise clash, T3_PROPOSED), every leaf already
    measured EQ. Needs coordinator tooling (today every catalog entry has its own Source and the Leaf comes from the source; 0 shared
    Sources in build_candidates.ps1). The same route would clear the precedent's 2 Alloc duplicates.
  - **A, the landed precedent form.** Each row = 0x0045C5F6's text with the Leaf moved. Measured set change above; LNK2005 moves from ??0 to
    Alloc (3, or 4 with a 0x0045C5C1 row). Excluded by the brief unless the owner accepts identical strong duplicates (the 2026-11-20
    static-pair precedent is the nearest rule, for statics only).
  - **N, the nf86 / nf88 helper (GOTCHAS).** Removes the strong duplicate, costs one per-TU helper per row (scaffolding debt). Measured on
    0x00455EF0 only; a unit proof needs the 0x0044C1BB / 0x0045C5DA variants compiled at their own paths and a whole-unit link. The
    deleting-destructor decision ("no forcing construct at scale") argues against.
  - **O, out-of-line strong ctor.** Keeps selection 1 and both LNK2005s, contradicts the inline model (retail's unreferenced 0x0044C1BB) and
    0x00455EF0 needs a header opt-in. Not recommended.
  - **Hold** until S exists.
- **NF359-Q2 (0x0045C5DA self).** No in-place respell exists (C++ cannot name ??_G). Every text that emits `??_GCThingNoiseDef` adds a strong
  Alloc (A) or a strong ??0 (O). It goes with Q1 (S proposed). Drop test of the stand-in `?CThingNoiseDef_ScalarDeletingDestructor@@YIPAXPAXHI@Z`:
  no reloc_bindings line, named only by its own row, test and self_symbols.tsv (evidence/droptest.txt).

## Table (TABLE_CHANGES.tsv, proposed, only with a landed ctor text for 0x00455EF0)
`delete ??0CMemberBase_0044c0c0@@QAE@XZ 0044c0c0 00455ef0`: its only witness and only landed source is 0x00455EF0; 0x0044C0C0 keeps
`??0CSubDefClassBase@@QAE@XZ` (witness 0044c57d). `??0CShoveHaPennyRegionDef` (126 sources, witness 0044c146) is not touched. No NAME_FIXES.

## Rule notes
- No repo writes, no git. Overlay header (CTOR_DECL probe) only in work/exp/ov/. clean.py only (exp / grade dirs cleaned by the tools).
- Locks: claimed NF359 on 00455ef0, 0044c1bb, 0045c5da, 0045c5f6, 0045c5c1, 00432c40, 004317d0, 004304e0, 00749810, 00749a70, 00455f3c
  (every test prechecked before its lock, every source after it, each in an earlier call than its compile: work/pre.log). All released
  (evidence/locks_final.txt). 0x004537E9 (T3 nf303) not locked or compiled. No B claim, Codex reservation or other agent's lock touched.
- One compile at a time (run1-run4.sh sequential, `timeout 900` per line). Retail-derived relocation values were cut from the retcmp
  evidence (symbol, length and verdict kept). Checkers' awk lookups compare addresses as strings.

## Tools (tools/)
NF355's nf.py, exp.py, pk2.sh (awk string compare), vtcmp.py, vtmap.py, leafcmp.py, xscan.py and the rest, repointed to NF359; new:
`comdatlist.py` (sections, selection, size, alignment), `setdiff.py` (COMDAT-set change HEAD v new under renames), `definers.py` (per-symbol
definers per option), `run1.sh`..`run4.sh` (the measurement rounds).
