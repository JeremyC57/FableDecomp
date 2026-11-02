# Hand chunk 712 (CScratchBuffer .. NParticleEngine::CPSCUpdateNormal): 10 of 10 byte-exact

All ten rows grade RELOCATION_MATCH (q.py / gall.sh with `FABLE_HDR_OVERLAY=hdr`, and the read-only vland check: WINS 10, no
RELOC_IDENTITY, no DIFFER). Tests PASS under g++ (tall.sh) and VC7.1 (tall.py). Every Leaf was confirmed with `objdump -t` and every
relocation target with `objdump -dr` (the PDB spellings below). Extents: every row is followed by int3 padding and preceded by
padding or the previous function's `ret` (checked in the exe); no fragments, no two-function rows. Eight of the ten catalog
labels were wrong (only 0x009F4950 and 0x007B0300 were right).

## What matched (10)
- **CScratchBuffer::Uninitialise 0x009F4950** -- `if (IsValid()) { if (Buffer) { delete[] Buffer; Buffer = 0; } } CInitBaseClass::Uninitialise();`
  (operator delete[] import 0x00BFEB1C). CInitBaseClass modelled from the PDB: virtual dtor + `Validate() const` (vtable 0x0129A7CC:
  ??_G, Validate), public `IsValid() const`, protected `Initialise()` / `Uninitialise()` (IAE). Names: Ego_r + FableWin name sets,
  sole caller CSystemManager::Uninitialise on g_ScratchBuffer (ctor 0x009F4A00 stores CScratchBuffer vtable 0x0129C790).
- **CMeshDataBank::FrameEnd 0x00A23AD0** (label `_Dest_val<...CSymbolInfo>` wrong) -- three `CResourceBank::FrameEnd()` calls on
  MeshResourceBank / AnimationResourceBank / PhysicsMeshResourceBank (+0x170 / +0x268 / +0x2e4), the last one the tail. Names: both
  PDB sets have ?FrameEnd@CMeshDataBank@@QAEXXZ; the sole caller CEngineResourceManager::FrameEnd 0x00B3B870 loops over the
  three-pointer array at +0x10 = Ego_r `CMeshDataBank* MeshBanks[3]`. Header: engine/CMeshDataBank.h with the new opt-in
  FABLE_RETAIL_CMESHDATABANK (hdr/), CResourceBank from the new engine/CResourceBank.h.
- **NProfileTimer::`dynamic atexit destructor for 'LogFile'' 0x0122A790** (label `_Dest_val<...>` wrong) -- `_$E2` of the landed
  initializer 0x01224100's TU: the same text (inline `virtual ~CDiskFileWin32() { Close(); }`, novtable CAFile) emits both; the
  local CBaseClassNonCopyable stand-in replaced by engine/CBaseClassNonCopyable.h (: CBaseClass) and engine/CDiskFileWin32.h
  included under a layout name for the size check; `$E1` 0x01224100 still RELOCATION_MATCH from this text. Names: Ego_r 0x0101E460 /
  FableWin 0x04012710 "NProfileTimer::`dynamic atexit destructor for 'LogFile''", the atexit registration in 0x01224100.
- **C3DMeshFileEngineMaterialChunk::~C3DMeshFileEngineMaterialChunk 0x00A9D270** (label CCreatureAction_FireMissileWeapon wrong) --
  implicit dtor: MapFilenames (CIVCountedPointer<CMapFilenames>) +0x34, Name +0x30, then ~CChunkedFileSuperChunk. Shape lesson:
  the counted pointer's `Release()` must copy Object into a named local first (probe 1, reading the member twice: DIFFER(46v43)).
  Names: FableWin ??1 (0x033D42F0); sole caller ??_G 0x00AC6290 = slot 0 of vtable 0x0129EFC8 (RTTI .?AVC3DMeshFileEngineMaterialChunk@@).
- **NScript::CV_FisticuffsClubScript::CFisticuffsMember::~CFisticuffsMember 0x00E839F0** (label CQ_MinionCampScript::
  StartMinionSpawnTimer wrong) -- implicit entity-script dtor: BooName, CheerName, ShoutTimer (inline ~CTimer = pDSTGame slot 88
  DeregisterTimer), ~CEntityScriptBase; the retail ctor 0x00E83920 in the TU emits vtable / ??_G / ??1 (the landed CBriarRose
  0x00D9FED0 model). Names: FableWin 0x03A0A280; vtable 0x012EB0F8 (no RTTI: the COL slot holds code) stored only by the landed
  ctor ??0CFisticuffsMember, factory 0x00E838A0 `push 30h` = Ego_r sizeof 48, registered as "FisticuffsMember".
