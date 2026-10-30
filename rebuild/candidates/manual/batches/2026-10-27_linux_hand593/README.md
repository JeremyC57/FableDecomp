# Chunk 593 (2026-10-30): 9 of 10 engine-range released pass 9 rows landed

9 RELOCATION_MATCH, all PASS (g++ and VC7.1). Proven: NParticleEngine::CParticleEmitterList::Insert (private AAE; both
PDBs + its only caller), four STLport `_M_fill_insert` instantiations (C3DBoundingSphere, CParticleEmitterInfo,
CEngineLightingManager::CThemeChannel, pair<EPSCType,CComponentInstance>) from their bound resize callers and callees,
`__chunk_insertion_sort` / `__rotate_adaptive` over CEngineSubPrimitiveRenderer::CSortElement (STLport 4.6 verbatim),
LZO 1.08 `swd_init` (same-TU static chain). Descriptive: CSoundEventManager::FindFirstEntryState (class by vtable 0x012B2748
slot 17; libpk has no PDB). Every chunk label except swd_init was wrong.
Parked (../../pending/2026-10-27_linux_hand593/differ/): 0x00C1A870 PKCDefaultFileManager::ReleaseFile (325v321): retail homes
the trace CString in the dead `void* value` out-slot that Find wrote, VC7.1 gives it its own slot (frame 0x14 v 0x10, knock-on
register moves); five source forms tried; looks like the chunk 589 lane gap (short-lived EH locals sharing one dead slot);
next idea: move the trace block into an inline assert helper.
NAME_FIXES.tsv: 12 rows (STLport helper placeholders -> exact std:: names incl. five-argument `__copy_backward`, `fill<T*>`,
`uninitialized_fill_n<T*>`; 0x00B4A2C0 QAE not IAE; 0x00AEB190 Insert AAE not InsertSorted; 0x00C196A0 CObjectMap_00c196a0::Find
for five caller placeholders; 0x00C119D0 CSoundEventManager slot 18 GetEntryState; 0x00C1AE50 `?Find@PKCTRRedblackMap@@QAEJPBKPAPAX@Z`).
Review items: 0x00C16750 is bound three ways (CDebugBreak / CAssertTrace / CTrace); the open-file record's +4 word is
`long m_pendingRequests` in QueueRead but tested unsigned here and traced as a reference count; 0x00BDF600 carries two catalog
names (check linker folding before picking).
Header gaps: CEngineLightingManager.h has no nested CThemeChannel; CParticleEmitter.h is flattened and global (cannot name
Priority +0x40 / DistanceToCam +0x154); CEngineSubPrimitiveRenderer.h has no nested CSortElement (SHARED_HEADER_AVAILABLE
warnings on 0x00B85B90 / 0x00B85CD0 but including it would redefine the class); none for CParticleEmitterInfo,
CComponentInstance, CCountedPointer, C3DBoundingSphere, libpk, LZO.
Lessons (LESSONS.md): `fill` over a refcounted element needs a same-TU noinline body in `_M_fill_insert` (else the release
splits, 330v318); where retail keeps the empty tag (dead `x` slot v own frame byte) decides whether the tag-taking helpers
need bodies; `__rotate_adaptive` copy chains must be `__forceinline` (151v320 with plain inline); a same-TU `extern "C"
static noinline` chain with a dummy caller gives swd_init retail's private registers; a named local for a nested call
argument gives retail's evaluation order (0x00C11AC0); copying a `void*` out-local into a typed pointer keeps the record in
a register.
