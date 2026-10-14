# 2026-10-14_linux_hand49 — hand-authored chunk 49 of the class-labelled 17–96-byte pool

Chunk 49 (`mkchunk.py 49`, 40 rows, CIPhysicsFacesSegmentBase .. CInputManager) was authored by the session coordinator on the
Linux VC7.1 lane; the lander re-graded every row: **38 of 40** landed as MATCH/RELOCATION_MATCH with behaviour tests (compiled with
the lane's cl.exe). One row is a cold block of another function (skipped), one is parked in the lane's `differ/`. The lane notes follow.


Linux VC7.1 lane (Wine + DDK cl.exe), authored by the coordinator. Chunk 49 (`chunk49.tsv`, 40 rows): **38 of 40** graded
`RELOCATION_MATCH`/`MATCH` with behaviour tests (g++ and the lander's cl.exe); one row skipped (not a function), one parked.
Sources from `gen.py`, tests from `gen_tests.py`, `payload.json` / `oracle.tsv` from `finish.py`.

## Label corrections
- 0x00A446FA "CIngredientOwner::Init" is a 31-byte cold block of the function at 0x00A446CE (`mov [ebp-4],-1 ... jmp 0xA446CE`): not a
  function, skipped.
- The nine "CIcicleData::CIcicleData" rows are unrelated constructors: 0x0045A0EE is NUISystem::CUILocaleGraphicsDef (vtable 0x01235344);
  0x0078E4A0 / 0x00793A80 / 0x00806AE0 are copy constructors of intelligent-pointer records; 0x008E2E00 a default one; 0x00C44580 derives
  from the landed CAIStateGroup_CreatureUseCoverBase (vtable 0x012B7B88); 0x00CDB9E0 / 0x00E7E110 are no-RTTI CBase / CVtable_012c1648
  subclasses (vtables 0x012C2748 / 0x012EA478; the latter is the chunk-48 constructor's derived class).
- 0x008E7E10 "CIconText::ClearIcons" counts matching 8-byte icon entries against a required number (`HasEnoughIcons`); the matcher
  label "CAIStateGroup_VersusScorpionKing::FindChargeEvent" (0x008E8A10) is propagated.
- 0x0098C560 "CIndexBuffer::GetSize" is a COM-style `__stdcall` DrawText-in-RECT forwarder through the object at +0x1d4 (slot 94).
- 0x00C4E2A0 / 0x00C912F0 "CIndexBuffer::CIndexBuffer" are a no-RTTI flag constructor (vtable 0x012B87F4) and a text-buffer reset.
- 0x0070C050 "CInitRumble::CInitRumble" is a setter (eax is not `this` on return).
- The "CInputEvent::SetAsKeyboardKeyHeld" label covers six different kinds (1, 2, 3, 15, 16 and the mouse wheel 14); the
  "SetAsJoystickPosition2" label covers kinds 0x11..0x15 (two positions, three buttons).
- 0x00412970 / 0x006E5390 "CInputManager::EventScanStart" are retail vector push_back bodies for 28- / 44-byte records; 0x007FD3F0
  "UpdateChecksum" copies two 3-vectors and a word.
- 0x00A0AA70 ("$E2") is an empty `ret 8` stub called from CIndexBuffer::Lock on the render statistics object (system record +0x60 -> +8).
- The deleting destructor 0x0099A490 is 55 bytes (the chunk TSV length 64 includes padding).

## Lessons
- A `??_G` deleting destructor needs its vtable pulled into the TU: a `new T` factory (as the landed CLHFile model), not a `delete`
  helper (a `delete` through the vtable emits no vtable and the extractor grades the helper instead).
- COM interfaces (Direct3D, the font object): `virtual ... __stdcall` slots with `this` pushed; `CreateIndexBuffer` is slot 27,
  `Lock` slot 11, `GetDesc` slot 13 of the index buffer; the system record (0x009A4EC0) holds the render device at +0x60 -> +0x58.
- A call result consumed by a chain (`GetDeviceRecord()->a->b->Method(arg)`): the lane pushes the argument BEFORE the call; retail
  calls first, loads the argument, then walks the chain = `record = GetDeviceRecord();` then the chained call (0x00AC1FB0).
- The mapped pointer of `Lock` is a local homed in the dead `size` parameter slot (the lane does it too).
- An end pointer re-read each iteration in the lane = take it into a local before the loop (0x008E7E10); the early-return `return false`
  before the body gives an inline `xor al,al ; ret`, the `if (...) { ...; return true; } return false;` form shares the tail (0x00A5FAC0).
- A member zeroed through its address register (`mov [edi],0` after the vtable and another store) = a body assignment while the other
  member is an initialiser (0x00AC2010).
- Two 4-byte retail vector push_backs (28- and 44-byte records): `new (last) T(value)` gives the null-checked `rep movsd`, the grow path
  `InsertGrow(last, value, SScalarTag(), 1, true)` homes the tag in the dead argument slot (as the landed PushBackConstant family).
- Double-to-float narrowing stores (`fst`/`fstp` pairs) and the device/kind/time words of CInputEvent reproduce from plain assignments
  in retail store order; a 16-bit key is `*(unsigned short*)&m_key`, the joystick position overlays +0 as a C2DVector.
- Ring-buffer index re-reads after stores through indexed members / out-pointers are natural (no CSE across possible aliases).
- A size-capped `jae` compare of a vector size against a count = `Size() < (unsigned long)count` with a `long` count.

## Parked (1; source in differ/)
- 0x00E7E110 `CVtable_012ea478(long a, long b)`: retail stores +0x44 (a) then +0x40 (b) before the derived vtable; the lane stores them in
  member order (+0x40 then +0x44) for an intermediate novtable base (initialisers or body), two stacked intermediate bases, an inline
  setter and derived-body assignments (53 v 53 otherwise identical) — store-order gap.
