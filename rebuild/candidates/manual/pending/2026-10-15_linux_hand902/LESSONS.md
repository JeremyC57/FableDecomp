# Chunk 902 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- The engine's float->long rounding is an inline-asm helper inlined everywhere. Its shape is `fstp [f] ; fld [f] ; fistp dword [r] ;
  mov eax,[r]`, or the out-pointer form `mov [po],&x ... fld [f] ; mov edx,[po] ; fistp dword [edx]` (0x00BA1140, 0x00BD71B0,
  0x00B84720). No asm-free form reproduces it: /QIfist gives `fistp qword` into an 8-byte temporary. The closest stand-in is
  `volatile float vf = f; volatile long r = (long)vf;` (`__ftol2`), which keeps the homing slots and the pointer spill. Any row
  containing it is a lane gap.
- `C3DVector::Length` taking the vector BY VALUE with `X*X + Z*Z + Y*Y` gives the retail in-place sequence `fld st(2) ; fmulp st(3)
  ; fld st(0) ; fmul st(1) ; faddp st(3) ; fld st(1) ; fmul st(2) ; faddp st(3) ; fxch st(2) ; fsqrt` (0x00BA1140, 0x00BD71B0). By
  reference, or written X/Y/Z, every sum starts with Z.
- A CMatrix3x4 transform whose three components are loaded first (`fld x ; fld y ; fld z`) and then reused through `fld st(n)`
  copies the components into float locals inside the inline operator (`float x = v.X, y = v.Y, z = v.Z;`). Each row then starts
  z*m2 + y*m1 and ends with the in-place `fmul` / `fxch` on the last row (0x00BA1140).
- `fcom K ; test ah,41h ; je keep ; fstp st(0) ; fld K` is `if (!(d > K)) d = K;` (a NaN-correct clamp), not `d <= K` (that gives
  `jp`). A float read into a local BEFORE that statement stays on the x87 stack and is used as `fmul st(2)` (0x00BA1140 LOD scale).
- `if (t < a || t > b) f = 0; else if (t < c) ... else if (t > d) ... else f = 1;` lays the shared zero block out LAST (`jnp`/`je`
  to it). The early-return helper form puts it right after the first two tests (0x00BA1140 mesh-effect fade).
- `cond ? 10 : 5` on bit 9 of a flags word as `shr ebx,9 ; and bl,1 ; neg bl ; sbb ebx,ebx ; and ebx,5 ; add ebx,5` is
  `((unsigned char)(flags >> 9) & 1) ? 10 : 5`; `(flags & 0x200) ? 10 : 5` gives `and ebx,200h ; neg ebx`.
- The same out-of-line constructor or destructor can be called at one site and inline-expanded at another site in the same function
  (retail VC7.1 inline budget): `new CEngineSubPrimitiveBoundingBox(prim)` calls 0x00B98B10 at the occlusion exit and expands it
  inline at the not-loaded exit, and ~CCountedPointer calls 0x00A36EC0 on the early returns and is inline at the end. The lane
  inlines much less in a 2 KB body: force the retail-inlined helpers with `__forceinline`, and model the split sites explicitly
  (an out-of-line declaration plus an inline twin).
- `mov eax,[mem] ; cmp eax,K1 ; jne ; ... cmp eax,K2` with a zero register stored to the result home first is an if / else-if
  chain on the type field (0x00BD71B0 hidden-material filter). A `switch` puts the second case first.

Review at landing:
- `chunk98_oracle.tsv` still holds the stale 48-byte, three-function row for 007460d0, and chunk34/chunk35 hold the 28-byte
  prefixes of 00bd71b0/00ba1140. `$S/tools/q.py` grades against those (007460d0 shows DIFFER 5v41 there). Grade with this chunk's
  oracle.tsv or delete the stale rows (GOTCHAS wave 25 line).
- 00ba1140 needed 6 full grades (12 were allowed). The model is local next to the existing
  `engine/CEnginePrimitiveManagerStaticMeshesBase.h`, because the header leaves +0x08 (`Enabled`) as a pad and the primitive and mesh
  classes differ from the PDB: retail C3DPrimitive2 is 0xA0 bytes, the static mesh RenderInfo sits at +0xA0, the animated
  RenderInfo sits 4 bytes below the PDB at +0xE0, and CArray is 12 bytes.
- The CTCPerceiveThing sources use local vtable models (`CTCPerceiveThing_Calls`, `CTCPerceiveThing_Slot8`) next to
  `engine/CTCPerceiveThing.h`. The header has no virtual surface, so this follows the landed CTCChest/CTCDoor vfunc10 sources.
- 007460f0 is a `list<T>::back()` COMDAT catalogued under CTCPerceiveThing. Its line 1 keeps the catalogue name; the leaf is `back`.
