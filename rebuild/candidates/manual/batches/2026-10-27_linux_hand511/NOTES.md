# Chunk 511 notes

15 RELOCATION_MATCH (0 MATCH, 0 retail_asm), 5 DIFFER parked, 0 skipped. All 15 tests PASS under g++ (tall.sh) and VC7.1 (tall.py). The vland dry run gave WINS: 15.

## Proven
- 0x00A4B670 NBBBMaths::IsPosInFOVWithWeight (`?IsPosInFOVWithWeight@NBBBMaths@@YI_NABVC3DVector@@0M0AAM@Z`, Ego_r + FableWin)
- 0x00A55F90 CMatrix3x4::SetFromVectorAndAngle (`?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z`; no such ctor exists in either PDB)
- 0x00A5C360 NVectorMath::GFNormalise3DVectors (`?GFNormalise3DVectors@NVectorMath@@YAXPBVC4DVector@@KPAV2@KK@Z`)
- 0x00A60200 GFRemoveCommentsLeavingNewlines (`?GFRemoveCommentsLeavingNewlines@@YIXPADPBD1@Z`)
- 0x00A68910 CVolumeTexture::GetAsTextureList (`?GetAsTextureList@CVolumeTexture@@QAEXPAVCTexture@@@Z`)

## Probable
- 0x009C75A0 std::vector<CFileInstaller::CBufferChunk>::_M_fill_insert (element type from the resize 0x9C7850)
- 0x009CAF60 std::vector<NGameText::CTextTag>::_M_fill_insert (element type from the resize 0x9CB7D0)
- 0x009E7D30 std::__introsort_loop<pair<CCharString,CSymbolMap::CSymbolInfo>*, ..., CCompareString<CSymbolInfo>> (from the landed same-TU helpers)
- 0x009F6AC0 std::vector<CInputEvent>::_M_insert_overflow (from the existing binding; protected IAE)
- 0x00A4A710 std::vector<CLipSyncPhoneme>::_M_insert_overflow (from the binding)
- 0x00A55070 std::vector<C3DAnimationSequenceData::C3DVectorWord>::_M_insert_overflow (from the binding chain)
- 0x00A55190 std::vector<CQuaternion>::_M_fill_insert (from the landed overflow 0xA54FA0)
- 0x00A552B0 std::vector<C3DAnimationSequenceData::C3DVectorWord>::_M_fill_insert
- 0x00A618D0 CBusyResourceManager::OnPostSwapScreens (Ego_r layout: FreeQueoryObjects +0, PendingQueories +4, PendingFrame +8, FinishedFrame +0xC, D3DDevice +0x10; the neighbours are its ctor, ClearAllQueries and dtor)
- 0x00A6BCD0 std::vector<CIVCountedPointer<CManagedTextureData>>::_M_fill_insert
- 0x00A70CB0 std::vector<C3DVector>::_M_range_insert<const C3DVector*> (via CPhysicsMesh::TotalPoints +0x3C)
- 0x00A71550 std::vector<C3DTriangle2>::_M_insert_overflow (from the binding)
- 0x00A71FC0 std::vector<pair<CCharString,C3DHelperPoint>>::_M_insert_overflow (from the CVectorMap Add 0xA72B90)
- 0x00A720E0 std::vector<pair<CCharString,C3DDummyObject>>::_M_insert_overflow
- 0x00A72CC0 std::vector<pair<long,C3DMeshVolume>>::_M_insert_overflow (from the CVectorMap Add 0xA737B0)

## Descriptive
- 0x009FA450 area: the IDirect3D dummy slots and the ESurfacePool enum value names in the 0x00A68910 headers are descriptive stand-ins.

## Matched
See STATUS.tsv. Grades: 0x00A55070 2, 0x00A71FC0 2, 0x00A68910 4 (grade-3 version in probe/00a68910_g3.cpp), the rest 1.

## Parked
- 0x00A4B670 NBBBMaths::IsPosInFOVWithWeight DIFFER(312v314): retail spills the Newton rsqrt result r to memory and re-reads it per component; ours keeps r on the x87 stack. A retail_asm candidate (inline GFFastCos FloatToLong fistp). | tried: Z,X,Y length order (fixed one hunk), a float& alias, operator*=(const float&) | next: a volatile-free forced spill through an out-of-line-looking temporary (C3DVector temp returned by value), or land as retail_asm citing the fistp.
- 0x00A55F90 CMatrix3x4::SetFromVectorAndAngle DIFFER(259v259): the xy/xz products load operands in the order fld [Y]; fmul [X], and the Position zero-store scheduling differs. | tried: both source operand orders (VC canonicalizes them), axis component locals (worse) | next: express the products through C3DVector member helpers or the inline CMatrix3x4 setters from the header, and try the Position store before the rotation block.
- 0x00A5C360 NVectorMath::GFNormalise3DVectors DIFFER(386v421): retail's aligned-frame prologue copies the return address (sub esp,8 ... mov ebp,[ebx+4]; mov [esp+4],ebp); frame slots sit 0x10 lower; the x87 fallback spills differently and retail saves edi. The SSE asm block itself matches. | tried: optimize("y",off), /Oy- | next: a lane gap. Needs the retail flag set for an aligned frame (an __declspec(align(16)) local) under the QFE toolset; land as retail_asm once the prologue reproduces.
- 0x00A618D0 CBusyResourceManager::OnPostSwapScreens DIFFER(421v421): register allocation only (edi/ebx swapped in the move block around the dead size() walk). 1 grade used. | tried: the direct transcription | next: reorder the list/deque locals, or hoist the size() call into a named local.
- 0x00A6BCD0 std::vector<CIVCountedPointer<CManagedTextureData>>::_M_fill_insert DIFFER(285v276): the inline ~CIVCountedPointer tail reloads x_copy after dec [eax+4]; retail does dec [ecx+4]; jne. | tried: a local copy, an inline pointee ReleaseReference, a 5-variant probe (A-E) | next: check the landed CIVCountedPointer instantiations for the exact dtor body (a member ReleaseReference that tests the decremented value directly).

