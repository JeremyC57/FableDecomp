# Hand chunk 602 (CReverbDiffuserStage .. lzo1x code_match): 7 of 10 byte-exact

## What matched (7)
- **CReverbDiffuserStage::Process** 0x00C891A0 (428, RELOCATION_MATCH, grade 2) -- libpk reverb diffuser stage: ReadRing of the
  history, three AddFromRing taps, ScaleCopy into the output, Scale, AddFromRing on the stage's own ring, a split CDspGain::Apply,
  Mix into the mix buffer, then the ring advance. Shape: the ring advance is an inline member on the sub-object
  (`m_ring.Advance(count)`); VC7.1 then reuses `&m_ring` and spills `this`, which frees ebx for the loop index (the first form put
  `this` in ebx and spilled the index, 419v428). `__declspec(align(16)) float block[128]` gives the `and esp,-16` frame.
- **CStreamBuffer::FinishLoading** 0x00C26090 (431, RELOCATION_MATCH, grade 2) -- end of a stream buffer's load: unlock, drop the
  users, unwrap a compressed chunk in place (ReadChunkExtent), decode ADPCM into a newly allocated PCM block that replaces the old
  one, then move the buffer from the loading queue to the idle and cached queues with a fresh LRU stamp. Shape: the chunk source
  must be a named local `void* src = (char*)pool->GetPointerFromHandle(h) + m_dataSize;` (m_dataSize then lands in ebp ahead of
  the out-pointer pushes); the inline expression schedules it differently (2 lines).
- **CParticleSimulation::WriteVectorArray** 0x00AE18C0 (436, RELOCATION_MATCH, first grade) -- narrows count C4DVectors to a
  CArray<C3DVector> and writes it as one block. Shape: the landed ReadVectorArray model reversed; the inline
  `C4DVector::ToC3DVector()` in the loop makes VC7.1 unroll it by 4.
- **CImaAdpcmCodec::EncodeS16** 0x00C5F4E0 (438, RELOCATION_MATCH, first grade) -- DirectX SDK IMA ADPCM stereo encoder
  (upstream name and body: `min(cSamples, 8)` groups, dwLeft/dwRight words), modelled on the landed EncodeM16 0x00C5F3B0.
  Identity: the encoder row of Initialize's converter table 0x012B9274 (its only reference, .rdata 0x012B9280).
- **NScript::CQ_AwakeningTheOracleScript::PlayCutscene** 0x00CFF9A0 (443, RELOCATION_MATCH, first grade) -- casts the hero as HERO
  in cs_chars, fixes the movie camera, runs the cut-scene with cs_args (inside a paused movie sequence when the bool is set), then
  releases the camera and clears both maps. Shape: the landed CQ_DragonBossFightScript::PlayCutscene TU (STLport 4.6 map,
  out-of-line CCountedPointer<STB>::operator=) with the ScytheInfo body; map::clear out of line for both map types.
- **CVtable_012b33f0::Read** 0x00C1E300 (450, RELOCATION_MATCH, grade 2, /EHsc) -- under the audio scoped lock, copy the sample
  into the buffer, unwrap a compressed chunk, decode ADPCM to PCM through a temporary `new[]` block; offset and size out through
  pointers. Shape: the chunk source is a named local (as 0x00C26090); `operator new[]` must be DECLARED or VC7.1 calls ??2
  instead of ??_U; the block goes back through scalar `delete` (??3).
