# Chunk 128 lessons (for GOTCHAS)

- The vtable store names a constructor, not the catalog label: 6 of this chunk's 22 constructors were propagated
  labels over byte-identical shapes (DragonFireBeamInto, OpenChest, PickUpGoods, LearnExpression, NymphSummonOutOf,
  ScorpionRecover). Check `rtti-vtables.tsv` for the stored vtable before naming.
- CCreatureActionBase is 0xa8 bytes in retail (PDB 0xb4) and CCombatActionBase 0x114 (PDB 0x12c): subtract 0x0c / 0x18
  from the PDB member offsets of every CCreatureAction_* class; the generated engine headers are PDB layouts.
- `CCreatureActionBase(owner, -1.0f, speed, anim, CSoundPair(), 0, 0)`: with `speed == 0.0f` every zero stays an
  immediate (`push 0` x3 plus immediate CSoundPair stores); with a non-zero speed one register zero serves the two
  long pushes and the CSoundPair; when ebx already holds the owner the zero goes to edi and even 0.0f is `push edi`.
- A 12-byte argument copied into a constructor through ecx/edx/eax at fixed offsets is THREE scalar members
  (a C3DVector member copies through `lea ecx,[esi+N]`); a C3DVector array element assignment keeps the `lea`.
- A position raised on Z (`mov` X/Y, `fld [eax+8] ; fadd k ; fstp`) is a frame C3DVector filled field by field
  (`p.X = c.X; p.Y = c.Y; p.Z = c.Z + k;`); `C3DVector p = c; p.Z += k;` copies Z through the frame first.
- Component lookups: a component that is zeroed before the third GetPItem (`xor edi,edi`) is an out-parameter lookup
  into a local initialised to 0 before it; a nested second lookup whose flags dword is loaded (`mov ecx,[eax+28h] ;
  test ch,1`) and whose component goes straight into ecx is `unsigned long HasTC()` + a pointer-returning lookup
  (the out-parameter template parks the component in eax). The esi/edi colouring of an inlined IS_THING_ALIVE on an
  intelligent pointer matched when the pointer was bound to a REFERENCE local (0x008DBA90); the same body taking the
  pointer as a `__fastcall` parameter swapped esi/edi (0x0072ABD0, parked).
- CThing +0x60 is the physics TC: CTCPhysicsBase slot 31 = ResetPosition(const C3DVector&) (0x006B01D0), slot 36 is a
  one-bool setter whose base body is an empty `ret 4`; a named creature local makes `[eax+60h]` load straight into ecx.
- Lane gaps seen: a dead `add eax,N` left by an inlined copy constructor's reference parameter (0x008B6E60); an
  address-escaped counted local re-read after its add-ref store where retail shares one null test and keeps ecx
  (0x005EE3B3).
- Housekeeping: never run q.py with the cwd inside the scratchpad -- verify_and_land creates a literal
  `D:\Documents\FableTLC\rebuild\build\landverify` directory under the cwd.
