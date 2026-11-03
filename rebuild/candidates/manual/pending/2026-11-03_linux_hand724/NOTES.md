# Chunk 724 (agent724): 3 RELOCATION_MATCH, 1 parked, 2 compile-timeout gaps, 5 not attempted

Grade with `FABLE_HDR_OVERLAY=$S/w22/agent724/hdr`. Payload rows: 00b1bda0, 0087aa10 (/Gr), 0087aff0 (/Gr).

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

## Gaps
- 0x00875FB0 and 0x00879070 (Codex drafts): cl.exe ran at 100% CPU past the 600 s q.py timeout on the current tree.
  - 0x00875FB0: once, killed at 9.5 min.
  - 0x00879070: twice.
  - The drafts include the camera / CThingGameObject header chain. Stopped under the loop-prevention rule.
  - 0x0087AA10, which has a light header, compiles in about 1 s, so something in that header chain (or the optimiser on those
    bodies) is pathological now. Worth a coordinator check: compile both drafts with /E, and time cl on the repo headers alone.
  - The Codex identities (DrawSquare, GetHeightParamForPostionWithPhi) were not re-verified here; the catalog says
    EditorDrawHighlightPoints_Impl / ProjectileWeapon_CalcHeightForAngle.

## Not attempted (budget)
- 0x008764E0 and 0x008771A0.
- 0x00879900 CProjectileWeapon slot 1.
- 0x00B1E380 DatabaseSerialise and 0x00B1F200 PreparePrimitives. Both are analysed in work/*.ann; STATUS.tsv has the
  shape notes.

## Header overlays (hdr/engine; hdrcheck2: default=same)
- CDataInputStream.h: FABLE_RETAIL_CDATAINPUTSTREAM_READERS (forceinline readers, ReadFloat/ReadUBYTE/ReadEBOOL/ReadColour decls).
- CRGBColour.h: FABLE_RETAIL_CRGBCOLOUR (PDB Initialise + Initialise-based copy ctor).
- Overlay headers must be #included before any repo header. VC7.1 also searches the directories of previously opened
  includes, so a repo header included first makes the repo copy win.

## Housekeeping
- A stray copy, /w22/agent724/hdr/engine/CDataInputStream.h (outside $S; written while $S was unset), needs deleting by the
  user. Removing it was blocked by the safety check.
