# Chunk 2022 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Two vectors reset to the same constant pair with retail's interleaved store order (X of both, then Y of both, one register per value) and
  the stored 0.0 forwarded into a later product (`fmul [0.0]`) = chained member stores `A.X = B.X = 0.0f; A.Y = B.Y = -0.2f;`; `= C2DVector(x, y)`
  materialises a temporary through the frame, `Initialise(x, y)` keeps per-vector order (0x007D39BF).
- `X *= m; Y *= m` with the magnitude kept on the x87 stack and popped after the Y product (`fld [Y] ; fmul st(1) ; fstp [Y] ; fstp st(0)`) is a
  named `float magnitude = v.GetMagnitude(); v *= magnitude;`; `v *= v.GetMagnitude()` consumes it at the last product (0x007D38F1).
- Retail rewriting a GetCamera look-at local in place (`fld [l.X] ; fsub [c.X] ; fstp [l.X]`, Z stored 0, no Z subtraction) = `look_at -= cam_pos;
  look_at.Z = 0.0f;` (C3DVector::operator-= opt-in); a named direction vector gets its own slot (0x007D3A0D).
- `float == -0.2f` compiles to an integer `cmp dword ptr [m],0BE4CCCCDh` (`== 0.0f` keeps fucompp) (0x007D3F32).
- A clamp `x = std::max(x, 0.0f); x = std::min(x, 1.0f);` behind five && tests sits at static frequency 1/32 (c1 16, score 86): no control-flow
  spelling moves it; /d2inlT87 is the minimal device (CoinGolf 0x007D3820; ShoveHaPenny needed T116 for its own site).
- `(long)(a / b * GetConstantFPS())` is reassociated to `fild ; fdiv ; fmul`; retail's `fld a ; fdiv b ; fimul [fps]` needs the parentheses
  `(long)((a / b) * GetConstantFPS())` (0x007F2BB7).
- Retail's magnitude order X, Z(memory), Y followed by a popped reciprocal (`fstp [r] ; fxch ; fmul [r]` x3) = `C3DVector difference = a - b;
  float d = difference.GetMagnitude(); C3DVector direction = difference; direction /= d;` (or `difference * (1.0f / d)`); in-place `direction /= d`
  gives X, Y, Z and `fst [r]` (0x007F2802). Every inline magnitude helper form squares X, Y, Z; only the direct expression reverses the order.
- A same-TU noinline body of an out-of-line callee that takes a local's address (GetEffectFromString, FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY)
  can switch VC7.1 to reusing the dead parameter homes for the whole frame (measured with the c2 packer hook: params become the first two
  slots); the declaration-only form leaves them unused (0x007F26B0).
- The c2 frame packer joins a node to the LATEST compatible slot first (verified on 0x007F26B0's 4-byte nodes); with parameter homes available
  they are tried first.

Review at landing:
- 0x007D3820 carries `// Flags: /d2inlT87` (Inline-budget line explains the trace); the TU also defines GetCamera 0x007D3560 (landed separately
  with a local model) and GetSubDef<CTavernGameCoinGolfDef> 0x007D4BD0 (landed) -- the lander must grade the ProcessInput leaf only.
- hdr/ builds on the c2007 (and c2002) overlays: land those first.
- Landed lightning rows 0x007F23D0 / 0x007F24E0 use wrong leaves / local models (NAME_FIXES.tsv); landed CoinGolf rows use local models.
