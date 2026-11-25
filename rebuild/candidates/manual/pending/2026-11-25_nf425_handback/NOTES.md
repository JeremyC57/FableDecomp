# NF425 hand-back (2026-11-25): the CLONE / COPY virtual family (rtti_identified)

Status: final hand-back (tree as seen during the run; rebuild/oracles/reloc_bindings.tsv changed under me at 08:39, every gate ran after that).

## Family and shape histogram (tools/wl425.py -> evidence/worklist.tsv, evidence/shapes.tsv)
518 rtti_identified addresses whose RTTI vftable placement names `?Clone@<C>@@...` (350) or `?Copy@<C>@@...` (168); all landed.
RTTI symbols: 307 `?Clone@<C>@@UBEPAVCCreatureActionBase@@XZ`, 22 `EBE` (private), 1 `MBE` (protected), 13 `...CAnimComponentBase`,
3 `...CAnimFilterBase`, 2 `...CAFile`, 2 navigator interfaces; 168 `?Copy@<C>@@UAEXPBVCDefClassBase@@@Z`.

| shape | rows | retail body |
|---|---:|---|
| CLONE_INLINE | 234 | `new C(*this)`, C's implicit copy ctor inlined: base copy ctor call (mostly CCombatActionBase 0x0062F3B0) + vftable store + member copies |
| COPY_JMP | 123 | `*this = *(const C*)other;` -> 5-byte `jmp` to C's (or an ancestor's) out-of-line operator= |
| CLONE_OOL | 100 | `new C(*this)`, C's copy ctor out of line (call) |
| COPY_BASE_MEMBERS | 44 | operator= inlined: base operator= call + member copies |
| CLONE_VCALL | 13 | CAnimComponentBase clones: virtual create call + member copies |
| CLONE_OTHER / COPY_OTHER | 3 / 1 | residue |

HEAD selves: 175 `?Clone@<C>@@QAEPAVCCombatActionBase@@XZ`, 99 `...QAEPAVCCreatureActionBase...`, 38 `CBase_<addr>` returns, 92 extern "C"
`_<C>_Copy` forwarders, others free `__fastcall` `<C>_Copy` stand-ins (worklist column self_leaf).


## Result
- **402 of 518 rows ACCEPT in 14 batches; 11 HOLD in batches/ihold** (CLONE_INLINE rows with real row files and per-row reasons).
  The other 105 family rows were never generated (no shape generator fits them, see T3.txt): they have no row files, so they are
  listed as residue lines in T3.txt with a reason per row instead of a HOLD folder.
- **i1..i4 and h1..h3 (186 rows) are ACCEPT only if RULINGS_NEEDED r1 is granted.** They pass through two finalize allowances: objgate
  NOT_EQ (the class ??_7 COMDAT and an added pick-any ??_G) and pk copies 1 -> 2 (that ??_G). j1..j3, s1, o1..o3 (216 rows) are clean
  without any ruling.
- Late re-forms (after advisor review): 66 CLONE_INLINE rows (h1..h3) and 5 CLONE_OOL rows (o3) moved from a local class onto the
  class's own engine header where a polymorphic branch declares the member and gives retail's leaf; every gate was re-run for them.
- Locks: the 402 ACCEPT rows stay NF425. Released: the 11 HOLD rows, the 105 residue rows and every definer row I locked only to compile
  it (216 operator= / copy-constructor owners, 49 base-ctor / NF424 ??_G rows).
- No header edits, no catalog renames (catalog Module / Name already match the RTTI class for all 518), no FLAGS changes.

| batch | rows | shape | generator | table |
|---|---:|---|---|---|
| j1 / j2 / j3 | 40 / 37 / 18 | COPY_JMP | gen_copyjmp.py | j1 40 add + 16 delete, j2 32 add, j3 15 add |
| s1 | 26 | COPY_JMP as Option S thin rows (UNITS kind S) | gen_copys.py | 26 add + 9 delete |
| o1 / o2 | 47 / 43 | CLONE_OOL, local class | gen_cloneool.py | none |
| o3 | 5 | CLONE_OOL on engine/<C>.h (a branch declaring Clone + the copy constructor) | trial_oj.py + o3 former (NOTES) | none |
| i1 .. i4 | 34 / 23 / 32 / 31 | CLONE_INLINE, local class (engine/<C>.h has no usable polymorphic branch; reason per row in its Header line) | gen_cloneinl.py | i1: base copy-ctor stand-in lines (1 rewitness, 1 delete) |
| h1 / h2 | 50 / 10 | CLONE_INLINE on engine/<C>.h + its NF329 VIRTUAL_MODEL opt-in | gen_cloneinlh.py | none |
| h3 | 6 | CLONE_INLINE on another engine/<C>.h branch that declares Clone | gen_cloneinlg.py | none |
| ihold | 11 | CLONE_INLINE HOLD | - | - |

