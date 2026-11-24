# NF363 (Account A naming queue, 2026-11-24): NF360 b1 finish (0x0042AF3C) + NF359 option A unit (0x00455EF0 / 0x0044C1BB / 0x0045C5DA)

Folder `$S/w22/agentNF363/` (S = the 40effb6c scratchpad). Lock name NF363. Two land_scripted run dirs, both RESULTS **ACCEPT**:
- `batches/b1/` 0x0042AF3C CDefinitionManager::GetDef<CThingBaseDef> (rulings R1 / R2 applied).
- `batches/b2/` 0x00455EF0 ??0CThingNoiseDef, 0x0044C1BB ??0CTCNoiseDef, 0x0045C5DA ??_GCThingNoiseDef (option A, one unit, UNITS.tsv).
No header change. No repo write, no git. Every object HEAD and new compiled at one path per row (tools/pass.py).

## Part 1: 0x0042AF3C (batches/b1)
- **Text.** NF360's b1 text plus two comment edits and nothing else:
  - the cast line carries `// Fidelity: scaffolding, not retail` and a two-line reason (R2; it goes with the nf349 header port);
  - "the def type is opaque" became "GetDef<CThingBaseDef> (Leaf and Identity lines)".
  Landed source and test equal NF360's orig copies, so head/payload.json is current.
- **vland --recheck-landed.** HEAD and new are both RELOCATION_MATCH PASS.
  - HEAD warnings: leaf matches no symbol, STANDIN_LEAF, 2x SHARED_HEADER_AVAILABLE.
  - New warnings: only the 2x SHARED_HEADER_AVAILABLE (CDefPointer, CDefinitionManager), because the row keeps its local model.
- **objsec.** HEAD v new is OTHER_SAME, symdelta 0. The leaf's data, relocations and characteristics are equal; selection is 1 on both
  sides; .drectve is equal. The new object is also OTHER_SAME against NF360's new object.
