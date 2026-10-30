# Chunk 599 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 599) **A libvorbis C TU uses VC `<stdlib.h>`'s min/max, not os.h's.** `#define max(a,b) (((a) > (b)) ? (a) : (b))`
  matches; the os.h `((x)<(y)?(y):(x))` form flips the compare and jump. The psy.c level step is `.1f`: the pool constant is
  0.1f widened, 0.10000000149. Example: seed_curve 0x00C92CA0.
- (chunk 599) **A memberwise user copy ctor on a small vector class stops VC7.1 fully unrolling a short loop of vector
  copies.** Without it, the 4-iteration splat loop in CalcFrameConstantSIMDVars 0x00B99860 unrolls to 1444 bytes; with
  `C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){}` it stays a loop (386).
- (chunk 599) **An inline member call in a loop body makes VC7.1 unroll by 4 where plain field stores do not.** In
  ReadVectorArray 0x00AE1730, `vectors[i].Initialise(temp[i], w)` gives the 4x unroll, 391 bytes; plain field stores give
  234 bytes.
- (chunk 599) **A reference local to a global object changes register assignment.** With `CAudioContext& context =
  g_AudioContext;` the first field load goes straight into esi, matching retail. Direct `g_AudioContext.` calls swap esi and
  edi (384v385). Example: CAudioSource::Stop 0x00C72620.
- (chunk 599) **Out-of-line same-TU callee bodies under `#pragma auto_inline(off)` let VC7.1 see which locals they modify.**
  The caller can then home an out-param local in a dead argument slot (no `push ecx` frame) and keep a member in a register
  across the call. Examples:
  - PKCTRArray::RemoveValue 0x00C322E0, with FindValueIndex 0x00C32160 in the TU.
  - PKSerialiseLoad<CBankData> 0x00C17940, with PKCAutoDelete GetChecked and Release in the TU.
  Declaration-only callees cost 1-2 bytes.
- (chunk 599) **STLport `__merge_adaptive` needs the copy chain `__forceinline`** (copy, __copy_ptrs(__false_type) and
  __copy over CSortElement). Plain inline leaves out-of-line calls (338v389). Example: 0x00B862C0.
- (chunk 599) **For pointers that advance by a stride, write `base + i * stride`.** VC7.1 then keeps one induction pointer
  plus the constant (other - base) delta, as retail does. Two `p += stride` pointers cost a register and a bigger frame.
  Example: CImaAdpcmCodec::CalculateError 0x00C5F150, 0x2C frame.
- (chunk 599) **A virtual override takes its slot's bound name.** Before naming an override, look up the base slot's
  binding: 0x00C72620 is slot 2 of the voice base, which is bound `?Stop@CVtable_012B5C10@@UAEXXZ`. That makes it
  CAudioSource::Stop, not a descriptive "Uninitialise".
- (chunk 599) **"Lane gap" labels retested**: render_line 0x00CA1E80 (hand448 "LTCG register argument") matched on the
  second grade as a same-TU noinline static with its landed caller in the file.

Review at landing:
- seed_loop 0x00C92E20, the chunk-447 parked row, grades RELOCATION_MATCH from src/00c92ca0.cpp (the seed_curve TU). Land it
  from this TU as an extra row (leaf `_seed_loop@24`).
- The landed `_vp_tonemask` TU (rebuild/src/compiled/00/c9/libvorbis_vptonemask_00c93e30.cpp) carries a seed_curve copy with
  os.h min/max and `.1`. That copy is not retail-exact. Its own leaf is unaffected, but align it with 0x00C92CA0 if the TUs
  are shared.
- 0x00C6A080 is landed as "NScriptCQGuildTrainingSkillScript_TransformGeometry", but it is the reverb block process. Its
  StageTwo::Blend and Backend::Write declarations and its Backend_0143ccdc global are wrong spellings; see NAME_FIXES.tsv.
- 0x0143CCDC has five bound spellings. CDspObject::s_pOps, from its ctor 0x00C74420, is the one the reverb rows use; retire
  the others.
- 0x01436EA0 also has many spellings. I used `g_pEngineCamera`, the dominant one; confirm it.
- Vtable 0x012B4E80 is modelled by three landed classes: PKCTRArray, CResourceArray_00c32bf0 (dtor 0x00C32BF0) and
  CActionParamString (ctor 0x00C32BA0). One retail vtable should have one class.
- CBankData's class key: the landed Process_00c172f0 says `class`, while most rows (and 0x00C17940) use `struct`. Pick one at
  landing.
- 0x00C59710 is landed as `C_00c59710::M`, leaf "Clear". Its bound name is `?Stop@CVtable_012B5C10@@UAEXXZ`.
- CImaAdpcmCodec +0x14 is the SDK's m_fEncoder in the landed rows, but 0x00C5FA50 uses it as a 0..3 mode switch. The landed
  converter typedef has six arguments; the EncodeM16 notes say five.
- The libpk trace stream 0x00C0DEC0 / 0x00C0DF00 / 0x00C0BA40 is spelled CDebugStream in 59 landed sources, but
  reloc_bindings.tsv holds 4+ other spellings per address (CString_00c62c70::Append, Log::Text, String::~String ...).
  Settle it in a naming wave.
- engine/CEngineSubPrimitiveRenderer.h lacks the nested CSortElement. 0x00B862C0 and the landed 0x00B85660 model it
  locally; see NOTES.md "Header debt".
- Grades: no row needed more than 4. 0x00AE1730 and 0x00C72620 used 4 (0x00C72620's fourth grade was the regrade after the
  rename to Stop).
- Local models next to existing headers: none. CEngineCamera, CEnginePrimitiveManagerRepeatedStaticMeshes and
  CDataInputStream come from engine/ headers. The CSIMDVariableBlock, CEngineCameraDesc, CPlane, C3DVector, C4DVector and
  CArray models are local because no header exists.
- Required reading was incomplete. GOTCHAS.md got lines 1-134 plus targeted greps (the file is too large for one read).
  FINDINGS_LOG.md got the newest entries plus greps. purity.py got its function list, and purity.classify was run on every
  source.
