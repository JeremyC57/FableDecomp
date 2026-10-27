# Hand chunk 495 (CItemGridTable_012b501c .. CDSoundStreamBuffer): 36 of 40 byte-exact

All 40 rows are libpk / GameCODA audio middleware (0x00C34E20..0x00C57110). That code has no PDB and no RTTI, so every
name here is descriptive or reuses an existing landed or bound name. Read-only vland (tmp/vland.log): 36 WINS, all tests
PASS, no RELOC_IDENTITY. Counts: 16 MATCH, 20 RELOCATION_MATCH, 0 retail_asm, 2 parked, 2 skipped.

## What matched (36)
- **Item tables (vtables 0x012B501C / 0x012B504C, abstract base CItemTable 0x012B3444; slot layout from the landed Validate
  0x00C348F0):**
  - 00c34e20 GetAt(row, column), slot 6. The row must go into a named local (`entry = m_rows.m_data[row]`). Without it,
    VC loads the column first; a named data-pointer local or an inline operator[] does the same.
  - 00c34ff0 Remove(row, column), slot 8. A row left empty is removed from the row list and deleted. `delete` of a derived
    row class with an implicit dtor gives null test + base dtor call + operator delete.
  - 00c350f0 Insert, slot 3. Unsigned `row > 0` compiles to `test ; jbe`.
  - 00c35120 GetColumnCount, slot 4. Compiles to `!= 0` with setne.
- **CSemaphore 00c36170 Release:** ReleaseSemaphore(handle, 1, 0) through the dllimport IAT.
- **CMaxValueVisitor 00c36cc0 Visit:** follows the landed GetMaxValue model. The getter is called twice.
- **CSoundLoop_00c377f0 00c377e0 GetSoundSystem:** a three-load pointer chain. Uses the bound name.
- **CSoundEmitter_00c37ad0 00c37a40 SetPosition:** slot 5 of vtable 0x012B5370. A 16-byte struct copy plus a byte flag.
- **CActivationReceptor_00c38b20 00c38b10 Final:** uses the bound name. Passes a `push ecx` frame word to the mailbox
  DispatchMessage.
- **CSegmentFile 00c3aa40 GetPosition:** cursor + first.
- **CLibpkStateA4 00c3d390 SetFloat94:** retail multiplies by the pooled constant, `fld value ; fmul [K]`, ahead of the
  dword copy. Only a LITERAL `1.0f / 330.0f` gives this; an extern float global loads first in either operand order.
- **CMorphAction_00c3dcd0 00c3d9a0 `??_E`:**
  - The array arm calls `eh vector destructor iterator`, which needs `// Flags: /EHsc`. Without it the loop is inlined.
  - The array free goes to 0x00C3C370 only when CPKObject also declares `operator delete[]`. Otherwise VC7.1 frees through
    global `??3`, which is a RELOC_IDENTITY.
  - The `??_E` is emitted by an array-new factory (a model device).
- **CLimitedMixBuffer 00c3f000 Reset:** slot 1. `for (i < 6) m_limiters[i].Clear()` compiles to a down-counter and a
  stride of 0x17C.
- **CCodaVoice 00c3f460:** slot 3. Clears +0xc and returns 0.
- **CLibpkObject78 00c40c70 / 00c40cc0 / 00c40cf0 setters:** follow the landed SetFloat58..64 shape, plus an `or` of the
  global change flag 0x0143BC28.
- **CLibpkObject1D8, the 3D audio object (vtable 0x012B5AB0, base CAudioObject 0x012B5A88):**
  - 00c412d0 GetEffectiveGain, slot 5. Spilled product. The attenuation arm computes `call result * gain`.
  - 00c415e0 SetFloat154.
  - 00c417f0 SetFlag40000 and 00c41840 SetFlag80000. Each if-arm must read the state word itself; VC merges the two reads,
    and that gives retail's `mov al`. A single shared read before the `if` gives `mov dl`. The dirty mask is the
    `mask = A; if (!bit) mask = B;` form; a ternary becomes neg/sbb/and/add.
  - 00c447a0 SetLoopPointPair and 00c44800 SetVolumeParamPair. Two bound Update* helper calls through the CAudioObject
    base, then the mutually exclusive dirty-bit swap.
- **StreamReader 00c42720 Read:** slot 3. memcpy is the /Oi intrinsic, `this` stays in eax.
- **CAudioObject 00c43890 Devirtualize:** uses the landed Reset model (inline IsVirtual / GetState / RaiseStateChange).
- **CEntryArray_012b7d50 00c46360 Add:** the pair member is copied first, then the key; the stores make data and count
  re-read.
