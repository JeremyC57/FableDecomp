# Naming pass wave 11 (2026-10-30): 173 rows respelled from the post-wave-10 NAME_FIXES queue

Inputs (brief `../briefs/BRIEF_W11.md`): the 201 NAME_FIXES_QUEUE rows from chunks 586-593, migration waves 6-8 and the asm
re-author pass (QUEUE_586_593_mig6_8_asmre.tsv). Every spelling checked against both PDB name lists; `_W` lands as `G`.
Sub-waves, each with its own recheck (logs/recheck_sub_*.log): mig8 67 rows (the 55+3 entity classes now nested as
`NScript::<Script>::<Entity>`; CQuestInfoElementTimer; the SetPredicate fix; CMurderTwistSheriff private members; 7 queue
spellings belonged to other bodies and were corrected from the vtable slot-1 factory registration strings: CDW5_Bandit,
CChestHidingAssassin, CTH_SnowTroll, CFollower, three CDummy rows), asmre 32 (CompileDefs SI, CStartupDefinitionManager::Get,
GetDef<T>, GFAllocEngine, ~CEngineInit, GetCurrentGameTime / GetPredictedTimeSinceLastRenderFrame IBE, NInventory::CItem,
SwapToHero, CTCBase::GetPlayerManager, CTBaseSingleton<CConsole>::Get, CConsoleCommandParameters(EGenericVarType),
CFrontEndManager::GetInstance, CManager::GetInstance, the NUISystem::CIME family), chunks 586-593 55 (CAudioSound ->
CAudioObject in 8 rows + the 0x00C596E0 base; the CEngineScene render family; the enum-keyed map operator[]; the 6
CHeroBoastsScript rows nested in NScript over CScriptBase.h, 0x00CE4B70 = BoastHobbeCavePerpertratorKilled; the CBankProcessor
CString::operator<< rows; 0x00C355E0 one spelling; CActivationReceptor part dtors; 00c54bd0's callee is CAudioRingBuffer::Clear;
ImeUi allocator globals + three ImeUi defining rows; NParticleEngine GetSystemTransform / GetCurrentPosition(Local) /
CParticleUpdateParameters; vector<C3DGroup2>::reserve, CArray<C3DGroup2>::push_back, public `_M_fill_insert`; libpk
PKCDefaultFileManager / PKCTRRedblackMap / QueueChunk / CRingBuffer / PKCTimerManager::InitiateShutdown), wave 10 DEFERRED 19
(CalcObjectMatrix 4, GFIntToCharString / operator+, MoveToPosition 7, IsValid 5, GetMainPlayer 0x005E3EA0, GetMainPlayerNumber
0x008FD0A0). DECISIONS_W11.tsv covers all 201 queue rows: applied 83, applied with corrected spelling 7, name-only 1
(asm_bake 005bd993 GetHighlightedItemMaxInCategory, body untouched), partly 9, no change needed 57, decision 3 (GConsole*
keep retail's fastcall spelling), open 41 (mostly libpk / STL names with one source).
Renames (RENAMES.tsv, 29 rows / 58 files, applied with git mv + build_candidates.ps1): the 6 boasts, CIME and ImeUi rows, the
scene rows, the MurderTwist rows, 00dcf4c0 CFarmRearEntrance, 005bd993.
Bindings: 94 retired, 57 added, 18 kept (wrong spellings still emitted by unedited rows outside the queue: CIMECalls
accessors, `?Get@CConsole@@SAPAV1@XZ`, `??6CDebugStream@@QAIAAU0@PBD@Z` in 58 libpk rows); multi-check 0. Pre-existing
RELOC_CONFLICT warnings on 00401b80 and 00e48a30 were in the baseline. Labels: 37 proven
(`ghidra_out/labels_naming_wave11_2026-10-30.tsv`); 35 catalog names corrected.
Agent recheck 173/173 PASS (5 MATCH, 168 RELOCATION_MATCH) from the composed overlay. Coordinator: overlay applied with no
overlap against the library-landing commit; full recheck of the 174 changed rows 173/173 PASS (the 174th is the asm_bake row 005bd993, name-only, not compiled; coordinator_recheck.log).
Process note: the row edits reached FullDecomp inside the chunk 594 queue commit (a `git add -A` before this recheck finished);
this records commit carries the result.
Deferred (DEFERRED.tsv, 19 items with plans): 00b33010 CEnginePrimitiveRenderer::Render (arguments do not match the PDB
camera / clip-volume parameters); the CUnknown PBD/PBG swap (identical bodies, nothing decides the order);
CHandleBasedMemoryPool::Free (one name over two retail addresses needs a coordinator convention); ~CTimerHost; the
CString::operator<< and CCharString copy-ctor mega families (scoped only); the CArray->vector _M_insert_overflow model;
GetDesc; AudioObject_* wrappers still on struct CAudioObject; CConsoleCommandBase_Retail; data names with no PDB name
(0x013B7CD8, 0x013B8760, 0x0143E8C0, 0x013AC74C); leftovers outside the queue for GetCurrentGameTime, CConsole::Get,
CConsoleCommandParameters, CIMECalls, CWorkerThread_00c48640; 11 const-GetMainPlayer rows; the rest of wave 10's list.