## The generators (one per shape; tools/)
- **gen_copyjmp.py (COPY_JMP, retail = one 5-byte `jmp` to an out-of-line operator=).** Row = the real `void C::Copy(const CDefClassBase*)`
  with the def idiom `*this = *(const C*)other;` and `C& operator=(const C&)` declared out of line (the landed 0x0046F4EB CSkyDef form).
  Target identity per row, never by +5: OWN (the jmp target carries ??4C in the table / owner Leaf / tracker) or PLUS5 (owner is the
  address-named `CBase_<base>_Derived_<addr>` stand-in: ??4C from the PDB name sets + the RTTI-proven caller binding + the target body
  opening with the ancestor operator= call). Form F1 = own header with a landed prelude where the header declares Copy and operator= (13);
  F3 = local class over the RTTI direct base header (CSubDefClassBase / CParentDefClassBase / ...) (108). Symbol check: DEF == [leaf],
  target in UND, no ??_7 / ??_R / ??_G / ??4 definitions. Test links the row object, defines C::operator= and checks (this, &other, count).
- **gen_copys.py (s1).** 26 COPY_JMP rows whose operator= owner row's TU already defines the same Copy strongly (object DEF census of all
  216 owner objects): a second definition would be an LNK2005. Both PDBs put Copy and operator= in tc_register.obj (per-row indices in the
  row header), so each row becomes an Option S thin row `#include "../src/compiled/<owner>.cpp"` (form T, the nf376 shape) with its own
  Leaf. sgate425.py: the thin row object equals the owner's HEAD object section by section, and the owner TU's Copy equals the HEAD leaf.
  The owner rows are NOT edited. HEAD tests kept (self-contained; leaf bytes and relocation targets unchanged: GOTCHAS NF419 r1).
- **gen_cloneool.py (CLONE_OOL, retail = new(SZ) + call of C's out-of-line copy ctor).** Row = the real `virtual B* C::Clone() const`
  (access from the RTTI symbol: U public, E private, M protected) on a local class over the RTTI direct base header (CCreatureActionBase /
  CCombatActionBase MELEE prelude of the landed 0x008CAA70), C's copy ctor declared, C's own members one opaque block sized from retail
  (`_body[SZ - sizeof(Base)]`, as HEAD), PerformAction declared where a PDB name set gives C's override (the base's is pure). Callee
  identity: PDB name set + retail callee storing C's vftable first (+ the landed row defining it: all 95 owners do). Leaf bytes compared
  with retail inside the generator. Test links the row object, defines operator new and the copy ctor (size, this, other, null path).
