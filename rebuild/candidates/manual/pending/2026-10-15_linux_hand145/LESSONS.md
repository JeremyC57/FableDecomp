# Lessons from chunk 145 (for GOTCHAS)
- Three separate `mov eax,N ; pop ; ret` exits for a status merge come from NESTED `if (x != FAIL) { ... }` blocks; flat early returns let VC merge two of them into `mov eax,1 ; jne` (0x00BF8430).
- A bare dword `fistp` after `fstp [argslot] ; fld [argslot]` is an inline-asm FTOI helper: /QIfist gives a qword fistp without the reload. Lane gap (four PackVertexNormal rows).
- `X ? false : true` on an `unsigned char` bit field gives the 8-bit `shr al,N ; not al ; and al,1`; `!X` and explicit shift/mask forms widen to eax (0x00BF9AA0).
- A float compare whose left side retail squares first (`fld st0 ; fmul st1` right after the sum's operand) is written with that operand on the LEFT: `range * range < lenSq` (0x00BF9AA0); `stored != value` loads the argument first (0x00C631E0).
- A template member Sizeof with a loop is not inlined by VC7.1 unless `__forceinline`, and the default leaf then grades the template instantiation (`CArray<..>::Sizeof`) -- pass the mangled leaf of the intended member (0x00A43960).
- Three identical member destructors in a row are three named members; an array member compiles to a counted loop (0x0050F980).
- The CMainGameComponent 0x416xxx TU is `optimize("s", on)` + `("y", off)`; CheckSync additionally needs `/GS`.
- A width read before the remaining (right-to-left evaluated) call arguments is a named local (0x004163B7).
- Label corrections: 0x0076E620 = CTCWeaponTrail ctor, 0x00873A10 = NCameraModes::CSimplePointBase ctor, 0x00BFA8A0 = ZSpriteBatch::RelocateData, 0x00686800 = CTCBase::CTCBase(CThing*).
