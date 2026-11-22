# NF270 notes (2026-11-22, round 28b)

Base: the repo tree with nf268 landed (a coordinator commit at 20:11:56 landed during my first minutes; every base copy I use, orig/ and
work/head/, was re-compared with the repo afterwards: byte-equal). NF269 runs beside me: none of its rows touched (every row I compiled was
`released` or had no lock directory before I claimed it: work/locks.log); none of my rows is in codex_pass28 RESERVED.tsv (checked).
Tools: NF268's set re-pathed to agentNF270, plus tools/vtmap.py (NF266's, re-pathed), tools/lnk2.py (lnk.py + an unresolved-symbol list)
and tools/eq.sh. Read-first list: the brief chain NF270 -> NF268 -> NF266 -> ... -> NF215, NF214 and its list (COORDINATION.md, NF92
BRIEF / LESSONS / NOTES, NF44 BRIEF, NF88-90 LESSONS, GOTCHAS.md and EXTRA_RULES.md in full in line-boundary chunks, ASM_RULES.md), the
nf268 landing README and hand-back (NOTES, LESSONS, REJECTED, NAME_FIXES, evidence list).

## Entry 1: slot 1 via the port (0x0069F0F0, 0x006A0D50) -- done
- 0x0069F0F0 (UnsheatheItemFromInventory 6-argument ctor, rows/0069f0f0.cpp): every row-local model goes (CCreatureActionBase with its
  Validate, CSoundPair, CAnimationEntry, CIVCountedPointeeBase, CBaseIntelligentPointer / CIntelligentPointer, the derived class, the
  `#define CCreatureActionBase CCreatureActionBasePdb` aliased include). The row now takes the exact define / include set of its landed
  sibling 0x006A0150 (same class, same header; ANIMCTOR, CAnimationEntry TRANSITION_CTOR, CIVCOUNTEDPOINTER release in place) and the
  header's member names PUnsheatheItem / PFollowUpAction. RELOCATION_MATCH on the first form.
