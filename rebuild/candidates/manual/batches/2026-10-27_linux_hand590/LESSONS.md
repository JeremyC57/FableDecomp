# Chunk 590 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `/EHsc` can change a function that has no EH frame. The only sign is a `mov [esp+N],esp` that saves the temporary's
  address before a by-value class argument is built in its slot. Plain /O2 is 11 bytes short
  (0x00C2E7A0 CBankProcessor::AddEventTrigger, 286v297).
- A two-float state pair copied as `a = b` needs three things together:
  - the addresses computed for earlier calls are reused (address CSE);
  - the first float is stored before the second is loaded;
  - the source models the pair as a 2-float struct with a user element-wise `operator=`.

  `float m[4]` recomputes the addresses, and a compiler-generated struct copy loads both floats first
  (0x00C74D30 CDspGain::Crossfade).
- A clamp that reads `cmp reg,59h ; jb` after a `jns` comes from the SDK's `else if (i >= NUMELMS(table))`, which is an
  unsigned size_t compare. A signed `> 88` gives `cmp 58h ; jle` (0x00C5F3B0 CImaAdpcmCodec::EncodeM16).
- A named local reused across two call sites, such as `nStepSize = table[i]; f(x, &p, nStepSize);` written twice, gets
  ONE register at both sites and can split a shared epilogue. Retail's different registers at the two sites (edx, then
  eax) mean the value was a call-argument temporary, `f(x, &p, table[i])` (0x00C5F3B0).
- The OpenAL wrapper members (Sourcei, DeleteSources, DeleteBuffers: resolve the entry on first use, then call) expand
  inline in the caller, while SourceStop stays a call. Their landed out-of-line copies are not the callees
  (0x00C579C0 ReleaseBuffer).
- libvorbis `_fetch_headers` (0x00C8A0A0) has the same lane gap as `_get_prev_page` 0x00C89D70: VC7.1 picks the static's
  register parameters itself (vf in ecx, serialno in eax), where retail has og_ptr in eax and serialno in ecx. Park
  after 2 grades.
- The libpk IMA ADPCM converters are seven-argument `__fastcall` statics, `ret 14h`; the extra argument is
  cSamplesPerBlock. A six-argument model mangles every converter wrongly (0x00C5F3B0 / 0x00C5F4E0 / 0x00C5F6A0 / 0x00C5F7F0).

Review at landing:
- **One address, two names in this chunk.** 0x00C0DEC0, 0x00C0DEA0 and 0x00C0DF10 are spelled two ways:
  - CRACBuffer 0x00C4C080 uses the chunk 589 trace model `CString::operator<<`. Its trace local is destroyed by
    ~CString 0x00C0BA40, which is evidence the stream class is CString.
  - AddEventTrigger 0x00C2E7A0 uses `CDebugStream::operator<<`, as bound by AddEvent, with
    `CLoadLog::AddMessage` returning `CDebugStream&`.

  The two models call the same retail functions. The coordinator should pick one class and respell the other rows; I
  lean to CString because of that destructor. reloc_bindings already carries both `??6CDebugStream@@QAIAAU0@PBD@Z`
  (from 0x00C09920) and `??6CString@@QAIAAV0@PBD@Z` (from 0x00C099F0) for 0x00C0DEC0.
- **CDspGain::Crossfade 0x00C74D30 used all 4 grades.** The landed Apply 0x00C74CE0 models the same object with
  `float m_gain[4]` and needs the SRampState model when it is retyped.
- **CImaAdpcmCodec**: the landed codec rows declare six-argument converters, so Initialize 0x00C5F9E0's
  apfnConverter relocations name nonexistent symbols. Respell them to the seven-argument forms in NAME_FIXES.tsv. The
  access (public static S vs protected K) is unproven; the landed public model was kept.
- **0x00C8DB10** is landed as `std::_Dest_val<allocator<CFlash>>`. By call site and body it is libogg
  `ogg_stream_packetout` (a two-argument forward to _packetout(os,op,1)). Check for ICF before renaming.
- **0x00C1EEE0** is landed as CItemAction::CItemAction, but the AddNumberTriggers / AddEventTrigger rows call it
  CEventTriggerRef. These are two descriptive names for one ctor.
- **0x00C2EBC0** has three placeholder spellings bound (`CAutoDeleteArray_00c3b7a0`, `CResourceItems_00c3b710::At`,
  `CAutoDeleteArray_00c2ebc0::At`). This chunk uses `PKCAutoDeleteArray<unsigned long>::operator[]`.
- **0x0143BDB0** has five spellings bound (g_AudioContext, g_audioContext, g_CStatic_..., g_animationDevice,
  g_procResolver_...). This chunk uses `g_AudioContext`.
- **Already-landed 0x00CE4B70** (a CHeroBoastsScript boast) spells BoastHobbeCaveKillPerpetrator. That name is not in
  the Ego_r PDB name set; the catalog says BoastOrchardFarmKillNoBandits. Re-check it against ControlNewBoast's
  literal/address table.
- vland RELOC_NAME warnings: the current labels conflict with spellings bound elsewhere.

  | Address | Spelling used here / bound elsewhere | Current label |
  |---|---|---|
  | 0x00C3ABD0 | `CDriverMap::Find` (bound) | CIndexMap_00c3abd0::Get |
  | 0x00C1F4E0 | `CEventTriggerRef::InsertDriver` (bound) | CItemAction::Handle |
  | 0x00C74510 | `CDspGain::ApplyRamps` (landed Apply's spelling) | Audio_IIRFilterProcess |
  | 0x00AB13A0 | `__uninitialized_copy` | _Copy_impl<CActionInputControl*> |
  | 0x00AB2CE0 | `_M_insert_overflow` | Vector_Reallocate_0x34Bytes |
- vland warns SHARED_HEADER_AVAILABLE CBaseObject on 0x00CAB470. The row uses the same local strmbase model as the
  landed CBasePin::QueryPinInfo 0x00CA8420, so move both together if engine/CBaseObject.h fits the strmbase class.