- **PKCBufferedReader 00c4aa20 Tell.**
- **CRACBuffer 00c4bff0 GetAlignedSize and 00c4c000 GetPointer:** const getters.
- **CEAXManager 00c4e350 GetEffectProperty:** the out word is homed in the dead fourth argument slot.
- **CDecodedStream 00c547a0 Open:** slot 2. `f() ? 0 : -9` compiles to neg/sbb/and/add.
- **CAudioSource 00c556e0 Setup:** slot 1 of 0x012B8AE0.
  - Calls the base setup non-virtually, then CAudioRingBuffer::Init (bound), then the stream-descriptor select.
  - The source word must go into a named local; passed directly it loads into ecx before `mov ecx,esi`.
- **CAudioContext:**
  - 00c55d70 SetDistanceExponent, slot 7. `pow(2.0, -(double)n)` is the /Oi `_CIpow`.
  - 00c56060 SetDefaultSpeakerConfig, slot 3. Global stores.
- **CDSoundStreamBuffer, slots 9 and 10 of 0x012B8D90:**
  - 00c570d0 LockChunk: COM Lock, four frame locals, `hr < 0 ? 0 : p1`.
  - 00c57110 UnlockChunk: COM Unlock, then the cursor modulo the buffer size.

## Parked for Fable 5.1 (2)
- 00c3d030 CAudioSystem::GetChannelCounts DIFFER(92v94): retail loads `counts` into eax before the device branch and keeps values in edx, then tail-duplicates the last device store (`jle ; mov ecx,[ecx+64h] ; mov [eax+0Ch],ecx ; ret`); ours loads counts per arm into edx and merges the last store | tried: four ternaries; if-form with if/else last store; requested[0] hoisted into a local before the branch (closest, 92 bytes) | next: hoist `counts` too (e.g. write through a local reference struct `SCounts& out = *(SCounts*)counts`) or a per-element inline helper with the device test inside
- 00c42fb0 CAudioObject::SetMillibels DIFFER(40v36): retail keeps `this` in ecx across the cdecl MillibelsToAmplitude_00c42c90 call (`push eax ; mov [ecx+1Ch],eax ; call ; fstp [ecx+18h]`, no esi frame); ours saves this in esi | tried: plain C++ body | next: lane gap: LTCG register contract (callee known not to clobber ecx)

## Skipped
- 00c35460: the row packs four functions: `mov eax,[ecx] ; jmp [eax+14h]` at 0x00C35460, then `ret 8` at 0x00C35470,
  `ret 4` at 0x00C35480 and `ret 4` at 0x00C35490, separated by int3 padding. The catalog must re-split it.
- 00c3c4b0: the row packs two functions: the bit-2 virtual dispatch (`jmp [edx+0Ch]` / `jmp [edx+8]`) at 0x00C3C4B0, and
  `mov eax,-9 ; ret 4` at 0x00C3C4D0. The catalog must re-split it.

## Names
### Proven
00c36cc0
00c377e0
00c38b10
00c3d9a0
00c3f000
00c4aa20

Evidence for each:
- 00c36cc0 CMaxValueVisitor::Visit: slot 0 of 0x012B51E8, named in the landed ctor 0x00C36DA0 and GetMaxValue 0x00C37000.
- 00c377e0 CSoundLoop_00c377f0::GetSoundSystem: the bound callee name, from three landed callers.
- 00c38b10 CActivationReceptor_00c38b20::Final: the bound callee name, from the landed dtor 0x00C38B20.
- 00c3d9a0 `??_ECMorphAction_00c3dcd0`: slot 0 of 0x012B5A80, whose ctor 0x00C3CE50 is landed under that class. The
  compiler-generated array form is proven by the bytes.
- 00c3f000 CLimitedMixBuffer::Reset: slot 1 of the CLimitedMixBuffer vtables, which is CMixBuffer's Reset slot in the
  landed model. Named as the caller in landed CLimiter::Clear 0x00C592D0.
- 00c4aa20 PKCBufferedReader::Tell: FINDINGS chunk 445 ("0xC4AA20 = Tell"), and the class of the landed GetSize / Seek.

### Probable
00c34e20
00c34ff0
00c350f0
00c35120
00c36170
00c3aa40
00c42720
00c570d0
00c57110

Evidence for each:
- 00c34e20, 00c34ff0, 00c350f0, 00c35120: the slot roles come from the CItemTable slot layout. The class names
  CItemGridTable_012b501c and CSingleItemTable_012b504c are descriptive.
- 00c36170 CSemaphore::Release: the class is the landed CSemaphore and the body calls ReleaseSemaphore.
- 00c3aa40 CSegmentFile::GetPosition: the class is the landed SetRange's.
- 00c42720 StreamReader::Read: a vtable slot; the class name follows landed 0x00C426F0.
- 00c570d0 CDSoundStreamBuffer::LockChunk and 00c57110 UnlockChunk: COM Lock / Unlock in the slots after the landed
  IsChunkReady.