## Skipped
None.

## Label corrections
- 0x00A51C60 "UninitCopy_C3DTriangle2" is the C3DVectorWord __uninitialized_copy.
- 0x00A6ED80 "UninitCopy_SNamedRecord14" is __uninitialized_copy<pair<CCharString,C3DHelperPoint>>.
- 0x00A6FE70 "UninitFillN_SPairLongNamedCountedPointer" is __uninitialized_fill_n<pair<long,C3DMeshVolume>>.
- 0x00A67440 / 0x00A67450 "Nop_ret4" are CVolume::Swizzle / CVolume::Unswizzle, in Ego_r donor order (Swizzle first).
- 0x00A55070 "Vector_InsertN_uint48" is vector<C3DVectorWord>::_M_insert_overflow.
- 0x00A13C60 binds 0x00A55F90 as the non-existent ctor ??0CMatrix3x4@@QAE@ABVC3DVector@@M@Z; the real name is SetFromVectorAndAngle.
- 0x00A737B0 "C3DAnimFile::AddKeyedChunk" is the CVectorMap<long,C3DMeshVolume> Add; 0x00A72B90 is the CVectorMap<CCharString,C3DHelperPoint> Add.
- The binding stand-ins SSkeletalMorphEntry, SNamedChunk and SElement12 are wrong. So are the one-parameter vector<T> spellings bound from the resizes 0x9C7850, 0x9CB7D0 and 0xA55670; the real symbol carries the allocator.
- 0x00A61880 / 0x00A61A80 / 0x00A61B10 (the last catalogued as CDirectInputDeviceLists_00a61b10) are CBusyResourceManager ctor / ClearAllQueries / dtor.
- The 0x009FA450 binding ?Lock@CTexture@@QAE?AUCLockedRect@@JK@Z should be the PDB spelling ?Lock@CTexture@@QAE?AVCSurfaceLockedRect@@KK@Z.
- 0x00A67460 has two ReleaseVolume spellings in the bindings; the PDB has a protected no-argument form, but retail takes a bool. This is unresolved, and the bool form is used here.
- All 20 oracle names are descriptive catalogue labels (vector_resize_8byte, Matrix_RotationAroundAxis, Deque_Process_And_Rebalance and so on); replace them with the names above.
- vland RELOC_DUPLICATE: 0x0066DDF0 serves both the PAV and PBV __uninitialized_copy<C3DVector> (an identical-COMDAT fold). RELOC_CONFLICT: __copy<const C3DVector*> is 0x0074CA20 here but 0x0075B560 in the landed 0x0075B8D0. These are byte-identical folded copies, so check at landing.

## Header debt
- 0x00A68910 uses local declarations for CPixelFormat and CTexture; vland reports SHARED_HEADER_AVAILABLE for both. Switch to engine/CPixelFormat.h and engine/CTexture.h at landing.
- C3DVector, C3DHelperPoint and C3DMeshVolume need the FableWin user memberwise copy ctors declared in their engine headers. Without them the pair copies differ.
- CVolume and CVolumeTexture need inline GetDimensions returning C3DExtentsI.

## Callee names
- 0x9C6510 __uninitialized_copy<CBufferChunk>, 0x9C6540 __copy_backward_ptrs, 0x9C6570 uninitialized_fill_n, 0x9C5EC0 fill, 0x9C73E0 vector<CBufferChunk>::_M_insert_overflow, 0x9C6020 ~CBufferChunk
- 0x9C9DD0 __copy_backward<CTextTag>, 0x99EC30 CCharString copy ctor, 0x99EAE0 ~CCharString, 0x99EFB0 CCharString::operator=
- 0x9E6530 CCompareString FastCompare, 0x9E6C80 __unguarded_partition, 0x9E7B40 __partial_sort
- 0x9F64A0 __uninitialized_copy<CInputEvent>
- 0xA51C60 __uninitialized_copy<C3DVectorWord>; 0xA54FA0 vector<CQuaternion>::_M_insert_overflow
- 0x66DDF0 __uninitialized_copy<C3DVector>, 0x75B5B0, 0x74CA20 __copy<const C3DVector*> (COMDATs from other TUs)
- 0xA6E350 __uninitialized_copy<C3DTriangle2>
- 0xA6ED80 / 0xA6EDE0 __uninitialized_copy / __uninitialized_fill_n<pair<CCharString,C3DHelperPoint>>
- 0xA6EE30 / 0xA6EE90 the same pair for pair<CCharString,C3DDummyObject>
- 0xA6FE00 / 0xA6FE70 the same pair for pair<long,C3DMeshVolume>; 0xA70E80 __destroy_aux
- 0x9E3830 CPixelFormat::Initialise, 0x9E3800 CPixelFormat::GetByteColourDepth, 0xA685E0 CVolumeTexture::Lock, 0xA67EA0 CVolumeTexture::GetVolumeLevel, 0xA67450 CVolume::Unswizzle, 0xA67440 CVolume::Swizzle, 0xA67460 CVolume::ReleaseVolume(bool), 0x99A2E0 ~CBaseClass, 0x9FA280 CTexture::Initialise, 0x9FA450 CTexture::Lock, 0x9FA180 CTexture::Unswizzle, 0x9FA140 CTexture::Swizzle, 0x9F9DE0 CTexture::Unlock
