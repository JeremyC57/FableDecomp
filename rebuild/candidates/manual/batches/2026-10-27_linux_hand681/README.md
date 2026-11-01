# Chunk 681 (NA024, 257-512 band) -- 9 of 10 landed (2026-11-01)

Nine RELOCATION_MATCH rows landed (`verify.log`; bindings multi-check 0), most at the first probe: seven quest/entity-script implicit
destructors proven by the slot-0 ??_G -> vtable -> ctor -> Alloc -> registration-string chain (S_QRET, S_VEM, S_QMC, S_LGWM, S_QRV, S_VB;
CFishCompOrganiser 0x00E7F210 is an entity script whose factory 0x00E7EEF0 is stored by CV_FishingCompetitionScript::Main next to
"FishCompOrganiser"), plus the user destructors ~CEnginePrimitiveManagerRepeatedStaticMeshes 0x00B999F0 and ~CEngineSurfaceCompositionManager
0x00B2D1B0 (RTTI COL chain + Ego_r). Four labels were wrong (0x00E7F210, 0x00E24C80 placeholders, 0x00DA2F90 "CV_FisticuffsClubScript" is
CQ_MinionCampScript, 0x00E46A50 "CSystemManager" is CV_BordelloScript: its vtable has no RTTI and the body walks a script layout), three lacked
the NScript nesting, and two propagated ctor labels (0x00E398F0, 0x00E7EF70) were corrected too: `ghidra_out/labels_chunk681_2026-10-27.tsv`.

Decisions at landing (coordinator, under the owner delegation):
- 0x00994B40 is landed as `CCreatureActionBase::GetFramesRemaining` (a propagated twin label of a two-instruction body). It sits in the
  data-stream code block, is called on CMemoryDataOutputStream objects (here by ExecuteProgram right after PeekData 0x00995E70), one landed
  binding already spells it `?GetLength@CMemoryDataOutputStream@@QBEKXZ` and Ego_r has that symbol: the real GetFramesRemaining is the
  donor in the CCreatureActionBase block. Respell queued (c681) with the file rename for the next naming wave, which also rechecks its callers.
- The nine slot-0 ??_G stand-ins (seven `CLandscapeBackgroundPatch_vector_deleting_destructor` twins), the seven script ctors with
  void*/unnested parameters, Alloc 0x00CF91C0, DeallocateAligned 0x009F2B20, PeekData 0x00995E70, the STLport clear/_M_erase rows: queued c681.
- Header debt (CLevelScriptBase restated locally without a dtor; flattened CEnginePrimitiveManagerRepeatedStaticMeshes / CEngineSurfaceCompositionManager /
  CTextureManager / CResourceList / CEngineComponent headers) stays with migration wave 10.

Parked: CParticleProgram::ExecuteProgram 0x00AE1A90 (DIFFER 296v292, 21 probes): VC7.1 gives the empty `__false_type` tag passed to
`_M_insert_overflow` its own stack slot (frame 14h vs retail 10h) where retail reuses the live `simulation` argument slot; two register choices in
cases 1/2. Next: find what makes VC7.1 place an empty temporary on a live argument slot (landed 0x00B1B380 places it on a dead one). Best source
in `../../pending/2026-10-27_linux_hand681/differ/`. NAME_FIXES.tsv: 31 rows (queued c681). Lessons in LESSONS.md.
