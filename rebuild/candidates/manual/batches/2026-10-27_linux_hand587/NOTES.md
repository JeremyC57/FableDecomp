# Hand chunk 587 (CDSoundAccelerationQuery .. CActivationReceptor_00c38b20): 7 of 10 byte-exact

The chunk has 10 rows, all `global` in the manifest. 7 are byte-exact, 3 are parked, none skipped. Every test prints
`<addr>_TEST PASS` under both g++ and VC7.1, including the three differ tests.

## What matched (7)
- **libvorbis mdct_butterfly_generic** (0x00C9C2F0, MATCH): the Xiph mdct.c body. It is a TU-local
  `static __declspec(noinline) void __stdcall` inside `extern "C"`, with the verbatim caller mdct_butterflies in the same
  TU. The plain static gave `ret` (267v269). Adding `__stdcall` reproduces retail's LTCG callee-pop `ret 4` and keeps the
  private register convention.
- **CEnumPins::Next** (0x00CAAFC0, RELOCATION_MATCH; DirectShow strmbase amfilter.cpp, vtable 0x012BF69C slot 3): the
  model is the landed CEnumPins_Refresh_00ca7b60 strmbase one, plus POSITION, CBaseList::FindI (0xCB13C0) and
  AddTailI (0xCB1460). Refresh is defined out of line before Next, so it gets inlined. The body is the DX8.1/9 form:
  `min(m_PinCount - m_Position, cPins)`, S_FALSE on 0, and the loop `while (cRealPins && (m_PinCount - m_Position))`.
- **std::vector<C3DGroup2>::_M_fill_insert** (0x00AE82C0, RELOCATION_MATCH): a real STLport 4.6 model
  (_Vector_base, _STLP_alloc_proxy). It needs a same-TU noinline `__uninitialized_copy` body (0xAE7480), because the
  `__false_type` tag is homed on the live `x` argument slot. Without that body: 270v270 with the wrong tag lea.
  CArray<T> derives from std::vector<T>.
- **NScript::CQ_AwakeningTheOracleScript::OnPersist** (0x00CFAF70, RELOCATION_MATCH; vtable 0x012C78B0 slot 4): ten
  `CPersistContext::Transfer` calls in retail string order. The bool template is 0x004045C0 and the long template is
  0x00410BE0 (TabletsCollected and GraveNumbersGivenOut default to 0L). The model uses the shared `engine/CLevelScriptBase.h` / `engine/CScriptBase.h` (FABLE_RETAIL_NSCRIPT); the first grade used the landed Init 0x00CFADD0 local model and also matched.
- **CAudioObject::Update(__int64)** (0x00C43A10, RELOCATION_MATCH, descriptive libpk name): the voice class has real
  virtuals at slots 7..10 (Play, UpdateQueue, UpdatePlaying, UpdatePending). Retail's queue test `je ; je ; cmp ; je`
  is `m_capacity != 0 && !IsEmpty()`, where `IsEmpty()` is `m_capacity != 0 && m_stats[0] == 0`.
- **std::__merge_without_buffer<CSortElement*, int, less<CSortElement>>** (0x00B86140, RELOCATION_MATCH; spelling
  already bound by caller 0xB86570): the SGI body. lower_bound and upper_bound are inline front ends to __lower_bound
  0xB85240 and __upper_bound 0xB85280 with `(int*)0`. rotate dispatches to `__rotate` 0xB85660. The second recursive
  call becomes a loop.
- **CActivationReceptor_00c38b20::CActivationReceptor_00c38b20()** (0x00C39130, RELOCATION_MATCH, `/EHsc`; landed
  placeholder class from dtor 0xC38B20; 0x180-byte element built by __ehvec_ctor at 0xC2D326): five parts, each a
  struct of {head MEMBER, entry} with `: m_entry(this, (void*)1)`. Then m_state(0) at +0xF8,
  CAsyncDecoder_00c48640 m_registry, CInterpolatedFloat m_138(0, 0), m_170 = m_174 = m_178 = -1, m_pSink = 0 and
  m_scale = 1.0f. Grade 1 used the heads as base classes and got extra vptr stores (301v265).

