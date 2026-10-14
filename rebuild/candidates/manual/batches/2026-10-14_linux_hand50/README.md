# 2026-10-14_linux_hand50 — agent-authored chunk 50 of the class-labelled 17–96-byte pool

Chunk 50 (`mkchunk.py 50`, 40 rows, CInputManager .. CInputProcessManager) was authored by the session's single sub-agent from
`lane_tools/chunk_pool/BRIEF.md`; the lander re-graded every row: **39 of 40** landed as MATCH/RELOCATION_MATCH with behaviour
tests (compiled with the lane's cl.exe). The parked source stays in the lane's `differ/`. The agent's notes follow.


Linux VC7.1 lane (Wine + DDK cl.exe). Chunk 50 (`chunk50.tsv`, 40 rows): **39 of 40** graded `MATCH`/`RELOCATION_MATCH`
with behaviour tests that pass under g++ (64-bit host; layout checks guarded by `g_is32`) and under the lane's VC7.1 cl.exe
(`tcl.sh`, the lander's `/Od /W3` compile + link + run). One row parked in `differ/`. Grades in `grades.txt`; sources in `src/`,
tests in `test/`; `payload.json` / `oracle.tsv` from `finish.py`. No flags or pragmas were needed except the two frame-pointer
size bodies (0x0048623F / 0x00594DF4: `optimize("y",off)` + `("s",on)` in the source).

## Label corrections
- 0x00691FE0 "CInputProcessControlSpirit::Init" is **CInputProcessDebugControls::Init** (rtti-vtables: slot 2 of 0x01237F9C); it
  builds its temporary from CPlayer::GetPlayerThing @ 0x00487DD0 with the CIntelligentPointer vtable 0x01237F1C, where the real
  CInputProcessControlSpirit::Init (0x006A9180) and CInputProcessCreatureMovement::Init (0x00691D20) use GetPlayerCreature @ 0x00487DC0
  and vtable 0x01237F14.
- 0x009F4A90 `UpdateChecksum` takes `(long timeStamp, const CChecksum& checksum)` -- the record is the SECOND argument.
- 0x00AAF800 "CInputManager::EventScanStart" is a 48-byte-record retail vector push_back inlined into a one-argument member of an owner
  whose vector sits at +0x40 (`CRecordOwner_00aaf800::AddRecord`); the grow callee 0x00AAFE50 ("Vector_InsertAt") is its InsertGrow.
- 0x0048623F / 0x00594DF4 "CInputProcessGameBase::IsOtherQuickAccessItemButtonHeld" never touch a game-base member: each builds a
  0x1c-byte entry temporary (0x009E9AB0 / 0x00436560 -- the latter zeroes two bytes, two words and a CRGBColour(0,0,0,255), not a
  CDateAndTime) and calls `Set(index, entry)` on the array heading the object (0x004861F7 / 0x00594CEE bounds-check `index`
  against `(last-first)/0x1c`; the "CArray<pair<ulong,CSoundPair>>::push_back" label is propagated).
- 0x006B84B0 "AddToInterface" is a constructor of a non-polymorphic record (stores arg2 into +0, returns `this`): id +4,
  CScreenEffectPriorityStack (0x00881370) at +0x90, two flag bytes +0x78/+0x79.
- 0x00B4FC10 "AddToInterface" is CEngineShadowRenderer::SetShadowMapSize(width, height) (+0x338/+0x33c, then
  OnReleaseDefaultPoolResources @ 0x00B4B830 and InitialiseTextures @ 0x00B4F8E0).
- 0x00751590 "~CInputProcessManager" is a constructor of a 16-byte {long, CCharString, long, CCharString} record (CCharString::CCharString
  @ 0x0099E4B0 twice); 0x007E0B90 is **~CTCEffectOnDie** (vtable 0x0126FEB4, DeathSoundCriteria +0x1c, base ~CTCBase @ 0x00686830 --
  the "~CChunkedFileChunk" label is propagated); 0x00836D00 is **~CThingTrackNode** (vtable 0x01272CE4, TrackName +0xe8, base
  ~CThingGameObject @ 0x00838A00; CThingGameObject is 0xd0 bytes in retail, 0x18 less than the PDB).
- 0x00863750 "CInputProcessJumpingAndRolling::Init" is a constructor of a 0x28-byte non-polymorphic record taking an owner pointer
  (+0x24) and setting its enabled byte from the global flag byte at 0x0137573C being clear; 0x00AC0250 "Init" is an engine
  reset method (three CRGBColour(0,0,0,255) members +0x34..+0x3c, a byte +0x30, three words +0x40..+0x48; eax is not `this`).
- 0x00687DB0 "CInputProcessMain::vfunc_8" is `IsEventFromPlayerDevice(const CInputEvent&)`: a device-1 (joystick) event is accepted
  only when CPlayer::IsUsingJoystickDeviceNumber(event.GetDeviceNumber()) says so, everything else is accepted.
- Callee labels: 0x00A03B40 / 0x00A03B50 / 0x00A03B60 ("CWorld::DrawGetEnvironment") are CInputEvent::GetType (+0x28) / GetDevice (+0x20)
  / GetDeviceNumber (+0x24); 0x00487510 ("DrawGetEnvironment") is CPlayer::GetPlayerNumber (+0x228); 0x0049D9D0 is
  CWorld::GetCurrentPlayer (PlayerManager +0xc -> 0x00449970); 0x00A0D390 ("CWomanToAttract::Init") is CProcessedInput::SetHandled
  (NoGameEvents = 0, Type = 2); 0x009EA530 ("CThingFilter_IsUnconscious::operator()") is the CConsole::ProcessInput stub
  (`xor al,al ; ret 4`) and 0x009ECD80 the CConsole constructor (vtable 0x0129C600) behind the counted pointer at 0x013CAA40;
  0x00487DC0 / 0x00487DD0 are CPlayer::GetPlayerCreature / GetPlayerThing (`add ecx,2Ch ; jmp GetPItem`); 0x00A0C380
  ("Math_ComputeAngleXY") is a thiscall GetAngleXY on the 56-byte camera record CWorld slot 6 returns by value.

## Lessons
- The retail CInputManager is 0x80 bytes: the PDB's four 16-byte std::vectors are 12 bytes, so PMouse/PKeyboard sit at +0x24/+0x28,
  PJoysticks (8-byte counted pointers) at +0x2c, Loading/Saving +0x38/+0x39, CChecksum +0x3c (Checksum4 +0x48 is what
  LoadEventPackage refreshes), LoadedEventPackages (48-byte CInputEventPackage: TimeStamp +0xc) +0x4c, CurrentFilePackageIndex +0x58,
  TimeStamp +0x5c, the embedded CDiskFileWin32 +0x60 (vtable 0x0122D06C: Open slot 1, Close slot 2, Write slot 4).
- `scoped_ptr` members used as `this` of a null-tested virtual call must go through the inline `operator->` (`if (p.get()) p->f(x)`):
  a raw pointer member loads eax, tests, then copies to ecx (79 v 75); the accessor form loads straight into ecx.
- `*EventStore.begin()` through the retail vector iterator OBJECT keeps `this` in ecx and the out pointer in eax for EventScanStart;
  a raw `first[0]` makes the lane swap the two registers (60 v 58).
- An inlined `~CDiskFileWin32() { Close(); }` stack temporary reproduces the devirtualised `mov [esp+4],vtable ; call Close` followed by
  the member (~CWideString) and root (~CBase) destructors; the file name is read before the constructor pushes and the version after.
- `PCreature = CIntelligentPointer(MyPlayer->GetPlayerCreature())` -- a converting constructor `{ SetPItem(p); }` plus
  `operator=(const&) { SetPItem(o.GetPItem()); }` -- reproduces the argument fetch before the temporary's base constructor, the
  derived vtable store after it, and the base destructor with no derived vtable store (the implicit derived destructor emits none).
- The out-parameter GetInterface template (0x008CDCF0 shape) stores straight into the member when the MEMBER's address is the out
  pointer (`GetInterface(&PInventory, 0x6f)` -> `mov ecx,[eax+4] ; mov [edi+34h],ecx`); a local out pointer copied to the member
  loads through eax instead (71 v 71 on register choice alone).
- `member = C2DVector(0.0f, 0.0f)` with an inline two-float constructor is the 8-byte frame temporary copied through eax/edx (loads
  kept when the immediates are stored, forwarded from the zero register when one exists).
- A constructor that stores three zero words before the argument pointer that follows them (+0x18..+0x20 then +0x24): the words are a
  member record cleared by its inline constructor and the pointer a body assignment; plain initialisers, body stores and a member
  record holding the pointer all let the lane hoist the pointer store first (55 v 55 three times).
- CEngineShadowRenderer::SetShadowMapSize: the compiler loads the arguments in source order but stores the second first; writing
  the stores in retail order makes the lane mirror the swap (41 v 41) -- write the source in load order.
- `CRGBColour(0,0,0,255)` temporaries assigned to three members each rebuild the dword in the single frame slot (the landed D3DCOLOR
  Set order +2,+1,+0,+3) and reload it through esi/edx; the byte clear between the second and third member stores is scheduling.
- CInputProcessManager: CLinkedList<CAInputProcess> (16 bytes) with an inline zeroing constructor lands its four stores after the
  vtable store; the ten CProcessedInput records (0xb0, CGameEvent[4] through the vector constructor iterator) are an inlined
  `dec ebx ; jne` loop and NoQueuedProcessedInputs is a body assignment after it (the 0x009F1A80 pattern).
- CInputProcessJumpingAndRolling is 0x54 bytes in retail (two 4-byte STLport lists at +0x38/+0x3c, not 12-byte ones):
  StickPulled +0x4c, WFLastRoll +0x50.
- Behaviour tests on this 64-bit host: `long` and pointers are 8 bytes under g++, so sizeof/offset checks are guarded by
  `g_is32 = sizeof(void*) == 4 && sizeof(long) == 4` and hold under the lane's cl.exe; the GetInterface not-found path reads
  `last->component` (as retail does), so the test map carries a sentinel entry at `last`.

## Parked (1; source in differ/)
- 0x009F4C00 `CInputManager::IsMouseEventInQueue(long type, CInputEvent& event)` (83 v 84): the loop forms the element with
  `mov ecx,[edi+10h] ; add ecx,ebx` where retail loads first into eax and `lea ecx,[ebx+eax]`, and the found path loads first into
  eax where retail uses edi -- register choice only; raw `first[i]`, the retail vector iterator `operator[]`, a `const&` local for
  both uses (75 v 84, the local is reused for the copy) and the swapped compare / `*(begin()+i)` copy all give the same loop.
