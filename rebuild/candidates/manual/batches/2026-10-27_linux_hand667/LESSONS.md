# Chunk 667 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 2026-11-01 (chunk 667): a virtual stub whose whole body is `throw CBBBFileException(CCharString("") + "text", type)` (CCachedFile::Write
  0x00A5AFB0 / SetLength 0x00A5B190) is an int3-after-noreturn row (97v96: retail + the 0xCC after the final `call _CxxThrowException`);
  the PDB ctor ??0CBBBFileException@@QAE@VCCharString@@W4EFileExceptionType@0@@Z takes the message BY VALUE, which is the copy-ctor /
  operator const char* / dtor triple on the dead empty-string slot.
- 2026-11-01 (chunk 667): x87 frame-word assignment follows the named float locals: `float startAlpha = 255 - n*51; float alpha =
  startAlpha - frames/total*51;` gives retail's {frames temp, total, alpha} = {+0, +4, +8}; one `alpha` local with `-=`, a block-scope
  fraction, a float total or a named creature all permute the three words or add a spill (0x008C3F20, 13 probes).
- 2026-11-01 (chunk 667): a jump-table switch with NO range check (`mov eax,[esp+4] ; jmp [eax*4+table]`) is `default: __assume(0)`;
  `mov eax,K ; push eax` in every arm is one local assigned per case plus a single call VC7.1 tail-duplicates; write the cases in
  the byte order of the arms (0x00A75B70: 3,2,1,0).
- 2026-11-01 (chunk 667): four more compiler-generated destructors emitted exactly by defining the class's PDB constructor in the TU
  (0x00B3AB10, 0x00A6B5B0, 0x009AC7C0, 0x00B0A060), first or second probe; read the RTTI base-class array (contained-bases counts)
  for the real chain: CPSCSplineBase : CPSCBase : CParticleUpdateParamCallback<unsigned long, unsigned long> : CBaseClassNonCopyable
  (all mdisp 0, one chain), which is why ~CPSCSplineBase tail-jumps to ~CBaseClassNonCopyable with no intermediate store.
- 2026-11-01 (chunk 667): boost::scoped_ptr<IDirect3DVolumeTexture9> in a dtor is a bare `push [x] ; call operator delete` (COM
  interface: no virtual dtor, no null test) -- checked_delete over a minimal complete interface struct reproduces it (0x00A6B5B0).
- 2026-11-01 (chunk 667): a vtable shared by a dozen classes is the base's own slot when the base's RTTI vtable carries it (0x0091D8C0 is
  CAIStateGroup_EnemyCreatureBase's slot 2 in 0x0128B8BC, not HoverMovementBase's); a pure slot's meaning comes from the leaves' stub
  bodies (true/false per leaf = IsCombatStateGroup).

Review at landing:
- NAME_FIXES.tsv (16 rows, incl. 0x006925C0 and 0x0099A300 catalog labels): own-row respells for the three wrongly labelled destructors and the landed ??_G 0x00A6B590's
  `@CManagedTextureData_dtor@4` stand-in callee; caller respells for 0x009AA780 (CUnproven -> the proven map<CCharString,
  CContainedBank> _M_erase; every caller is in the CRetailBank / CBankFileManager region), 0x008F7860, 0x008F7950, 0x006EDC50
  (six wrong spellings), 0x008FCFC0, 0x006925B0 (GetTotalTime -> GetTotalTimeWorldFrames), 0x00692910, 0x00692C10, 0x004BFAC0,
  0x009FC550.
- 0x0091D8C0: retail's dead-target path tail-calls IsCombatStateGroup() and drops the result; the source writes that bare statement
  (`else IsCombatStateGroup();`). The bytes prove the call; the original reason (a release-mode check macro that still evaluates
  its argument?) is not visible. Owner may want to note it.
- 0x00A5AFB0 / 0x00A5B190: int3-after-noreturn: land against the N+1 retail span (differ/ sources + .test.cpp; tests use try/catch).
- Same-TU constructor definitions that exist only to emit the implicit ??1 (0x00B3AB10 ??0CEngineInternalAttachedPrimitiveInfo,
  0x00A6B5B0 ??0CManagedTextureData, 0x009AC7C0 ??0CRetailBank, 0x00B0A060 ??0CPSCSplineBase): PDB-named and (for 0x00A6B5B0 /
  0x009AC7C0) real out-of-line retail functions, but their bodies are not reconstructed (only the dtor is graded).
- `__forceinline` on CEnginePrimitiveHandle::Clear / ~CEnginePrimitiveHandle and CIVCountedPointer::Release / Reset / ~ (0x00B3AB10),
  copied from the landed 0x007CA8E0 model (owner 2026-11-01: allowed for Lionhead counted-pointer helpers).
- Landed rows with poor models met on the way: 0x00A6B590 (??_G as a free __fastcall with an invented dtor callee), 0x00A69980
  (STextureDims_00a69980 / STextureFlags_00a69980 stand-ins for CManagedTextureData's real members), 0x008FE0F2 / 0x00900C4D etc.
  (free __fastcall functions for virtual members), 0x009A9D60 bound as ??1?$CCountedPointer@UCRetailBank@@@@QAE@XZ (FableWin
  spells the class VCRetailBank@NBankFileManager; not re-verified here).
- Every row needed at most 2 probes except 0x008C3F20 (13); none was parked on shape.
