# Chunk 266 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Bit-field reads.**
  - `(bool)Bit`, or a bool accessor, gives retail's `xor eax,eax; mov ax,word; shr; test al,1` for a ushort
    bit-field, or `mov cl,byte; shr cl,5; test cl,1` for a low-byte bit.
  - A plain `if (Bit)` folds to `test byte,mask`.
  - This fixed the dead-bit test in 00aebaf0 and the Sleeping test in 00aee9f0.
- **Loop bounds.** Separate named bound locals (`float x = c.X - r; float endX = c.X + r; for (; x <= endX; ...)`),
  with the start computed before the end, reproduce retail's store/reload loop compares (00a77570).
- **Iterator classes passed by value.**
  - An iterator class needs a user copy ctor to get the `push ecx; mov eax,esp; mov [eax],ebp` by-value argument
    and the hidden-slot return.
  - Naming an out-of-line erase result (`It next = Erase(it); it = next;`) reads the result from the slot; a direct
    assignment reads through eax (00aeb2d0).
- **Call arguments.**
  - An if/else of two identical calls with different arguments tail-merges into one call that pushes from either
    slot. A ternary argument spills a temporary instead (00aeb2d0).
  - A shared `CIEngine* engine = info.Engine` local for a call pair keeps it in edi (00aeb2d0).
- **STLport map size.** `_Rb_tree` with an empty `key_compare` is 12 bytes. An 8-byte model triggers the
  `and esp,-8` aligned frame (00aca0c0).
- **CMovementSetup-style `max(a, b)`** uses the dead argument slots for its temporaries (00861a80).
- **TC lookup.**
  - The lookup is the `HasTC` bit test on m_presentBits (+0x20), plus `GetTC` via LowerBound 0x40F020 with the
    `(it == last || key < it->type) ? last : it` pick.
  - Type 0x43 gives `test byte [+0x28],8`; 0x4B gives `test ch,8` on the dword; 0x41 gives `test byte [+0x28],2`
    (008db460, 008dc900).
- **Load order.** `CThing* me = Owner;` loaded before `CThing* target = PTarget.GetPItem();`, then
  `(target->GetPosition() - me->GetPosition()).GetLength2D()` inline, matches retail's load order (008db460).
- **Position copy.** Declaring a position and then assigning it (`C3DVector p; p = expr;`), with a user copy ctor
  on C3DVector, gives retail's temp-then-int copy (008dc900).
- **LTCG callee knowledge.** Retail sometimes keeps ecx/edx live across same-module calls (00ac7750). Only the real
  callee bodies under `#pragma auto_inline(off)` reproduce this (GOTCHAS chunks 154/187).
- **Inline-asm lane gaps.**
  - An inline-asm SSE block (fixed xmm registers, pointers reloaded from frame slots) is a lane gap like the fistp
    helper rows (00ae0e60).
  - Intrinsics pick their own registers.
- **Container sizes.** The retail CArray/vector in the particle and primitive classes is 12 bytes; the PDB says 16.
  - Offsets after the vector are shifted: COptimisedPrimitive Faces +0x1C, Edges +0x28; CParticleEmitter fields -0xC.
- **Out-of-line dtor on one exit.** The GOTCHAS line-400 scoped-timer pattern did not outline ~CTimer 0xCD4470 on
  the single terminate exit of 00cef3b0.

Review at landing:
- **Rows that needed more than 4 grades:** 00b50370 (9 probes), 00bde680 (8), 00aeb2d0 (7), 009aed70 (5),
  00a77e80 (3 + src), 00ab5710 (4 + src).
- **Local base models next to an existing engine header:**
  - 00aee9f0 and 00aedaf0 model CParticleEmitter locally, because the header uses the PDB 16-byte vector (fields
    -0xC). 00aee9f0 needs `#pragma pack(push,1)` for OldPos at +0x46.
  - 00ac7750 / 00ac8ae0 model COptimisedPrimitive locally, for the same reason (Faces +0x1C, Edges +0x28).
  - Check `rebuild/include/engine/COptimisedPrimitive.h` and `CParticleEmitter.h` against these offsets.
- **Matched rows that use local `// Layout:` models where an engine header exists:** 00547c90 (CMovie.h),
  00a77570 / 00a77e80 (CNavQuadTree.h) and 00aedaf0 (CParticleEmitter.h).
  - The headers pull in PDB-sized members that shift the retail offsets, or too much include weight.
  - Retype them with `retype_landed.py` where the header offsets agree.
- **Catalog rows to fix:**
  - 00410be0 holds more than one body (a jump table plus neighbours).
  - Names to correct: 006301a0 (~CUIDef), 00b50370 (CEngineShadowRenderer ctor), and the three operator= rows
    (CCameraModeDef / CSpecialAbilitiesLightningSpellDef / CHeroStatsDef).
- **Leaf directives.** 00547c90, 00ab5710 and 00679740 carry `// Leaf:` directives, because the catalog name is
  decorated or qualified.
