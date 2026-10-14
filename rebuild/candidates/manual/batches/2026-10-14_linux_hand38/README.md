# 2026-10-14_linux_hand38 — agent-authored chunk 38 of the class-labelled 17–96-byte pool

Chunk 38 (`mkchunk.py 38`, 40 rows, CEngineVSConstantLayoutLightsBones .. CEnvironment) was authored by the session's single
sub-agent from `lane_tools/chunk_pool/BRIEF.md`; the lander re-graded every row: **38 of 40** landed as MATCH/RELOCATION_MATCH
with behaviour tests (compiled with the lane's cl.exe). Parked sources stay in the lane's `differ/`. The agent's notes follow.


Parked (best source in differ/):
- 0045647d CEntitySoundDef ctor (4 attempts): 52v52, only the `lea ecx,[esi+48h]` of the SoundIdentifier CCharString constructor sits
  after the second vector-map's three word stores in retail (just before its flag byte); the lane hoists the lea above them. Flat record,
  nested CArray record and optimize("s") all schedule it the same way. Lane gap: lea scheduling across a run of register-zero stores.
- 004ebba3 CObjectAugmentationsDef ctor (4 attempts): 73v73, the same gap -- retail emits `lea ecx,[esi+58h]` (first def-string map
  constructor) after all six vector zero words, the lane after the third; init-list zeroing, body zeroing, /Oa and /GS identical.

Label corrections (manifest labels on this chunk are mostly propagated):
- 004ebba3 is CObjectAugmentationsDef::CObjectAugmentationsDef (vtable 0x01243974), not CEngineVSConstantLayoutLightsBones.
- 00c3baa0 / 00c3baf0 are the libpk auto-map entry constructors (keyed / default; the EH-framed factory @ 0x00C31560 calls 00c3baa0),
  not CEngineWaterBackgroundSubPatch.
- 006b5b80 / 006b5d80 are destructors of two classes holding an array of three 0x18-byte buffer pairs (+0x1a8 / +0x1ac), not
  CEngineWaterBackgroundSubPatch::OnPostRelocation.
- 0085b090 is a `__fastcall` delete helper for a non-polymorphic record {CCharString +4, CBaseIntelligentPointer +0x14, malloc'd buffer +0x1c};
  the CEngineWaterRenderer attribution is wrong.
- 00c558a0 / 00c558f0 / 00c55940 / 00c56250 / 00c56590 / 00c73380 are CAudioContext EAXSet/EAXGet wrappers (listener GUID @ 0x012BEE90,
  source GUID @ 0x012BEEA0); 00c56290 is CAudioContext::CloseDevice (alcCloseDevice). None belong to CEngineWaterRenderer.
- 00b51f00 is the snow twin of StartRaining (StartSnowing: +0x5cc/+0x5d1, 1024.0f); both take ONE float (`ret 4`).
- 00b424d0 FrameStart: retail layout differs from the Ego PDB (12-byte CArrays): StaticMapFile +0x34, FirstWorldUpdate +0x1a5, ChangedAreas +0x1b4.
- 00a0bac0 is CSoundBankLH::CSoundBankLH(long) (vtable 0x0129C9DC); 005177e0 is CMemoryFile::CMemoryFile() (vtable 0x0122E0FC);
  00ab3520 is some other CCharString-headed record (named by address). Not CEntityScriptInfo / CEntitySoundDef.
- 00c592d0, 00cb1ac0 are void Clear/Reset members (no `this` return); 00cb1d00 is the constructor twin; 00c8f080 / 00c954a0 are
  `__fastcall` libpk init functions returning 0; 00c96140 is a `__fastcall` code-table WriteCode(index, stream) -> length. None are "CEntry::CEntry".
- 00b6d6a0 SetEffectQuality, 00b6d480/00b6d4d0/00b6d420: retail CEngineWaterRenderer offsets are 0x34 (and 0x44 for the tail) below the PDB's.

Shapes learned (for GOTCHAS):
- A vtable-fetch shared by two if/else arms (`mov edx,[ecx]` before the branch, `push imm ; call [edx+N]` in each arm) is an if/else whose
  arms each reach the object through the SAME inline accessor chain; a pre-computed local keeps the vtable load inside each arm, a ternary
  float-immediate argument builds a stack temporary.
- A zeroed member word stored AFTER the derived vtable while its neighbours are stored before it (00a0bac0): the late ones are a member
  object with an inline constructor, the early ones plain initialisers (body assignments get hoisted exactly like initialisers).
- A bool member assigned `a && b` is a full-register 0/1 (`mov eax,1`/`xor eax,eax`) with the byte store tail-duplicated.
- `fld [esp+4]` before `push esi` and `fld [esp+8]` after it are the SAME single float argument (`ret 4`).
- A C runtime import called through `call dword ptr [__imp_x]` must be declared `extern "C" __declspec(dllimport)` (calloc @ IAT 0x01440058)
  without including the CRT header that declares it.
- A member array of class objects with inline destructors is destroyed last-to-first by an inlined loop (`lea esi,[end] ; mov edi,n ;
  mov eax,[esi-N] ; sub esi,size ; ... dec edi ; jne`).
- Naming a plain-record member in the initialiser list (`SoundMap()`) value-initialises it: the DDK zero-fills the whole record first.
