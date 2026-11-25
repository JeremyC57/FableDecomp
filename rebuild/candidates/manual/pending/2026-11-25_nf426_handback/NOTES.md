# NF426 (Account A naming queue, 2026-11-25): the two shared counted-pointer stand-ins

Families: `?Reset@AutoTinyCountedHandle@@QAEXXZ` (341 addresses) and `?Release@CIVCP@@QAEXXZ` (205 addresses), 546 in all.
**Result: 112 rows ACCEPT in `batches/b1` (38), `b2` (38), `b3` (36); 434 HOLD with evidence (`FAMILY_TABLE.tsv`, `T3.txt`).**
No header change, no table change (TABLE_CHANGES.tsv is header-only), no caller respell.

## What these rows really are (premise correction)
- **They have no callers.** A scan of the whole retail image finds 0 `call`/`jmp` rel32 and 0 absolute references to any of the 569 copies
  of the two byte shapes (the 9 absolute hits are coincidences inside `call` displacements). reloc_bindings.tsv binds neither stand-in. So
  there is nothing to respell on the caller side: each row is the **unreferenced out-of-line COMDAT copy of a header inline** (GOTCHAS wf3:
  retail links without /OPT:REF, so VC7.1 keeps every inline function the front end referenced, even when every call was inlined).
- **They are not CCountedPointer / CIntelligentPointer.** The body is `if (Object) { if (--Object->IVRefCount == 0) Object->Delete(); Object = 0; }`
  with `Delete` at vtable slot 1 and the count at +4: the intrusive `CDefPointer<T>` / `CIVCountedPointer<T>` release. FableWin.pdb's module
  streams list 558 such names (249 `?Release@?$CDefPointer`, 249 `??1?$CDefPointer`, 30 + 30 for CIVCountedPointer) against 569 retail
  copies; Ego_r.pdb has none (its donor build used /OPT:REF).
- **The two stand-in names are not two shapes.** Each occurs in both byte shapes. Shapes by bytes: `T` (27 B, `mov dword [esi],0`, /O2 TU)
  411 rows; `S` (24 B, `and dword [esi],0`, size TU) 135 rows. A `Release` and the `~` that inlines it are byte-identical in both shapes
  (VC7.1 /O1 makes `~` a `jmp Release` unless Release is `__forceinline`, which engine/CDefPointer.h has: FABLE_DEFPTR_INLINE).

## Shape histogram (546 family rows)
| shape | rows | ACCEPT |
|---|---:|---:|
| T, 27 B (`#pragma optimize("t", on)` or none) | 411 | 110 |
| S, 24 B (`#pragma optimize("s", on)`) | 135 | 2 |
Generator: ONE (`tools/gen426.py`), the HEAD row's own `#pragma optimize` line kept; both shapes come out of the same text.

## T identification method and hit rate
1. **TU of each copy**: the nearest `agree_pdb` plain (non-template, non-`??_`) anchors on both sides, mapped to their FableWin.pdb module
   (`tools/tu.py`, `tu2.py`). Both sides in the same module: 410 rows; boundary (anchors in different modules): 136 HOLD.
2. **Count gate** (`tools/gate.py`): retail copies in the TU v the module's CDefPointer / CIVCountedPointer `?Release@` + `??1` names.
   Count-equal: 254 family rows; mismatched (the kept copy follows link order, which differs between the builds, e.g. localisation 1 v 3): 156 HOLD.
3. **Single T** (`tools/single.py`): the module's two names are `?Release@` and `??1` of ONE T: 62 TUs (123 family rows).
   Multi-T count-equal TUs (tc_register 56, world_map 12, lib_mesh_data_bank 10, ...): the T set is proven, the per-copy order is not: 131 HOLD.
4. **Second source**: Ego_r.pdb's object of the same name instantiates a function on T: `CDefClassBase::GetSubDef<T>` or
   `CDefinitionManager::GetDef<T>` (the generator requires one of these two; it cites it in each row): 56 TUs (112 rows). The other 6
   single-T TUs (11 family rows: main, player, quest_manager, Engine_Sky_Renderer, lib_3d_mesh_file, tc_tavern_game_card_sorting) have one
   source only: HOLD, locks released. First-user check (`evidence/first_user.tsv`): no compiled definer of an accepted name sits at a lower
   address in another TU (the linker places objects in link order, so a lower first user would own the kept copy): 0.
