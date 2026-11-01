# Chunk 664 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 2026-11-01 (chunk 664): a masked RELOCATION_MATCH can hide a wrong tail-jump target: with engine/CLevelScriptBase.h's
  declared `virtual ~CLevelScriptBase();` the implicit ~CQ_AwakeningTheOracleScript (0x00D005E0) jumps to ??1CLevelScriptBase<T>
  and still grades RELOCATION_MATCH; `objdump -dr` showed it. Retail's ~CLevelScriptBase<T> is compiler-generated (0x00CDD100..
  0x00CDD150 = `jmp ~CScriptBase`); a base without a dtor declaration makes the derived implicit dtor bind ~CScriptBase.
- 2026-11-01 (chunk 664): an implicit dtor that retail keeps out of line under its ??_G is emitted by defining the class's real
  ctor in the TU (vtable -> ??_G -> implicit ??1, called, not inlined), no harness needed (0x00D005E0).
- 2026-11-01 (chunk 664): `delete p` of a trivially destructible type emits no null test under VC7.1; retail's `test eax,eax;
  je` before operator delete is an explicit `if (p) delete p;` (CTCPhysicsRigidBody::PPhysicsMeshInfo, 0x0075A480).
- 2026-11-01 (chunk 664): a byte-array stand-in for a member class drops its alignment (CEntityRuntimePersistence at +0x9D
  instead of +0xA0, every later offset shifted); give the model one real typed member (its `long SaveFormatVersion`) (0x00523440).
- 2026-11-01 (chunk 664): STLport list::remove_if: retail calls the out-of-line remove_if COMDAT (0x00A155C0 = `jmp
  _S_remove_if` 0x00A16EB0); VC7.1 inlines the in-class body here and calls _S_remove_if (168v164) -- keep remove_if declared
  only (0x00A1CB40).
- 2026-11-01 (chunk 664): the real STLport _List_base text (_list.c clear() out of class) reproduces both retail per-site
  choices: clear() called from an inlined ~_List_base (0x0055B760 x4, 0x00442610), and inlined into an out-of-line ~_List_base
  (0x00A214C0, called by 0x00A1CB40); no declared-only stand-ins needed.
- 2026-11-01 (chunk 664): `if (x > -1)` gives `cmp [..],-1 ; jle`, `!= -1` gives `je` (0x00F37570, signed long index vs the
  unsigned size(): `jb`).
- 2026-11-01 (chunk 664): the 0x00CA4xxx-0x00CA7xxx block is DirectShow strmbase (CBaseRenderer / CBaseVideoRenderer, renbase.cpp),
  not libpk audio: CBaseVideoRenderer adds primary-vtable slots 45 RecordFrameLateness, 46 OnDirectRender (0x00CA5570),
  47 ResetStreamingTimes, 48 SendQuality; its own vtable 0x012BF5D8 has no COL.
- 2026-11-01 (chunk 664): an intermediate base with an implicit dtor shows no vtable store of its own: CTCPhysicsBase in
  ~CTCPhysicsRigidBody (its CDefPointer release expanded, then `jmp ~CTCBase`), CNavBlockedQuadTreeNode in ~CNavigatorAStar
  (only the CANavQuadTreeNode store before ~CBaseClassNonCopyable).

Review at landing:
- NAME_FIXES.tsv (21 rows): own-row respells of the eight slot-0 ??_G rows (all landed as CLandscapeBackgroundPatch /
  `ScalarDeletingDestructor` / `*_scalar_deleting_destructor` stand-ins) and their `Dtor` callee spellings of this chunk's
  rows; 0x009FC020 pointer -> reference parameter (landed 0x009FC5C0 caller); stand-in families on 0x00CBD510, 0x00686830,
  0x0099A300; STLport _M_erase spellings 0x00A21640 / 0x005278A0 / 0x005286D0 / 0x00CD344A / 0x009AA830; 0x00A155C0 (landed
  as an `extern "C" Forward()` forwarder), 0x00A214C0 (landed as SortTreeRecursively), 0x005356A0, 0x00440730.
- 0x00440730: landed as std::_List_base<CBaseIntelligentPointer>::clear (instantiation stated unknown); this chunk proves
  list<CMiniMapDisplay::CMarkerInfo> from the ~CMiniMapDisplay call site, but 0x0082A303 / 0x0082A313 call it from another TU:
  confirm that TU also uses list<CMiniMapDisplay::CMarkerInfo> before respelling.
- engine/CLevelScriptBase.h: drop the `virtual ~CLevelScriptBase();` declaration (owner decision; see NOTES.md Header debt).
- Local base models next to existing flattened headers (NOTES.md Header debt): every row except 0x00CA5570 / 0x00D005E0 (partial).
- Every row matched within 3 probes; none needed more than 4 grades.
