# Chunk 268 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Stream readers.** A non-template `__forceinline void ReadValue(float& out) { float v; Read(&v, 4); out = v; }` (one overload per field type) puts the read locals in the dead argument slot and keeps `this` in ebx (0x006FFC30).
  - An inline `ReadULong()` that returns by value keeps the count and key in registers (0x0081AED0).
- **Chained zeroing.** `a.X = a.Y = 0` gives retail's Y-then-X zero-store order (0x00A873E0).
- **Inline helpers on floats.** Helpers that take `const float&` (e.g. `ToViewportX(const float&)`) avoid integer copies through the dead argument slot (0x009DC2C0).
- **Size tests.**
  - An unsigned `size()` gives `sar ; test ; ja`.
  - A signed `size()` gives `test edx,0FFFFFFF0h ; jg` (0x009E1280).
- **Bool from a byte vcall.** `x ? true : false` on an `unsigned char` vcall result gives `test al,al ; setne al`; `!= 0` gives `neg/sbb` (0x00C31B90).
- **String load order.** An inline `operator const char*` on the string class makes the text load before the size (0x00C31B90).
- **Dead-slot homing.** A `const CCharString&` parameter lets a local CCharString path be homed in its dead argument slot (0x006C3620).
- **/GS buffers.**
  - The position of the /GS cookie fixes the buffer size: PreloadBoneConfigs is `wchar16[264]` (0x006C37D0).
  - `wchar_t` is not a built-in type in the lane, so use `typedef unsigned short wchar16`. `L"..."` still converts.
- **Discarded insert results.** Twelve discarded `insert` results in one flat function share one 8-aligned slot (`and esp,-8`) (0x008B08B0). Three such results in a function with intervening locals do not share (0x004B2510, parked).
- **STLport and smart-pointer constructors.**
  - The STLport list ctor zeroes the node, mallocs it, self-links it in a register, then stores it.
  - CIntelligentPointer runs its out-of-line base ctor, then the vptr store (0x004B4590).
- **CVectorMap::Add.** Both `CVectorMap::Add` and the STLport `push_back` must be `__forceinline` for the map add to inline. The inline Sort then re-reads `Dirty` right after the store, and the growth call homes the `__false_type` tag in a dead argument slot (0x00CB8960).
- **User copy constructors.**
  - A C3DVector user copy ctor (`X(v.X), Y(v.Y), Z(v.Z)`) makes VC copy X and Z through the FPU when they feed a second vector (`fld [src] ; fst [dst]`) (0x007617A0).
  - A CRGBColour copy that reads bytes +2, +1, +3, +0 is a copy ctor that assigns R, G, B, A in that order (the members are declared B, G, R, A) (0x007617A0).
- **Inlined line primitive.** CEnginePrimitiveLine (type 6, 0x3c bytes) has an inline ctor in some TUs. Use the same parameter list as the out-of-line 0x004C3090: (id, start, end, colour, width, blendMode, bankIndex, useZBuffer, interpolation, blendOp).
- **Asm SSE signature.** In the original, an SSE block written in inline asm shows these signs: `movss/movhps` loads through argument and pointer locals re-read by name (a pointer local spilled to [esp+n] just before), fixed xmm0..xmm3, NO `and esp,-16` frame, and the result returned as `movss [esp+n] ; fld [esp+n]`. Intrinsics always give the aligned frame, so park as a lane gap (0x00B9C2E0).
- **Fastcall struct returns.** A `__fastcall` struct return passes the hidden pointer in ecx.
- **Line splice.** Never end a `//` comment line with a backslash: it splices the next line into the comment.

Review at landing:
- These rows took more than 4 grades:
  - Matched: 006c37d0 (the /GS buffer size).
  - Parked: 00852e10 (14 probes), 00872110 (20+), 004b4aa0, 004b2510, 00a873e0 and 00a52450. The advisor agreed to park 00852e10 and 00872110.
- Rows modelled with local structs (no engine header exists):
  - 00cb8960 (CScriptBase, CActiveEntityScriptBase)
  - 007617a0 (CShape, CEnginePrimitiveLine)
  - 006c37d0 and 006c3620 (CSkeletalMorphResourceManager)
  - 00b9c2e0 (CShadowClipVolume)

  Their layouts are in `// Layout:` comments. Retail CScriptBase is 0x10 below Ego_r.pdb (12-byte vectors), and retail CShape puts SelectedPoints at +0x14 (PDB +0x18).
- 00aab9c0: the catalog row holds several int3-separated functions (a 5-byte vcall thunk, a byte getter, ...). Split it before re-queueing.
- 00c31b90 needs `/EHsc` and 006c37d0 needs `/GS`. Both have `// Flags:` lines; g.sh without the flag shows DIFFER.
- 00dfaeb0 is the CQ_TraderConflictGoodScript copy of WatchTimeLimit. Make sure it does not bind to the same symbol as 00df7980 (Evil).
- 00cb8960 names LowerBound 0x00CB9840 exactly as the landed 0x00CB88B0 does (one retail copy, same address).
