# agent52 — hand-authored chunk 52 of the class-labelled 17–96-byte pool (CItem leftovers .. CLandscapeBackgroundLoadTask)

Linux VC7.1 lane (Wine + DDK cl.exe). Chunk 52 (`chunk52.tsv`, 40 rows): **37 of 40** graded `RELOCATION_MATCH`/`MATCH` with
behaviour tests (all 37 PASS under g++ and under the lane's cl.exe via `tcl.sh`); three parked. Sources from `gen.py`, tests from
`gen_tests.py`, `payload.json` / `oracle.tsv` from `finish.py`. 34 rows matched on the first grade, 3 on the second; no flags or
pragmas were needed by any matched row.

## The chunk's shape
- CItem leftovers: 0x00C62500 is `CHandleOwner_00c62500::RemoveHandle` (the sibling of the landed 0x00C62460 PlaceAt: the handle at +8
  taken into a local before the test, slot 22 of the table from 0x00C62440); 0x00CB9C20 / 0x00CBA520 are the copy constructor and
  assignment of a 0x10-byte counted record (two words, object, count block) — the landed 0x00A7FAF0 form with two leading words.
- CJoystick / CKeyboard: retail layouts differ from the Ego_r headers — the maintained-event list is a ONE-WORD VC6 Dinkumware head
  (CJoystick: list +0xd0c, NoStoredEvents +0xd10, NoEventScansRunning +0xd14, 0xd18 bytes; CKeyboard: +0x3404/+0x3408 counters, list
  +0x340c, 0x3418 bytes; CJoystickDX: device +0xd38, Acquired +0xd3c, InputManagerDX +0xd40 — the header's +0xd40/+0xd44/+0xd48 are
  8 bytes off). The list node holds `{ long; CInputEvent }` at +8 (event at +0xc, kind +0x28, time +0x2c).
- DirectInput bodies: IDirectInputDevice8 COM slots 2/7/8 (Release/Acquire/Unacquire), IDirectInput8 slot 4 (EnumDevices with
  DI8DEVCLASS_GAMECTRL = 4, DIEDFL_ATTACHEDONLY = 1, the callback 0x00AB6B10 and `this`); CSystemManager (0x009A4EC0) `IsApplicationActive`
  @ 0x009A57B0 and the +0xa byte getter 0x009A4FB0 (labelled "GetCanItemBeQuickAssigned") drive `SetHasInputFocus(true/false)` as two
  virtual call sites (`if (a && b) Set(true); else Set(false);`).
- Two `??_G` with a vtable (CJoystickDX 0x0129EA7C slot 9, CKeyboardDX 0x0129EA60 slot 0): `__forceinline virtual ~T()` over a base
  with a declared out-of-line virtual destructor (0x009E4420 / 0x00A66990) and a `new T` factory.
- Eight "CLandscapeBackgroundLoadTask" `??_G` rows are non-polymorphic classes with a `delete` helper (leaf `??_G<cls>@@QAEPAXI@Z`): a
  VC6 Dinkumware tree (0x004AE7B0: erase the root through the landed 0x004AE490, relink the head, free it), two POD-vector pairs
  (0x007F3F20 / 0x0090BA50, reverse member order), two `delete[]` buffers (0x008413F0; 0x009A0630 with a 31-bit count bitfield),
  three global instance counters with two flag bytes (0x00A205C0), a handle-pool record (0x00A8D490, CHandleBasedMemoryPool::Free).
- 0x00BF63E0 / 0x00BF6410 are CLandscapeBackgroundLoadTask's plain destructor (`??1`, agent53's 16-byte model: `delete m_pOwner` through
  slot 0, ReleaseRequest when issued) and `ReleaseFileBlockLoad`.

## Label corrections
- 0x00A667C0 "CJoystick::EventScanStart", 0x00A668E0 / 0x00A66ED0 "CJoystick::AddMaintainedEventsToEventStore" are CKeyboard's (the
  +0x3404/+0x340c layout, 256 events); 0x009E4230 / 0x009E4360 are the CJoystick ones (64 events).
- 0x00AB4830 "CJoystickDX::UpdateAcquired" is CMouseDX::UpdateAcquired (slot 14 = CMouse::SetHasInputFocus @ 0x00A673A0), 0x00AB6100 is
  CKeyboardDX::UpdateAcquired (slot 3), 0x00AB6D60 the CJoystickDX one (slot 16 = 0x00AB6D10 SetHasInputFocus).
- 0x00C57040 "CJoystickDX::~CJoystickDX" stops (IDirectSoundBuffer slot 18) and releases a DirectSound buffer at +4 and clears +0x10: a
  sound-buffer owner's release method, not a destructor (the real CJoystickDX destructor is inlined into 0x00AB7950).
- 0x009FDBA0 "CKilledThing::ModifyHeroStats" loads a volume graphic (0x009FD9B0 with GetEntryDataType @ 0x009CC210 == 2); 0x00C783B0
  mixes a channel block (+0x58 of the argument) into a 4- and a 2-wide record (0x00C77D90 twice, `__cdecl`); 0x008F5BF0 / 0x008F5C20 are
  the real twins (SetMorality @ 0x0057C2E6 of hero +0x28 plus def +0x28, AddRenown @ 0x0057D8BE of def +0x38).
