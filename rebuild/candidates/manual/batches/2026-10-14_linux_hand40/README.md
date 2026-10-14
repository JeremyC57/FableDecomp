# 2026-10-14_linux_hand40 — hand-authored chunk 40 of the class-labelled 17–96-byte pool

Coordinator-authored (no sub-agent) on the Linux VC7.1 lane. Chunk 40 (`mkchunk.py 40`, 40 rows,
CFileInstaller .. CGameCameraManager): **34 of 40** landed as `RELOCATION_MATCH`/`MATCH` with behaviour tests.

## Label corrections (manifest labels are propagated across identical bodies)
- 0x009C7150 "CFileInstaller::CFileInstaller::RemoveRange" is `std::vector<CFileInstaller::CBufferChunk>::erase(first, last)`.
- 0x004E1722 "CFireheartMinigameDef" is `CRumbleDef::CRumbleDef` (vtable 0x0124273C); 0x00692120 / 0x00692270 are the
  `NCreatureAction::CActionEventTriggerParticle` / `CActionEventAddObject` constructors (vtables 0x0125C0D8 / 0x0125C178).
- 0x008B64D0 "CFirstPersonInit::Alloc" is the `__fastcall` factory of `CCombatSequence_WillPhysicalShieldNonMelee` (vtable 0x0127A3B4).
- 0x00C2D1E0 "CFontBankStateBlock::CreateCRC" sets the bank-update status on two 0x180-byte CBankFile members.
- 0x004AECF0 "CFrameData::ResetTexture" is a constructor of a non-polymorphic 0x20-byte record (named by address).
- 0x00A69980 "CGPUScratchBufferTextureManager" is `CManagedTextureData::CManagedTextureData` (vtable 0x0129DC24).
- 0x00A5F110 / 0x00A5F1C0 are the `CStreamingFontDataBankEntry` constructor / AcquireFont (vtable 0x0129D5B4).
- 0x00A635D0 / 0x00A5F0A0 ("CTCInventoryAbilities::GetAbilityLevel") are the font banks' entry accessors (`[bank+120h][index]`).

## Lessons (also in GOTCHAS, wave 24j)
- A byte cleared BEFORE the derived vtable store while the string members follow it: the byte belongs to an intermediate
  `__declspec(novtable)` base whose inline constructor runs between the out-of-line base constructor and the vtable store
  (a plain initialiser of the derived class is scheduled after the vtable store in this module).
- `_Destroy_range(first, last, alloc)` called with `lea eax,[esp+N] ; push eax` where N is a dead argument slot: the allocator
  parameter is a `const&` bound to a default-constructed temporary (the temporary lives in the dead slot).
- A member read for `this` of a virtual call that is re-read from memory for each arm (`mov ecx,[esi+14h]` twice) is a POINTER member.
- Two constructor arguments loaded before `operator new` = the object is built by an inline helper (arguments evaluated first).
- A counter re-read after the null test for the push (`mov eax,[esi+74h] ; test ; je ; mov ecx,[esi+74h] ; push ecx`) is `volatile`.
- `optimize("s")` on a double-narrowing body makes the lane copy the double through `fld st(0)`; the retail module is not size-optimised.
- A C runtime `__ehvec_ctor` / `__ehvec_dtor` unwind funclet (`cmp [ebp-1Ch],0 ; jne ; push ... ; call __ArrayUnwind`) is not a function
  the grader can select: two such rows (0x00BFEF20, 0x00F3FBE0) are parked as CRT SEH funclets.

## Parked (6; sources in the lane's `differ/`)
- 0x00BFEF20, 0x00F3FBE0: CRT `__ehvec_dtor` / `__ehvec_ctor` `__finally` funclets (CRT source + SEH; no named symbol).
- 0x0049B9A0 `CFrameRateSmoother::CalculateSmoothedTime`: 60 v 59 — retail reads the count into eax, spills it and copies it into
  esi inside the shrink-wrapped loop block; the lane keeps it in esi from the entry (register choice; five shapes tried, the
  `double` sum and the do/while form are right).
- 0x0087B690 `CFixedDolly::UpdateDesiredPos`: 89 v 87 — the C3DVector temporary's copy into the hidden return slot: retail loads
  X/Y into ecx/edx before the third add, the lane copies the hidden pointer and loads after it (four operator+ shapes tried).
- 0x009E2CE0 `CFontManager::GetFreeVertexBuffer`: 30 v 31 — retail reloads the vector's first pointer after the `div`; the lane keeps
  it in esi (and saves edi) (four shapes tried).
- 0x00A63630 `CFontDataBank::CFontDataBank`: 75 v 75 — the member state block's `lea edi` / vtable store / `mov ecx,edi` order inside
  the inlined member constructor (four shapes tried).
