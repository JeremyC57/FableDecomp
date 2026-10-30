# Chunk 595 (2026-10-30): 7 of 10 engine-range released pass 9 rows landed

7 RELOCATION_MATCH, all PASS (g++ and VC7.1). Proven from public source plus callers / vtables / class strings: DirectShow
BaseClasses (strmbase, ctlutil.cpp) `CDeferredCommand::Invoke` 0x00CAED50 (inlines CBaseDispatch::GetTypeInfo and reads its
function-local statics directly, so the landed 0x00CACD90 body rides in the TU); DX SDK ImeUi.cpp static
`GetReadingWindowOrientation` 0x00CB3420 (/GS; `static __declspec(noinline) __stdcall` with a same-TU caller); STLport 4.6
`std::vector<CTVertexSeaBackground>::_M_insert_overflow(__false_type)` 0x00B80400 (the uninitialized_copy/fill helpers
`__forceinline`, `_M_clear` out of line); libpk `PKCTRRedblackTree::DeleteFixup` 0x00C18C00 and `PKCTRRedblackMap::DeleteFixup`
0x00C1B560 (class strings from the ".\PKCTRRedblackTree.cpp" assertion text; twins that differ only in whether the rotation
bodies are in the TU). Descriptive (libpk / GameCODA have no PDB): `CSoundSystem_00c131c0::PlaySoundList` 0x00C131C0 (/EHsc,
scoped PKCProtectionInstance lock) and `CAudioContext::InitialiseListener` 0x00C568F0 (OpenAL/EAX). All 7 manifest labels
were wrong (Vector_PushBack_CActiveFile_3Floats, CEnginePrimitive_UpdateParticleTrail, COM_InitializeTypeLibrary...).
Parked (../../pending/2026-10-27_linux_hand595/differ/): 0x00C5F6A0 CImaAdpcmCodec::DecodeM16 327v327 (block-head scheduling
only: cBlocks temp edi vs ebx, lea placement; used its 4 grades); 0x00C20580 CSoundPlayParameterOverride::ApplyTo 332v330
(register allocation in the fade-in / group-mask merge; the merge needs an inline helper with `unsigned short` parameters);
0x00C5E4E0 CUnprovenKsDevice_012B9148::OpenByDirectSoundGuid 336v332 (esi/edi swap + `setne` bool tail).
NAME_FIXES.tsv: 13 rows (CDeferredCommand::Cancel 0x00CAEC80 slot 3 of vtable 0x012BF8B8, the vector `_M_insert_overflow` /
`_M_clear` members 0x00B80400 / 0x00B80550 bound as CArray members, the four PKCTRRedblack* rotation spellings,
CSoundPlayParameters::SetLoopStart/End take float (M not J), the EAX listener GUID 0x012BEE90 placeholders, three libpk audio
rows 0x00C29F70 / 0x00C29EC0 / 0x00C12CD0 carrying propagated combat/hero/trail labels) + 2 coordinator review rows
(0x012BEE90 C++ vs extern "C" DEFINE_GUID linkage, 0x0143E8B8 `_g_hklCurrent` vs `?g_hklCurrent@@3PAUHKL__@@A`). The
GetReadingString stand-in in 0x00CB3420's TU is not bound to retail 0x00CB4340 (checked at landing: only `__GetReadingString`
0x0143E684 is bound).
Header gaps: strmbase CDeferredCommand / CBaseDispatch, libpk PKCTRRedblackTree / PKCTRRedblackMap / CSoundSystem /
CAudioContext (no PDB); the SHARED_HEADER_AVAILABLE CBaseObject warning on 0x00CAED50 is the strmbase false positive.
Lessons (LESSONS.md): a file-static callee-pop helper reaches retail register allocation only as a TU-local `static
__declspec(noinline) __stdcall` with a same-TU caller; STLport uninitialized helpers `__forceinline`; DeleteFixup twins pick
ecx or edi for `this` by whether the rotation bodies are compiled in the TU; `p->Alloc()->Init(a, b++, c)` needs a named local
to put Alloc first; in an EH frame two `return`s store state -1 before the branch, a ternary after it.