- **gen_cloneinl.py (CLONE_INLINE, retail = new(SZ), base copy ctor, C's vftable store, member moves).** Row = the real Clone on a local
  class derived directly from CCombatActionBase (MELEE prelude, MELEE_COPYCTOR 0x0062F3B0) or CCreatureActionBase (COPYCTOR 0x0062EBC0);
  never a layout-rename (the inlined ctor emits C's vftable). C's members are the Ego_r.pdb members at retail offsets (PDB - 0x18 /
  PDB - 0xC), each checked against retail's copy (offset, width). Test links the row object (vftable slot targets mapped to one unused
  stub by `/alternatename`), defines operator new and the base copy ctor, compares base bytes and each member.
- **gen_cloneinlh.py (CLONE_INLINE on the shared header).** For a class whose engine/<C>.h has the NF329 `FABLE_RETAIL_<C>_VIRTUAL_MODEL`
  opt-in (declarations only, retail size, slot order probed against retail, Clone declared): the row is the out-of-class
  `CCreatureActionBase* C::Clone() const { return new C(*this); }` after the MELEE prelude + the model opt-in + its `Needs` defines.
  Checked: leaf EQ, DEFs only leaf / ??_7 / ??0 / ??_G, base copy ctor + ??2 in UND, ??_7 slot agreement >= the local-class form. Test =
  the i test with its /alternatename list rebuilt from this object's UND set.
- **gen_cloneinlg.py (h3)** = gen_cloneinlh.py for an explicit branch found by tools/trial_gen.py (every #ifdef FABLE_RETAIL_* block of
  engine/<C>.h, and the default branch, that defines `class C : public ...` and declares Clone; leaf EQ and ??_7 >= local required).
  trial_gen.py also gives each remaining i row its Header reason (hdrnote425.py): no header 49, branch without Clone 67, Clone declared
  protected while the RTTI symbol is public/private 4, flat byte model only 3, leaf length differs 2.
- **o3**: tools/trial_oj.py compiled the real member on every engine/<C>.h branch declaring Clone + `C(const C&)` (12 o classes) or Copy +
  operator= (1 j class): 5 o rows give retail's leaf with no extra definitions and became o3 (row = the o row's identity lines + the
  header include + `return new C(*this);`; the o test kept: it links the row object, defines operator new and the copy constructor); the others do not compile in the row's prelude (C2143 / C2039 /
  C2144). The other 40 o headers declare Clone but no copy constructor (the implicit one would be inlined, not retail's call).

## Gates (every ACCEPT row; batches/<b>/CHECKS.tsv + evidence/)
- vland HEAD / new / mutant at one LANDVERIFY_WORK path per batch: grade kept (COPY MATCH / RELOCATION_MATCH, CLONE RELOCATION_MATCH),
  tests PASS, 0 LITERAL warns; mutant (Copy: `other` -> `this`; Clone: `return 0`) FAIL on every row. New warn kinds: only
  SHARED_HEADER_AVAILABLE on 74 COPY_JMP F3 rows (j1 30, j2 32, j3 12) (a local class while engine/<C>.h exists: header debt, precedent NF417 / NF423 r2; T3).
  Every HEAD RELOC_NAME warn on these rows is gone. i-batch tests were fixed once (padding after a 1-byte member is not copied) and rerun
  (vland_<b>n2 logs) before the mutation runs; the i rows' Header comment was then corrected (comment only) and vland re-run on exactly
  the hand-back text (vland_<b>n3 logs, all PASS; the mutation logs stand).
- objgate (gate425.py, renames inferred from the leaf and checked against expect.tsv): COPY_JMP ALL_EQ except `.drectve +/DEFAULTLIB:"libcp"`
  on 8 rows; CLONE_OOL the same `libcp` line on all 95 (o3 included) (STLport map behind engine/CCreatureActionBase.h; NF412 / NF417 precedent);
  CLONE_INLINE: only the class's ??_7 COMDAT (HEAD's was a 1-slot made-up vftable) and an added pick-any ??_G<C> (RULINGS_NEEDED r1).
- retg425.py: every added ??_G is byte- and relocation-EQ to retail's vftable slot 0 (186/186). vtcmp425.py: retail slots named right
  HEAD 0 -> new 6..68 (evidence/vtcmp.tsv). vtall425.py (evidence/vtall.tsv): no landed ??_7 copy of the same class (729 objects compared)
  names a retail slot an ACCEPT row's copy misses.
- pk_all + pickany_vs_retail with the leaf named (HEAD = old self, new = new self): 0 NEQ on both sides; copies unchanged except the ??_G
  (CLONE_INLINE) and the owner TU's own copies in s1 (all EQ at their retail addresses).
- /FORCE link (link377 via linkgate425.py): rows + every jmp / ctor owner row + the base copy ctors + every landed ??_7 / ??_G emitter of
  the same classes (CLONE_INLINE): duplicates j1 4 -> 0, j3 1 -> 0, i1 14 -> 14, i2 2 -> 2, i3 12 -> 12, i4 4 -> 4, h2 4 -> 4, h3 2 -> 2
  (pre-existing among the definers), all others 0 -> 0 (s1: HEAD 52 objects v the 26 owner objects once, 0 -> 0); every new self
  DEFINED; callee renames: 74 + 28 unresolved -> resolved, 21 unresolved before and after (owner rows still define address-named stand-ins:
  NAME_FIXES.tsv), 0 lost.
- census425.py (text) + object DEF census of every owner: no other landed object defines a new self except the 26 s1 owners (handled by
  Option S). No table line binds any old or new self; no landed caller binds an old self (they are reached only through vftables).
- apply_table --dry: 0 misses, multi-target 1 -> 1 per batch and all together. reserved_check (RESERVED_REFS="", final batch files incl. ihold): 413 rows, 0 reserved, 0 mentions.
- census: the h3 classes' new Clone names appear in the 0x00667450 test only as its own stub definitions (test-local, not linked with
  these rows); no caller binds an old self.

