# Chunk 2017 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A same-TU callee that only READS a local's address must be visible for retail's scheduling of that local even when the call stays
  out of line: with the landed CCreatureAction_DragonBurninateInto ctor 0x009537B0 defined in the TU (noinline), IntoGroundSwoop
  0x00951430 stores the flame end point component by component (fld/fadd/fstp X, Y, Z) and loads ahead in the flame start copy;
  declaration-only gives the Z-first "compute three, pop three" sum (a captured local). Measured first in a 10-line probe.
- `p = a + b + C3DVector(0.0f, 0.0f, 8.0f)`: VC7.1 folds the +0 terms but keeps the intermediate's Z in a frame word and re-adds 8
  (0x00951821 `fstp [esp+5Ch]` ... `fld [esp+5Ch]; fadd [8.0f]`); `p.Z += 8.0f` keeps Z on the x87 stack. The extra temporary node
  also merged the facing-vector results into one frame slot (frame 0x1B0 -> 0x1A4) (0x00951430).
- `if (Mgr.GetDef(i, &def)) { ...whole body... }` (no early return) lets the failed lookup share the natural exit's def release; an
  early `return` emits its own release copy right after the call (0x00974110).
- FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY (visible, out of line) turned a CDefPointer release into `dec [ecx+4]; jne` and
  fixed the whole register colouring of 0x00974110 (family in edi, ebx = 0 in the second half): 0.84 -> 0.99 in one grade.
- A 2D distance written as one expression `GFFastSqrt(v.X*v.X + v.Y*v.Y)` squares Y first and keeps both components on the x87 stack
  until after fsqrt (`fld st0; fmul st1; fld st2; fmul st3; faddp; fsqrt; fstp st2; fstp st0`, 0x009745E1); C3DVector::GetMagnitude2D's
  `squared = X*X; squared += Y*Y` squares X first and pops it (`fld st1; fmulp st2 ...`).
- In the IntoGroundSwoop TU VC7.1 expands the first 13 push_back<C3DVector> / clear() sites and calls push_back 0x0066E630 at the last
  three by itself (caller-size budget) -- plain STLport, no device.
- remove_if is expanded in Summon's TU (find_if's __find_if and remove_copy_if called) but called (0x008AC060) in the landed
  GetRushToPosForLunge: per-TU/site inline choice; stlport/position_algorithms.h needs the opt-in body (FABLE_RETAIL_STLP_REMOVE_IF_INLINE).

Review at landing:
- 0x00951430 and 0x00974110 needed 3 and 4 grades (+1 re-grade each after comment edits); no device, no flags.
- 0x00974110 writes the 2D distance as an explicit expression rather than C3DVector::GetMagnitude2D: either retail's GetMagnitude2D was
  `return GFFastSqrt(X*X+Y*Y);` (the CTCTARGETINGAI_ATTACK branch of engine/C3DVector.h has that body) or the row wrote it out; worth a
  family check of the GetMagnitude2D users (c2009's 0x00952890 matched with the two-statement body).
- The landed FlyOutOfGround 0x00951B20 / StartFlying 0x00950C30 / DragonBurninateInto ctor 0x009537B0 use local models with invented
  spellings (NAME_FIXES.tsv); 0x00977820 binds the 8.0f literal 0x0125916C as an invented global `g_MinTeleportMarkerDistance`.
- vland RELOC_NAME warnings for 0x008AAC50 / 0x008AB710 are demangled catalog labels of the already-bound STLport spellings (benign).
