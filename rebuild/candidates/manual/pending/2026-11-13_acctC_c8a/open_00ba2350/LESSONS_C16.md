- (agentC16, 0x00BA2350) Two-function TU: c2's record / scheduling counters are PER FUNCTION (rulers in row 1 never move row 2), but row 2
  depends on the BODIES of earlier same-TU callees through IPA: a non-capturing body of every callee that receives a local's address
  (here C3DTextureStages& into SetupStaticBlocks / SetupFirstTextures / RenderMeshPrimitiveBlocks* / SetupRenderMode) removes the
  memory-dependence edge that pins an initialising store early (0x00BA558B). Check retail TU order for callees defined earlier.
- (agentC16, 0x00BA5260) FlushRender written with the folded PD3DDevice->SetStreamSource(0, 0, 0, 0) (cloth rows' text) instead of the
  inline SelectVertexBuffer(0, 0) also fixed this row (4093 -> 4056 bytes): an uncross-jumped SetRenderState copy came back when the
  test/or band shifted. Use the matched siblings' helper texts first.
- (agentC16, 0x00BA2350) c2prio --trace shows the register CHOICE costs, not only priorities: sub_primitive took edi because ebx cost
  2300 (byte candidates left with one register). When the forced-priority what-if fixes a prologue, check whether the competitor
  must be coloured first or only made cheaper (here forcing the byte temps first did not help: table order still picks edi).
- (agentC16, 0x00B783F0) Before proposing a helper-with-constant-argument explanation for dead-unit residues, place the dead units
  per call site: a per-call helper text repeats its dead arm in EVERY expansion (exp1/exp2/exp3), so it cannot give "2 units before
  site X and none between X and site Y" -- measure the positional constraint first (w3/p probes).
- (agentC16) Retail row text for the faded-shadow bool: the existing FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD setter
  called with a constant `true` gives retail's `mov al,1; je; xor al,al` shape naturally (no hand-inlined bool needed).
- (agentC16) Copy-ctor-free tint path: `tint_colour.A = alpha; SetTextureFactor(CRGBFloatColour(tint_colour))` with the Initialise ctor
  evaluates A,B,G,R (retail order) and is code-identical to a hand-ordered named alpha float.
- (agentC16) Tool path hygiene: when copying another agent's tools, re-point subdirectories too (sed stopped at a directory; v.sh and
  tools/sched2 still pointed at agentC13 -- grades silently used the old overlay until fixed).