## Names (two sources each)
- Selves: the retail RTTI vftable placement (labels_rtti_port.tsv) + the PDB name sets (Ego_r and/or FableWin; per row in the header).
- Callees: COPY_JMP ??4C: table / owner Leaf / tracker PDB spelling + PDB name set (+ order_match where it predicts) or, for the 21 PLUS5
  rows, the PDB name set + the RTTI-proven caller binding + the target body (ancestor operator= call first). Copy ctors (CLONE_OOL): PDB
  name set + retail callee storing C's vftable first. Base copy ctors: both PDB name sets + the MELEE_COPYCTOR / COPYCTOR opt-ins.
- Members (CLONE_INLINE): Ego_r.pdb names; enum-typed members written `long /* E... */` (type name only, no layout effect).

## Debt and notes for the coordinator
- Shared ??_7 review (late in the run, after advisor review; final numbers in the Result table above): the 47 NF424 ??_G rows include engine/<C>.h with its NF329 VIRTUAL_MODEL
  opt-in (the header declares Clone), and their ??_7 named more retail slots than my local class in 33 of 47 classes (evidence/vt424cmp.tsv).
  65 of my 197 CLONE_INLINE classes have that opt-in. tools/gen_cloneinlh.py re-formed them onto the header: 60 give retail's leaf with a
  ??_7 at least as good as any landed copy (now h1/h2; vland, mutation, objgate, pk, retg, link all re-run); 5 do not give the leaf (the
  model's trailing padding array is copied) -> HOLD. Then every landed row that can emit one of these ??_7 (413 constructor /
  deleting-destructor / Clone objects, compiled HEAD into work/g) was compared slot by slot (tools/vtall425.py, evidence/vtall.tsv):
  4 more local-class rows lost slots to a landed copy -> HOLD; a second pass over 316 more objects (rows that build or define the
  class elsewhere) found 2 more -> HOLD (11 HOLD in all). A per-branch trial (trial_gen.py) then moved 6 more rows onto another header
  branch (h3).
- T3 queue lines resolved: nf324 lines on 0x004536A9 0x00453810 0x00453899 0x00453BE3 0x00453C1B 0x00453CE5 0x00454038 0x0045417E 0x0045473A
  0x0045C549 0x0045D500 0x0045FEDE 0x004600C6 0x0046244C 0x00462B10 and nf386 q1 (all ACCEPT here: port done).
- Tools (tools/): wl425.py, lib425.py, gen_copyjmp.py, gen_copys.py, gen_cloneool.py, gen_cloneinl.py, gen_cloneinlh.py, vt424cmp.py, vtall425.py, t3debt425.py, fixtest_ci.py, mkbatch425.py,
  runpair425.sh, chain.sh, mut425.sh, retest425.sh, gate425.py, sgate425.py, pkgate425.py, warncmp.py, linkgate425.py, link377.py, defobj.py,
  census425.py, retg425.py, vtcmp425.py, finalize425.py, split425.py, gen_cloneinlg.py, trial_gen.py, trial_oj.py, hdrnote425.py,
  retest3_425.sh, trial_hdr.py.
- head/payload.json in each batch is the vland HEAD payload; oracle.tsv files (retail bytes) stay in work/.
- T3.txt: 105 residue lines (COPY_BASE_MEMBERS 44, CLONE_INLINE 37, CLONE_VCALL 13, CLONE_OOL 5, CLONE_OTHER 3, COPY_JMP 2, COPY_OTHER 1),
  then 267 debt lines (tools/t3debt425.py): port 11 (the ihold rows), header 74 (SHARED_HEADER_AVAILABLE on j F3 rows, with what the
  header lacks: neither Copy nor operator= 51, Copy without operator= 20, operator= without Copy 2, both but no compile 1), vftable 120
  (i1..i4 classes need a polymorphic branch declaring Clone, then move onto the header like h1..h3), respell 21 (NAME_FIXES owners),
  unit 26 (s1 Option S), resolved 15.
- apply_table --dry over all 14 TABLE_CHANGES together (add 113, delete 26, rewitness 1): 0 misses, multi-target 1 -> 1, no new.
- orig/ copies re-checked against the repo at hand-back: all 413 rows (402 ACCEPT + 11 HOLD) unchanged since the batches were built.
- Gate tool fix during the run: gate425.py had cut objgate_summary detail at 400 characters, which made 7 long-named CLONE_INLINE rows
  look HOLD (a cut `libcp` / `??_G` item); the summary is now uncut and i1..i4 were re-gated and re-finalized.
- Final checks: batch rows / tests byte-equal the payloads vland graded (work/p_<b>n); no mutant body left in any row; no retail byte
  dump in the hand-back (oracle.tsv stays in work/); no NF425 process left running.
