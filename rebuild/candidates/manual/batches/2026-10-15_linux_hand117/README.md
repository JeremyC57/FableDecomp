# Hand chunk 117 (C2DBoxF .. C3DMeshFilePrimitiveChunk): 31 of 40 byte-exact

All 31 matched rows grade MATCH / RELOCATION_MATCH in `grades.txt` with no flags or pragmas, and every behaviour
test prints `<addr>_TEST PASS` under g++ (`tall.sh`) and the lane's VC7.1 (`tall.py`).

## What matched (31)
- **C2DBoxF::ClipTo** 0066c560 -- clamps each corner coordinate into another box; an `if (<) .. else if (>)` per
  coordinate gives the `test ah,5 ; jp` / `test ah,41h ; jne` pair, float copies through eax.
- **C2DLineF::IsWithinDist** 00a57a40 -- |p - mid|^2 < |P1-P2|^2/4 + dist^2. Needs C2DVector value operators with a
  user constructor (keeps everything on the x87 stack in source order), the compare written `bound > rel^2`, and the
  squared length in a NAMED float before `* 0.25f` (else VC distributes the scale into both squares).
- **C2DVector::Normalise** 00a14540 -- the table-driven fast 1/sqrt + one Newton step; the leading `mov eax,ecx` means
  the function returns `*this` (C2DVector&).
- **C3DAnimFile** 00cbd670 / 00a737b0 / 00a73840 (catalog "C3DAnimFile::C3DAnimFile", not constructors) --
  AddNamedChunk / AddKeyedChunk / AddStringKeyedChunk: a record temporary ({name, counted chunk}, with a long or a
  CCharString key) is pushed into the vector at +0 through its out-of-line push_back, then destroyed (inline
  CCountedPointer release with the pointer-to-member destroy at +4/+8, `--count; if (count == 0)` re-read form),
  and the changed flag at +0xD is set.
- **C3DAnimFileSequenceChunk::WriteToFile** 00aaf650 -- the matrix count is a signed divide held in an unsigned local;
  the loop becomes a count-down with a running byte offset. Model reused from the landed chunk WriteToFile family.
- **C3DAnimFileTimingEventChunk::GetChunkInfo** 00aae190 -- `CCharString("Name: ", -1) + Name + ", Time: " +
  CCharString::FormatDouble(6, Time)`: chained __fastcall operator+ evaluated right to left.
- **C3DAnimFileTimingEventChunk::ReadFromFile** 00aae440 -- `Name = in.ReadNullTerminatedString(); Time = in.ReadFLOAT();`
  with the stream Read inlined (`__forceinline`, else it stays out of line) and `class` stream/loader types so the
  mangled leaf matches.
- **C3DAnimFileXSequenceChunk::ReadFromFile** 00aa48d0 -- resize(size, 0) then the inline Read straight into the array;
  branch polarity decides: accessor `if (size != 0) return begin;` and read `if (size <= remaining) copy else overflow`.
- **C3DAnimationBlendState::Validate** 00a3c0c0 -- base animation + every combination blend's virtual slot 5; the base
  animation must be a CIVCountedPointer with an inline `operator->` (raw pointer member: vtable lands in edx not eax).
- **C3DAnimationBlendState::SetBaseAnimation** 00a3c260 -- `Data->BaseAnimation = new C3DAnimationState(state)`: the
  implicit copy constructor copies the base's ref-count word between the two vtable stores; the release is an inline
  member of the pointee so `dec [ecx+4] ; jne` keeps the pointer in ecx.
- **C3DAnimationInfo::GetAnimationStats** 00a269c0 -- two inline `Data && member` predicates, synchronous load, the
  inline resource-list move-to-tail, counted-pointer copy into the hidden result.
- **CLipSyncDataBank::GetLipSyncData** 00a42fc0 (catalog label "C3DAnimationInfo::GetAnimationStats") -- same shape on
  the lip-sync bank; the re-indexed entry must be a reference local inside the `if`.
- **C3DAnimationInterpolator::operator=** 00bd85f0 -- PDB StartState/EndState are C3DAnimationBlendStates; their
  implicit assignment moves only the CIVCountedPointer<CSharedData> words (+8/+0x10).
- **C3DBoundingBox::InitialiseFromSphere** 00a31530 -- `MinCoord = centre - extent; MaxCoord = centre + extent;` with
  C3DVector value operators (frame temporaries copied word by word).
- **C3DClothPrimitive::SaveBinary** 00ac3b00 -- particle program serialised into a CMemoryDataOutputStream local, then
  size + bytes (the size argument evaluated before the data pointer).
- **C3DGroup2::C3DGroup2** 00ac4150 -- push_back loop over the chunk's blends; the array member must be the real STLport
  structure (`_Vector_base(const allocator&)` + `_STLP_alloc_proxy`) to get retail's `xor eax,eax` / `mov ecx,edi` order.
- **C3DMesh2::C3DMesh2** 00a89190 -- init list for the handles/zeros, the two flags as body assignments (+0xE5 first).
- **C3DMesh2::Sizeof** 00a893c0 -- `size += Primitives[i].Sizeof() - sizeof(C3DPrimitive2)`.
- **C3DMesh2::InitialisePointers** 00a896a0 -- carve the pooled block; materials/primitives are constructed by an
  inline helper taking the count BY VALUE (count-down loop, placement new with null test).
- **CMatrix3x4::GetRotationAngles** 00c1cc10 (catalog "C3DMeshFileBoneChunk::SetLocalizationMatrix") -- three Atan2 /
  rotate steps on a whole-matrix local copy.
- **C3DMeshFileClothChunk / HelperPointChunk / PrimitiveChunk ::CreateNewInstance** 00ac66e0 / 00aa9da0 / 00abd0d0 --
  `return InitNewInstance(new T);` with the constructor inline; the primitive chunk's MaterialIndices is the STLport
  vector structure, resized to one element set to -1.