- 0x006A0D50 (UseObject copy ctor, rows/006a0d50.cpp): onto engine/CCreatureAction_UseObject.h (FABLE_RETAIL_CCREATUREACTION_USEOBJECT:
  CRightHandedSet Dummy, C3DVector DummyPos, the private members a copy ctor may name) with the add-only header opt-in
  FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR (hdr/README.md) for the base copy ctor 0x0062EBC0, and FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
  (the inline base ctor + vptr + GetPItem / SetPItem copy, as 0x0062EBC0 itself uses). Form 1 (no COPY: the implicit member-wise copy)
  DIFFER(239v247); form 2 RELOCATION_MATCH. Leaf line: the full mangled ??0CCreatureAction_UseObject@@QAE@ABV0@@Z (HEAD had the bare class
  name; the object's leaf symbol is the same before and after).
- Symbols (evidence/symdiff_*.txt): in both rows the only undefined-symbol changes are the two vtable slots below; new pick-any copies are
  shared-header inlines. pk (evidence/pk_n_*.tsv): every copy with an owner EQ (0x004A96F0, 0x0048C5C0, 0x00668610, 0x00695880 in
  0x0069F0F0; 0x004A9730 and the new ??0CRightHandedSet@@QAE@ABV0@@Z = 0x0049CF80 in 0x006A0D50); the rest NO-OWNER (CSoundPair default
  ctor, CCPPointerInfo ctor, CIVCountedPointer<CAnimComponentMap> release / dtor, ~CAnimationEntry, the ??_G copies, the C3DVector copy ctor).
  Copies that went: the local-model ?Release@CIVCountedPointeeBase@@QAEXXZ (0x0069F0F0), the local CBaseIntelligentPointer calls (0x006A0D50).
- Vtables (vtmap, evidence/vtmap_[hn]_*.txt; every slot of both 70-slot tables checked against retail 0x0125CCBC / 0x0125CEFC):
  slot 1 -> ?Validate@CBaseObject@@UBEXXZ (retail 0x0099A400, its landed self leaf: SAME; was ?Validate@CCreatureActionBase@@UBEXXZ, a
  name no PDB has). Slot 11 (Clone) UBE -> the private EBE both PDBs spell (decision 1). Every other slot is byte- and symbol-identical to
  HEAD. The new 0x0069F0F0 table equals the landed header-based 0x006A0150 table slot for slot. The pre-existing DIFF lines (slot 8
  GetAnimationTransitionInTime, slot 43 GetWeaponTrailUsedInStrikeAction: other rows' stand-in self leaves; the CIntelligentPointer<CThing>
  table's ??_E v ??_G) are unchanged from HEAD.
- Slot-11 census (evidence/slot11_census.txt): each Clone address is referenced once in Fable.exe (its own slot); only these two rows' local
  models declare a virtual Clone; 0x006A0E50 3 -> 2 spellings, 0x006A0D20 2 -> 2 (UBE replaced by the PDB EBE). The self rows still define
  QAE stand-ins (NAME_FIXES).
- Link (evidence/lk_e1_*.txt; LINK unset, /DLL /NOENTRY /FORCE /NODEFAULTLIB): HEAD and new 0x0069F0F0 / 0x006A0D50 with controls 0x0099A400,
  0x006A0E50, 0x006A0D20, 0x0062EBC0, 0x006A0150 (HEAD sources, q.py RELOCATION_MATCH / MATCH each): 0 duplicates both ways. HEAD leaves
  ?Validate@CCreatureActionBase@@UBEXXZ unresolved (2 refs, no definer); new resolves slot 1 to 0x0099A400's body. The Clone symbols are
  unresolved before and after (no landed EBE / UBE definer); the PerformAction slot symbols were proven by nf268 and are unchanged.
  Positive control: a second copy of the new 0x0069F0F0 object gives LNK4006 on its leaf.
- Header proof: whole-tree equiv_check of the COPYCTOR opt-in, 57,125 rows, 0 differ (evidence/equiv_e1.log).
- vland --recheck-landed (evidence/vl_e1.log, overlay = the merged tree): 2 RELOCATION_MATCH + PASS (HEAD tests, unchanged: standalone
  host models, test debt as before).

## Entry 2: 0x0069FCD0 WithHasteAction::PerformAction -- done
- rows/0069fcd0.cpp: the July extern "C" forwarder (self leaf _CCreatureAction_UnsheatheItemFromInventoryWithHasteAction_PerformAction,
  callee the placeholder _ForwardTarget_0069f9e0, no Leaf line) becomes the member on the shared headers (0x006A02B0's define set + the
  add-only opt-in FABLE_RETAIL_CCREATUREACTION_UNSHEATHEHASTE_PERFORMACTION, hdr/README.md): `CCreatureAction_UnsheatheItemFromInventory::PerformAction();`,
  which VC7.1 emits as retail's 5-byte tail jump. RELOCATION_MATCH on the first form; the object defines only the leaf and references only
  ?PerformAction@CCreatureAction_UnsheatheItemFromInventory@@UAEXXZ, which xref binds at retail 0x0069F9E0 (evidence/symdiff_0069fcd0.txt).
- Self rename (RENAMES section 1): both PDBs spell ?PerformAction@CCreatureAction_UnsheatheItemFromInventoryWithHasteAction@@UAEXXZ (Ego_r
  0x006C4FB0, FableWin 0x02212790). Binders of 0x0069FCD0: none (its only Fable.exe reference is vtable 0x0125CDDC slot 3; the one landed
  WithHaste table emitter 0x0069F1F0 has a 1-slot stand-in table), so 0x0069FCD0 1 -> 1 spelling (placeholder -> PDB). 0x0069F9E0 3 -> 2
  (the placeholder ForwardTarget_0069f9e0 goes; the self leaf UAE and 0x005CA68E's UAE remain one spelling plus the table line).
- Name census: no other landed source, header or test defines WithHasteAction::PerformAction or either placeholder (grep).
- Link (evidence/lk_e2_*.txt): HEAD 0x0069FCD0 + HEAD 0x0069F9E0: 0 duplicates, _ForwardTarget_0069f9e0 unresolved. New + HEAD 0x0069F9E0:
  0 duplicates, nothing unresolved; new alone: the UAE reference unresolved (so it is the control that resolves it).
- Test (rows/0069fcd0_test.cpp): the HEAD test only printed its PassPattern. The new test links the REAL row object (the 0x00692670
  pattern): it defines the callee as a probe that records `this` and counts calls, calls the row qualified on raw storage of the retail
  size 0xBC and checks one forwarded call on the same object; PassPattern PASS_0069fcd0 kept.
- vland --recheck-landed (evidence/vl_e2.log, merged tree): 0x0069FCD0 + the header's two includers 0x006A02B0 / 0x008CDD40:
  3 RELOCATION_MATCH + PASS.

## Entry 3: q12744 census, then the mirror -- done (mirrored)
- Census (evidence/e3_census.txt): VC7.1 objects (q.py, catalog flags; HEAD sources, each clang-prechecked first, each lock claimed) of all
  72 landed rows whose source names map<unsigned long,long> or its tree. Besides 0x00AD9770 / 0x00AD9980, four landed rows DEFINE
  ??1?$_Rb_tree@KU?$pair@$$CBKJ@std@@...@QAE@XZ pick-any: 0x00A96DA0 (~CVertexGroupBlock), 0x00AD9F10 (AddBlocks), 0x00AD94F0
  (FindBestStartingBlock), 0x00ADD020 (list<CVertexGroupBlock>::operator=); 0x00ADCE10 / 0x00ADC310 (the DECL_ONLY users) and 0x00ADCE60
  reference it. The other 63 objects show neither (for 17 of them the grade string is cut at 90 characters in the file, so their compile
  status is not shown there; the [] column reads the object if q.py left one).
- Owner: none in retail. No reloc_bindings line and no catalog row names a ~_Rb_tree / ~map / ~_Rb_tree_base of this instantiation
  (grep of both tables); the twelve landed member rows of the same tree / map in other TUs (0x0045C79E operator=, 0x0045C811 _M_copy,
  0x0045C8A1 _M_create_node, 0x0045CA09 / 0x005D87B6 insert_unique, 0x0045CA8F _M_insert, 0x005D83B3 _M_find, 0x005D8446 _M_upper_bound,
  0x005D84F2 _M_lower_bound, 0x005D8548 map::find, 0x005D8916 map::insert, 0x005D8933 map::operator[]) carry other members' leaves. 0x00A96DA0's leaf (retail ~CVertexGroupBlock, RELOCATION_MATCH) calls _M_erase 0x004507A1 directly (xref), so
  retail expands the tree destructor there; the reference in every DECL_ONLY object comes from the pick-any ~map<ulong,long> COMDAT
  (objdump -dr: the 5-byte jump), in .text of that COMDAT, not from a leaf; 0x00AD9770's own copy is referenced only from .debug$F.
- Mirror (rows/00ad9770.cpp, rows/00ad9980.cpp: the NF268 one-line define, re-commented): both RELOCATION_MATCH, every leaf relocation
  unchanged. Symbols (evidence/symdiff_00ad9*.txt): the tree / tree-base / alloc-proxy / allocator destructors, _Destroy<pair>, deallocate
  and the NEQ _M_erase / clear copies (owners 0x004507A1 / 0x00451046, 51 v 51 bytes NEQ at HEAD: pk_h_*.tsv) are no longer defined; the
  tree destructor becomes a reference; the unowned ~map COMDAT goes from 66 bytes to the 5-byte jump 0x00ADCE10 / 0x00ADC310 already emit
  (nf258 D1's NEQ copy is gone). pk: no new copy; every other owned copy as at HEAD (EQ / the pre-existing NEQ vector / CArray copies).
- Link (evidence/link_e3.txt): A (HEAD 4 objects) 0 unresolved tree dtor; B (mirror + HEAD 0x00ADCE10 / 0x00ADC310, NF268's set) 4
  unresolved; C (B + HEAD 0x00A96DA0) 0 unresolved tree dtor, 0 duplicates; D (B + every census definer / referencer) 0 unresolved tree dtor.
  The map copy ctor / tree operator= references (MAP_COPY_OUTOFLINE) are unresolved in every set, as at HEAD. D shows two pre-existing
  LNK4006 lines on ??1CVertexGroupBlock@@QAE@XZ (0x00AD94F0 / 0x00ADCE60 HEAD objects against the 0x00A96DA0 self row; not caused here).
- Tests: 0x00AD9770's HEAD test PASS unchanged. 0x00AD9980's HEAD test links the real row and binds the object's undefined externals by
  /alternatename probes; with the mirror the tree destructor is a fourth undefined external, so the first vland gave LINK_FAIL
  (evidence/vl_e3_first.log). rows/00ad9980_test.cpp adds one /alternatename line and a counting probe (same pattern, nothing else changed):
  RELOCATION_MATCH + PASS (evidence/vl_e3b.log).

## Entry 4: 0x0072CAA0's test -- done
- rows/0072caa0_test.cpp replaces the 177-line standalone host model (own bitset / map / CThing / action models and its own copy of the
  body, which never linked the row) with a test on the row's own define / include set that links the REAL row object: the private PDB leaf
  is reached through an explicit instantiation (access is not checked there, C++03 14.7.2/8; the EXTRA_RULES 2026-11-20 form, test-only);
  SetCurrentAction / PeekPlayer are test-defined members; the two CCreatureAction_CastAssassinRush constructors (thiscall, by-value
  CCountedPointer) and ~CCombatActionBase are __fastcall probes bound by /alternatename (test-only, nf226-nf228 ruling). Raw storage at the
  retail offsets the row documents. Eight cases: untargeted (no targeting component), 0xA9 present, 0xA8 present, targeted with the
  player's Z-targeting flag, targeted without it, dead target, non-creature target, no target (each checks the call counts, arguments,
  the action address handed to SetCurrentAction and its destruction). PassPattern 0072caa0_TEST PASS kept. vland: RELOCATION_MATCH + PASS
  on the first form (evidence/vl_e4.log). The row source is unchanged.

## Entry 5 (q12659, 0x00418DCA): open (not started; context)

## Whole-pass checks
- recheck_final.log: one vland --recheck-landed run (overlay = the merged tree: the repo as of 20:33 + my two headers) over every changed
  row: 0x0069F0F0, 0x006A0D50, 0x0069FCD0, 0x00AD9770, 0x00AD9980, 0x0072CAA0: 6 RELOCATION_MATCH + PASS, 0 RELOC_CONFLICT, 0 WARN.
- New symbol names (each defined by exactly one body; grep of rebuild/src, rebuild/include and rows/): only
  ?PerformAction@CCreatureAction_UnsheatheItemFromInventoryWithHasteAction@@UAEXXZ, defined by rows/0069fcd0.cpp alone (its test calls it
  qualified). The two header opt-ins add declarations only; the test probes are test-only extern "C" names.
- Headers: two add-only opt-ins (hdr/README.md); both whole-tree equiv_checks 0 differ; orig/ re-compared with the repo at hand-back: equal.
- Funclets / folds: no landed source, test or header names any of the six changed files (grep of each file name in rebuild/src,
  rebuild/tests, rebuild/include, the files themselves excepted: 0 each).
- Descriptive names added: none. Flags lines added: none (no changed row has CompilerFlags; none needs one).
- NF268's NAME_FIXES line "0x0072CAA0 (test)" is closed by entry 4.
- Locks (work/locks.log): the six hand-back rows stay NF270 (rows.txt: 0x0069F0F0, 0x006A0D50, 0x0069FCD0, 0x00AD9770, 0x00AD9980,
  0x0072CAA0); every control / census row is back to `released`. locks.log: 87 addresses claimed (71 had no lock directory: plain mkdir; 16 read
  `released`), 3 of them released again uncompiled (0x0093EA90 / 0x00940960 / 0x00694430: clang precheck fails on HEAD). Intersection
  of those 87 with codex_pass28 RESERVED.tsv and with NF269's / the brief's held addresses: empty.
- Nothing written in the repo (work/root_before.txt v root_after.txt; `D:\Documents\FableTLC` in the repo root predates this session,
  Oct 24); no git; no sleep (jobs waited on by PID: kill -0, or a pidfd select on the PID); no rm -rf (tools/clean.py).

## Rule slips
- vl_e1 ran the HEAD tests of 0x0069F0F0 / 0x006A0D50 before a pre.sh call covered them (prechecked afterwards: pass).
- vl_e2 ran the HEAD tests of the includers 0x006A02B0 / 0x008CDD40 before their pre.sh; prechecked in the next call: clang errors only in
  <functional>:154, <cwchar>:9, <ostream>:51, <istream>:39, reproduced exactly by a two-line VC7.1 control (work/clang_ctl*.cpp, the nf258
  precedent): a clang limit on the VC7.1 SDK headers, not a missing include or #error. 0x00AD9770's HEAD test shows the same control errors.
- Waits: the equiv run 2 and census waits used a pidfd select on the job's PID in a background call; the vl_e3, vl_e3b, vl_e4 and
  recheck_final waits used the same pidfd select in the foreground (a blocking wait on the PID, no sleep command): judge whether that
  fits "wait by PID in a later call".
- The first run-2 equiv_check was aborted (killed by its own PIDs) when the nf271 landing changed a header under it; rerun on a rebased tree.

## Decisions for the coordinator
1. Entry 1: land the two ports + the COPYCTOR opt-in. They also move vtable slot 11 (Clone) from the local models' public UBE to the private
   EBE both PDBs spell (0x006A0E50 3 -> 2 spellings, 0x006A0D20 2 -> 2): beyond the brief's slot 1, so it needs your yes. The Clone self rows
   still define QAE stand-ins (NAME_FIXES; their ports need the copy constructors declared).
2. Entry 2: land the 0x0069FCD0 self rename (placeholder -> PDB leaf, no binders) with the UNSHEATHEHASTE_PERFORMACTION opt-in and its new
   linking test.
3. Entry 3: land the mirror (census: four other landed rows define the tree destructor pick-any, no retail owner) together with the
   0x00AD9980 test change (one /alternatename probe). Note the pre-existing ~CVertexGroupBlock LNK4006 finding (NAME_FIXES).
4. Entry 4: land the new 0x0072CAA0 test (catalog PassPattern unchanged).
5. q12659 to the next agent.