5. **Member** (which copy is `Release`, which `~CDefPointer`; their bytes are identical). VC7.1 itself orders the two copies, and the order
   flips with optimisation (`evidence/toys.txt`, 21 optimised + 3 /Od compiles of 6 toy TUs, the real engine/CDefPointer.h included, with
   and without /GX, inline and `__forceinline`, 1 to 5 Ts):
   - /O1 and /O2: every `?Release@<T>` copy precedes every `??1<T>` copy, 21 of 21 (and `??4<T>`, when emitted, follows `??1<T>` directly).
   - /Od: every `??1<T>` precedes `?Release@<T>`, 3 of 3, with the same grouping as FableWin.pdb's debug modules (tc_register: 26 dtors,
     then 26 Releases). FableWin.pdb is dtor-first in all 62 single-T modules: the toy mechanism reproduces the independent debug build.
   - The linker keeps the object order (toy clock3.map).
   - What the toys do NOT reproduce: retail's layout. Every optimised toy emits SetDynamicCast after both copies, while retail tc_bed /
     tc_clock have it between them; the `reg` toy emits the Release and destructor groups back to back, while retail tc_register's two
     runs of 28 sit about 0x3000 apart. So the toys support only the narrower rule "an inline's out-of-line copy precedes every COMDAT
     that expands it" (Release is expanded inside ~CDefPointer, SetDynamicCast and operator=), and that rule is tested on retail itself:
   - Retail test over all 56 TUs (`tools/assignscan.py`, `evidence/assignscan.tsv`): the CDefPointer assign bodies of each TU, found by
     their tails (operator= returning *this; the void SetDynamicCast), placed against the two copies. Below the lower copy: 0 of 60
     (no contradiction). Between the copies: 15 (14 SetDynamicCast, 1 operator=), which the reverse assignment would contradict.
     After the higher copy: 45. The body directly after the higher copy is operator= in 2 TUs (FableWin.pdb also lists `??1` beside
     `operator=`, tc_clock 16 / 18) and never directly after the lower one; 34 TUs have no assign body within 0x60 after either copy.
   - Calibration on named retail pairs (`evidence/order_cal2.tsv`): CCountedPointer<T>::ReleaseData below its inliner / caller
     (`??1`, `Reset`) of the same T in 7 of 7. (For template COMDATs that CALL each other, `evidence/order_cal.txt`, the callee is below
     the caller only 66% of the time: called and expanded callees are ordered differently, so that rate does not bear on this pair.)
   Lower address = Release, higher = `~CDefPointer`.
Hit rate: 112 of 546 (20.5%) proven to two sources; every ACCEPT row is a CDefPointer<const T> (`$$CBV`), none a CIVCountedPointer.

## Row form (one generator)
`#define FABLE_RETAIL_DEF_CLASS_BASE` + `#include "engine/CDefPointer.h"`, `class T;`, and an explicit instantiation of the one member:
`template void CDefPointer<const T>::Release();` / `template CDefPointer<const T>::~CDefPointer();`. `// Leaf:` the full PDB spelling
(`?Release@?$CDefPointer@$$CBV<T>@@@@QAEXXZ` / `??1?$CDefPointer@$$CBV<T>@@@@QAE@XZ`, both whole FableWin.pdb entries). Leaf bytes are identical
to HEAD; the COMDAT selection goes 1 (HEAD's plain stand-in function) -> 2 (select-any), which is what retail's inline copy is and what every
other landed definer of these names emits (so no LNK2005). Line 1 uses the plain template name (EXTRA_RULES finish.py rule).
The destructor rows also emit the select-any `?Release@<T>` copy (the inline callee the front end references); it is byte-identical to the
sibling Release row's leaf in the same batch (objsec `ALL_EQ+PICKANY`, pk_all EQ at the sibling's address, not leaf-referenced): R1 / R10.
Tests (the destructor test calls `q.~CDefPointer()` and checks the null store): new, one per row, declare a local `CDefPointer<T>` with only the member under test, so the self-contained link fails and vland links
the row object (hn426: SELF link fails, row link PASS); they print the row's existing catalog PassPattern. Mutation control
(`mut/`): a Release / destructor without the count-down fails both tests (rc 1).

## Gates (all three batches, 112 rows)
- vland --recheck-landed HEAD and new at one LANDVERIFY_WORK path, both with LANDVERIFY_STRICT_LEAF=1: MATCH PASS -> MATCH PASS for all
  112; 0 WARN either side (no LITERAL_DIFF, RELOC_NAME, STANDIN_LEAF, GENERIC_STRUCT).
