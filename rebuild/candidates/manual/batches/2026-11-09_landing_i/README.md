# Landing 2026-11-09i: 146 EH unwind funclets whose real parent has landed (chunk 896)

The user asked for the EH funclets with landed parents, on an agent. Pool: the Nov-3 funclet pass
(`batches/2026-11-03_funclets_parents/`) parked 183 rows `parent-unlanded` and 6 `parent-TU-emits-different-funclet`; the real
parent (FuncInfo -> `__ehhandler$` thunk -> push site, not the preceding manifest row) of 140 of the first group and of all 6 of the
second has landed since. **All 146 landed**, every one RELOCATION_MATCH + behaviour PASS + `offcheck2.py` OK (the match is at the
expected offset of exactly one `$L` block of the retail block length, and that block's `__ehhandler$` names the parent).

Mechanism: unchanged from the Nov-3 pass (include-the-parent stubs, the parent's flags, the parent's test with the pass line
renamed; `$S/funclets/mkpayload.py` + `offcheck2.py`). The agent graded read-only (`logs/` of the hand-back); the coordinator
re-graded the 140 independently before landing (`logs/verify_readonly_140.log`, `logs/offcheck2_140.txt`: 140 RELOCATION_MATCH PASS,
140 OK leaf-ok) and the 6 after applying the parent fixes (`logs/verify_readonly_6.log`, `logs/offcheck2_6.txt`).

## Six parent model fixes (the parents' own grades do not cover their `$L` blocks)
`parent_fixes/README.md` and `parent_fixes.diff` have the detail. Each changes the class model only, never the body:

| Parent | Funclet | Defect | Fix |
|---|---|---|---|
| 0x00C2D3B0 | 0x01213B0A | retail unwinds a holder at +0x320 whose destructor 0x00C162A0 destroys the member at +0x324 | holder `CUnprovenHolder_00c162a0` at +0x320 |
| 0x00C287E0 | 0x012134AB | vector at +0x440; retail +0x43C (empty allocator word first) | vector at +0x43C |
| 0x00C28AE0 | 0x012134E3 | 3 unwind actions v retail 1 (the `reserve(0)` stand-in ran with members alive; the grader's any-offset fallback had hidden it) | the stand-in moved into the vector's constructor |
| 0x00CAA790 | 0x01216DE0 | member at +4 v retail strmbase `CUnknown` BASE at +0 | `CUnknown` base, virtual dtor (vendor proof `??1CBaseMediaFilter@@UAE@XZ`) |
| 0x00CAAAC0 | 0x01216E60 | same | same (`??1CBaseFilter@@UAE@XZ`) |
| 0x00CB1890 | 0x012173E0 | one-word list at +0x14 v strmbase `CBaseList::CNodeCache` at +0xC | CNodeCache layout |

After the fixes all six parents re-grade RELOCATION_MATCH + PASS (`vland --recheck-landed`), and so do the three landed funclets
that include them (0x01213AF1, 0x012134A0, 0x012134B9): `logs/recheck_parents_and_siblings.log`. Three of the parents' standalone
tests changed with the offsets. Warnings there (RELOC_NAME on older binders, STANDIN_LEAF, VENDOR_PROOF_MISMATCH for the absent
dxsdk tree in this lane) are the parents' pre-existing ones.

## Also fixed
`build_candidates.ps1` row 0x00C14F90 (PKCDeferredBank) carried `CompilerFlags = '/O2 /Oy /W3 / E H s c /EHsc'`; now
`'/O2 /Oy /W3 /EHsc'`, re-graded RELOCATION_MATCH + PASS. Its 8 funclets landed with `/EHsc`.

## Names
Catalog and manifest names are `_global::Unwind@<addr>` / `Unwind@<addr>` as for the Nov-3 rows; labels
`ghidra_out/labels_funclets_2026-11-09.tsv` (146 rows, parent + FuncInfo + push site evidence). Queued `land0911i` (PRIORITY):
0x00CB1210 `CNodeCache` v the 0x00CB1890 TU's `CPatchNodeList_00cb1890`; 0x00C29CA0 `CBuffer_00c29ca0` v the 0x00C287E0 TU's
`CVector_00c287e0`; 0x00C162A0 catalogued `std::_Cons_val<...>` but its bytes are a destructor (holder class unproven).
`bindings_multi_check.py` prints 1 (CScriptThing::NullCoord), unchanged.

## Funclets still unlanded (61 of the 207 unworked)
43 + 1 catch: parent still unlanded; 6 real parent not in the manifest; 11 not EH funclets at all (ordinary functions with a
Ghidra `Unwind@`/`Catch@` label); 6 not in the Nov-3 records.
