# Chunk 96 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- An override of a virtual introduced by a SECONDARY base is compiled with `this` at that subobject (no adjustor thunk when the
  slot lives only in that base's vtable): member offsets in the body are the class offsets minus the base offset. Model the
  real multiple inheritance (`struct CTCLight : CTCBase, CAppearanceListener`) -- 0x00641210 (+0x20/+0x2c = +0x2c/+0x38 - 0x0c).
- A C3DVector copied word by word through two registers into a hidden result or a stack temporary needs a MEMBERWISE user copy
  constructor even with float members (the implicit copy is a block copy through esi, or another register order) -- 0x00751B30,
  0x00C33920.
- `x = x * K / m;` is re-associated by VC7.1 to `fdiv m ; fmul K`; retail's `fmul K ; fdiv m` order needs two statements
  (`x *= K; x /= m;`) -- 0x00C2D100.
- A global and a member pointer loaded BEFORE an unrelated call whose result is added to their product are named locals
  (`long fps = g_fps; const Def* def = PDef; tick = Frame() + (long)(fps * def->delay);`); the int is parked in a frame slot for
  `fild` -- 0x007F1170. An out-of-line getter whose result is never used is an unused local (the call survives).
- The out-parameter GetInterface template may be emitted out of line when it follows a call (GetPItem) in the same expression:
  `__forceinline` it -- 0x007FB4A0.
- `__forceinline virtual ~T()` (and its inline helper) for a `??_G` whose body inlines the destructor; without it the `??_G` is
  `call ??1 ; delete` -- 0x0071A810 (24i's rule confirmed for a non-counted body).
- An implicit (undeclared) destructor of a CTCBase-derived component is the plain `??1` with no vptr reset; get it graded with
  `// Leaf: ??1Class` and a `new T(owner)` factory -- 0x004E769F, 0x004DCEDD, 0x004E33BB. In a size TU its def-pointer member
  needs `__forceinline` destructor + Release to get `dec [ecx+4] ; jne` and the `and [esi+N],0` clear.
- Retail CThing (component owner): interface bitset +0x20, CVectorMap +0x44, PTCGraphicAppearance +0x64, ThingTypeFlags +0x6c,
  PBaseDef +0x70, the Dead bitfield bit 0 of +0x91 (PDB +0x74/+0x7c/+0x80/+0xa1 minus 16: retail vector map and list are 8 bytes
  narrower each).
- A CCountedPointer deleter (block = {count, deleter, object}, called as `mov ecx,[blk+8] ; call [blk+4]`) is a `__fastcall`
  free/static function with an explicit `if (p) { p->~T(); operator delete(p); }` -- 0x008EBDD0 (hundreds of byte-identical
  bodies carry the propagated label "CTCMapwho::OnDie").
- CTCLook retail offsets: members after TurningAnims (+0x8c) sit 4 bytes below the PDB (CachedHeadPos +0xa1 unaligned,
  TimeToStopLooking +0xb4, OneFrameDelay +0xb8). CTCNymph: everything after FlightSpline 4 low, IPs packed at +0x41/+0x49.

Review at landing:
- Local base models next to an existing engine header: 0071a670 / 0071a810 (CTCMapwho ctor / `??_G`) model the class locally
  with the engine/CTCMapwho.h offsets, because the generated header is a flat POD without base, vptr or constructor; 0071a6d0 /
  0071a750 use the header through a layout-only extension `struct CTCMapwhoMethods : CTCMapwho`.
- Rows renamed away from propagated labels (finish.py takes the new names from line 1): 004e33bb CTCNymph, 004e3401
  CTCDExplosiveTrail, 008f9e60, 00851cc0, 00c0a8c0, 00c2d100, 00c33920 (module `_global`), 00c377b0, 00c44880, 008ebdd0; the
  vfunc rows 00640d00 / 00641480 / 007f23d0 carry descriptive names; all of them have `// Leaf:` lines.
- Already-landed sources with wrong labels: 0x00C44860 (landed as CTCLook::SetHeadTurnState: an engine render setting, twin of
  0x00C44880), 0x00C0A8A0 (landed as CTCLook::IsTurningBody: the bit-mask twin of 0x00C0A8C0, written as a raw `__fastcall`
  with a dummy edx parameter); 0x00C44BC0's callee 0x00C377B0 is a float maximum, not `CTarget::Receive(long)`.
- 007884f5 (parked) has a 98v96 best form that drops the CDefPointer destructor for an explicit release through a raw copy;
  the honest destructor form is 102v96 (the lane re-reads the escaped slot) -- keep the honest form if the 5.1 lane caches it.
- No row needed more than 4 grades; no row spans two functions.