- **CThingPhysical::~CThingPhysical 0x006A5D50** (label CTCFishingSpot wrong) -- `GetThingManager().RemoveFromPhysicalList(*this);`,
  then POriginalCreator, PLastCreator, HealthOverTimeEffects (out-of-line ~_List_base 0x005283C0, chosen by VC7.1 from the real
  STLport text), tail ~CThing. Names: Ego_r + FableWin ??1, vtable 0x01244E84 (RTTI CThingPhysical : CThing : CClass : CBaseObject),
  callers ??_G 0x005297D0 and ~CThingGameObject 0x00838A00 (which already binds this spelling).
- **CTCFishingSpot::~CTCFishingSpot 0x007B0300** (label right) -- user dtor `{ RemoveParticles(); }` (protected, IAE), three
  CIntelligentPointer<CThing> members, tail ~CTCBase (engine/CTCBase.h retail opt-in). Names: both PDBs; vtable 0x0126D51C RTTI.
- **CTCScriptedControl::~CTCScriptedControl 0x00719C70** (label CExclusiveAssert wrong) -- implicit dtor over the real STLport list
  text: AttitudeTags (+0x20) gets the out-of-line ~_List_base 0x00718EC0 with clear inlined, PActionList (+0x14) the inline
  ~_List_base with the out-of-line clear 0x00719280, PScriptHandleImp (+0x0C), tail ~CTCBase. VC7.1 picked both splits itself.
  Names: both PDBs; sole caller ??_G 0x00719C50 (vtable 0x012658E4 RTTI).
- **CTCPhotojournalCapture::~CTCPhotojournalCapture 0x0055D590** (label CExclusiveAssert wrong) -- user dtor
  `{ RemoveFromGlobalTCs(); }`, then the STLport vector<NUISystem::CComponent*> (+0x2C), FileName (+0x28), Description (+0x0C), tail
  ~CTCBase. RemoveFromGlobalTCs was added to engine/CTCBase.h's retail branch (hdr/). Names: both PDBs; vtable 0x0124C4B4 RTTI.
- **NParticleEngine::CPSCUpdateNormal::~CPSCUpdateNormal 0x00B0CA90** (label CExclusiveAssert wrong) -- user dtor
  `{ KillAllDecalEmitters(); }` (protected, IAE), DecalEmitterList (+0x9C, inline ~_List_base, out-of-line clear 0x00B0F630),
  DecalEmitterName (+0x14), tail ~CBaseClassNonCopyable (CPSCBase / CParticleUpdateParamCallback implicit, the landed CPSCAttractor
  chain). Full Ego_r member list (bit-field words, floats, C3DVectors) at retail offsets, sizeof 0xA0 asserted. Names: both PDBs;
  vtable 0x0129FC1C RTTI.

## Parked for Fable 5.1 (0)
None.

## Skipped
None.

## Names and evidence
Every row's name has two sources (NAME_FIXES.tsv evidence column, and the per-row header comments). No descriptive names were
needed. Callee spellings used (all PDB, each checked in reloc_bindings.tsv; stand-ins replaced):
?IsValid@CInitBaseClass@@QBE_NXZ 0x0099A330, ?Uninitialise@CInitBaseClass@@IAEXXZ 0x0099A360, ??_V@YAXPAX@Z 0x00BFEB1C,
?FrameEnd@CResourceBank@@QAEXXZ 0x009FC430, ?Close@CDiskFileWin32@@UAEXXZ 0x0099A920, ??1CWideString@@QAE@XZ 0x0099B510,
??1CBaseClassNonCopyable@@MAE@XZ 0x0099A300, ??1CCharString@@QAE@XZ 0x0099EAE0, ??1CChunkedFileSuperChunk@@UAE@XZ 0x00AA8490,
?pDSTGame@@3PAVCGameScriptInterfaceBase@@A 0x0143E8F8 (ESTABLISHED_SPELLINGS), ??1CEntityScriptBase@@UAE@XZ 0x00F35B40,
?PCurrentThingManager@CThing@@0PAVCThingManager@@A 0x013B8A1C, ?RemoveFromPhysicalList@CThingManager@@QAEXAAVCThingPhysical@@@Z
0x0051DFE0, ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10, ??1CThing@@UAE@XZ 0x004C8310, ??1CTCBase@@UAE@XZ 0x00686830,
?RemoveParticles@CTCFishingSpot@@IAEXXZ 0x007B01C0, ?RemoveFromGlobalTCs@CTCBase@@QAEXXZ 0x00686DF0,
?KillAllDecalEmitters@CPSCUpdateNormal@NParticleEngine@@IAEXXZ 0x00B0C730, the STLport list members 0x00719280 / 0x00B0F630
(landed spellings) and 0x00718EC0 / 0x005283C0 (new: `??1?$_List_base@...` -- the catalog's `StdList_Clear_*` labels are wrong,
both bodies end by freeing the sentinel and each has a `jmp` ~list thunk, 0x007191A0 / 0x00528C40).

