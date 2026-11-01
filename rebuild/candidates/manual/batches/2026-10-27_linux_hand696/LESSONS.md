# Chunk 696 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 696) Lionhead CArray<T>::GetAsCArray() is `size() > 0 ? &front() : GetZeroSizeArray()` (the private
  GetZeroSizeArray returns the array object itself): an `empty()` test compiles to `cmp`, retail's `sub` comes from the
  size test (CMemoryDataOutputStream::PeekData 0x00995E70, MATCH).
- (chunk 696) One propagated "_Dest_val<allocator<CMacro>,CMacro>" label covered four implicit destructors of four classes
  (CSpawnedFuncBase 0x00CDD4C0, CModeJackOfBladesScream 0x00740540, CModeBriarRoseTeleportInvulnerable 0x008C36D0, CTB_Bard
  0x00ED7DD0): the three-byte-earlier slot-0 ??_G gives the vtable, RTTI or the ctor that stores it gives the class, FableWin
  lists the implicit ??1 even where Ego_r has only the ??_G.
- (chunk 696) The same implicit member destructor inlines at one inline level and not at two: ~CAnimFilterOfGroup expands
  inside the out-of-line implicit ??1 of a creature mode (0x00740540 / 0x008C36D0) but stays a call (0x00739DB0) inside a
  ??_G that already inlined the derived implicit dtor (0x008C37C0). Model it implicit with an inline
  `virtual ~CAnimFilterBase() {}` and let VC decide.
- (chunk 696) A switch whose explicit cases return the default's value keeps those cases as their own index-table entry (code
  merged with the default): GetCategory 0x005EDE37's index table needs the explicit `case 3: case 9: ... return 0;` group; the
  relocation-masked grader cannot see this, check the index bytes in the object.
- (chunk 696) engine/CCountedPointer.h's inline dtor is expanded inside the implicit ~CSpawnedFuncBase, where retail calls
  the out-of-line ??1?$CCountedPointer@$$CBVCPredicateBase@NScript@@@@QAE@XZ 0x00CDD4F0: keep a declaration-only local
  CCountedPointer there (76v35 with the header).
- (chunk 696) A byte twin's catalog label can sit on the wrong copy: 0x004AA840 (~CScriptThing, ~CBaseClass tail) carried
  "~C3DClothPrimitive" from 0x00AC3B80 (C3DClothPrimitive vtable, ~CBaseClassNonCopyable tail).

Review at landing:
- 0x00ED7DB0 is landed as CLandscapeBackgroundPatch::`vector_deleting_destructor' with an invented free `__fastcall`
  callee for 0x00ED7DD0: respell to ??_GCTB_Bard@CV_TalentlessBardScript@NScript@@UAEPAXI@Z calling ??1CTB_Bard (NAME_FIXES).
- 0x004AA840's ten bound spellings (nine invented + the non-virtual QAE form) and 0x00686830's twelve: respell the landed
  callers to ??1CScriptThing@@UAE@XZ / ??1CTCBase@@UAE@XZ in the same landing (NAME_FIXES.tsv).
- 14 landed sources declare `virtual ~CSpawnedFuncBaseRuntime()` for 0x00CDD4C0 (a local model class name; virtual
  declarations, so no binding); a naming wave can move them to CSpawnedFuncBase.
- New binding 0x00CDD4F0 ??1?$CCountedPointer@$$CBVCPredicateBase@NScript@@@@QAE@XZ (vland RELOC_NAME warn on the catalog label).
- Four sources (the implicit dtors 0x00740540, 0x008C36D0, 0x00CDD4C0, 0x00ED7DD0) carry an ungraded `DestroyOne` emission helper
  (`p->T::~T()`), the accepted chunk 656/657 pattern for out-of-line implicit ??1 rows.
- 0x005EDE37 is a jump-table row: its tables were hand-checked and vland's RELOC_JUMPTABLE check passed (re-run at landing).
- No row needed more than 3 probes; no flags, one `#pragma optimize("s", on)` (0x005EDE37, the packed /Os TU).