- **code_match** 0x00C076C0 (451, MATCH, first grade) -- LZO 1.08 lzo1x_9x.c code_match verbatim (M1a/M2/M1b/M3/M4 arms) as an
  `extern "C" { static __declspec(noinline) }` with a same-TU caller (retail's private convention: op in ecx, m_len in eax, c and m_off on the stack).

## Parked for Fable 5.1 (1)
- 0x00C1D320 CMatrix3x4::Invert DIFFER(440v440): only the first block (28 diff lines): ours computes the CSE'd cofactor temps c1, c2, c0, retail c2, c1, c0, and one operand pair (m5 before m7 in c0) | tried: separate cofactor locals (66/58 lines), swapped operands (58), raw float* view (58), unused-local k sweep 1..7 (no change), the engine SetInverse single-expression text (28), reversed det sum (worse) | next: a `__forceinline SetInverse(const CMatrix3x4&)` member called as `SetInverse(*this-copy)` plus a k re-sweep, or the flat E11..E43 member model; the epsilon 1e-10f at 0x0139C978 is shared with the unlanded 0x00C1D1C0 (probably the out-of-place SetInverse sibling) -- land them together. Grade budget overrun (about 5 strategies + a 14-compile k sweep).

## Skipped (2)
- 0x00C069D0 -- handled by the coordinator: LZO 1.08 src/i386/d_asm2/lzo1x_f1.asm `_lzo1x_decompress_asm_fast` (447 bytes,
  byte-identical; probe/00c069d0_identification.txt), landed as toolchain_library; the real lzo1x_decompress_safe is 0x00C08170.
- 0x00C1C460 (445, EH, thiscall, no args) -- libpk 3D-sound update: when enabled (+0x19) and the listener (+4) is active (+0x885),
  SwapYZ its position/up/forward (+8/+0x14/+0x20), build a view matrix and its inverse (0x00C1BF00), then under g_soundLock
  (0x0143BAFC) for each source of the listener's array (+0x224, count +0x228; PKCFrallocStatic::Get 0x00C13700) with a voice
  (+4, +0x48) and enabled (+0x25): read its record (0x00C34600), SwapYZ the position, transform it (TransformHelperPoint 0x00C1C3F0
  when the record's flag +0xC is set, else 0x00C1C330: view transform, scale z, back to world), copy (0x00C1BFF0) and pass it to
  CODA_SetSourcePosition 0x00C33FD0 with the voice handle (+0xC); always tail-calls 0x00C33690. Not attempted: it needs two
  unlanded same-TU helpers with private conventions (0x00C1BF00: edi, ebx, esi + two stack words, `ret 8`; calls Cross 0x00C25860,
  Dot 0x00C256A0 and the unlanded inverse 0x00C1D1C0; 0x00C1C330: eax, ecx, edx, esi, x87-heavy) plus the landed static
  TransformHelperPoint, and most callees carry 2-5 conflicting landed spellings (0x00C13700 x3, 0x00C34600 x2, 0x00C33FD0 x2).
  Next: land 0x00C1D1C0 / 0x00C1D320 first, then this row with all three helpers in one TU.

## Label corrections
- 0x00C1E300 "PathFinding_CompressAndQuery" -> CVtable_012b33f0::Read (slot 1 of the no-RTTI vtable 0x012B33F0).
- 0x00C26090 "CTCInventory_EquipItem" -> CStreamBuffer::FinishLoading.
- 0x00CFF9A0 "Main_InitializeHeroMorphComponents" -> NScript::CQ_AwakeningTheOracleScript::PlayCutscene.
- 0x00C891A0 "Graphics_ProcessStreamData" -> CReverbDiffuserStage::Process; 0x00AE18C0 "WriteAnimationData" ->
  CParticleSimulation::WriteVectorArray; 0x00C5F4E0 "Audio_DecodeADPCM_Stereo" -> CImaAdpcmCodec::EncodeS16 (an encoder);
  0x00C1D320 "Matrix_Invert" -> CMatrix3x4::Invert; 0x00C069D0 "LZSS_Decompress" -> lzo1x_decompress_asm_fast;
  0x00C1C460 "ParticleMorphs_UpdateEditor" -> libpk sound-source position update (unnamed).
- Callee labels proven wrong this chunk are in NAME_FIXES.tsv (13 rows: GetPointerFromHandle, ReadRing, s_pOps, the
  0x0143BB14 lock, GetDataSize, GetPCMBufferSize, CheckPlayable, RunCutsceneMacro, the two map clears, the caller spellings of
  0x00AE18C0 / 0x00C891A0 / 0x00C26090).

## Names proven vs descriptive
- Proven (two sources): CParticleSimulation::WriteVectorArray (Ego_r + FableWin PDB, callers);
  NScript::CQ_AwakeningTheOracleScript::PlayCutscene (Ego_r + FableWin PDB, existing binding, layout); CHandleBasedMemoryPool
  Lock / Unlock / GetPointerFromHandle (BSim 1.0 + PDB; but see the libpk pool conflict below); RunCutsceneMacro (PDB + family).
- libpk pool conflict: vland reports RELOC_CONFLICT for Lock / Unlock / GetPointerFromHandle at 0x00C283C0 / 0x00C283E0 / 0x00C28380
  vs the engine copies 0x00A026F0 / 0x00A02710 / 0x00A026B0 (same PDB spellings). The landed libpk Lock/Unlock definers already
  use those spellings, so 0x00C26090 follows them; 0x00C28690 keeps its bound FreeAllocation because the PDB Free(long) is held by
  the engine copy 0x00A02B30. The two copies cannot share decorated names in one image: the libpk copy needs its own scheme.
- Upstream third-party names: code_match (LZO 1.08), CImaAdpcmCodec::EncodeS16 (DirectX SDK imaadpcm.cpp).
- Descriptive (libpk has no PDB or RTTI): CReverbDiffuserStage::Process, CDspOps::ReadRing, CStreamBuffer::FinishLoading,
  CVtable_012b33f0::Read, CSoundFormat::CheckPlayable, DecodeSoundData_00c34630 (new: the ADPCM-to-PCM decoder, `__fastcall`
  format/src + dst/size, shared by 0x00C1E300 and 0x00C26090), CMatrix3x4::Invert, CSoundEventManager / CSoundEventSet.

## Header debt
- engine/CDataOutputStream.h flattens the vptr; 0x00AE18C0 uses a local model with virtual Write at slot 4.
- libpk audio (CSoundSample, CSoundFormat, CStreamBuffer, CSoundEventManager, the libpk CHandleBasedMemoryPool variant one word
  off engine/CHandleBasedMemoryPool.h) and libpk DSP (CDspOps, CDspGain, CRingBuffer) are local `// Layout:` models; a shared
  rebuild/include/libpk/ header set would retire about a dozen copies.
- NScript quest scripts: the CQ_AwakeningTheOracleScript members are written out locally (no engine header for the quest class).
