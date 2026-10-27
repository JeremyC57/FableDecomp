# Chunk 499 (agent499, 2026-10-27): 36 of 40 single functions byte-exact

Queued 40-row chunk of single functions under 128 bytes (address order, 0x00CAF430..0x00D55DA0: DirectShow strmbase,
then quest/entity scripts and defs). One Opus 5.5 agent at high effort; 32 rows matched on the first grade.

- **Landed:** 36 rows with `vland.py --land` (verify.log: 10 MATCH, 26 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY). Nothing parked.
- **Skipped (4), rows holding several functions:** 0x00CAF430 (COM thunks plus a scalar deleting dtor at 0x00CAF460),
  0x00CAFC40 (thunks, an E_FAIL stub, adjustor thunks), 0x00CDF120 (CIsThingAlive::operator() and ::Clone at
  0x00CDF130), 0x00CE1090 (two forwarders); catalog re-split needed.
- **Identities:** 34 proven (vtable slot plus landed Alloc site plus Ego_r name for the scripts; the SDK source for
  strmbase; ghidra_out/labels_chunk499_2026-10-27.tsv), 2 probable (0x00CD23B9 CScriptGameResourceObjectScriptedThingBase::
  IsValid, 0x00CF8850 CScriptGameResourceObjectMovieBase::Release).

Label corrections (NOTES.md): 0x00CD23B9 is IsValid (landed callers use an invented IsScripting; the catalog label
C3DMeshInfo::HasPhysicsMesh was propagated); 0x008AB970 SetImplementation; 0x008AB2B0
CCountedPointer<CScriptGameResourceObjectMovieBase>::Reset; 0x00CDCA70 Transfer<EGameflowPosition>; 0x00CDCF80
TransferVector<bool>; 0x00462E78 vector<bool>::operator=; 0x00704580 CCountedPointer<CScriptThing>::operator=;
0x0049B8D0 TransferVector<CCharString>; 0x004106F0 Transfer<unsigned long>; 0x00CB0D20 CAMThread::~CAMThread; the landed
generic getter structs at 0x00D02B10 and siblings are each entity script's GetParentScript. Header debt: CScriptThing.h,
CGameScriptInterface.h and CDefClassBase.h flatten the vptr (0x00D154A0 and 0x00D1FCE0 keep local models).
