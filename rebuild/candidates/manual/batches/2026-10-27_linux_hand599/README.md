# Chunk 599 (2026-10-30): 10 of 10 engine-range released pass 9 rows landed

10 RELOCATION_MATCH, all PASS (g++ and VC7.1), all plain C++. Proven (PDB or public source + a second source): libvorbis
1.0.1 psy.c `seed_curve` 0x00C92CA0 (`static __declspec(noinline) __stdcall` inside the landed `_vp_tonemask` TU; VC
`<stdlib.h>` min/max and `.1f`, not os.h's) and floor1.c `render_line` 0x00CA1E80 (/Gr; same-TU static with its landed
caller floor1_inverse2: the hand448 "LTCG lane gap" label was wrong); STLport
`std::__merge_adaptive<CSortElement*,int,CSortElement*,less<CSortElement>>` 0x00B862C0 (`__forceinline` copy chain);
`CEnginePrimitiveManagerRepeatedStaticMeshes::CalcFrameConstantSIMDVars` 0x00B99860 (a user copy ctor on C3DVector stops
the full unroll) and `CParticleSimulation::ReadVectorArray` 0x00AE1730 (a `C4DVector::Initialise` call in the loop body gives
retail's 4x unroll) from the PDB; `CAudioSource::Stop` 0x00C72620 (slot 2 of the voice base vtable, already bound as
`?Stop@CVtable_012B5C10@@UAEXXZ` by two landed callers: a virtual override takes its slot's bound name; reference local to
`g_AudioContext`). Class proven, method descriptive (libpk, no PDB): `CImaAdpcmCodec::CalculateError` 0x00C5F150
(`base + i*cbBlock` gives the single induction pointer), `CReverbDelay::Blend` 0x00C76C20 (class from the landed Process twin
0x00C76B60), `PKCTRArray::RemoveValue` 0x00C322E0 (/EHsc; vtable slot 6, PKCTRArray.cpp traces); descriptive:
`PKSerialiseLoad<CBankData>` 0x00C17940 (/EHsc; same-TU GetChecked/Release bodies under `auto_inline(off)` remove a frame).
All 10 manifest labels were wrong.
Bonus: seed_loop 0x00C92E20 (manifest row, chunk 447 park) also grades RELOCATION_MATCH from `src/00c92ca0.cpp` with leaf
`_seed_loop@24`; it needs its own behaviour test before landing -- handed to the parked-row sweep section that owns that
address range. The landed `_vp_tonemask` TU's own copy of seed_curve uses os.h min/max and `.1` (not retail-exact; its leaf
is unaffected).
NAME_FIXES.tsv: 10 rows (CBankData::Transfer returns bool 0x00C2EE80; PKCAutoDelete<CBankData> GetChecked / Release
0x00C17740 / 0x00C17850; the PKSerialiseCParser ctor 0x00C31970; 0x00C32160 CSortedPtrArray::FindIndex vs the bound
PKCTRArray::FindValueIndex; 0x00C59710 C_00c59710::M -> CVtable_012B5C10::Stop; 0x00B85510 non-template merge vs the bound
template; 0x00C76C20 StageTwo::Blend -> CReverbDelay::Blend; 0x00C76460 Backend::Write -> CDspOps::WriteRing; 0x0143CCDC five
spellings -> CDspObject::s_pOps) + 4 coordinator review rows (0x00C6A080 mis-filed under a quest-script name; vtable
0x012B4E80 modelled by three classes, g_pEngineCamera, the CDebugStream trace helpers; the CBankData class key; the
CImaAdpcmCodec +0x14 member).
Header gaps: no engine headers for CSIMDVariableBlock, CEngineCameraDesc, CPlane, C3DVector, C4DVector, CArray (modelled
locally from PDB members); engine/CEngineCamera.h keeps CameraDesc / FrustrumPlanes opaque; engine/CDataInputStream.h has
`void* CurrentSourcePtr`; engine/CEngineSubPrimitiveRenderer.h lacks the nested CSortElement (NOTES.md proposes the change;
cause of the SHARED_HEADER_AVAILABLE warning on 0x00B862C0).
Lessons (LESSONS.md): in libvorbis C TUs VC `<stdlib.h>` min/max match retail, os.h's do not; a memberwise copy ctor stops
VC7.1 fully unrolling a short vector-copy loop; an inline member call in a loop body triggers the 4x unroll; a reference
local to a global object flips the esi/edi assignment; out-of-line same-TU callee bodies under `auto_inline(off)` remove a
frame (an out-param homed in a dead argument slot); `base + i*stride` gives retail's single induction pointer; a virtual
override takes its slot's bound name.