- **Checkers** (leaf per side). pk_all and pvr give SAME / GATE PASS; reversed PASS; reach flips 0.
  - Planted control: FAIL, as it must; 1 reach flip.
  - The plant is at comparator level (tools/plant.sh adds one synthetic leaf-called NEQ line to N's pk_all tsv and pvr text).
- **Drop test.** Only the stand-in self DEF goes. It has 0 reloc_bindings lines and self_symbols.tsv line 327 (follows at the next pass).
  Source grep finds the row plus three CTCShop comment-only mentions.
- **Link** (/FORCE, LINK cleared, watch '.'):
  - Owner + binder 0x004CA010: HEAD leaves the PDB name unresolved, new resolves it; 0 duplicates; every other unresolved name is equal.
  - Owner + 0x004CA010 + the ten copiers: HEAD has 20 duplicates (9 of them GetDef among the copiers); new has 21. The one added is the
    owner v 0x005B43A2, a different body, which R1 accepts.
- **Ten strong copiers** (HEAD objects, read-only; `evidence/copiers363.tsv`, `copiers_groups.txt`). Every one DEFINES the PDB symbol
  ??$GetDef@VCThingBaseDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z at selection 1 (strong). Every one is NEQ to
  retail 0x0042AF3C (pk_all leaf-called, reach y; retcmp bytes DIFF) and differs from the owner's new object. The owner's own copy is
  98 B, EQ by retcmp.

  | copier | size | align | same body as |
  |---|---|---|---|
  | 0x007744A0 | 25 | 16 | 0x008ED1F0 |
  | 0x008ED1F0 | 25 | 16 | 0x007744A0 |
  | 0x005B43A2 | 27 | 1 | - |
  | 0x00801A50 | 31 | 16 | - |
  | 0x005B4795 | 50 | 1 | 0x005B5BE6 |
  | 0x005B5BE6 | 50 | 1 | 0x005B4795 |
  | 0x0071DA70 | 60 | 16 | - |
  | 0x005B4EBA | 105 | 1 | - |
  | 0x00811470 | 107 | 16 | 0x008115A0 |
  | 0x008115A0 | 107 | 16 | 0x00811470 |

  That is 7 distinct bodies; NF360 measured 0x00659164 at 31 B. Proposed T3 line text (nf360 line 3623): "eleven strong NEQ copies,
  object-verified (NF363 compiled ten, NF360 0x00659164); 7 distinct bodies of 25-107 B, none EQ to retail 98 B".

## Part 2: NF359 option A (batches/b2)
- **Texts.** Each row is the landed 0x0045C5F6 code lines (from the first #define on), byte-identical, with its own comment block:
  - Leaf; identity (two sources); emission;
  - a "Strong Alloc" paragraph naming the ruling and the nf184 pattern;
  - the pick-any note and model, and "Was".
  No 0x0045C5C1 row (as ruled).
- **Section equality.** Each new object is section-for-section identical to a fresh HEAD 0x0045C5F6 object (`evidence/setdiff_v_0045c5f6.txt`:
  16 sections SAME each, DEF and UND sets equal).
- **COMDAT set v HEAD** (`setdiff_head_v_new.txt`), as NF359 measured:
  - 0x00455EF0 and 0x0044C1BB: the self goes from selection 1 to 2 and alignment 16 to 1; the 0x0044C0C0 call is respelled onto
    ??0CSubDefClassBase; both vftables grow from 4 B to 100 B.
  - Added COMDATs: +11 for 0x00455EF0, +13 for 0x0044C1BB, +15 for 0x0045C5DA, each including the strong Alloc.
  - 0x0045C5DA also drops its stand-in self.
  - .drectve is SAME. The UND additions are the added COMDATs' callees and the vftable slots (NF323 Q6 shape).
- **vland --recheck-landed** (`logs/`): all three are RELOCATION_MATCH PASS for HEAD and for new, the new tests included.
  - HEAD warnings: RELOC_NAME on the two 0x0044C0C0 stand-ins; SHARED_HEADER_AVAILABLE.
  - New: only 0x0045C5DA warns, STANDIN_LEAF on ??_GCThingNoiseDef. This is a standin_leaf.py gap, not a stand-in. cat_mc() decodes
    "scalar deleting" with a space, but the catalog spells `scalar_deleting_destructor' with underscores (1,917 catalog rows).
- **Retail comparison.**
  - retcmp of every function COMDAT with a retail home (8: both ctors, ??_G / ??1 CThingNoiseDef, ??1CThingBaseDef, ??_GCTCNoiseDef,
    ??_GCDefPointeeBase, Alloc) is EQ, bytes and relocation targets, in all three new objects and in HEAD 0x0045C5F6.
  - vtcmp: ??_7CThingNoiseDef and ??_7CTCNoiseDef are 25 slots slot for slot. The only "DIFF" is slot 0, ??_E (VC7.1's weak alias of
    ??_G) against the retail label ??_G. ??_7CDefPointeeBase is 10/10. HEAD 0x00455EF0 / 0x0044C1BB carried the 1-slot tables.
- **Checkers** (leaf per side; 0x0045C5DA HEAD = the stand-in, new = ??_G). GATE PASS: no TURNED_NEQ or NEWLY_NEQ; only EQ copies ADDED.
  Reversed: PASS (no fixes to invert). Planted: FAIL (6), as it must. Reach flips 0 (planted 3).
- **Tests** (`tests/<addr>_test.cpp`, catalog PassPatterns kept, `AUTO_TINY_0045c5da_TEST PASS` included).
  - Each is self-contained on engine/CThingNoiseDef.h / engine/CTCNoiseDef.h with the row's defines, plus test-only
    FABLE_RETAIL_DEF_CLASS_BASE_CTORS / _CDEFCLASSBASE_CTOR_SUBDEF_FRIEND / CDEFSTRINGTABLE / CCHARSTRING.
  - There is no local model of either class. Controlled stubs for the out-of-line ctors and dtors log the order and `this`; the slot-20
    GetSizeofClass overrides check each vftable store by dispatch.
  - 0x00455EF0 checks the base ctor, then Noise's CSubDefClassBase() at +0x70, and both dispatches.
  - 0x0044C1BB checks CTCNoiseDef built alone and as the member.
  - 0x0045C5DA checks "RCPR" with 0 deletes for an explicit virtual dtor call, and "RCPR" with 1 delete of the right pointer for a
    delete-expression.
  - VC7.1 /Od, standalone link, wine: PASS ×3, and PASS again under vland. One mutant per test FAILs (evidence/tests).
  - **land_scripted copies only rows/**. The coordinator copies tests/ over the landed TestSource files (PassPatterns unchanged). The old
    standalone tests would still pass the landing recheck, so a missed copy would go unnoticed.
- **Drop test** (mechanical: HEAD DEF+UND minus new; `droptest_*.tsv`, `symscan_part2.txt`).
  - Dropped DEF: only 0x0045C5DA's stand-in self, ?CThingNoiseDef_ScalarDeletingDestructor@@YIPAXPAXHI@Z. It has no reloc_bindings line.
    It is named only by its own row, test and self_symbols.tsv lines 1210 / 27219. The census finds no UND of it anywhere.
  - Dropped UND: ??0CMemberBase_0044c0c0, ??0CShoveHaPennyRegionDef (stand-ins for 0x0044C0C0), ?v00@CTCNoiseDef and ?v00@CThingNoiseDef
    (never defined anywhere).
  - The 1-slot ??_7 bodies are replaced by the 100 B EQ ones (the same names).
  - No pk_all EQ copy is lost.
  - reloc_bindings lines witnessed by the rows:
    - `??0CMemberBase_0044c0c0 0044c0c0 00455ef0` goes (TABLE_CHANGES delete; the drop test confirms NF359's proposal).
    - `??_7CTCNoiseDef 01231e9c 0044c1bb` and `??_7CThingNoiseDef 0123388c 00455ef0` stay named: the objects define and store them.
    - `??1CThingNoiseDef 0045c5f6 0045c5da` stays: the new ??_G still calls it.
- **Unit link** (`evidence/link/`), the three new objects + 0x0045C5F6 + 0x00432C40 / 0x004317D0 / 0x004304E0.
  - HEAD has 4 duplicates: ??0CTCNoiseDef 0x0044C1BB v 0x00455EF0, ??0CThingNoiseDef 0x00455EF0 v 0x0045C5F6, and the precedent's 2 Alloc.
  - New has 5: **3 on ?Alloc@CThingNoiseDef, byte- and relocation-identical, all selection 1** (cmpcomdat_unit.txt), plus the precedent's 2.
  - The unresolved set only shrinks: the four stand-in names go, and nothing is new.
  - definers_unit.txt: in the new set no NoiseDef symbol has two bodies.
- **Debt exposed, not added** (definers_census.txt, link/link_unit_census_749810_first.txt, maplink_new_749810_first.txt). Census
  link: + 0x00749810 + 0x00749A70, compiled read-only at HEAD.
  - 0x00749810's NEQ pick-any ??0CTCNoiseDef (29 B) and ??_GCTCNoiseDef (30 B) are the T3 debt lines 3617 / 3618.
  - HEAD already loses to them in link order: with 0x00749810 first, the HEAD link lists 0x0044C1BB's strong EQ ??0CTCNoiseDef as the
    ignored duplicate (`DUP H_0044c1bb.obj ??0CTCNoiseDef@@QAE@XZ in H_00749810.obj`). So the NEQ copy can win at HEAD too.
  - What option A changes: that reported different-body LNK2005 becomes a silent pick-any first-wins race. A /MAP link with 0x00749810
    first keeps 0x00749810's NEQ ??0CTCNoiseDef / ??_GCTCNoiseDef / ??_7CTCNoiseDef (its ??_7 is EQ).
  - Rule: pre-existing copies that differ from retail are queued debt, not a hold, when the change adds or alters none (NF326 Q2 / NF328
    D1, BRIEF_NF_BATCH.md "Differing copies"). Every copy the new objects add is retail-EQ, so this is debt to report. The fix is
    0x00749810's copy repair (T3 3617). Listed for coordinator confirmation (item 5); it does not block ACCEPT.
- **New-symbol census.** The three new objects define no symbol the landed 0x0045C5F6 object does not already define
  (setdiff_v_0045c5f6.txt: section for section identical, DEF / UND equal), so no new name enters the link.
- **Names.**
  - 0x0045C5DA's self is ??_GCThingNoiseDef@@UAEPAXI@Z: Ego_r + FableWin, slot 0 of 0x0123388C (labels_rtti_port_dtors.tsv), a scalar
    body ending `ret 4`.
  - ??0CThingNoiseDef is in both PDBs. ??0CTCNoiseDef is FableWin-only, with the retail RTTI vftable store as the second source.
  - No file rename is needed: the three Source / TestSource names already follow the family pattern. The catalog Module and names are
    unchanged.
  - Selection-only changes (not renames, so not in RENAMES.tsv): 0x00455EF0's ??0CThingNoiseDef@@QAE@XZ and 0x0044C1BB's
    ??0CTCNoiseDef@@QAE@XZ go from strong to pick-any under the same name (self_symbols.tsv lines 1043 / 737 unchanged).

## For the coordinator (no ruling blocks either run dir)
1. **T3 reservations.** land_scripted holds these on col-2 lines:
   - b1 0x0042AF3C: nf349 3590 (the brief parks it) and nf360 3623 / 3624. The nf360 lines are debt that b1 does NOT fix (the strong
     copiers and the different-body pick-any copies), so they must be parked and restored like 3590, not closed by --release-t3.
   - b2: 0x00455EF0 on nf355 3603; 0x0044C1BB on nf355 3604 and nf359 3617. 3603 and 3604 are the ports b2 does, so close them.
     3617 (0x00749810's NEQ ??0CTCNoiseDef copy) stays open debt: park and restore it, or re-add it after --release-t3.
   - 0x0045C5DA has no reserving line.
2. **UNITS.tsv** (unit `nf363_noisedef`): the three rows land whole or not at all. A partial landing (e.g. only 0x0045C5DA, which no T3 line
   holds) would add pick-any ctor copies beside HEAD's strong ones (more LNK2005). land_scripted skips its graded-symbol equality check
   for unit members whose unit id differs from their address (`binder = ... unit[a] != a`). Read the graded symbols from recheck.log
   (this run's logs/recheck_new.log shows the three new leaves).
3. **tests/** must be copied by hand (point above).
4. Proposed T3 text updates (not applied):
   - nf360 3623: the copier table above.
   - nf359 3617: "with the strong 0x0044C1BB copy gone (NF363 b2), the different-body LNK2005 becomes a silent first-wins race; the
     NEQ copy already won at HEAD when 0x00749810 linked first".
5. **Confirm (does not block ACCEPT):** the 0x00749810 race is debt under NF326 Q2 / NF328 D1, not a hold for b2 (Debt paragraph above).

## Process
- **Locks.** All claimed as NF363 and all released (`evidence/locks_final.txt`):
  - 0042af3c, 004ca010 and the ten copiers;
  - 00455ef0, 0044c1bb, 0045c5da (rows);
  - 0045c5f6, 00432c40, 004317d0, 004304e0 and 00749810, 00749a70 (HEAD controls, compiled once, never edited).
  - No other agent's lock, B claim or Codex reservation was touched. 0x00749A70 (T3 nf359 col 2) was compiled read-only for the census only.
- **Repo writes: none by NF363.** `find -newer work/STAMP_START` lists only the coordinator's NF361 b1 landing (14:39: 0x005FB7E0 /
  0x005FE090 / 0x006001E0 / 0x00600B60, agent_tools/nf361, reloc_bindings.tsv, T3 / claims). It landed after these gates ran and is
  disjoint: no GetDef<CThingBaseDef> or NoiseDef instantiation, none of this run's addresses. land_scripted's own recheck covers the
  new HEAD.
- **Prechecks** (work/pre.log, pre_zs.log). Every test was clang-prechecked before its lock and every source after it, each in an
  earlier call than its compile.
  - D1 `cl /Zs`: the tests of 0x005B43A2 / 0x005B4EBA (clang fails only in VC7.1 headers plus `_assert`) and the source of 0x005B4EBA
    (clang fails only in stlport/fable_map.h). A /Zs negative control (an undeclared identifier) gave ZS_FAIL, so /Zs is live.
- **Compiles.** One background script at a time, with `timeout 900` on each line:
  - p1 (b1 HEAD / new + 0x004CA010), cop (ten copiers), p2 (Part 2 HEAD x7 + new x3), ctl3 (two census controls);
  - vland chains b1 and b2; links lk1, lk2, ctl3; test runs rt1 (drafts) and rt2 (mutants).
- No retail bytes are in any hand-back file. The vland oracles stay in work/. The retcmp / vtcmp output carries only verdicts and addresses.
- **Tools** (tools/):
  - from NF360 / NF359, retargeted: nf.py, pass.py, objsec356.py, droptest.py, pkrun356.py, pkcmp346.py, reachflip_coordinator.py,
    cmpcomdat.py, comdatsel.py, symscan360.py, vlandchain356.sh, comdatlist.py, setdiff.py, definers.py, vtcmp.py, vtmap.py,
    retcmp_coordinator.py, objcmp.py;
  - new: copiers363.py (copier census), plant.sh (comparator-level planted control), runtest363.py (behaviour_of plus diagnostics),
    defall363.py (every multiply-defined symbol: definers, selection, body keys), maplink363.py (/MAP link: which object a pick-any symbol
    is kept from), mkrows363.py, mkbatch363_b1.py, mkbatch363_b2.py.
