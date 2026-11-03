# Chunk 724 (agent724 + agent724b): 3 RELOCATION_MATCH, 4 parked, 4 not attempted

Grade with `FABLE_HDR_OVERLAY=$S/w22/agent724b/hdr` (the complete merged 724 + 725 overlay; agent725b/hdr is identical; hdrcheck2 default=same for every CHANGED header). Payload rows: 00b1bda0, 0087aa10 (/Gr), 0087aff0 (/Gr).

## Matched
- 0x00B1BDA0 CPSCDecalRenderer::InterpolateTo (2770 B). PDB class model, NParticleEngine::GFInterpolate overloads (FableWin,
  __fastcall), rounding via the shared engine/GFFloatToLongNear.h. Levers, in order of effect:
  1. Several straight-line by-value CRGBColour copies need the PDB `Initialise(r,g,b,a)` copy ctor (all four bytes read before
     any store): FABLE_RETAIL_CRGBCOLOUR branch in hdr/engine/CRGBColour.h. 648 -> 0 lines.
  2. The colour GFInterpolate must be `__forceinline` (VC7.1 gave up inlining it within the large function).
- 0x0087AA10 / 0x0087AFF0 NMaths::GetRootsNumerically<CTanMethod / CCotMethod>, from the Codex pass-11 drafts (12 shapes at 4-5
  lines). Lever: `return (float)(<double expr>);` in GetValue. The implicit double->float return conversion lets VC7.1 consume the
  scaled angle early. Retail keeps it on the x87 stack and pops it late (`fld st(1)` / `fdivp st(2)` / `faddp st(2)` / `fstp st(0)`).
  The Leaf is the `?A0x` prefix (anonymous namespace). TWO_PI is now `static const double`, so the row defines no external data.

## Parked
- 0x00B1C880 ReadBinary: DIFFER(3266v3266), 4 lines. The or-operand order of the SizeFadeMinimum (+0x28 bits 25-31) merge.
  Needs FABLE_RETAIL_CDATAINPUTSTREAM(_READERS): __forceinline Read / ReadULONG / ReadSLONG / ReadType / ReadFloat / ReadUBYTE /
  ReadEBOOL, plus the out-of-line ReadColour 0x009945E0. 10 attempts; the last three did not improve it.

## Compile "timeout" (task 1) -- cause and fix
- Cause: the Codex pass-11 drafts include headers the repo does not have (0x00875FB0: engine/GFMaths.h,
  engine/CMainGameComponent.h, engine/CEnginePrimitivePrepareInfo.h; 0x00879070: engine/NCameraModes_CProjectileWeapon.h).
  Codex graded in its own 529-header context. cl prints fatal C1083 and then Wine's cl.exe spins at 100% CPU forever
  (reproduced with a one-line TU that includes a missing header: 30 s timeout; an ordinary error exits in 0.08 s).
  incres.py (recursive include resolver) lists the missing headers in a second.
- Fix (overlay hdr/engine): GFMaths.h (inline __fastcall GFSqr / GFSin / GFCos / GFTanh / GFExp, extern TWO_PI),
  CEnginePrimitive.h (CEnginePrimitiveLine ten-arg inline ctor), CEnginePrimitivePrepareInfo.h (size 0x74, Engine +0x34),
  NCameraModes_CBase.h (24 slots, FableWin names), CCameraModeDef.h, NCameraModes_CProjectileWeapon.h (size 0x108);
  CMainGameComponent through the repo fable_game.h retail branch. CIEngine.h slots 8/11/12/13 renamed to the FableWin names
  and 17..22 added. Both rows now compile in ~0.5 s and reproduce Codex's residues exactly.

## Parked this session
- 0x00875FB0 DrawSquare: DIFFER(1315v1315), 6 lines of loop-setup scheduling; 11 attempts (QFE 4035 identical).
- 0x00879070 GetHeightParamForPostionWithPhi: DIFFER(1249v1251), 9 lines; retail keeps c_gamma on the x87 stack; 10 attempts.
- 0x008764E0 NCameraModes::DrawPyramid (catalog RenderParticleEmitter_4particles is wrong): DIFFER(1329v1340), 131 lines,
  25 attempts. From 244 to 131 lines: member stores for x/y/z plus named `double cos_theta/sin_theta` keep theta on the x87 stack,
  and an outer per-square loop with locals declared inside it. Left: frame layout (the x temp at the frame top), y kept on x87 in
  `cos*y - x*sin`, the operand order of `pt += pos`, and the ascending line temporaries.
- 0x00B1C880 ReadBinary (task 3): 5 more shapes, still 4 lines. Probe TUs show VC7.1 emits retail's `shl v; and old; or v,old` only
  when the RHS has a conversion node (uchar/ushort/short/float source); a plain dword RHS always gives `and; shl; or old,v`.

## Not attempted (budget)
- 0x008771A0 CTools::GetThingPointIntersectionFromTo (1264 B), 0x00879900 CProjectileWeapon::Update (3415 B),
  0x00B1E380 DatabaseSerialise (3698 B), 0x00B1F200 PreparePrimitives (3195 B). Identities and callee maps are in STATUS.tsv,
  work/771a0.dis, work/1e380.ann, work/00b1f200.ann. The camera headers they need now exist in the overlay.

## Header debt
- NCameraModes_CProjectileWeapon.h keeps XAxis/YAxis/ZAxis/Attractors as opaque byte arrays (the retail C3DVector members are
  byte-aligned at +0xA9/+0xB5/+0xC1). Codex's model; needs a packed C3DVector member model before rows touch them.
- NCameraModes_CBase.h includes CDefPointer.h, so every includer needs FABLE_RETAIL_DEF_CLASS_BASE (725's 00697ab0 now defines it).
