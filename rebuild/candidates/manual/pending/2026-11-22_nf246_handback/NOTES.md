# NF246 notes (2026-11-22, round 16): the wrong DTOR_OUTOFLINE families (entry 1); entry 2 not started (context)

## 1. Entry 1: the three wrong-`~list` families, each as self + binders (17 rows changed, all RELOCATION_MATCH PASS)
Rule (nf241 / nf243): a list destructor relocation is `~list` only if its retail target is a 5-byte `jmp` thunk; a body with such a
thunk is `~_List_base` (STLport 4.6 `~list() {}`). Census: callers.py (E8/E9 sites) + a same-day HEAD compile of every referencer
(work/hc/, tools/hcpk.sh = sc.sh + pk.py on the HEAD object). pk.py needs this hand-back's owners.tsv (the ~list thunks are catalog-only).
No header was edited (no hdr/ / orig/): every row uses existing opt-ins or in-place respells of its own local model.

### 0x005127A0 (list<C2DLineF>; thunk 0x00514280): self + 8 binders; spellings 5 -> 4
- **Self:** `~list` -> `template std::_List_base<C2DLineF, allocator<C2DLineF> >::~_List_base();` on engine/C2DLineF.h +
  stlport/fable_containers.h (the 0x0063F360 / 0x004046B0 pattern); defines only shared-header inlines besides the leaf; new test on the
  shared headers (populated + empty list). At HEAD the self row strongly defined a 62-byte `~list` while 8 rows defined the 5-byte
  pick-any `~list` copy (EQ 0x00514280): that clash is gone.
- **Thunks:** 0x00514280 (`e9 1b e5 ff ff`, after the list<C2DLineF> ctor 0x00514260, 0x18 node) = ~list (layout); 0x00A81C40 is a
  second standalone `jmp 0x005127A0` in the heap TU: only one can be ~list<C2DLineF>; undecided (NAME_FIXES). No reference to either.
