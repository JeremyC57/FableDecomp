# Chunk 307 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `x *= h; x *= h;` reproduces retail's `fld x; fmul st1; fmul st1` for a squared factor; `x *= h * h` does not (0x006655A0).
- A strike/scale factor kept as one in-place variable (`scale = f(...) * scale`), not a separate `speed` local, gives retail's frame (0x006655A0).
- Script Main bodies: put the cutscene handle and the cast map in a nested `{}` block so their natural destructors run in reverse order at the block end. Destroy the movie handle by hand (inline DestroyInline, without a user destructor) as the block's last statement. Explicit destructor calls grow the frame by 4. A real movie destructor adds a vtable store before the PImp release (0x00CE15E0).
- A class with a vfptr and an `unsigned __int64` member: MSVC pads the vfptr to 8, so the UID lands at +0x20, not +0x18. Put the vtable and the leading pad in a base class and declare the UID in the derived class (0x006A64D0, CThing UID +0x18).
- 0x0049D150 (physics interpolated position) takes a float time. Its pushed argument is not a sprite/text parameter, and miscounting it shifts every later frame slot (0x00663C40).
- A temporary built inside a call (sprite or text primitive) gets the frame slot sized by its class. Size the local model from the retail `lea ecx,[esp+N]` of the ctor call (CEnginePrimitiveSprite 0x48 at 0x00663C40).
- A small struct size result (two halved longs) as a `C2DCoordI size(w, h)` local rather than two `long`s moves it to retail's frame slot (0x00A69F70).
- Coordinator recipe for the rough-area cell index (from 0x007FCEF0), confirmed on 0x00518130: an explicit `if (y < Y1)` guard, hoisted `level*20` / `level*12` offsets declared in that order, and integer-sum indexing. This removes the `[edx+eax+4]` vs `[eax+edx+4]` SIB residue, but not the loop-exit spill-reload order (0x00518130, 0x0063F4B0).
- Unsolved, for the next sweep: a non-virtual call on a looked-up TC component encodes as `mov eax,[eax+4]; push arg; mov ecx,eax` in retail. Ours (C cast, static_cast, named local, out-param GetTC, same-TU callee bodies) always gives `mov ecx,[eax+4]; push arg`. It is 2 bytes per call, at 0x00663C40 (TCs 0x4c/0xcd) and 0x006A64D0 (TC 0x3d). A virtual call on the same lookup matches.
- Unsolved: retail `and byte ptr [esi+0x15c],0x7d` (memory-operand RMW clear of two flag bits) vs our load/and/store (0x00666310). bool / uchar / ulong bitfields and a union `&=` all give the 3-instruction form.

Review at landing:
- Rows that took more than 4 grades: 0x00CE15E0 (about 8 grades; the nested-block destructor order is the key), 0x00663C40 (8 grades, parked 573v577).
- All matched rows use local class models. The relevant engine headers (CTextureManager, CGraphicFrame, CThing) are data-only, so the methods could not attach to them. Each file has a `// Layout:` comment with the retail offsets (CThing interface bitset +0x20 / map +0x44, CThingCreatureBase current action +0x100).
- Catalog row 0x0052F1B0 (CThingFilter_IsValid::operator(), 560 bytes) holds about 20 small functions: split it before re-cataloguing.
- Test 0x00836260 had a particle-source global whose name tripped the byte-purity substring check. It is renamed `g_particleSource`; check any landed tests copied from it.