New spellings introduced for addresses with no prior binding: 0x00718EC0 and 0x005283C0 (above; vland warns RELOC_NAME against the
catalog labels, which are the wrong ones). No new spelling collides with a landed row's spelling. Second spellings for already-bound
addresses (the PDB form replacing a stand-in): 0x0099A360 (IAE vs bound UAE/QAE), 0x007B01C0 (IAE vs the landed QAE leaf),
0x00B0C730 (IAE vs bound QAE) -- respell per NAME_FIXES.tsv.

## Label corrections
- 0x00A23AD0 `_Dest_val<...CSymbolInfo>` -> CMeshDataBank::FrameEnd; the landed caller 0x00B3B870 calls it CResourcePool_00a23ad0::Release.
- 0x0122A790 `_Dest_val<...CSymbolInfo>` -> NProfileTimer::`dynamic atexit destructor for 'LogFile''.
- 0x00A9D270 CCreatureAction_FireMissileWeapon::~ -> C3DMeshFileEngineMaterialChunk::~ (implicit).
- 0x00E839F0 CQ_MinionCampScript::StartMinionSpawnTimer -> CV_FisticuffsClubScript::CFisticuffsMember::~ (implicit); its ??_G
  0x00E839D0 is landed as CLandscapeBackgroundPatch `vector_deleting_destructor` (catalog CVtable_012eb0f8).
- 0x006A5D50 CTCFishingSpot::~ -> CThingPhysical::~.
- 0x00719C70 / 0x0055D590 / 0x00B0CA90 CExclusiveAssert::~ -> CTCScriptedControl::~ / CTCPhotojournalCapture::~ / CPSCUpdateNormal::~.
- 0x00718EC0 / 0x005283C0 `StdList_Clear_*` -> `_List_base<...>::~_List_base`.
- 0x009FC410 (catalog `_Cons_val<...>`, bound as CLoadingQueue_00a23b00::Process etc.) -> CResourceBank::ProcessLoadingQueue(bool, double).

## Header debt
- engine/CMeshDataBank.h: the opt-in branch only types the banks and adds FrameEnd; the vptr is still a data member. The retail
  class is CMeshDataBank : CBankFileAsync : CBankFile (vtable 0x0129CE94 RTTI) and needs engine/CBankFile.h (HOLD) plus a
  CBankFileAsync header.
- engine/CDiskFileWin32.h: flattened (vptr data member, no CAFile base). A retail branch needs CAFile (novtable : CBaseClassNonCopyable)
  and the 16-slot vtable 0x0122D06C named from the PDB set (Clone, Close, GetFileInformation, GetLength, GetPathname, GetPosition,
  IsOpen, MovePosition, Open, Read, SetLength, SetPosition, Write, IsIOFinished, GetAlignment ...); slot-by-slot evidence was not
  gathered here, so 0x0122A790 keeps the landed local model (sized against the header under a layout name).
- engine/C3DMeshFileEngineMaterialChunk.h, CChunkedFileSuperChunk.h, CChunkedFileChunk.h: flattened; 0x00A9D270 uses the landed
  sibling's local chain (sized against the material-chunk header under a layout name).
- engine/CThingPhysical.h: flattened; no engine/CThing.h exists. 0x006A5D50 keeps CThing's own members as one opaque block
  (+0x14..+0x98) over engine/CClass.h's retail opt-in, sized against CThingPhysical.h under a layout name (the landed 0x00838A00
  pattern).
- No engine header exists for CScratchBuffer, CInitBaseClass, CTCFishingSpot, CTCScriptedControl, CTCPhotojournalCapture,
  NParticleEngine::CPSCUpdateNormal / CPSCBase / CParticleUpdateParamCallback, CBaseIntelligentPointer / CIntelligentPointer<T>,
  CIVCountedPointer<T>, CCharString / CWideString: local models (PDB members at retail offsets, sizes asserted where known).