- **Binders:** 0x00A82BE0 / 0x00A83570 NF241's swap (DTOR_OUTOFLINE -> BASE_DTOR_VISIBLE_NOINLINE; removing DTOR_OUTOFLINE drops it from
  the FABLE_STLP_NOINLINE trigger list at fable_containers.h:32, measured harmless: same relocations, all list copies EQ);
  0x00A81D60 (pair ??_G): local C2DVector / C2DLineF / pair / list models -> shared headers + BASE_OUTOFLINE; 0x00A820E0 (__push_heap):
  local models and local body -> the shared DECAL_HEAP body with 0x00A82BE0's macro set (0x00A82BE0's pick-any __push_heap copy was
  NEQ 0x00A820E0 at HEAD only by its ~list relocation and is EQ now); its HEAD test linked a counting `~list` into the object and could
  not link any more (LINK_FAIL): rewritten on the shared headers (heap order over unsigned 64-bit keys, deep copies, root). Three rows
  with vector models respelled IN PLACE (no class token added): 0x00A84690 (the declared destructor moves from list to its local
  _List_base), 0x00A85240 (its same-TU 50-byte simplified ~list stand-in, NEQ, becomes ~_List_base with the STLport body: copy EQ
  0x005127A0), 0x00A852B0 (noinline moves from ~list to ~_List_base). Per row the HEAD v new relocation diff renames only the 0x005127A0
  relocation (0x00A85240's leaf is a prefix, so xref.py needed the full mangled leaf: both relocations renamed).
- **Pick-any copies (advisor check):** every HEAD NEQ copy of the family's rows was read (work/hcpk_f1.tsv): 0x00A84470 (sort_heap,
  DTOR_OUTOFLINE, no list destructor relocation in its leaf) carried a pick-any __pop_heap copy NEQ 0x00A83570 only by its `~list`
  relocation: the same swap, now EQ (leaf relocations unchanged). 0x00A83680 / 0x00A84A60: no such copy. The other HEAD NEQs are
  unrelated debt (0x00A81E20 / 0x00A82B10 _M_insert_dispatch / operator= bodies, 0x00A843F0's global-list _M_insert_dispatch).
  NF243 / NF245 HEAD symbol dumps: only 0x005EB590 / 0x00682810 (both moved) DEF a ~list<(const) CThingAICreature*> copy.
- **Left:** 0x00517650 (`??1CMember_005127a0`), 0x00A81030 (`?Dtor_005127a0@T`): ??_G-shaped rows of unidentified classes holding a
  list<C2DLineF> at +8 / +0; 0x00A83CC0 / 0x00A843F0 (`??1list@@`, a global stand-in; own leaves bare `__unguarded_partition` /
  `__linear_insert`). NAME_FIXES. 0x00A81E20 / 0x00A82B10 already bound ~_List_base.

### 0x004AB4F0 (list<CThingAICreature*>; thunk 0x004AC080): self + 2 binders + the list ctor; spellings 7 -> 5
- **Self:** the `SortTreeRecursively_00436900(TreeList*)` stand-in -> `~_List_base<CThingAICreature*>` (0x004A3740 already bound it).
- **Binders:** 0x005EB590 NF241's swap; 0x0051A2F0 (map<CThingBuilding*, list<..> > tree erase): NOT blocked like 0x00756D00: the shared
  fable_map.h has `_Rb_tree::erase(iterator)` (FABLE_RETAIL_STLP_TREE_ERASE + _BODY), so the whole local std block went (list via
  BASE_OUTOFLINE); RELOCATION_MATCH, only the list relocation renamed, Rebalance call unchanged.
- **0x004AC060:** the made-up `ListNode_AllocateSelfLinked_RegionLoad` -> `list<CThingAICreature*>::list(const allocator&)`, the spelling
  its only caller 0x004A3740 binds (2 -> 1); layout: right before the ~list thunk. Descriptive-by-library + layout (bytes equal a
  _List_base ctor: the 0x004046F0 ruling-2 situation, here with the caller's binding as the first source).
- **Blocked:** 0x0051BF90 (map::operator[]): one-parameter local list / map models (`??1?$list@PAVCThingAICreature@@@std@@QAE@XZ`), a
  3-argument stand-in own leaf, and same-TU stand-in bodies it needs for scheduling: needs a map::operator[] port. It defines its own
  noinline `~list` stand-in, so no binder names an undefined function; its table line stays as the witness.
- **Left:** 0x00516F10 (`@InnerDtor_004ab4f0@4`), 0x00519560 (`??1CValue_004ab4f0`), 0x006C5E90 (`N_004ab4f0::CThingList`, modelled as
  list<CThing*>): generated stand-ins of unidentified classes (NAME_FIXES). Note 0x006C5E90's model says list<CThing*> at an address
  that is list<CThingAICreature*>'s ~_List_base: worth a look when that row is ported (retail does not fold COMDATs).

### 0x00685BC0 (list<const CThingAICreature*>; thunk 0x006860E0): self + both binders + the list ctor; spellings 3 -> 1
- **Self:** SortTreeRecursively stand-in -> `~_List_base<const CThingAICreature*>`.
- **Binders:** 0x00682810 NF241's swap; 0x00986510: its global-namespace `allocator` / `list_node` / `list` models -> the shared std::list
  (BASE_DTOR_VISIBLE_NOINLINE); its local CTCHeroStats model's `Wives` member and `GetWivesInMarriageState(list&, long)` respelled in
  place onto the PDB signature engine/CTCHeroStats.h declares (`std::list<const CThingAICreature*>&, EMarriageState`, MS forward enum as
  the header does). Its three relocations now bind exactly what 0x00682810 binds: list ctor 0x006860C0, ~_List_base 0x00685BC0 (x2),
  GetWivesInMarriageState 0x0057AE53 (binder spellings 2 -> 1 at each). Its other local models (CThing, CTCHeroStats, CIntelligentPointer,
  CAIBrain, CAIStateGroup_OpinionReactionRoot ...) are HEAD debt (vland SHARED_HEADER_AVAILABLE warnings, unchanged).
- **0x006860C0:** the made-up `DoubleLinkedList_InitializeEmpty` -> `list<const CThingAICreature*>::list(const allocator&)` (both callers
  bind it; layout as 0x004AC060). Its object also defines a `_List_base(const allocator&)` copy (32 B, no retail owner) and a
  ~_List_base copy EQ 0x00685BC0.

### Checks
- recheck_final.log: part 1 one vland --recheck-landed run over all 17 changed rows; part 2 the 13 unchanged referencers /
  DTOR_OUTOFLINE rows of the three families compiled today (landed sources).
- Link (the 17 new objects together, lnk.py): 0 duplicates (recheck_final.log). pk.py (owners.tsv): every new list copy EQ its retail address (~_List_base
  0x005127A0 / 0x004AB4F0 / 0x00685BC0, ~list 0x00514280 / 0x004AC080 / 0x006860E0, list ctors 0x006860C0 / 0x004AC060); the NEQ copies
  left are all NEQ at HEAD too (0x005EB590 3, 0x00682810 4, 0x00A84690's 4 helper stand-ins, 0x00A85240's _M_fill_insert stand-in, 0x00A852B0 2): debt, nf203 (work/fin_final.pk.tsv).
- Anonymous-namespace hash: 0x00682810's UNKNOWN_THREAT_OPINION_SOURCE `?A0x<hash>` changes with the compiled file name (NF245 lesson).
- Funclets: no source includes a changed file (one test mentions 0x00682810 in a comment). Flags: no new `// Flags:` lines.
- Descriptive names: all STLport spellings (descriptive-by-library); 0x004AC060 / 0x006860C0 also by layout.
- File / Module renames for the coordinator: 00/51/stdlistC2DLineF_list_005127a0.cpp (+ test, Module std::list<C2DLineF>) ->
  _List_base; 00/4a/CGuiControlTreePane_SortTreeRecursively_004ab4f0.cpp and 00/68/..._00685bc0.cpp (+ tests; Module `_global`) ->
  std::_List_base<CThingAICreature*> / <const CThingAICreature*>; 00/68/global_DoubleLinkedListInitializeEmpty_006860c0.cpp and
  00/4a/global_ListNodeAllocateSelfLinkedRegionLoad_004ac060.cpp (+ tests, Module `_global`) -> std::list<..>. PassPatterns unchanged.
- vland STANDIN_LEAF warnings on 0x0051A2F0 / 0x00A820E0 (and others): those leaves are unchanged from HEAD (the warning compares the
  graded leaf with the catalog name), so they warn at HEAD too.
- Rule slip: one foreground `sleep 45` while polling a background compile (then kill -0 only).
- selfcheck.py does not exist on this lane; the checks were done by hand.

## 2. Entry 2: not started (context): QUEUE_LINES.tsv (39 lines), 0x0049D800, 0x01296F14.

## 3. Decisions needed from the coordinator
1. Land entry 1 as three units (17 rows). Spellings: 0x005127A0 5 -> 4, 0x004AB4F0 7 -> 5, 0x00685BC0 3 -> 1, 0x006860C0 3 -> 1,
   0x004AC060 2 -> 1, 0x0057AE53 binders 2 -> 1; no address grows.
2. 0x00A85240 (in-place respell) now defines a row-local `~_List_base<C2DLineF>` body, a symbol the self row owns: EQ 0x005127A0
   (62 bytes, pk.py) and links clean, but it falls under the in-place rule's condition (2): accept, or hold the row (it would then stay on
   `~list`, a NEQ 50-byte copy, as at HEAD)?
3. 0x00A820E0's HEAD link test could not survive the port (it supplied `~list` into the object); the new test is on the shared headers and
   exercises the template body in the test TU (/Od, cdecl), not the /Gr object copy (the 0x00A82BE0 test's approach). Accept?
4. 0x004AC060 / 0x006860C0: list ctor by the caller's binding + COMDAT-run position (bytes equal a _List_base ctor): as nf245 ruling 2.
5. Catalog: 0x00514280 / 0x004AC080 / 0x006860E0 (~list thunks) and the 0x00514260 placeholder (NAME_FIXES); 0x00A81C40 undecided.
6. Table lines (TABLE_CHANGES): 5 deletions, 1 replacement; the 0x0051BF90 line stays as the blocked witness.
7. Locks: the 17 changed rows stay NF246; the untouched family rows are released.