- **C3DMeshFileConvexVolumeChunk::WriteToFile** 00aaa9a0, **C3DMeshFileGroupChunk::WriteToFile** 00abaee0,
  **C3DMeshFileMaterialChunk::WriteToFile** 00abfcf0, **C3DMeshFilePrimitiveChunk::WriteToFile** 00abc390 -- chunk model of
  the landed family (retail base 0x2C); element reference locals; CRGBColour must be one dword (else byte offsets);
  the primitive chunk wants if/else with ONE shared super-chunk call (an early return allocates out to esi).
- **C3DMeshFileMapChunk::GetChunkInfo** 00abfe80 -- `CCharString info = "Description: " + GetName() + ", Filename: " +
  GetFilename(); return info;` (by-value getters = the two leading copies; named local copied into the result).
- **C3DMeshFileEngineMaterialChunk::CalculateMapFlags** 00ac5260 -- null-case-first `GetLength()` accessor per map name.

## Parked for Fable 5.1 (9)
- 004fab40 C2DBoxI::Overlapping DIFFER(127v157): retail recomputes both y half-heights for the compare (o.minY reloaded, this-half first), the lane CSEs them | tried: separate ifs, `&&` (fixes the branch layout), C2DCoordI centre struct, reversed y compare | next: model the y axis through a different inline (e.g. a GetHeight()/2 accessor vs centre helper), else lane gap: CSE across inlines
- 0066c880 CRegionMinimap::GetTGAPositionFromWorldCoordinates DIFFER(115v115): `sub esp,14h` with own slots for the two fidiv temporaries vs the lane's `sub esp,0Ch` homing them in the dead pos slot (load order follows) | tried: long locals, C2DCoordI topLeft local, extent struct local, interleaved named width/height | next: lane gap: frame-slot reuse of dead argument slots
- 00aaf5b0 C3DAnimFileSequenceChunk::GetChunkInfo DIFFER(151v149): lane loads the hidden result pointer into esi at entry (name pointer to edi); retail re-reads the slot after _snprintf | tried: direct form | next: lane gap: /GS hidden-pointer caching (GOTCHAS 23e)
- 00abae50 C3DMeshFileGroupChunk::GetChunkInfo DIFFER(100v98): same /GS hidden-result caching plus `mov eax,ecx` | tried: direct form, optimize("s") | next: lane gap: /GS hidden-pointer caching
- 00a4fd70 C3DAnimationSequenceData::InitialiseAsConstant DIFFER(132v130): lane caches the matrix reference in edi (push edi) under /GS, retail re-reads its slot | tried: direct form | next: lane gap: /GS argument caching
- 00a52650 C3DAnimation2::ConvertTimeToFrames DIFFER(110v117): `fsub 0.5 ; fistp` floor with frame-local 0.5f/1.0f and a frame pointer | tried: C++ floor stand-in (graded once) | next: lane gap: inline-asm helper (asm_bake only)
- 00a526d0 C3DAnimation2::ConvertTimeToFrames DIFFER(115v123): same inline-asm floor on a rate +4 / ushort count +8 record | tried: C++ stand-in | next: lane gap: inline-asm helper
- 00a4dea0 C3DAnimationSequenceData::GetKeyframeInterpolate DIFFER(111v108): same inline-asm floor | tried: C++ stand-in | next: lane gap: inline-asm helper
- 00ac6600 C3DMeshFileClothChunk::WriteToFile DIFFER(146v146): esi/edi swap (retail out=edi, &CompiledCode=esi) | tried: plain bool local, reference local, inline WriteOptionalArray helper, duplicated condition | next: lane gap: register colouring (or a different array/stream helper shape)

## Skipped
None (no row spans two functions).

## Label corrections
- 0066c880 "C2DCoordI::operator+=" -> CRegionMinimap::GetTGAPositionFromWorldCoordinates (reads the minimap's size, RegionMapBox and RegionMapSquare; inverse of 0x0066C810).
- 00cbd670 / 00a737b0 / 00a73840 "C3DAnimFile::C3DAnimFile" -> not constructors: record push_back + changed flag (named AddNamedChunk / AddKeyedChunk / AddStringKeyedChunk here; real names unknown).
- 00a42fc0 "C3DAnimationInfo::GetAnimationStats" -> CLipSyncDataBank::GetLipSyncData (entries array at +0x164, EntryExists 0x009CC190, CLipSyncDataBankEntry::BeginLoading 0x00A42980).
- 00c1cc10 "C3DMeshFileBoneChunk::SetLocalizationMatrix" -> CMatrix3x4::GetRotationAngles (`this` is the 48-byte matrix).
- 00a526d0 "C3DAnimation2::ConvertTimeToFrames" -> a twin on a C3DAnimationSequenceData-style record (rate +4, ushort count +8); no callers found.
- Callee labels: 0x00A026D0 "CDefinitionManager::GetDefClassIndexFromGlobalIndex" is CMovableResourceMemoryPool::GetAllocatedSize; 0x009FC440 "CTCActivationTrigger::OnKill" is CResource::AddToResourceList-like (`if (ResourceList) ResourceList->f(this)`); 0x00994B40 "CCreatureActionBase::GetFramesRemaining" is CMemoryDataOutputStream::GetSize; 0x00993E50/0x00993E80 "WriteCBYTE" are WriteUBYTE / WriteUSHORT; 0x00AAF800 "CInputManager::EventScanStart" is C3DAnimFileSequenceChunk::AddMatrix; 0x00AA8490 "~CCompressedMemoryDataInputStream" is ~CChunkedFileSuperChunk.
