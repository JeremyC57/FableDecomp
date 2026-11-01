# Chunk 671 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 2026-11-01 (chunk 671) A PDB accessor decides a member-of-member test: `GFGetSystemManager()->PDisplayManager.Get()` at the call
  site leaves a dead `add eax,60h` before `test ecx,ecx`; the PDB inline ?GetDisplayManager@CSystemManager@@QAEPAVCDisplayManager@@XZ
  (`return PDisplayManager.Get();`) gives retail's `mov ecx,[eax+60h] ; test ecx,ecx` (0x00AB7A70).
- 2026-11-01 (chunk 671) In-place destruction of pool-allocated arrays is the PDB template `GFExplicitDelete<T>(T*, unsigned long)`
  (__fastcall): defined in the TU, VC7.1 inlines it on its by-value copies (count loaded first, pointer walk with a count-down
  register, unsigned `jbe` guard, `push 0 ; call [vt]`); loops written on the members re-read them (221v175) (0x00A895F0).
- 2026-11-01 (chunk 671) `checksum += a + b + c;` with an unsigned long checksum and float members compiles to `fild checksum ;
  (2^32 fix-up) ; fadd c ; fadd b ; fadd a ; call __ftol2` -- VC7.1 (no /Op) folds the float sum onto the converted integer
  last-first; matched first probe (CInputEvent::GetChecksum 0x00A04160).
- 2026-11-01 (chunk 671) A dtor that walks another object's container through PDB accessors re-evaluated around a delete
  (`if (W->GetMap(i)) { delete W->GetMap(i)->P; W->GetMap(i)->P = 0; }`) gives the global reload and the zero store after the
  null-tested delete; the same code inlined from an out-of-line sibling (DeleteMap) zeroes inside the test, so it is not that
  sibling (0x00B59BE0). `long GetMapCount() const` makes the loop compare signed (`jl`) over `sar`.
- 2026-11-01 (chunk 671) One 27-byte "CTCInventoryItem::OnDie" label covered three classes' implicit dtors (`if (p) free(p) ; jmp
  base`): CTCHitOnCollision (vector), CNavSwitchableLeafNode (CVectorMap), C3DAnimFileBoneMaskChunk (CArray<bool>). Each named by
  its own slot-0 ??_G; each emitted by defining the PDB ctor, first probe (0x007C8560, 0x00A84F00, 0x00A9A7C0).
- 2026-11-01 (chunk 671) Retail CArray<bool> is a plain byte-per-element 12-byte vector (ctor zeroes three words, WriteToFile 0x00AB06B0
  reads `byte [start+i]`), not an STLport bit vector: model CArray<T> : std::vector<T> with the generic template (0x00A9A7C0).
- 2026-11-01 (chunk 671) Quest-script identity again settled by ctor -> Alloc -> registration string ("V_SwordInTheStone" ->
  NScript::CV_SwordInTheStoneScript); the ctor-stored data/interface words are CLevelScriptBase<CQ_SunnyvaleMasterData> (0x00ED7640).
- 2026-11-01 (chunk 671) Destructor labels in this band: 5 of 5 right this chunk; the five non-dtor labels (OnDie x3,
  AddLocalDetailDynamic, _Dest_val) were all wrong.
- Lane note: $S/tools/rstr.py hardcodes another session's scratchpad path (fails here); a local reader was used.

Review at landing:
- NAME_FIXES.tsv (24 rows): the five wrong-label own rows; all nine slot-0 ??_G rows (landed as free __fastcall
  `*_scalar_deleting_destructor` / `ScalarDeletingDestructor` / CLandscapeBackgroundPatch stand-ins) and their `*_dtor` / `Dtor()`
  callee spellings, which name this chunk's rows; the CV_SwordInTheStoneScript ctor 0x00ED4AA0 (landed in the global namespace with
  (void*, CGameScriptInterface*) parameters) and its Alloc 0x00ED75F0; callees 0x00BDED60 (~CLocalDetailCacheMap, catalog
  ~CPatchTesselationEdgeStrip), 0x0099A330 (CInitBaseClass::IsValid; vland WARN RELOC_NAME), 0x00A62ED0 (FreeResource; the landed
  ~C3DMeshStats 0x00A2F190 calls it Free_00a62ed0 on CHandleBasedMemoryPool), 0x00AA75C0, 0x00A76A80/0x00A80340, 0x0099A2E0.
- 0x00A8D700: the PDB sets list ??_EC3DMesh2@@UAEPAXI@Z, not ??_G; retail's body is the scalar form. Confirm which spelling the
  catalog keeps.
- vland WARN RELOC_NAME on 0x0099A300 (??1CBaseClassNonCopyable@@MAE@XZ labelled CBaseClass::~CBaseClass) is the catalog fix already
  raised by chunks 662-669.
- TU emitters (empty-bodied PDB ctors, not retail bodies): CTCHitOnCollision, CNavSwitchableLeafNode, C3DAnimFileBoneMaskChunk,
  CV_SwordInTheStoneScript. Only the dtor is graded.
- 0x00A895F0's TU emits the two GFExplicitDelete COMDATs under the PDB spellings (no retail out-of-line copy was searched for).
- Header debt: see NOTES.md (17 flattened headers; CLevelScriptBase implicit dtor).
- Every row matched within 5 probes; none needed more than 4 grades.
