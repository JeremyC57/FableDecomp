# Chunk 520 (agent520, 2026-10-28): 19 of 25 re-queued single functions landed (up to 90 bytes)

Re-queue chunk of 25 rows up to 90 bytes (0x00BFAC70..0x00F3FBE0): landscape meshes, the libpk sound library (event
manager, instances, audio objects, ambient triggers), libogg / libvorbis, and quest-script classes. One Opus 5.5 agent at
high effort, the first under the owner's "names right the first time" rule.

- **Landed:** 19 rows with `vland.py --land`, every test PASS under g++ and VC7.1; no RELOC_IDENTITY, CONFLICT or DUPLICATE.
- **Held (3, pending/2026-10-27_linux_hand520/held/):** 0x00C28380 GetPointerFromHandle, 0x00C28640 ValidateResources,
  0x00C28690 Free match byte-for-byte, but libpk links its own copy of CHandleBasedMemoryPool (vtable 0x012B4098, handle
  table at +0x440) whose PDB names are already bound to the engine pool (0x00A026B0, 0x00A02AD0, 0x00A02B30). One address,
  one name: the libpk copy's class name is a family decision for the naming pass.
- **Parked (1, differ/):** 0x00C52C40 CCpuInfo::GetCyrixModelName (75v75, register choice only).
- **Skipped (2):** 0x00BFEF20 and 0x00F3FBE0 are `__finally` funclets of the CRT eh vector destructor / constructor
  iterators, not functions.

Name fixes (NAME_FIXES.tsv, 17 entries): 0x00CBE512 is GetDistanceBetweenThings (its body ends in `fsqrt`) and 0x00CBE583
is GetSquaredDistanceBetweenThings (no `fsqrt`; FableWin.pdb order Dist / DistThingAndPosition / Squared /
SquaredThingAndPosition). Every landed caller had them swapped; 13 caller rows were respelled and rechecked (14 rows
RELOCATION_MATCH, tests PASS). 0x00AC1F60 is CIndexBuffer::OnPostRelocation (Ego_r donor order), not OnRelocated (2
callers respelled). The libpk respellings (address-suffixed classes at 0x00C38850, 0x00C442F0, 0x00C62440; 8 spellings of
0x00C20720; ~40 of 0x00F35B30 IsActiveThreadTerminating) are deferred to the naming pass.

Label corrections (NOTES.md): 18 propagated manifest labels were wrong, e.g. 0x00C10C90 is
CSoundEventManager::GetPhysicalPlayer (not CInputProcessRebootGame::ProcessInput), 0x00E376D0 the CV_AssassinAttacksScript
ctor (not CInputProcessManager), 0x00E7DFE0 `CPredicate_And<CIsThingAlive,CIsVariableFalse>` `??_G`.
