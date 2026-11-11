# Chunk 2006 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00B399A0) VC7.1 reorders unparenthesised x87 sums of products canonically: in a 12-element matrix product the written term
  order of every sum is irrelevant once the inline nesting is right, and the nesting decides the result. Measured on the three
  expanded matrix products of UpdateOutputPrimitive: `PreMultiply(m) { CMatrix3x4 copy(*this); SetMultiply(m, copy); }` (both PDB
  members, FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY) gives retail's order in all 36 sums; one PreMultiply body with the copy inside, a
  caller copy passed to SetMultiply, a free helper and hand-written code leave 2-20 sums in another order. Search the inline
  structure (which helper copies, which takes references), not the term order; per-element "forms" searches are a dead end.
- (0x00B399A0) The spelling of an expanded same-TU predicate changes the canonical x87 order of later sums: GetParentTransform
  written `bool result = false; switch (..) { case X: result = f(); break; } return result;` fixed block 1's E11 / E12 sums that the
  `case X: return f();` form left in another order (same branch code either way).
- (0x00B399A0) A visible same-TU callee body lets c2 reuse a caller-saved register across the call: RedoMatrixConversion's body in
  the TU gives retail's `lea edx,[transform] ; push edx ; call ; ... mov ecx,edx ; push ecx` (0x00B39EEE).
- (0x00E621F0) The reverse also holds: a callee whose COMDAT retail emits right after its first caller (GetGoldCost 0x00E5FC70
  after OnPredicateFail) is an inline member; defined out of line in the TU, c2 knows it keeps edx and colours a value across the
  call in edx where retail has ebp (0x00E622A9). Same TU, the call is expanded at one site and called at another by frequency.
- (0x00E621F0) Jump-table cases are laid out in source order; retail's table 0 -> BAD_01 0x00E62380 ... 2 -> GOOD_01 0x00E622D6
  means the source wrote the cases 2, 3, 0, 1. q.py masks the table relocations: check the $L labels of the object by hand.
- (0x00E621F0) A vector position written as `Me.GetPos() + C3DVector(0, 0, 1)` inside a call argument: GetPos runs after the string
  temporaries and only Z is added (the +0 adds fold under /O2); a named `const C3DVector& pos = Me.GetPos()` calls GetPos first.
- (0x00D91E00) The generated-script return form `TRetVoid _Fail_Val; ... return _Fail_Val.Return();` changes block layout against
  plain `return;`: it kept retail's in-line expanded DeregisterTimer exits after the first terminate checks (0x00D91E31, 0x00D91E95).
- (0x00D91E00) tools/inl.sh (hand883 inl.py via agent2003's wrapper) lists every ~vector / ~CTimer site with its score: compare the
  call / expand sequence with retail's pads before touching loop shapes -- here 27 of 28 sites were already right and one folded
  `&& true` (score 64 -> 74 at threshold 74) closed the row; rewriting the inner `for` as `while` or the scan loop as `do / while`
  moved a dozen other sites.
- (0x00550DC0) CRGBFloatColour(const CRGBColour&) converting A, B, G, R (retail 0x00550EA8..) is the ctor written through
  Initialise(r, g, b, a) (right-to-left argument evaluation); the member-initialiser ctor converts R first. The PDB's operator*
  is a member (??DCRGBFloatColour@@QBE?AV0@ABV0@@Z); the shared header has a free one (same bytes here).
- (0x00550DC0) Product colour assigned to a loop-outer local (`child_colour = CRGBFloatColour(c) * colour;`) gives retail's product
  temporary + copy; copy-initialisation inside the loop builds in place (262 lines worse).
- (0x00550DC0) `C2DVector render_position = relative_position + RelativeRenderPosition; child->SetRelativeRenderPosition(render_position);`
  gives retail's `fst` of the scaled Y and the late fmul (0x005511D3, 0x00551292); passing the sum straight into the call stores
  and reloads.
- (0x00550DC0) A visible STLport map::operator[] (FABLE_RETAIL_STLP_MAP) lets the state-key temporary share a frame word (frame
  0xd4 -> 0xcc) although the call stays out of line.
- (all) md.py (agent2004) and rel.py are the fastest per-probe readers: md.py `-n 0` line count and `--frame` mask, rel.py for the
  relocation identity before vland.

Review at landing:
- 0x00D91E00 carries an `// Inline-budget:` device (folded `&& true` on one terminate check; retail's construct unproven).
- All three matched rows need the hdr/ overlay (six headers, additions only, opt-in blocks; hdr/README.md). CV_BodyGuardScript.h
  and CQ_HobbeCaveScript.h are the pending hand-back H40g's versions plus (Hobbe) one new block: merge H40g's first.
- 0x00B399A0 defines same-TU bodies of RedoMatrixConversion, GetParentTransform and GetChildInverseTransform (the latter two
  landed / unlanded rows of the same TU); check the link for duplicate symbols with the landed 0x00B39900 / 0x00B38E30 objects.
- Landed 0x00B38E30 (RedoMatrixConversion) and 0x00B39900 (GetChildInverseTransform's callee) use pointer-parameter spellings the
  PDBs do not have; landed 0x00E5FC70 is named GetHenchmanGoldCost (PDB GetGoldCost) -- NAME_FIXES.tsv.
- FABLE_RETAIL_CMATRIX3X4_PREMULTIPLY (chunk 764) and the new FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY define PreMultiply differently;
  the 764 body may be revisitable with the SetMultiply nesting (its landed rows match as they are, so nothing is required).
