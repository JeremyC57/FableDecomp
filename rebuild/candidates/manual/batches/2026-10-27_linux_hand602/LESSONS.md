# Chunk 602 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- VC7.1 calls `operator new` (??2) for `new T[n]` unless `void* operator new[](unsigned int)` is declared in the TU; declare it
  when retail calls ??_U (0x00BFEB22).
- A `__fastcall` call whose first register argument is `ptr + member` schedules differently when that sum is a named local: the
  named-local form loads the member early (into a callee-saved register or ahead of the pushes), the inline form loads edx first
  (0x00C1E300, 0x00C26090).
- VC7.1 homes locals whose address is taken (call out-words, temporaries) in dead argument slots (`lea edx,[esp+4Ch]` = an arg
  slot); this is not a sign of a by-reference parameter.
- An inline member on a sub-object (`m_ring.Advance(n)`) makes VC7.1 keep `&m_ring` and spill `this`, freeing ebx for a loop
  index (0x00C891A0).
- Unused-local (symbol-count k) sweeps did not move the x87 cofactor order of CMatrix3x4::Invert; spend no more than one sweep.
- libpk vtable 0x012B33F0 (sound-sample reader) has six landed class names across its slots, ctor and dtor; name new slots by the
  vtable (CVtable_012b33f0) until one name is chosen.

Review at landing:
- libpk CHandleBasedMemoryPool copy (0x00C28380..0x00C28690) vs the engine copy (0x00A026B0..0x00A02B30): same PDB spellings on two
  addresses (vland RELOC_CONFLICT on 0x00C26090; the landed 0x00C283C0 / 0x00C283E0 definers already collide). Pick a libpk scheme.
- 0x00C1D320 parked over the 4-grade budget (about 5 strategies + a 14-compile k sweep); land with 0x00C1D1C0.
- CStreamBuffer rows (Attach / AddUsers / Release / RemoveUsers) call the manager CPanel and +0x15 m_bLooping; the sweep1 name is
  CSoundEventManager and +0x15 is the compressed flag (set from the sample's slot 4, tested before ReadChunkExtent). The bound
  `?Release@CStreamBuffer@@QAEXPAUCPanel@@@Z` spelling therefore changes too.
- 0x00C28610 landed as CHandleBasedMemoryPool::FreeHandle; the PDB has the private InternalFreeHandle(long) that Free calls
  (0x00C28690, bound FreeAllocation, is the libpk Free).
- The landed Initialize 0x00C5F9E0 declares the IMA ADPCM converters with 6 parameters; retail Encode*/Decode* take 7 (`ret 14h`).
- CDspGain::Apply 0x00C74CE0 has two bound spellings (`...PBMPAMHI@Z` and `...PBMPAM_NK@Z`); 0x00C891A0 uses the definer's HI form.
- CParticleSimulation::WriteToFile is const in the PDB (QBE); the landed 0x00AE2050 is non-const.
- CMatrix3x4::Invert, CReverbDiffuserStage::Process, CStreamBuffer::FinishLoading, CVtable_012b33f0::Read and
  DecodeSoundData_00c34630 are descriptive names (libpk, no PDB/RTTI).
- The landed CQS_ScytheInfoScript::PlayCutscene 0x00E2A900 uses RunScriptedCutscene / CCutsceneCastMap::clear /
  CCutsceneArgMap::clear (NAME_FIXES rows for 0x00CBFB7D, 0x00CD3699, 0x009AACE0).