## Parked for Fable 5.1 (3)
- 0x00C5DC60 CDSoundAccelerationQuery::OpenDeviceRegistryKey DIFFER(265v267): loop induction-variable allocation. Retail spills `this` to [esp+8] at entry and keeps offset=ebx, pointer=esi, start=eax, count=ebp; it rebuilds start after strncat as p + (1 - this). Ours keeps this in ebp and the count in memory, and the -1 exit block sits at the end instead of inline | tried: int-index loop (244v267), pointer + index (252), unsigned index (244), pointer walk `p - m_interfacePath` (265) | next: an if/else `return result` tail to pull the -1 block inline; an ebx-index plus separate-pointer form
- 0x00C89D70 libvorbis::_get_prev_page DIFFER(265v269): the two `mov eax,esi` before each _get_next_page call are missing. Retail _get_next_page takes vf in eax; our same-TU static gets it in esi (the callee chooses the private convention). Every other byte matches | tried: static __stdcall noinline bodies of _get_data/_seek_helper/_get_next_page with a scaffold _open_seekable2 caller; an extra _fetch_headers scaffold caller; declaring _get_data only | next: lane gap: LTCG private-convention register choice for a cross-function static (needs the full vorbisfile.c TU's call graph)
- 0x00AE3920 CParticleProgram::RemapBoneIndices DIFFER(266v270): retail stores the unsigned long key temporary [esp+18h] before the inlined map find and reuses it for operator[]; ours stores it at the operator[] call. The local vector dtor keeps _M_start in ebx and compares against the ebp zero; ours reloads into ecx | tried: a named `unsigned long bone` local (273v270, ebx/edi swap and a lazy ebx push), a const-ref local (same), bone declared before the group ref (same) | next: `map::find(key) != end()` written via an explicit `iterator it` then `it->second` instead of operator[]

## Skipped
None.

## Label corrections
| Address | Manifest label | Correct |
|---|---|---|
| 0x00C5DC60 | Audio_QueryHardwareDevicePath | CDSoundAccelerationQuery::OpenDeviceRegistryKey (descriptive; class pairs with landed FindDeviceInterface 0xC5DB40) |
| 0x00C89D70 | AsyncBankFile_Process | libvorbis vorbisfile.c `_get_prev_page` |
| 0x00C9C2F0 | FFT_Radix2VariableStride | libvorbis mdct.c `mdct_butterfly_generic` |
| 0x00CFAF70 | sub_00cfaf70 | NScript::CQ_AwakeningTheOracleScript::OnPersist |
| 0x00AE3920 | InsertFromVector | CParticleProgram::RemapBoneIndices (private static) |
| 0x00AE82C0 | Vector_Insert_Pair128BitMultipleResize | std::vector<C3DGroup2>::_M_fill_insert |
| 0x00B86140 | std_introsort_binary_search | std::__merge_without_buffer<CSortElement*,int,less<>> |
| 0x00C43A10 | CDisplayManager_ProcessStateChange | CAudioObject::Update(__int64) (descriptive) |
| 0x00CAAFC0 | PopNodes_FromList_UpToCount | CEnumPins::Next |
| 0x00C39130 | CTCTavernGameSpotTheAddition_Constructor | CActivationReceptor_00c38b20 default ctor |
| 0x00C432F0 | CGuiWindow::OnDelete (neighbour) | CAudioObject::ReportQueueInfo (descriptive) |
| 0x00C437C0 | CDisplayManager::DestroyDevice (neighbour) | CAudioObject::FlushQueue (descriptive) |

## Names and evidence
- **mdct_butterfly_generic, _get_prev_page:** Xiph sources mdct.c and vorbisfile.c match byte for byte (or to the
  residue). The callers sit in the same retail cluster: mdct_butterflies, and the _get_next_page/_seek_helper landed
  differs.
- **CEnumPins::Next:** amfilter.cpp body, plus slot 3 of the CEnumPins vtable 0x012BF69C (IEnumPins::Next order).
- **_M_fill_insert:** STLport 4.6 body, byte-exact. Called by resize 0xAE8540. The element is C3DGroup2 (0x18 stride,
  Ego_r.pdb).
- **OnPersist:** vtable 0x012C78B0 slot 4 (Init is landed at slot 3). The member strings are the quest's persisted flags.
- **__merge_without_buffer:** spelling already bound by its caller 0xB86570 (__inplace_stable_sort). The SGI body matches.
- **RemapBoneIndices:** Ego_r.pdb names CParticleProgram's private static RemapBoneIndices(map&, CArray<C3DGroup2>&).
  The body calls C3DGroup2::SetBoneIndex 0xAC4070.
- **CAudioObject::Update, ReportQueueInfo, FlushQueue, OpenDeviceRegistryKey:** descriptive (no PDB entry for libpk or
  the DSound query helpers).
- **Globals:** g_audioDirtyFlags 0x0143BBFC (`?g_audioDirtyFlags@@3KA`, descriptive).

## Header debt
- vland's SHARED_HEADER_AVAILABLE warnings that stay: `CBaseObject` (0x00CAAFC0) is a name collision. engine/CBaseObject.h
  is Fable's CBaseObject (vtable 0x0129A7D8, 8 bytes); the DirectShow strmbase CBaseObject (m_pName, vtable-less
  root of CBaseUnknown) is a different class with the same spelling. `CEngineSubPrimitiveRenderer` (0x00B86140): the
  header has no nested CSortElement (only an opaque SortBuffer +0x180), so the row declares the class locally with
  the nested element.
- `engine/CParticleProgram.h` has no member functions and a flattened vftable, so 0x00AE3920 uses the layout-rename
  include.
- There are no engine headers for C3DGroup2, CArray<T>, CEngineSubPrimitiveRenderer::CSortElement, the libpk audio
  classes, the strmbase classes or OggVorbis_File. Local models are used.
- The landed CActivationReceptor_00c38b20 dtor model splits the part at +0xCC into 0xC + 0x20. It is one 0x2C part
  (head CUnprovenBase_00c3a520 plus entry CScriptEntry_00c39990, dtor 0xC39CE0).
- The landed head models (CUnprovenBase_00c39d30 / _00c3a520) declare virtual dtors, but the retail heads have trivial
  dtors: the ctor shows no derived vptr store and the part dtors do not call through the vtable.
