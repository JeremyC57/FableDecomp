# 0x00852E10 NCameraModes::CScriptModuleFOVZoomStayRelative::CScriptModuleFOVZoomStayRelative -- READY (p25Q, Account B, 2026-11-21)

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00852E10 | CScriptModuleFOVZoomStayRelative(const CIntelligentPointer<CThing>, const C3DVector& x3, float, long, bool) | 445 | RELOCATION_MATCH (no flags) | PASS (links the real row; 4,000 scenes: call sequence, arguments, both vptrs, Mag / Angle / Axis against a model) | 14 / 14 KILLED |

Strict check: `LANDVERIFY_STRICT_HEADERS=1 FABLE_HDR_OVERLAY=hdr vland.py payload.json oracle.tsv` -> RELOCATION_MATCH, behaviour PASS
(the only warning is the usual RELOC_ALIAS for the `_CIacos` import thunk, as in 0x007602F0).

## Text
Shared headers only: C3DVector, CThing, CTCPhysicsBase, CIntelligentPointer (COPY), GFAngles (GFArcCos), and the new
engine/NCameraModes_CScriptModuleFOVZoom.h. Ego_d's parameter names, its two locals (facing first, then rel) and its eight statements; the
source comment decodes every Ego_d line size (384..391). The old parked draft (447v445, pending/2026-10-21_linux_hand268, "Newton r stays on
the x87 stack") used a local fast-normalise helper and explicit sums. The residue goes away when the helper texts are the ones Ego_d shows:
- GetMagnitude line 417 is 53 B with no local: the sum (44 B, as GetSquaredMagnitude's line 429) plus `push ecx ; fstp ; call GFRoot`
  (9 B), so `return GFRoot(X*X+Y*Y+Z*Z);` = existing opt-in FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF (GFRoot is in Ego_d and FableWin);
- Normalise has one local (one_on_magnitude) and line 687 is 74 B (three GFSqr calls) = existing FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
  (the named-`squared` NAMED_SQUARED text also grades, but Ego_d has no such local);
- GFFastOneOverSqrt's locals are n, seed (TFloatIntUnion) and r = existing FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION;
- operator^'s parameter is `vector`: FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22.
With those, the r spill (`fstp [r] ; fxch ; fmul [r]`, 0x00852F0D) and the product-sharing Mag / Normalise sums come out as retail.

## Labels
- `// Inferred:` the parenthesised cross-product components of ORIGINAL_GROUPED_CROSS_PASS22 (/Od sizes equal; the ungrouped
  CROSS_ORIGINAL_FORMAL text grades 445v445 with the Axis copy's integer moves no longer interleaved, 0x00852F7C / 0x00852F98).
- `// Inline-budget:` GFFastOneOverSqrt is FABLE_FORCEINLINE in every branch of C3DVector.h (retail expands it here); no other control.
- No Fidelity copies, brackets or flags.

## Header changes (hdr/, hdr.diff) -- strictly add-only
- NEW engine/NCameraModes_CScriptModuleFOVZoom.h: NCameraModes::CScriptModuleFOVZoom (Ego_d LF_CLASS 0x3C12F, 0x84; members, access and
  methods as Ego_d's field list; Ego_r layout) and CScriptModuleFOVZoomStayRelative (Ego_d LF_CLASS 0x570C2, 0xa0: Thing, Axis, Angle, Mag),
  both over the existing NCameraModes_CScriptModuleBase.h. Declarations only; sizeof asserts (new file, nothing to pin).

## Mutants (mutants/): 14 / 14 KILLED
Cross-product operand order, rel's operands (start_pos, reversed), Mag after Normalise, raw acos, Dot(facing), no Normalise, rel ^ rel,
base-ctor argument order, the flag, Thing() default, facing read from the member, squared magnitude, an extra Axis.Normalise.

## Names (NAME_FIXES.tsv)
None: the row's landed name is right (Ego_r + Ego_d + FableWin 0x0283B2F0).

## Newly flagged rows (owner policy: a new shared header may flag landed local copies)
The ten landed TU-mates that model these two classes locally: 0x00852560 (FOVZoom ctor), 0x00854380 (dtor), 0x008520F0
CalculateIncrementVector, 0x00852160 Update, 0x008520C0 OnInitLeaf, 0x00854360 IsShortenOnOcclusion, 0x00854370 ResetCameraOnEnd,
0x00854480 / 0x008544C0 scalar deleting dtors, 0x00852FD0 StayRelative::Update.