- 0x009F7D10 "CKinematic::operator=" is an unoptimised retail list `Insert(where, value)` (BuyNode @ 0x009F7AD0, the iterator
  constructor @ 0x0041C780 labelled "__EH_epilog3").
- 0x00ADD160 / 0x00ADD190 CLHFile vfunc_0 / vfunc_2 are `GetSize(unsigned long*)` and `GetMode()` (0 -> 0x200, 1 -> 0x800, else 1) over
  the counted file data (+4: mode +4, size +0x10, open byte +0x19); 0x00AAB7A0 / 0x00AAB7D0 CLHFileManager vfunc_1 / vfunc_0 are
  `new CLHFile(this, spec)` with the two constructors 0x00ADD380 / 0x00ADD4A0 (labelled "CPlayerGuiDef_LoadAnimationSet" / "PlayerGuiDef_ctor").
- 0x00557BD0 CKeyRedefiner::CancelSelection: the class has two bases (a root vptr at +0, a GUI control at +4); the selected redefiner is
  the global 0x013B8AC8; CFrontEndManager::GetInstance @ 0x0041E5F2.
- 0x00D430F0 CKillBird::Init / 0x00EB7960 CLadyGrey::Init call a wide script interface at +4 (slots 70/597 and 517/477).
- 0x00A66880 CKeyboard::UpdateMaintainedEvents: 0x0143FDFC is the `timeGetTime` import (held in ebx), 0x01266300 = 0.001f.
- 0x004AE7B0's tree node is `{ ?, parent +4, left +8, right +0xc }` (the landed FreeTree 0x004AE490 is `_Erase`).

## Lessons
- A COM `__stdcall` virtual call on a member interface and a `this` push reproduce from `virtual ... __stdcall` slots (as chunk 49).
- `if (!focus) { if (Acquired == true) ... } else { if (!Acquired) ... }` puts the lose-focus arm first (`test cl,cl ; jne` to the acquire
  arm, `cmp cl,1 ; jne` in the fall-through) — the `if (focus)` form swaps the arms (0x00AB6D10).
- A second virtual call's vtable fetched BEFORE the first call (`mov edx,[ecx] ; mov eax,ecx ; mov edi,[eax] ; call [edx+..]`) is the plain
  nested form `m_p->B(&m_rec, m_p->A())` — VC treats the vptr as invariant and re-reads the member for `this` (0x00D430F0).
- Two call sites of the same virtual with the tail duplicated after each (`SetHasInputFocus(1/0)` then two calls and `mov al,1`) is an
  inline `UpdateAcquired()` helper followed by the calls: VC7.1 duplicates the short tail (0x00AB69F0).
- A byte store between the pushes of a COM call is a plain `m_flag = true;` statement before the call (0x00AB6DE0, 0x00EB7960).
- `(float)timeGetTime() * 0.001f` gives `fild` + the 2^32 add for negatives (unsigned) + `fmul [const]` with the import pointer hoisted
  into ebx; the `dllimport` declaration is enough (0x00A66880).
- The retail vector `push_back`/tree/list family: `if (m_size != 0) { Erase(head->parent); head->left = head; head->parent = 0;
  head->right = head; m_size = 0; } if (head) free(head);` reproduces the head re-reads (0x004AE7B0); `delete[] p` inside
  `if (p) { ...; p = 0; }` keeps the clear before the join and lets the following member clears sink past the flag test (0x008413F0).
- VC7.1 DOES reuse a member value across a call when it is still in a callee-saved register: `GetHeader()` (= m_pHeader) after a call
  became `push esi` with no reload in one 0x009FDBA0 variant — so a retail re-read after a call means the value was NOT kept (register
  pressure), not that the compiler refused to CSE.

## Parked (3; sources in differ/)
- 0x0051C760 `??_G SVectorPair_0051c760` (74 v 72): retail addresses the object vector's destroy-range tag at `[esp+8]`, the ??_G's
  still-live `flags` slot; the lane allocates a frame byte (`push ecx ; lea eax,[esp+7]`). A tag returned by an inline helper, a POD tag
  (adds a zero store) and `#pragma optimize("s")` (out-lines the destructor, 28 bytes) do not change it.
- 0x009F7D10 unoptimised list `Insert` (94 v 94): retail's frame is 0x1c (three untouched dwords between the named locals and the `this`
  spill at [ebp-1Ch]); the lane's is 0x10. Unused pointer locals declared before/after/with the named ones always land ABOVE the node
  slot (the call-result local sinks to the bottom), so the gap's origin is unknown.
- 0x009FDBA0 volume-graphic load (61 v 61): the pre-call order matches with an `entry` local taken before the bank; after the call retail
  loads the header (ecx) before the size argument and uses `sete dl`, the lane loads the size first (`sete al`/`sete cl`). Four forms tried.
