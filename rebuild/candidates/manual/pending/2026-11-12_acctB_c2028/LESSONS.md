# Chunk 2028 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00B8DF50) Retail `fld y ; fld x ; rep movsd ; fstp E41 ; fstp E42` + E43 as one dword copy = a C3DVector COPY (user copy ctor) handed to
  SetTranslateOnly(const C3DVector&) that forwards to SetTranslateOnly(float, float, float): arguments evaluated right to left read Z first, and only then
  does VC7.1 drop the copy's dead Z frame store; written E41 = X first (or with the copy declared before the matrix copy) the Z store stays; from a plain
  reference all three are dword copies.
- (0x00B8DF50) x87 commutative order of an inlined matrix row depends on earlier reads of the same point in the function: one-line row sums and the
  E13*x-first accumulation each gave a one-swap residue; the Z-term-first accumulation matched. Fragment probes reproduce it only with the earlier reads.
- (0x00B8DF50) CEngine::ClipSphere 0x00B24150 is the landed twin of the 2-argument CEngineCamera::ClipSphere inline (no retail out-of-line copy).
- (0x00B9A4A0) A large render pass whose helpers retail expands at scores 80..534 while it calls others at 76 / 91 (c2 inline trace) needs per-helper
  FABLE_FORCEINLINE (bottom-up: CIVCountedPointer::Reset inside SetupOcclusionInfo), not a /d2inlT value.
- (0x00B9A4A0) An inlined helper returning `a || b` on an out-of-line call result keeps retail's `cmp ; jbe`; an if-return chain materialises `seta al`.
- (0x00B9A4A0) `if (x) alpha = true;` after `bool alpha = alpha_test;` gives retail's in-register select; the ternary spills both arms to the frame.
- (0x00B9A4A0) The LOD-search `while (HasLOD(l+1) && !(GetLODError(l+1) > e)) ++l;` keeps the je exit and the block frequencies; a for+break moves the
  ~CCountedPointer call / expand split of the function's two returns.
- (0x00B9A4A0) A by-value class return copied into a named const local (PeekSceneData) calls the declared copy constructor once; declaring the copy
  constructor in the header is what keeps it out of line (retail 0x00B38600).
- An unquoted shell heredoc runs backticks: write hand-over text with `<<'EOF'`.

Review at landing:
- 0x00B9A4A0 parked after pass 1 only (8 grades; passes 2-3 unused for context): one frame byte; differ/00b9a4a0.cpp needs the overlay opt-ins listed
  in hdr/README.md. No behaviour test was written for it.
- 0x00B8DF50 used 8 grades (6 + 2 regrades).
- Global spelling of 0x01436EA0 (g_pEngineCamera here and in CEngine::Initialise; NEngineInternal::GPCamera in c2018 / c2024).
- The two "one inline body, two retail contexts" notes (NOTES.md Header debt).
