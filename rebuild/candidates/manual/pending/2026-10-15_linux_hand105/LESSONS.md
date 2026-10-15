# Chunk 105 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A float literal passed by value as `fld [__real] ; push ecx ; fstp [esp]` and a 0.0f member store as `fldz ; fstp` mark
  a SIZE TU: under `#pragma optimize("s", on)` VC7.1 routes float constants through the FPU, the speed build folds
  them into `push 40000000h` and the shared zero register (/Op and float-returning inline helpers change nothing)
  (0x00656CAA CQuestInfoElementMiniGameBar ctor).
- A counted def-pointer read of one field (`test ecx,ecx ; je ; inc [ecx+4]` ... `dec` with no second null test) is
  `CDefPointer def(rawObject); return def->Field;` with an `if (Object) AddRef()` constructor and an unconditional
  `Object->Release()` destructor; when the value is tested afterwards, read it into a local inside its own `{ }` so the
  release precedes the test (0x005DA850..0x005DAD60 CTCWeapon getters, all first-try).
- `x = y = z = 0.0f` (chained assignment) stores z, y, x -- the reversed store order of a zeroed C3DVector; an inline
  Set(x, y, z) keeps x, y, z with VC scheduling the zero-register stores before the 1.0f ones (0x00C40410).
- Chunk rows that read past a `std::map` member of a PDB class sit 4 bytes LOW in retail (12-byte retail map): CTCWife
  LoveAttitudeValue +0x8c / ReceivedWeddingRing +0x91 / FrameLastCulledGiftsReceived +0x88 vs header +0x90 / +0x95 /
  +0x8c; CWeaponDef sits 0x0c below Ego_r from +0x50 and 0x10 below past ArrowTrails. The retail std::list is 4 bytes
  (STLport node pointer): `lea eax,[edx+eax*4]` indexes a vector of lists (0x0076E5F0).
- `if (i >= n) return false; ...` puts the false tail first; retail's false tail last needs `if (i < n) { ... } return
  false;` (0x0076E5F0); likewise `if (type == 0) return Call(t); return false;` (0x005DAD60) and `if (range != 0) return
  seed % range; return 0;` for the world-random helper when its zero case sits after the division (0x006CEFE0).
- A call whose result only feeds a member chain used as `this` (`GetDefinitions()->m_pDefs->GetSubDef(&m)`) must take the
  HOLDER into a named local (`CDefinitions* d = GetDefinitions(); d->m_pDefs->GetSubDef(&m);`): the bare chain pushes the
  argument first, a local for the inner pointer adds `mov ecx,eax` (0x006CEFE0).
- A value tested by a float compare against a def field read through a member pointer: take the def pointer into a local
  BEFORE the intervening call to get retail's edi (`mov edi,[esi+0Ch]` hoisted above the frame call) (0x006CEEA0).
- A function whose two outcomes both run the same bool call but only one uses it (`call ; xor al,al` vs `call ; test ;
  jne false ; mov al,1`) is `if (cond) { if (!Call()) return true; } else { Call(); } return false;` -- a bool local or
  result-local formulation if-converts to `neg ; sbb` (0x00EE13B0).
- `ptr ? ptr->Virt() + k : g_zero + k` with the vptr fetched into edx is the named-local-pointer + store-per-arm form;
  the one-expression ternary fetches it into eax (0x00732D00).
- A pool node destructor with own vtable first and the root vtable last is `virtual ~Node() { Unlink(); }` over a root
  with a user-declared empty virtual destructor; grade it with the `??1` leaf (0x00C4CF40).
- `x ? 3 : 0` on a masked word lowers to `and ; neg ; sbb ; and 3`; `(byte & 8) ? 2 : 0` to `shr 2 ; and 2` (0x0076EF40).

Review at landing:
- Rows that needed the full four grading attempts and matched on the fourth: 00656caa (optimize("s")), 00c40410
  (chained assignments), 00ee13b0 (night-arm guard). None needed more than four.
- Local base models next to existing engine headers: CTCWeapon ctor 0x005DA6A0 (engine/CTCWeapon.h flattens CTCBase into
  pads, so a constructor cannot derive from it); the CTCWife sources 0x006CEFE0 / 0x006CFE80 / 0x006CFEC0 use a local
  retail-offset model because engine/CTCWife.h is 4 bytes high past GiftsReceived (the header needs the 12-byte retail
  map); 0x007C6A40 CTCWillEnflame uses a local model (retail stores a long at +0x20 where Ego_r has IsZTargetting).
- engine/CTCWeapon.h has no ParentThing name (pad at +4): 0x005DA540/0x005DA5A0 read it through an accessor over
  `_pad_0x04`; regenerating the header with the CTCBase members named would remove that cast.
- Already-landed sources with wrong models: rebuild/src/compiled/00/5b/CTCWeapon_GetAnimationSpeedValue_005bd404.cpp is
  an inline-assembly body (asm_bake, not reconstruction) and CTCWeapon_IsAnalogueLoading_005bd4ce.cpp reads
  +0x68 -> +0x58 of a different class: both carry propagated CTCWeapon labels (the real CTCWeapon getters are this
  chunk's 0x005DA850 / 0x005DA8F0). rebuild/src/compiled/00/c5/CVtable012b5c10_CVtable012b5c10_00c59ba0.cpp calls the
  0x00C4CF20 base "CIndexBuffer"; check that name: the same constructor (vtable 0x012B8778) is the base of the pooled
  combat/morph actions, whose +4/+8 are the prev/next links used by Link/Unlink 0x00C4CED0/0x00C4CEF0 and the
  destructor 0x00C4CF40.
- 0x005DA8F0 (CTCWeapon::IsAnalogueLoading, def +0x90) is not in this chunk; it should take that name instead of
  0x005DA950.
- No catalog row in this chunk holds two functions.
