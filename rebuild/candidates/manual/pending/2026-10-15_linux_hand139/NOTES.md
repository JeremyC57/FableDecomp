# Hand chunk 139 (CFadeinSummoner .. CGameScriptInterface): 36 of 40 byte-exact

All 36 sources in `src/` grade MATCH / RELOCATION_MATCH under the Wine lane with no flags (one `#pragma optimize("s", on)`:
00450bff), see `grades.txt`; every test prints PASS under g++ (`tall.sh`) and under the lane's VC7.1 cl.exe (`tall.py`).
Several engine headers are PDB layouts that disagree with the retail bytes (CFontDataBank 0x64 high, CGameCameraManager 0xc/0x18/0x24
high depending on the member, CGameScriptInterface PlayerManager at +0x14 vs retail +0xc); those classes are modelled locally with the
PDB member names at retail offsets. CGameEvent and CGamePlayerInterface headers agree with retail and CGameEvent's is included.

## What matched (36)
- **CFeatDef** -- operator= 00f2d200 and copy constructor 00f2d270: memberwise in declaration order over the 0x28-byte
  CDefClassBase (base op= 0x00431F10 / copy 0x0046644C out of line); the copy constructor's vtable store follows the base call.
- **CFileInstaller** (PDB names, retail = PDB - 4 after the 12-byte CArray, - 0x20 after the 8-byte lists) -- Pause 009c31e0 (volatile
  Paused/Running as the landed Unpause; explicit lock.Unlock() before SetEvent + WaitForSingleObjectEx), GetPercentComplete 009c32c0
  (the quotient must divide by the MEMBER TotalSizeOfAllFiles so it is computed before the lock destructor; two locals let VC7.1 sink
  it after the call), AllocateBuffer 009c3a60 (STLport `*(begin()+i)` with an unsigned `size()` loop), InitBuffers 009c5310 (inlined
  GFAllocateFromGlobalAllocator, the block homed in the dead chunkSize slot).
- **CFirePoint** -- SetAsBurning 00780540 (the CThing emitter getter is a zero-initialised local assigned under the bit test --
  a ternary adds a `jmp`; `PEffect = CIntelligentPointer(effect)` temp assignment), copy constructor 007813d0 (inline
  CIntelligentPointer copy constructor = base ctor, vtable, SetPItem(other.GetPItem())).
- **CFishCompOrganiser / script entities** (slot model of the landed CBriarRose::DoSpeech) -- SpeakConversationLine d3ace0 (no
  argument, FireHeart code -- label suspect, see corrections), SpeakConversationLine(CCharString) e80cf0 (by-value string destroyed by
  the callee), CheckForFish e81a60 / e81ad0 (frame-byte `found` flag; the IsDone check before `found = true` returns TRUE in e81a60 and
  FALSE in e81ad0).
- **CFixedDolly::OnInitLeaf** 0087b6f0 -- out-parameter GetInterface(0x3f, uninitialised out pointer) then
  `(c->GetKind() == 1 ? c : pFallback)->GetCentre()` where pFallback is a never-assigned pointer sharing the key's frame word: the only
  form that gives retail's `mov ecx,esi ; je ; mov ecx,[esp+8]`.
- **SGI __pop_heap instantiations** (all labelled with propagated names) -- a462a0 (C3DSkeletalBoneMorph, fastcall compare fn),
  a701d0 (pair<CCharString,C3DHelperPoint>), a34420 / a71b40 (pair<key, {CCharString, CCountedPointer}>, 16 bytes), cbbad0
  (pair<CCharString, CCountedPointer<CEntityScriptBindingBase>>). Lesson: C3DVector with a memberwise user copy constructor but no
  operator= gives the block copy in the assignment and the word-by-word copy into the argument area; the by-value value's
  CCountedPointer destructor keeps its two zero stores (object then block).
- **CFontDataBank::CloseFonts** 00a633a0 -- `while (!empty())` over back()/pop_back with `delete entry` = bare virtual deleting
  destructor; Filename = "".
