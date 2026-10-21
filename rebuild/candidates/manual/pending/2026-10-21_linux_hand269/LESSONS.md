# Chunk 269 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Out-of-line callees that take a local's address need same-TU `#pragma auto_inline(off)` stand-ins (GetSubDef, GetEffectFromString, LowerBound). With the body in view, VC7.1 caches the slot value in a register, which fixed the def-pointer load timing at 0x0079CC80/0x007294A0.
- A polymorphic local base must be modelled as `virtual void v00()`, not an explicit `__vftable` member: a derived virtual otherwise adds a second vptr (0x0079D110).
- Dot product order: the expression forms in an inline Dot all gave Y first. Statement accumulation `float d = a.Z*b.Z; d += a.Y*b.Y; d += a.X*b.X;` gives retail's Z, Y, X (0x007294A0).
- VC7.1 inlines a small member at only ONE call site ("inline-once"); a plain inline does not reproduce which site. Use a `__forceinline` copy of the landed body at the inlined site (SetExpressionInline, 0x007294A0).
- Inline getters that return temporaries (`GetPixelFormat()`, `GetDimensions()`) let VC7.1 pack every temporary into shared frame words and even into dead argument slots. Named locals for the same values grow the frame (0x009F37E0: 0x50 -> 0x40).
- Two CPixelFormat shapes from one callee 0x009E3830: an inline `CPixelFormat(D3DFORMAT f) { Initialise(f); }` gives no store; the default ctor `FormatIndex(-1)` + Initialise gives the `mov [x],-1` store (0x009F37E0).
- C2DBoxI -> RECT for D3DX: named RECT locals with the pointers zeroed up front, members assigned top, bottom, left, right, reproduce retail's +4/+0xC/+0/+8 store order (0x009F37E0, the same tail at 0x009F4390).
- A struct local copied with `T box = area;` loads members in another register order than retail. A member-by-member fill matches, and VC7.1 then scalar-replaces two members into registers (0x009F4190).
- A function with a shared `return true` exit placed after the epilogue (`pop ebx; jmp; mov al,1`) could not be reproduced. Do/while+break, goto, nested-if and inline-helper forms all lay the true block right after the last test (0x00490A22). Record as a lane gap.

Review at landing:
- 0x007294A0 uses SetExpressionInline, a `__forceinline` copy of the landed SetExpression 0x00729410 body; keep both in step.
- 0x0079D110 declares PlaySoundW as `(const CCharString&, unsigned long)`; the landed 0x0079CE80 declares `(unsigned long, unsigned long)`; retail passes a CCharString address.
- 0x0079D110 models CTCBulletTime locally with real member types next to the opaque engine/CTCBulletTime.h (header's members are untyped pads).
- 0x009F37E0 replaces an ALREADY-LANDED asm_bake (baked body in rebuild/src/compiled/00/9f/CSurface_CopyFromSurface_009f37e0.cpp); land as a de-bake.
- 0x009F37E0 and 0x009F4190 model CSurface locally (virtual dtor + PDB names) beside engine/CSurface.h, which models the vptr as a data member, as the landed ctor 0x009F2D60 does.
- 0x00490A22 declares 0x0048E072 as `IsTextFullyShown()`; the name is a guess from the call sites.
- Rows that needed more than 4 grades: 007294a0 (many variants), 006c0970 (11), 0078f8f0 (advisor), 00490a22 (8), 009f37e0 (5).
