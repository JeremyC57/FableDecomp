# Chunk 601 (2026-10-31): 5 of 10 engine-range pass-9 rows landed, 1 byte-match held for naming, 4 parked

Rows 412-427 bytes from the released ChatGPT pass 9 reservation. Landed 5, all RELOCATION_MATCH + PASS: 0x00C07420 LZO 1.08
`swd_findbest` (upstream; bound `_swd_findbest`; the landed find_match TU with swd_remove_node as an out-of-line __fastcall), 0x00CAC640
strmbase `CMemAllocator::Alloc` (older DX9 SDK text, no overflow checks), 0x00C18870 / 0x00C17BE0 PKCTRMorphable::MorphToArray /
MorphToTree (libpk; class from `.\PKCTRMorphable.cpp`, method names descriptive) and 0x00C104F0 CMemberHandler_00c08940::Handle
(the landed CItemDispatcher::RunAllItems model run twice; spelling from the landed caller's binding). Held (not landed) in
`../../pending/2026-10-27_linux_hand601/name_review/`: 0x00C313C0, byte-exact 412v412, named `CSampleTableReader::OnRecord` by the
agent with no binding — an LHAudio bank sample-table record copy (652-byte LHAudioBankSampleTable record, one-slot vtable
0x012B4D34); the coordinator holds it until the reader class names are proven (the landed 0x00C31660 carries that class name
but is, per the agent, the criteria/array reader). Parked in `../../pending/.../differ/` (4 grades each): 0x00C213C0
CSoundEvent::UpdateTrackedParameters 413v415 (first record copy through eax vs the frame temp), 0x00C49470 CCodaFrameStreamer::
SubmitFrame 415v415 (frame 0x28 vs 0x2c, descriptor store order), 0x00C19340 PKCTRRedblackTree::RemoveValue 419v419 (frame-slot
packing into the dead comparator slot), 0x00C64420 CEaxContext::SetPrimaryFXSlotID 424v426 (dirty mask in ebp vs esi + `mov ebx,esi`).
NAME_FIXES.tsv: 4 rows (CMemAllocator::ReallyFree protected; vtable 0x012BF6E0 is `??_7CMediaSample@@6B@`; 0x00C32BA0 is the
PKCTRArray ctor, not CActionParamString's; 0x00C194F0 is the PKCTRRedblackTree ctor, ~17 bound spellings) + 6 review rows (the
name_review hold; libpk descriptive names; 0x00C34600 three spellings; three usage-disputed CODA spellings; 0x00C19610; the split
PKCTR class spellings). No engine header exists for libpk / CODA / EAX / strmbase / LZO classes (not in the PDB type set); the
SHARED_HEADER_AVAILABLE CBaseObject warning on 0x00CAC640 is a name clash with DirectShow's CBaseObject.
Lessons: in a flag-plus-value block assign the value before `|=` the flag; `(mask & bit) ? &local : 0` gives the and/neg/sbb
pointer select; the libpk list/visitor drain model gives the exact EH states (two blocks → states 0..3 and 4..7); the libpk
trace-macro model matches morph rows on the first grade. The agent read GOTCHAS.md / FINDINGS_LOG.md by tail + search, not in full.
Coordinator: land_chunk.sh 5/5, bindings_multi_check 0.