- objsec (`tools/objsec426.py`, sections paired by COMDAT symbol under the self rename): 56 ALL_EQ (Release rows), 56 ALL_EQ+PICKANY
  (destructor rows, the copy above); `.drectve` equal; selection 1 -> 2 on the leaf.
- pk_all (run in place of pickany_vs_retail; `--addrs` = this hand-back's names and addresses): HEAD 0 copies; new 56 copies, all EQ,
  0 NEQ, 0 leaf-referenced, 0 reached, so 0 reach flips.
- Exposed existing debt (NF419 r2), stated per row in CHECKS `exposed_debt`: 36 unchanged landed rows emit 40 unreferenced copies of 9 of
  these names whose bodies differ (other header variants / modes; tavern-game TUs, tc_hero_appearance, 0x0094D780, 0x0083CC10, 0x00902EEF,
  0x00902F49): not leaf-referenced, not reached (`evidence/pk_definers.tsv`); queued in T3.txt section 1.
- /FORCE link (`tools/link426.py`, response file; `evidence/link_all.txt`): the 112 rows + 161 definer objects (every landed source that
  names CDefPointer / GetSubDef / GetDef of one of the 56 Ts: 402 compiled, 161 define a family or new name). HEAD: 131 duplicates, of which
  110 are the two stand-ins (61 + 49 LNK2005 between the NODUPLICATES copies: the control); new: 21 duplicates, exactly HEAD's other 21
  (pre-existing GetSubDef / GetHandValue copies). Unresolved: the same 8 watch-matching names both sides (pre-existing), 1687 lines both.
  No new name is duplicated or unresolved.
- Strict-leaf control (`evidence/strict_leaf_control.txt`): row 0x00746B60 with a deliberately wrong `// Leaf:` through the same
  runpair with LANDVERIFY_STRICT_LEAF=1 grades LEAF_MISMATCH (the switch is live).
- Same-TU lower definers (`evidence/lower_definers.tsv`): 0x0071EF40 (0x0071C7C0, 0x0071C9F0, 0x0071DBD0, 0x0071DE80) and 0x0083CEC0
  (0x0083CC10) have landed rows of their own retail TU below them that emit a DIFFERING copy of the name: an address-ordered full link
  keeps that first copy and drops the row's EQ one (GOTCHAS nf399_b2 link-order consequence, recorded in those RESULTS rows and T3 lines).
  110 other lower definers' copies are EQ.
- The final row texts differ from the ones pk_all and the link ran on only in comment lines; all 224 HEAD / new objects were recompiled
  and their non-debug sections compared equal to those runs' objects (vland, objsec, hn tests rerun on the final texts).
- reserved_check (RESERVED_REFS=""): 0 reserved. apply_table: no table lines (nothing to apply). No header change, so no includer recheck.
- Rows stay locked NF426 (112); the 11 generated-but-unaccepted single-T rows were released.

## Hand-back layout
`batches/b{1,2,3}/`: rows/, tests/, orig/rows + orig/tests (HEAD by address), head/payload.json (vland HEAD payload), RESULTS.tsv
(`new` = the row's graded self symbol), CHECKS.tsv, UNITS.tsv (each row its own unit, not Option S), TABLE_CHANGES.tsv (header only),
SELF_SYMBOLS.tsv, RENAMES.tsv (catalog Module `CDefPointer<const T>`, name `Release` / `~CDefPointer`, proposed files
`CDefPointerconst<T>_{Release,CDefPointer}_<addr>.cpp`), PLAN.tsv, evidence/ (objsec, pk). Objects are not in the batches (work/obj_<b>/). Root: FAMILY_TABLE.tsv (all 546), T3.txt,
NAME_FIXES.tsv (none), RULINGS_NEEDED.tsv (none), evidence/, tools/.

## Next ideas for the HOLD rows
- Multi-T count-equal TUs (131): the toys show the within-run order is "first referenced, then the rest in reverse"; to prove per-copy T,
  rebuild each TU's front-end reference order (concatenate the TU's landed rows) and compare the emitted order with retail's interleaved
  per-T bodies (tc_register's runs of 28 are split by `mov eax,ecx; ret` / `ret` pairs at fixed positions).
- Count-mismatch TUs (156) and boundaries (136): retail link order differs from the debug build's, so first-user placement must be
  modelled per T across TUs (which TU retail links first); start from Ts used by one TU only.
- The 11 one-source rows need a second source (Ego_r type records tie T to a class, not to the TU).

## Callee names / header debt
None: no callee declared, no header touched. The landed definers with NEQ copies are header debt (T3 section 1).