### Descriptive
00c37a40
00c3d390
00c3f460
00c40c70
00c40cc0
00c40cf0
00c412d0
00c415e0
00c417f0
00c41840
00c43890
00c447a0
00c44800
00c46360
00c4bff0
00c4c000
00c4e350
00c547a0
00c556e0
00c55d70
00c56060
00c3d030
00c42fb0

Each class name follows the landed siblings: CSoundEmitter_00c37ad0, CLibpkStateA4, CCodaVoice, CLibpkObject78,
CLibpkObject1D8, CAudioObject, CRACBuffer, CEAXManager, CDecodedStream, CAudioSource, CAudioContext and CAudioSystem.
CEntryArray_012b7d50 is new. The method names describe the body.

## Label corrections
- 00c377e0: catalog "CPlayer::GetCurrentMode" is CSoundLoop_00c377f0::GetSoundSystem.
- 00c38b10: "CComponent::GetCurrentState" is CActivationReceptor_00c38b20::Final.
- 00c3aa40: "CGameScriptThing::GetPThingUniqueID" is CSegmentFile::GetPosition.
- 00c4aa20: "CGameMap::GetMemoryFootprint" is PKCBufferedReader::Tell.
- 00c4bff0: "CWorld::DrawGetEnvironment" is CRACBuffer::GetAlignedSize.
- 00c4c000: "CBasicString<char>::GetChar" is CRACBuffer::GetPointer.
- 00c36170: "_global::SemaphoreHandle_Release" is CSemaphore::Release.
- Landed 0x00C35170 (CBaseIntelligentPointer::CBaseIntelligentPointer) is the ctor of the single-item table, vtable
  0x012B504C (here CSingleItemTable_012b504c). Its `m_pItem` is the table's entry pointer.
- Landed 0x00C34DD0 (CMember_00c34dd0) is the ctor of the grid table, vtable 0x012B501C (here CItemGridTable_012b501c).
- Landed 0x00C42AB0 (CSmallVector<CANavQuadTreeNode const*,8>::operator[], free fastcall) and 0x00C42AC0
  (DrawGetEnvironment) are CPtrArray<CTableEntry*>::GetAt / GetCount. The row arrays share the class of the landed
  ~CPtrArray<CTableEntry*> 0x00C42AD0. 0x00C42B80 (unlanded) is RemoveAt(index).
- Landed 0x00C5A1D0 (CTCStealth::OnCreate) is the software voice's slot-1 setup (base of CAudioSource 0x00C556E0).
- Landed 0x00C3D7B0 (leaf `??1CRenderStateManager@`, catalog CVtable_012b5a80 dtor) is ~CMorphAction_00c3dcd0.
- Landed 0x00C4BFA0 (CBankFile::IsAligned) works on the same CRACBuffer view as 0x00C4BFF0 / 0x00C4C000 / 0x00C4C050.
- 0x00C54510 ("Audio_InitializeParameters") is the CDecodedStream initialiser called by 0x00C547A0.

## Callee names (new spellings, no correct one existed)
- CPtrArray<CTableEntry*>::GetAt / GetCount / RemoveAt: 0x00C42AB0 / 0x00C42AC0 / 0x00C42B80.
- ??1CMorphAction_00c3dcd0@@UAE@XZ: 0x00C3D7B0.
- ??_VCPKObject@@SAXPAX@Z: 0x00C3C370. This is a second name for an address already bound as ??3CPKObject@@SAXPAX@Z;
  RELOC_DUPLICATE, see LESSONS.
- ?Initialise@CDecodedStream@@QAE_NXZ: 0x00C54510.
- ?Setup@CSoftwareVoice_00c5a1d0@@: 0x00C5A1D0.
- ?GetProperty@CEAXEffect_00c66330@@: 0x00C66330.
- Globals g_speakerWord_0139c994 / _0139c9ac / _0139ca00 and g_scratchRing_0143bcb0.
- Reused bound names: CAudioObject::UpdateLoopPoints / UpdateVolumeParams, CVoiceManager::Uncount with g_pVoiceManager,
  CAudioRingBuffer::Init, CAudioSource::Open, MillibelsToAmplitude_00c40ee0, ?M@C00c361e0, g_flags_0143bc28,
  g_speakerChannels_0139ca04, g_speakerMask_0139ca08, g_SubSystemA1C.

## Header debt
- None of these classes has an engine header (libpk).
- The landed StreamReader model at 0x00C426F0 has no vptr: it puts the buffer at +0 and `m_flags` at +4. 0x00C42720 is
  vtable slot 3 and reads the data pointer at +4, so the class is polymorphic and +4 is the buffer. The cursor and size
  offsets agree.
- Base 0x00C4CF20 is named CBase_00c4cf20 in the CMorphAction rows, which this chunk reuses, and CPooledNode in the
  CCodaVoice rows. Pick one name for it.
