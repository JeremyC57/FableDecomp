# Retry sec6b -- parked-row retry sweep (99 rows: 88 sec6 + 11 sec5b)

Counts (STATUS.tsv): 99 rows; 84 attempted with graded edits; 7 RELOCATION_MATCH (1 src/, 6 retail_asm/), 29 IMPROVED (differ/),
48 HELD, 15 NOT_REACHED (the far end of the worklist: four CPSCDecalRenderer / quest stubs of 0.9-2 KB and all eleven sec5b
1.1-1.9 KB stubs/skeletons; the context budget ran out). Dry vland: src/ 1/1 PASS (00c0c270), retail_asm/ 6/6 PASS.

## Per-lever tally (matches + improvements)
| lever | rows |
|---|---|
| retail_asm SSE kernel (landed 00bf7df0 model) | 00b99bb0, 00b9c2e0, 00b49810, 00b9c500, 00b4ced0 (5 matches) |
| box ctor member assignment (keeps both vector temporaries) | 00b46a30 (match) |
| __forceinline accessor + inline_depth(0) site selection | 00c0c270 (match), 00e2eac0, 00d2d1a0, 00ef4320 |
| __forceinline dtors to beat the inline budget at a late return | 00cddcb0 (63 -> 14 lines) |
| GFFloatToLongFloor / FloatToInt / rounding Retail-asm helpers | 00b93b30, 00f361d0, 00c41fb0, 00bfad90, 00bade40, 00b1d550 |
| first full reconstruction of a parked stub | 00bfad90, 00bfa2a0, 00b1d550 |
| partial prefix on a shared header (macro-renamed layout) | 00b277a0, 00b30b50, 00be6880 |
| catalog over-span cut (oracle_extra + BOUNDS_FIX) | 00b49810, 00b9c500, 00c72200, 00c17640, 00f36d30, 00b5c120, 00becde0, 00f37660 |
| statement order / ascending unchained stores | 00c61610, 00b8d0d0 |
| do-while stops the unroll | 00b8d0d0, 00bf2b50 |
| STLport shapes (erase/copy, const& resize fill, max by const&) | 00f3ba70, 00bfd7c0, 00b94540 |
| hoisted common assignment against cross-jump | 00c72200 |
| vector operator forms / by-value Clamp / memberwise copy | 00be95f0, 00c41fb0 |
| operand order of a product | 00b61dd0 |
| locals for loads (camera, pool fields, entry reference) | 00b5c120, 00f37660, 00becde0 |

## HELD residue families
register allocation / esi-edi swaps (00bee070, 00f28990, 00becde0), exit-cleanup tail merging and cross-jumping (00dc08c0,
00e7de60, 00f04000, 00d43db0), constant-1 register promotion (00bf1ee0), LTCG inter-procedural register use and mod/ref
(00c17640 register args, 00be95f0 `this` in edx across calls, 00f37660 dtor elided / fields forwarded across operator=),
frame-slot sharing and C3DVector copy count (00b20ba0), inline_depth on a new-expression outlining placement new (00f277f0),
reassociated constant products (00b1d550 `* (1/2047) * 20` folded into one constant).

## Names: proven vs descriptive
Proven (PDB spelling + retail address/binding): every Leaf in src/ and retail_asm/ (see payloads), CalcShortestDistanceToPoint
(00b9c500), ClipPrimitive / ZSpriteBatch::Load respellings (NAME_FIXES.tsv), DecalCollides (Ego_r mangled name; STLport
iterator spelling in retail not proven), CPSCDecalRenderer bitfields (struct_layouts_egor.tsv).
Descriptive (no second source): CString::ConcatInPlace library name (00c0c270; the binding table's ConsumeMapBrush at that
address is stale), CEnginePrimitiveRenderer fields +0x90/+0x94/+0x98, g_pLocalDetailGenerator +0x28, callees 0xB3B4A0 /
0xB2F740 / 0xB2FD60 / 0xB2FC50, g_renderStateDefault_01396f78/7c, g_pWorldFrameGlobals_01436e48 fields, RoundToDecimalPlaces
and FloatToInt helper names (00b1d550), the CDecal +0x58 Active bit (header spells the byte group LandscapeOnlyDecal).
