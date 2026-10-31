# Chunk 601 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- LZO swd_findbest 0x00C07420: retail inlines swd_remove_node into swd_accept but CALLS it (0x00C07140) from swd_findbest. Take the
  landed find_match TU and replace only the `static __forceinline swd_remove_node` body with the landed
  `void __fastcall swd_remove_node(lzo_swd_p, lzo_uint);` declaration inside the extern "C" block. It matched on the first grade.
- Flag-plus-value blocks (0x00C313C0): retail `mov ecx,[src] ; or word ptr [e+20h],dx ; mov [e+28h],ecx` comes from
  `e->m_value = src; e->m_flags |= 0x20;` (value first). The flag-first source puts the `or` ahead of the load.
- Same-TU GetMode/CheckState stand-ins keep a live `edx` (the +0xAE track word) across both GetMode calls in 0x00C213C0, not
  only `this` in ecx. Use the landed CSoundEvent stand-in pair whenever a CSoundEvent member keeps edx/ecx across 0x00C20EE0.
- `p = (mask & bit) ? &local : 0` gives retail's `mov cl,dl ; and cl,bit ; neg cl ; sbb ecx,ecx ; and ecx,eax` pointer
  select (0x00C213C0).
- A by-value 16-byte getter result copied out member-wise: `*p = obj.Get().m_member` copies through the returned eax
  pointer. A named local reads the frame slot but can reshuffle register allocation (0x00C213C0 parked on this).
- libpk drain loops (0x00C104F0): the landed CItemDispatcher::RunAllItems model (CTypedListBase(&m_impl) base init + CListImpl member,
  inline forwarders, a visitor whose base has an inline virtual dtor) reproduces retail's EH states exactly. Two sequential blocks
  give the 0..3 / 4..7 state numbering. An unused virtual call (impl slot 10) is a plain `list.GetCount();` statement.
- strmbase CMemAllocator::Alloc 0x00CAC640 is the DX9 SDK text (no lSize/lPrefix overflow checks, CMediaSample ctor without
  `length < 0`). Later SDK text adds instructions. Copy the model from the landed SetProperties 0x00CAA290.
- libpk PKCTRMorphable morph rows (0x00C18870 / 0x00C17BE0) match on the first grade with the trace-macro model
  (`{ CDebugStream s(g_pDebugBuffer); s << ".\\File.cpp" << "(" << line << ") : " << msg << "\n"; if (g_pfnErrorReport) g_pfnErrorReport(s.m_pBuffer); } CDebugBreak();`, /EHsc).
- Pairing libpk bank readers with their record types: the loader 0x00C30E90 registers reader objects {vtable, bank} in order
  (0x00C31120 WaveData 0x012B4D3C, 0x00C31142 SampleTable 0x012B4D34, 0x00C31168 array/criteria 0x012B4D40). Each reader vtable
  has one slot, and the vtables share the block that starts at 0x012B4D30 with two _purecall base vtables (0x012B4D30 / 0x012B4D38).
- Frame-slot packing residues (0x00C19340 dead-argument slot, 0x00C49470 CDebugBreak slot) survived block-scope and
  helper-function variants. They are likely lane gaps unless a sweep finds the source form.

Review at landing:
- GOTCHAS.md (617 KB) and FINDINGS_LOG.md were too large to read whole. I read the newest sections (GOTCHAS 1560-1676, FINDINGS
  874-920) and searched both for specific topics.
- NAME_FIXES.tsv: 0x00CAA3C0 ReallyFree should be protected (`IAEXXZ`; landed leaf `QAEXXZ`); 0x012BF6E0 is `??_7CMediaSample@@6B@`;
  0x00C32BA0 is the PKCTRArray constructor (landed as CActionParamString; 0x00C32B80 "CActionParamString::Set" is then a PKCTRArray
  member, probably Reserve); 0x00C194F0 is the PKCTRRedblackTree constructor (about 17 bound spellings). The PKCTRArray
  constructor's parameter type (`unsigned int` capacity) is my choice; the bound spellings use `long`.
- 0x00C31660 is landed as `CSampleTableReader_012b4d40::OnRecord`, but it is the THIRD reader (array records: the
  LHAudioBankCriteiaInfo `{tag; n; ids[n]}` shape), not the sample-table reader. 0x00C313C0 is the sample-table reader. Rename
  0x00C31660 (for example CCriteriaInfoReader) so the two spellings do not collide in meaning.
- The landed CWaveDataReader::OnRecord header says "slot 3 of the vtable at 0x012B4D30". It is the only slot of the one-slot
  vtable 0x012B4D3C (comment only; the mangled name is unaffected).
- 0x00C34600 has three spellings: the landed definer `CQuadHolder::GetValue` (leaf "GetValue"), the bound
  `?GetRecord@CSoundParams@@QBE?AUSEventRecord@@XZ` (used here) and `?GetRecord@CRecordHolder_c34600@@...`. The same 16-byte
  record is `CUnprovenVector_00c345b0` in Set3DPosition 0x00C20E50; 0x00C213C0 bridges the two by inheritance.
- 0x00C213C0 has a second bound spelling `?Bind@Object@Row00c21c20@@QAEXPAUOwner@2@@Z` (from 0x00C21C20). I used the
  0x00C21A50 spelling `?UpdateTrackedParameters@CSoundEvent@@QAEXPAUCSoundEventManager@@@Z`.
- 0x00C49470 (parked) now uses the landed spellings `?operator_new@@YGPAXI@Z` for 0x00C36080 and `_CODA_StopSource@8` for 0x00C35DD0,
  but usage disputes both: 0x00C36080 takes a buffer-descriptor pointer, not a size, and 0x00C35DD0 is called with `true` right after
  IsSourcePlaying returned 0 (possibly a play/stop switch; a second bound spelling `?RenderBuffer_00c35dd0@@YGXPAXJ@Z` exists).
  0x00C491D0 is the landed `DrawGetEnvironment` (fastcall), really the CCodaFrame +0x10 data getter.
- 0x00C19340 (parked) defines a same-TU stand-in `PKCTRAllocator::FreeAndClear` for 0x00C19610, which is landed as CTexture GetNoLevels
  (a propagated label; the body frees *pp and clears it).
- Morphable/comparator/tree class spellings are split across landed rows: PKCTRMorphable (trace file) v CMorphableNode_00c185b0 /
  CUnprovenMorphableHandle_00C185B0; comparator CUnprovenArrayComparator v PKCTRComparator_00c19010; tree PKCTRRedblackTree v
  PKCTRRedblackTree_00c18b20. CCodaPlayback_00c49420 is really slot 5 of the CCodaFrameStreamer vtable 0x012B8138.
- 0x00C64420 uses `SGuid` (the landed CEaxContext family spelling) where the EAX SDK says GUID. The mangled names follow the landed family.
- New vtable symbols `??_7CCollectFreeMatchesA@@6B@` (0x012B2614) and `??_7CCollectFreeMatchesB@@6B@` (0x012B261C) take their class
  names from the landed Visit rows 0x00C10150 / 0x00C10190. There were no bindings before.
- vland warns SHARED_HEADER_AVAILABLE CBaseObject for 0x00CAC640. That is a name collision: the strmbase model's CBaseObject is
  DirectShow's (combase.h), not the game's CBaseObject (rebuild/include/engine/CBaseObject.h, vtable 0x0129A7D8). Keep the local
  model. vland's other warnings are RELOC_NAME notices against propagated catalog labels. There are no RELOC_IDENTITY or DIFFER failures (6 WINS).
