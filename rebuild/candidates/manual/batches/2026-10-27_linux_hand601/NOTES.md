# Hand chunk 601 (CSampleTableReader .. CMemberHandler_00c08940): 6 of 10 byte-exact

All ten rows are library code inside the ENGINE range: libpk (the Lionhead audio library: PKCTR containers, the sound
event, the bank readers, the event queue), GameCODA, EAX 4.0, LZO 1.08 and DirectShow strmbase. Every manifest label was
wrong (propagated game names). None of these libraries is in a PDB, so the libpk/CODA names are descriptive. Where a name
was already bound or landed, that spelling is reused (one address, one name).

## What matched (6)
- **CSampleTableReader::OnRecord 0x00C313C0** (descriptive): copies one `LHAudioBankSampleTable` 652-byte record
  (docs/formats/AUDIO.md B.2.4: id +260, waveId +264, group +320, playback params +576..) into the bank's sample map
  entry (the 0x30-byte CEntry_00c31560 from the landed CAutoMapOwner_00c31560::CreateEntry 0x00C31560), unpacking the
  param flags. It is the only slot of the one-slot vtable 0x012B4D34, registered second by the bank loader (0x00C31142),
  between the WaveData reader (0x012B4D3C) and the array reader (0x012B4D40). Shape: the landed CWaveDataReader::OnRecord
  model (key local homed in the dead record slot, inline find-or-create `Get`). In the two flag-plus-value blocks the
  value is assigned before the flag bit; with the flag first, the `or` comes before the load (grade 1 412v412 DIFFER, grade 2 match).
- **swd_findbest 0x00C07420** (proven: LZO 1.08 upstream, bound `_swd_findbest`): the landed find_match TU
  (0x00C07610) with `extern "C" { static __declspec(noinline) }` statics and a same-TU caller; the one change is
  swd_remove_node as the out-of-line landed `void __fastcall swd_remove_node(lzo_swd_p, lzo_uint)` declaration (retail
  inlines it into swd_accept but calls 0x00C07140 from swd_findbest). First grade.
- **CMemAllocator::Alloc 0x00CAC640** (proven: strmbase amfilter.cpp, DX9 SDK text): CBaseAllocator::Alloc and the
  CMediaSample constructor inlined. This is the older text, with no size/prefix overflow checks and no `length < 0` test.
  The model is the landed CMemAllocator::SetProperties one. First grade.
- **PKCTRMorphable::MorphToArray 0x00C18870 / MorphToTree 0x00C17BE0** (class from the `.\PKCTRMorphable.cpp`
  trace file; the method names describe what they do): slots 2/3 of vtable 0x012B2DEC. A null implementation and EMEM
  each emit a trace. Otherwise the code builds the PKCTRArray(0) / PKCTRRedblackTree(0), calls MoveAllItems, deletes the
  old implementation and installs the new one. The libpk trace macro model matched on the first grade.
- **CMemberHandler_00c08940::Handle 0x00C104F0** (bound spelling from the landed caller CEventQueue_00c08940::Process):
  the landed CItemDispatcher::RunAllItems 0x00C10400 model run twice. Each pass has a local typed list (CItemList
  0x012B1B78 / CVtable_012b1c40 0x012B1C40) and a key-matching visitor (CCollectFreeMatchesA/B 0x012B2614/0x012B261C)
  passed to the container's Enumerate. It then makes an unused `GetCount()` virtual call and drains the list
  (Remove, item slot 7(this), slot 8(1)). Two inner blocks give retail's 8 EH states. First grade.