- **CFrameData::StopLoading** 009fdc20 -- the unfinished case is an `else return false;` arm (an early `if (!f()) return false`
  returns the call's zero al inline); inline CCountedPointer reset; inlined Sizeof.
- **CGameCameraManager** (retail offsets, PDB names) -- UpdateCreatureControlAngleXY 00697760, SetGameScriptedCameraMode 00697b70
  (by-value CCountedPointer: op= then the parameter's destructor), PreparePendingMode 00698840 (the map iterator must be a NAMED local --
  chaining off find() uses eax), PushCurrentManualMode 006988b0 (CDefPointer local copied again into the by-value argument, release
  tail-jumps Destroy), ReleaseScriptedControl 00698950, IsObservedThingInMelee 006994f0, UpdateObservedThingInBuilding 0069b650,
  UpdatePrepareFlags 0069b6f0 (the subtitle query is a named bool taken before the module tests).
- **CGameDefinitionManager::~CGameDefinitionManager** 00450bff -- size TU, no virtuals, four CDefPointer members with
  `__forceinline` destructors (`if (p) { p->Release(); p = 0; }` -> `and [edi],0`), base destructor tail jump.
- **CGameEvent::InitFromCompressedBuffer** 009f1870 -- the Replacement bool must be the FIRST declared local (else the player byte
  stays in a register and the stores reorder); size widened once into a long.
- **CGameEventPackage::AddEvent** 009f1650 -- implicit 0x28-byte struct copies (`rep movsd`).
- **CGameMap::CGameMap** 006b7760 -- the resize fill value is a `CGameMapCell()` TEMPORARY of a cell with an empty user constructor
  (frame byte 1, `lea [esp+0Dh]`); a named uninitialised local lands at frame byte 0.
- **CGamePlayerInterface** -- SavePacketEvent 00445a60 (the loader's mouse device as a named local fixes an eax/edx swap),
  GetControlMovementFromGameActionEvent 00445c30 (UpperBound first; the input type object a named local per entry).
- **CGameScriptInterface** -- GiveHeroGold 00435000 (retail PlayerManager +0xc; GetInterface 0x8e), Rest 006e71f0 (float end frame,
  `while` with the test duplicated at the bottom).

## Parked for Fable 5.1 (4)
- 00d9fe40 CFadeinSummoner::GetAlphaChange DIFFER(112v112): block placement only -- retail keeps the shared `fld [0.0f] ; ret` in the `<= 0` arm and the `> 0` arm's IsDone jumps back into it; the lane keeps it in the later arm | tried: `> 0` arm first, `<= 0` arm first, `!IsDone()` first-arm return, one shared trailing `return 0.0f` | next: lane gap: tail-merge block placement
- 00f37030 CGPUScratchBufferTextureManager::FreeMemoryPage DIFFER(146v140): retail overlays push_back's empty SScalarTag on the LIVE page parameter slot (`lea edx,[esp+18h]` and `[esp+1Ch]` both = &page, no frame); the lane gives the tag its own `push ecx` word and re-stores ebx into the page slot | tried: const& push_back (page reloaded every pass), a local index copy (137v140), by-value push_back (146v140, body otherwise identical) | next: lane gap: empty tag temporary over a live argument slot (wave 24l)
- 0069a920 CGameCameraManager::RemoveCombatCameraMode DIFFER(116v104): the local CDefPointer destructor -- retail `mov ecx,[esp+4] ; test ; je ; dec [ecx+4] ; jne`, the lane `mov eax,[slot] ; ... mov esi,[eax+4] ; add eax,4 ; dec esi ; mov edx,esi ; test ; mov [eax],esi`; also the local's zero store before the index load (retail: after `push eax`) | tried: member decrement, local-copy decrement, pointee-side Release(), CDefPointer::Release() | next: a CDefPointer whose destructor calls an out-of-line-looking helper, or lane gap: def-pointer frame-slot release
- 0069ac90 CGameCameraManager::SetCameraManagerSetDef DIFFER(136v124): the same local CDefPointer destructor shape and zero-store order as 0069a920 (the operator= part matches) | tried: as 0069a920 | next: as 0069a920

## Skipped
None (no row spans two functions).

## Label corrections
- 00a462a0 / 00a701d0 "CFlammableDef::operator=" -> `__pop_heap` of the C3DSkeletalBoneMorph (compare fn) and pair<CCharString,C3DHelperPoint> heaps (callees 0x00A46040 / 0x00A6FA20 carry the `_Adjust_heap<...>` names).
- 00a34420 / 00a71b40 / 00cbbad0 "CGameCameraManager::PushCameraMode" -> `__pop_heap` (callees 0x00A33A70 / 0x00A71220 / 0x00CBB4D0 `_Adjust_heap<...>`); the 0x00A33A70/0x00A71220 manifest element type pair<key, CCountedPointer> (12 bytes) does not fit the 16-byte stride: the element is key + {CCharString, CCountedPointer}.
- 00d3ace0 "CFishCompOrganiser::SpeakConversationLine" takes no argument and sits in CQ_FireHeartScript code (callers 0x00D3A5DA / 0x00D3A7F3); a FireHeart entity with a long id at +0x20 (CFH_Prisoner / CProphetCage shape), not the fishing organiser. Kept under the catalog name.
- Callees: 0x0049D870 "GFGetPreMainMemoryUsed" is the game-frame getter (`mov eax,[0x013B89BC]`); 0x004C73D0 "CVertexBufferWin32::DoSizeof" is CThing::GetPosition; 0x00449960 "Catch@0040d959" is CPlayerManager::GetMainPlayer; 0x00487DD0 "_Cons_val<...>" is CPlayer::GetControlledCreature; 0x00416225 "CWorld::DrawGetEnvironment" is CMainGameComponent's frame getter; 0x00A69AD0 "CViewport::ClearHandle" is called on a CManagedTexture; 0x009C5110 "~CPatchTesselationEdgeStrip" is CFileInstaller::FreeBuffers (already noted when landed); 0x00F36F40 is CGPUScratchBufferTextureManager::FreeTextureSlot; 0x00A0C380 is CCamera::GetAngleXY.
