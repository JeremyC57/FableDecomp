# Chunk 585 (2026-10-30): 8 of 10 engine-range released pass 9 rows landed (the first engine chunk)

1 MATCH + 7 RELOCATION_MATCH, all PASS (g++ and VC7.1). Rows: std::vector<C3DGroup2>::operator=,
vector<pair<EPSCType,CComponentInstance>>::_M_insert_overflow, vector<CArray<C3DVector>>::_M_insert_overflow,
CMatrix3x4::SetFromVectorAndAngle, ov_time_seek / ov_time_seek_page (vorbisfile), mdct_butterfly_first (static, MATCH),
NParticleEngine::CPSCOrbit::COrbitState::GetSplinePosition.
Parked (../../pending/2026-10-27_linux_hand585/differ/): 0x00BEBAA0 CEngineInternalPrimitiveMeshBase::CalcObjectMatrix
(255v257: VC7.1 pops esi mid-function, retail keeps it to the epilogue) and 0x00C77C80 CReverbProcessor::
UpdateLineAndTapLengths (259v259, one esi/edx swap in the tap loop; descriptive name, libpk has no PDB; retail_asm row).
Coordinator fix before landing: the landed reserve 0x00AE80F0 bound the const `__uninitialized_copy<const C3DGroup2*>`
spelling to 0x00AE7480, which is the NON-const instance (reserve copies from _M_start; landed 0x00AE81F0 agrees); its
_M_allocate_and_copy now takes `_Tp*`, regrades RELOCATION_MATCH + PASS, and the binding is respelled (multi-check 0).
Header gaps (NOTES.md): no engine headers for C3DVector, CMatrix3x4, CRightHandedSet, CEngineInternalPrimitiveMeshBase,
CPSCOrbit/COrbitState, CComponentInstance, CCountedPointer, CArray (local retail-layout models); Ego_r sizes C3DGroup2 20 vs
retail 16 and Dinkumware CArray sizes shift CEngineInternalPrimitiveWeaponTrail offsets; the landed libpk SReverbTap model
is 8 bytes off (state +0x88). SetFromVectorAndAngle is QAI (retail __fastcall) where the PDB says QAE.
NAME_FIXES.tsv: 7 rows queued. Lesson that reopens parked rows: a TU-local `static __declspec(noinline)` with a same-TU
caller gets a private register convention at plain /O2 (0x00C9C1E0 MATCH without /GL) -- the libvorbis statics parked as
LTCG lane gaps (e.g. 0x00C8F200) are worth retrying.
