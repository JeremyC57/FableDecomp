# chunk 721 continuation (agent721b): CEngineLightingManager + CNavigatorAStar

Nothing matched; payload.json is empty. differ/ holds the best sources (each line 2 is the `// Grade:` line).

## 0x00B46C80 UpdateGlobalLightingStates -- DIFFER(1531v1531), 10 attempts, parked
- Same length from the first draft; sections 1 (diffuse/backlight blend, faded-shadow dimming, the CShaderDirectionalLight
  local + InitialiseLight + SetLightGlobal, the 4-wide SIMD diffuse fill) and 3 (shadow fade factors) are byte-exact.
- Residue: section 2 (ambient) has eax/edx swapped in all three blocks (blend arm, copy arm, final copy). Ten probes
  (reference alias of PrimitiveAmbientColour, ternary, named blended temp, channel reference in the else arm, `!= 0` test,
  clamped float locals + CRGBFloatColour(r,g,b,1), statement order) did not move it.
- Levers that worked: CRGBFloatColour member operator*(float) / operator+ returning temporaries, written
  `ch0.X * (1.0f - blend) + ch1.X * blend` (VC evaluates operator+'s argument first = retail's ch1 first); the self-illumination
  lift as `float blue = B, green = G, red = R;` then `CRGBFloatColour ambient(sqrt(red*red+lift), .., 1.0f)` (retail loads B,G,R
  before any sqrt); `float unshadowed = 1 - shadow; SetShadowFadeFactors(unshadowed, 1 - faded - unshadowed, faded)`;
  CShaderLight : CBaseClassNonCopyable with implicit inline destructors (the call lands on ~CBaseClassNonCopyable 0x0099A300).
- No header overlay needed (self-contained model, offsets from the landed rows of this TU).

## 0x00B484B0 FrameStart / 0x00B47BC0 UpdateShadowedSpotLights -- still parked
- FrameStart: 10 more probes (see differ line 2), the single push placement never moved: a scheduler artefact.
- UpdateShadowedSpotLights: 3 more probes, register colouring unchanged.
- Both still need hdr/stlport/fable_containers.h (FABLE_RETAIL_STLP_TAG_CTORS; the original 721 copy, not 719's).
- Name for the previous FastInvSqrt helper: Ego_r ?GFFastOneOverSqrt@@YIMM@Z (a __fastcall float(float); inline here).

## 0x00A1CEB0 CNavigatorAStar::Initialise -- not attempted (budget)
Shape notes: `ret 24h`; CAStarState instance counters 0x013D27B8/BC/C0 incremented/decremented inline by the state
ctor/dtor with temporary-destruction flags; state map _M_erase 0x00A21640 + header reset, insert 0x00A221B0, state copy
0x00A20240, open-list push 0x00A22460, the closed list cleared with free() 0x00BFEA14 per node, a vector cleared through
memmove 0x00BFEAE6 (`erase(begin, end)` inline), the heuristic 0x009E2460 (__fastcall, two C2DVector+4 pointers) doubled
(`fadd st0,st0`).