## Parked for Fable 5.1 (4)
- 00c213c0 CSoundEvent::UpdateTrackedParameters DIFFER(413v415): retail copies the first GetRecord result into *pPosition from the frame temp (`mov eax,[esp+34h] ; mov ecx,[esp+2Ch] ; mov edx,[esp+30h] ; mov [ebx+8],eax ; mov eax,[esp+10h] ; mov [ebx],ecx ; mov [ebx+4],edx`); ours copies through the returned pointer in eax | tried: expression copy `*pPosition = m_params.GetRecord().m_position` (413), named block-local record (399: ebp becomes a zero register, the position pointer spills), an inline accessor returning the position (399), non-POD record with an empty user constructor (413) | next: a record type with a user copy-assignment or a CVector3 with a user copy constructor, or function-scope record local shared by both reads
- 00c49470 CCodaFrameStreamer::SubmitFrame DIFFER(415v415): frame 0x28 v 0x2c: retail gives the CDebugBreak temporary its own lowest dword ([esp+0Bh]); ours packs it below the string, so every esp offset is 4 lower, and the descriptor stores are ordered channels/flags/bytes v retail channels/rate/bytes/flags | tried: `&frame` passed directly (418), local copy of frame (415), inline ReleaseFrame helper (415), aggregate-initialised descriptor (415) | next: the descriptor as a struct with a constructor, or the trace block in its own inline function
- 00c19340 PKCTRRedblackTree::RemoveValue DIFFER(419v419): retail homes the trace string and the spliced node y in the dead `comparator` slot [esp+24h] and the CDebugBreak temporary in the `value` slot; ours puts all three in the `value` slot | tried: function-scope y (435), block-scope y + same-TU FreeAndClear (419), inline DeleteNode (305), __forceinline DeleteNode (419) | next: same-TU DeleteFixup body, a named CDebugBreak local, or lane gap: frame-slot packing
- 00c64420 CEaxContext::SetPrimaryFXSlotID DIFFER(424v426): retail computes `m_dirty | 0x4000000` in esi and moves it to ebx (`mov ebx,esi`; ebp = memcmp zero); ours keeps it in ebp (ebx = memcmp zero) | tried: `m_dirty |=` (430), local after the GUID copy (424), `|=` then read back (430), local before the copy (424) | next: `m_dirty = dirty = m_dirty | X` or `dirty` declared at function top

## Skipped
- none

## Label corrections
- 0x00C313C0 `sub_00c313c0` -> CSampleTableReader::OnRecord (libpk bank reader, descriptive).
- 0x00C213C0 `Thiscall_CreatureOpinionUpdate` -> CSoundEvent::UpdateTrackedParameters (libpk sound event; spelling bound by the landed 0x00C21A50).
- 0x00C19340 `RedBlackTree_RemoveByKey` -> PKCTRRedblackTree::RemoveValue (vtable 0x012B2F0C slot 6; trace ".\PKCTRRedblackTree.cpp").
- 0x00C64420 `CCharacter_SetActiveSkill` -> CEaxContext::SetPrimaryFXSlotID (EAX 4.0 context property).
- 0x00C104F0 `CTCHero_ProcessBlockParryStrafe` -> CMemberHandler_00c08940::Handle (libpk event queue member handler).
- 0x00CAC640 / 0x00C49470 / 0x00C18870 / 0x00C17BE0 `sub_*` -> CMemAllocator::Alloc / CCodaFrameStreamer::SubmitFrame / PKCTRMorphable::MorphToArray / MorphToTree.

## Names: proven vs descriptive
- Proven (upstream source text plus a second source): `_swd_findbest` (LZO 1.08, bound by the landed find_match), `?Alloc@CMemAllocator@@MAEJXZ`
  (strmbase, body verbatim), `?SetPrimaryFXSlotID@CEaxContext@@...` (EAX 4.0 SDK property set; landed CEaxContext family).
- Class proven by its trace file, method descriptive: PKCTRMorphable::MorphToArray / MorphToTree, PKCTRRedblackTree::RemoveValue,
  CCodaFrameStreamer::SubmitFrame. PKCTRRedblackTree: its constructor 0x00C194F0 loads `.\PKCTRRedblackTree.cpp` at 0x00C1955E.
  PKCTRArray: the class of vtable 0x012B4E80, whose destructor 0x00C32BF0 (vtable store 0x00C32C0F) loads `.\PKCTRArray.cpp` at 0x00C32C2D.
- Descriptive, reusing a landed or bound spelling: CSoundEvent::UpdateTrackedParameters, CMemberHandler_00c08940::Handle, CAutoMapOwner_00c31560::CreateEntry,
  CSoundEvent members, CItemList / CVtable_012b1c40 / CCollectFreeMatchesA/B / CItemVisitor / CListImpl, CODA calls.
- Descriptive and new: CSampleTableReader::OnRecord (no binding existed for 0x00C313C0 or vtable 0x012B4D34);
  vtable symbols `??_7CCollectFreeMatchesA@@6B@` / `??_7CCollectFreeMatchesB@@6B@` (classes named by the landed Visit rows).

## Header debt
- None of these classes has an engine header (libpk, CODA, EAX, strmbase and LZO are not in the PDB type set). All models are local
  and copied from the landed relatives named in each file.
- 0x00C213C0 bridges two landed spellings of one 16-byte record: `SEventRecord` (the 0x00C34600 getter) and
  `CUnprovenVector_00c345b0` (Set3DPosition 0x00C20E50). The model makes SEventRecord derive from the other.
