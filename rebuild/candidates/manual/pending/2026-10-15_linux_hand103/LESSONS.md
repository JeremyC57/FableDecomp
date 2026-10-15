# Chunk 103 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A `delete p` (virtual destructor) after `p->Method()` has NO null test: VC7.1 drops it once the pointer
  has been dereferenced (`push 1 ; call [edx]` straight away; without the earlier call the `test ebx,ebx ;
  je` comes back) -- 0x00A3EB60. So a bare slot-0 `push 1 ; call [vt]` is not necessarily a hand-written
  Destroy(1).
- `dec reg ; jne` with the zero result preloaded (`xor eax,eax` before the test) is a one-case `switch`
  storing into a zero-initialised result local; `case 1: return x;` splits the exits -- 0x007D5F90.
- A float member zeroed next to an integer member shares the integer zero register (`xor eax,eax ; mov
  [Score],eax`): `Score = 0.0f` and `= 0` both reproduce it -- 0x007CF4A0.
- A loop-invariant `t * t` written inside the loop condition is hoisted after the entry test and parked in
  the dead argument slot (`fld [esp+4] ; fmul [esp+4] ; fstp [esp+4]`) -- 0x008EF1B0.
- A loop walking `m_pHand->first .. m_pHand->last` must take `m_pHand` into a named local to get
  retail's register colouring (pointer edx, total ecx) -- 0x007DA8B0.
- `max((ULONG)m_cRef, 1ul)` in DirectShow strmbase is the windef.h MACRO over a plain (non-volatile)
  LONG (`cmp eax,1 ; ja ; mov eax,1`); the Dinkumware reference-returning max spills both operands --
  0x00CACD00.
- A record whose byte member is copied by push_back RELATIVE to an embedded sub-object (`mov al,[esi+0Dh]`
  after the sub-object's copy constructor) holds that byte inside the sub-object; modelling it as a
  member of the outer record moves it to +0x18 and grows the frame -- 0x0086BDE0.
- The world's member object used as `this` of a virtual call comes through an inline accessor
  (`mov ecx,[eax+34h]`), the plain member read loads eax and copies -- 0x007D4A90 (the 24a rule again).
- A 16-byte POD record assigned whole copies dword by dword through ONE register with
  `lea esi,[dst] ; mov edi,src` -- 0x0070A3E0.
- STLport list `erase(it)` inlined in a loop with a `found` flag reproduces with
  `it = Erase(it); found = true;` / `else it = it->Next;` and the end re-read per pass -- 0x0070A430,
  0x00A3EB60.
- `for_each(begin(), end(), CKillThing(flag))` with a __fastcall for_each: the functor (user constructor,
  no copy constructor) is built in the frame and pushed as a dword, the iterators (user copy
  constructor) are built in reserved slots, the non-POD functor result comes back through the hidden
  pointer in ecx into the frame's top byte -- 0x0071B200.
- SGI `__pop_heap` (/Gr) for a 16-byte pair: `*result = *first; __adjust_heap(first, 0, last - first,
  value, comp);` in a frame-pointer TU = `optimize("y",off)` + `("s",on)` -- 0x004E1D20.
- The tavern-game classes: slot 30 LeaveGame, 31 NewGame, 34 QuitMidGame, 11 DrawPrepare, 1 persist; the
  retail CTCTavernGame members sit 8 (after UIComponents) / 0x1C (after the handle vectors) bytes below
  the PDB, so engine/CTCTavernGame.h does not fit any retail body.

Review at landing:
- No row needed more than 4 grades. Two rows parked (007d3510, 007d7ec0); both share a schedule-only
  difference (null out-pointer store) that looks like a lane gap.
- Local models next to existing engine headers: CTCTavernGame.h (offsets do not fit retail, see above);
  CAIStateGroup_HoverHornetQueen.h (flat, hides PBrain in `_pad_0x04` -- 0095aa00 uses
  engine/CAIStateGroupBase.h + engine/CAIBrain.h instead); CTCPhysicsBase.h exists but only vtable
  slots are needed in 00968090/00947c30 (local vtable model).
- Already-landed `rebuild/src/compiled/00/5e/CTCTavernGame_NewGame_005e9a70.cpp` uses `field_*` names and
  a `__fastcall` free function on a local struct; with the retail shift (PDB - 8 / - 0x1C) its +0x34 is
  BaseBet, +0x40 CurrentMoney, +0xa0 NewBestScore, +0xa2 GameStarted (`CurrentMoney -= BaseBet;
  NewBestScore = GameStarted = false`) -- worth a rename pass. The landed
  `CTCTavernGameShoveHaPenny_vfunc34_007cf520.cpp` is QuitMidGame (slot 34) with GameFinished/GameWon.
- Catalog rows holding several functions: none in this chunk.
