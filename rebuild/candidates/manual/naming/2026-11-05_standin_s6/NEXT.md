# Stand-in naming (step 6, first pass) -- what is left (2026-11-05)

Work list: STANDIN_INACCURATE.tsv, 9,739 rows. This pass fixes 1,793 of them (8 waves, 1,796 changed rows incl. 3 callers).

| Reason | Rows | Left after s6 |
|---|---|---|
| generic placeholder | 3,386 | 2,459 |
| names another function | 2,974 | 2,534 |
| address-only name | 2,745 | 2,704 |
| free function lacks its class | 634 | 249 |
| total | 9,739 | 7,946 (121 of them phase-4 owned) |

## Tools (`$S/plan/s6/tools`)
- `frename.py` (free Class_Method renames), `mrename.py` (member class/method renames, method-only mode for header classes),
  `callers.py` (respell landed binders of a renamed definer), `chk.py` (gate changes must equal the plan), `w1/w6/w7_gen.py`
  (CCountedPointer members via engine/CCountedPointer.h with a typed-caller/PDB T, else CUnprovenPointee_<addr>),
  `s6pe.py` / `s6vtscan.py` (retail PE reader; vtable -> RTTI class), `finalize.py`, `left.py`, `gradewave.sh` / `regrade.sh`.
- Gotcha: tests are standalone programs; a rename must cover `&Name` too, not only `Name(` (w2 -> w2b).

## Families that need a header model first (no repo-header edits were allowed in this pass)
- `?Clear@List@@QAEXXZ` / `SortTreeRecursively_<addr>` StdList_Clear (88 + 58 + 3): Lionhead's out-of-line COMDAT is
  `std::list<T>::clear()` (Ego_r: 45 `?clear@?$list@...` symbols, no `_List_base::clear`). The pilot through
  `_List_base<T>::clear` passes gate, but the accurate name needs `list::clear` declared out of line in
  stlport/fable_containers.h behind a new sub-macro (e.g. FABLE_RETAIL_STLP_LIST_CLEAR). T unknown -> CUnprovenElement_<addr>.
- `??1list_T_<addr>@std@@` (131) / `??0list_T_0x8_<addr>@std@@` (97): ~list / list ctor; ~list is declaration-only in the header.
- `?map@map_T_<addr>@std@@` (53), `??0_Rb_tree_base_node0x18_<addr>@std@@` (36): std::map / _Rb_tree_base ctors (fable_map.h).
- `?Fixup@T@@` / `?M@S@@` RbTreeIterator_PreIncrement (33): `_Rb_tree_iterator::operator++` calling the out-of-line
  `_Rb_global<bool>::_M_increment`; fable_map.h defines _M_increment inline, so it needs a declaration-only sub-macro.
- `?TransferBinaryIn_<addr>@@YG...` (101): `CPersistTraits<T>::TransferBinaryIn(CMemoryDataInputStream&, T&)` (Ego_r: 49, QAE
  thiscall, `this` unused, `ret 8`); needs the member in engine/CPersistTraits.h and the T per row (read width).
- 164 RTTI-proven members whose stand-in class is generic (S/T/C/address) but whose real class has an engine header (w5
  DEFERRED): the method must be declared in engine/<Class>.h (game-class header waves), or converted to a free Class_Method
  with the body's `this` rewritten.
- 12 CCountedPointer converting ctors (w7 DEFERRED: callers bind `??$?0<Derived>@?$CCountedPointer@<Base>@@`): need Base/Derived.
- 17 `?ShareData@CCPtr@@QAEXPAXPAJ@Z` (pragma s, out-of-line release call): the header cannot express the call.

## Families that need more evidence (one source today -> PROPOSAL / owner)
- `??1T@@QAE@XZ` SetMemberConst (179): `mov [ecx],imm32; ret`. 160 store vendor vtables with no RTTI (class unknown), 4 have
  RTTI. **0x009F1720 stores literal 0 in retail** (exe bytes c7 01 00 00 00 00 c3) while the landed row binds a vtable reloc
  `??_7T@@6B@` -> a reloc-masked false match; it needs a non-polymorphic model (bytes of the object change: owner/coordinator).
- `?VecDel@T@@` / `?VecDel@Slot@@` ScalarDeletingDtor (105): not in any retail vtable; the class is only inferable from the dtor
  callee (CCharString / CTexture::Uninitialise ...), which names a member, not the owner.
- `?CThingSwitch_CompositeScalarDeletingDestructor@@` (138 addresses, one stand-in spelling -- a consistency defect): not in
  any vtable, so only the catalog class is evidence. Proposal: `<CatalogClass>_scalar_deleting_destructor` per address once a
  second source exists (PDB layout / caller).
- `?GlobalMethodTailThunk_<addr>@@` (288): atexit destructors of unnamed CCharString globals (AtexitDtor_CCharString_at_<global>);
  the accurate name needs the global's name.
- `?Get@T@@` / `?GetAddr@T@@` / `?Hook@T@@` / `?Run@T@@` with real catalog names but no RTTI slot (~1,000): catalog is the only
  source for tiny bodies (0x00401377 shows why: catalog/title said CEngineInternalPrimitiveBase::AddChildPrimitive, but the CRT
  caller in crtexew.obj binds it as `__setargv` -- dllargv.c `_setargv` returning 0. Two sources: library caller + body.
  NAME_FIX/label for the coordinator: 0x00401377 = `_setargv`, toolchain_library candidate).
- w2 DEFERRED: 85 one-source (catalog class only, untyped this), 90 this-parameter typed as another class and 53 names carrying
  another class (these are "names another function": decide which side is wrong), 35 non-unique new names (template
  instantiations such as 22 LTextBinTree<>::LTextTreeWalkThrough::BuildTreeArray, 12 std::_Tree<>::_Lbound).
- w1 T_PROPOSALS.tsv (16): typed callers disagree on T (CFont vs CFontBank ...) or T is nested (CDrawQuestInfo::CQuestInfoElement*)
  or const.

## Pre-existing defects found (not caused by this pass)
- 5 landed rows grade DIFFER(40v39) PASS at HEAD: 008d04b0, 0093a890, 009445c0, 00944ce0, 0095ee40 (w3 DEFERRED).
- 005dacf0 grades NOT_GENUINE at HEAD.
- engine/CCountedPointer.h line 29 attributes 0x00CE1040 to CCountedPointer<CScriptThing>; 36 typed callers + FableWin say
  CEntityScriptBase (w6). Comment fix only.
