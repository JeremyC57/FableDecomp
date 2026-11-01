# Chunk 681 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 2026-11-01 (chunk 681) Destructor labels in the quest-script range were right 5 of 9 times. The four wrong ones were two
  CParticleEmitter placeholders, CV_FisticuffsClubScript and CSystemManager. A dtor label naming an RTTI engine class at a
  0x00E4xxxx address is suspect: the slot-0 ??_G's vtable 0x012E38B0 had no COL, and its ctor -> Alloc -> "S_VB" chain gave
  NScript::CV_BordelloScript (0x00E46A50).
- 2026-11-01 (chunk 681) An Alloc that loads its two arguments in the other register order (`mov edi,edx ; mov esi,ecx`,
  0x00E7EEF0) and has no registration-string caller is an entity-script factory. Its xref lands inside the parent quest's
  Main, where `mov [esi+10h],<factory>` sits beside `push "<EntityName>"` (CFishCompOrganiser, 0x00E7E513).
- 2026-11-01 (chunk 681) Quest/entity implicit dtors with STLport `vector<CCharString>[3]` arrays, CCharString[10]/[5],
  `list<CScriptThing>` and `map<CCharString,CCharString>` all matched on the first probe from the chunk 665/672/673
  template. A member array of STLport vectors gives retail's stack-counter loop (`mov [esp+10h],3 ; ... dec [esp+10h]`),
  with no EH vector iterator (0x00E24C80).
- 2026-11-01 (chunk 681) VC7.1 keeps STLport `_List_base::clear` out of line even when its _list.c body is defined in the TU
  (0x00DA2F90 -> 0x00CD4DF0). Define it, as retail's text does, rather than leaving a bare declaration.
- 2026-11-01 (chunk 681) An inline intrusive release written on the member (`if (FrameData) { if (--FrameData->ReferenceCount
  == 0) delete FrameData; FrameData = 0; }`) re-reads the member after the decrement and keeps delete's null test. Retail's
  CGraphicFrame::UnInitialise goes through a local `CGraphicFrameData* data = FrameData;` (as landed 0x00B2EEC0). That local
  takes ~CEngineSurfaceCompositionManager 0x00B2D1B0 from 383v374 to RELOCATION_MATCH.
- 2026-11-01 (chunk 681) A switch whose retail jump table covers cases that all branch to the end of the switch had real
  case bodies that VC7.1 removed only after switch lowering. Empty `case N: break;` labels are folded into the default and
  give a compare chain. Calls to inline empty member functions (the FableWin-only
  CParticleSimulation::SatisfyMinMaxDistance / Unbend / SphereCollisionConstraints) keep the 0..5 table and retail's register
  assignment (0x00AE1A90).
- 2026-11-01 (chunk 681) FableWin.pdb's STL member names show which container calls a body makes: CArray::push_back,
  vector::empty QBE, vector::back QAE, pop_back and clear for ExecuteProgram. A non-const back() inside a const member means
  the container is `mutable`. Ego_r lacks those inline members.
- 2026-11-01 (chunk 681) vland SHARED_HEADER_AVAILABLE fires for every locally defined class with an engine header. Include a
  trivially compatible header (engine/CPixelFormat.h, a single long). For a flattened header, write the `// Layout:
  engine/X.h ...` line next to the local restatement (CTextureManager).

Review at landing:
- 0x00AE1A90 needed 21 probes and is parked (differ/00ae1a90.cpp, residue in NOTES.md). It is the only row over 4 grades.
- NAME_FIXES.tsv has 31 rows. Nine are own rows: the four wrong catalog labels, three unnested, and the two engine rows whose
  landed ??_G callers use stand-in callee spellings. Nine are the slot-0 ??_G stand-ins: seven
  `@CLandscapeBackgroundPatch_vector_deleting_destructor@12`, one free fastcall and one `ScalarDeletingDestructor` member.
  Seven are the landed ctors of these classes (void* / CGameScriptInterface* parameters, some unnested; PDB:
  CScriptDataBase*, CGameScriptInterfaceBase&). The rest: Alloc 0x00CF91C0 (`_global::Create_*` stand-in) and callees
  0x009F2B20, 0x00995E70, 0x00994B40, 0x00B2EA90, 0x00B2EF20.
- 0x00994B40 is landed as CCreatureActionBase::GetFramesRemaining (`mov eax,[ecx+0Ch] ; sub eax,[ecx+8]`). Ego_r has a real
  ?GetFramesRemaining@CCreatureActionBase@@QBEJXZ, but at a donor address in the CCreatureActionBase block. Retail 0x00994B40
  sits in the data-stream block and is called on CMemoryDataOutputStream objects: respell to GetLength and re-check its other
  56 callers.
- Local models next to engine headers: CLevelScriptBase (implicit dtor). Also the flattened engine headers listed under
  NOTES.md "Header debt".
- TU emitters: each quest/entity row defines the PDB ctor with an empty / init-list body. That is not retail's body; only the
  dtor is graded.
- No row runs past its function end and no manifest split is needed. 0x00AE1A90's 292 bytes include its case table.
