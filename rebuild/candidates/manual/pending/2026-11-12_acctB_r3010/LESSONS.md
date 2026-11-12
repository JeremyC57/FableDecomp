# Chunk 3010 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00A4E300) An in-place 3x3 Transpose expanded between two quaternion conversions shows in the x87 spill set: retail spills
  exactly E12 / E13 and reloads E23 right after the first ToMatrix (0x00A4E567..0x00A4E571) = the UPPER triangle copied into three
  named temps (`float e12 = E12, e13 = E13, e23 = E23; E12 = E21; ...`) with Transpose() called BETWEEN the two ToMatrix calls.
  That fixed the frame (0x100 -> 0x104) and the whole start-matrix shape (unmasked equal 160 -> 349 of 399).
- (0x00A4E300) Retail's x87 commutative orders of the inlined SetMultiplyNoTranslation / quaternion norms are c2 RECORD-ID keyed
  and the decision is taken after ALL inlining: dead IL temps (32..40 statements before the first ToMatrix) or a dead extra inline
  expansion after the last statement both flip them, outer dead names (k = 1..48) and preceding functions in the TU do not (records
  reset per function). Early IL records and late expansion records are not equivalent (late ones also flip the FIRST norm).
  Measure with tools/m.sh (t2 store-order issues) + tools/rc.sh (record of TransformVector's xx) before grading.
- (0x00A4E300) TransformVector(C3DVector&)'s X row is `xx*E11 + (yy*E21 + zz*E31)` -- the same grouping as TransformPoint's X row
  in the header -- retail 0x00A4E8A5 adds zz*E31 + yy*E21 first; the landed users 0x00A11AE0 / 0x00A11C20 / 0x00A22F80 are
  byte-identical either way.
- (0x00A52450) Retail's out-of-line C3DKeyframe::ToMatrix expands PreScale(MMM): plain inline leaves it a call (341 v 447 bytes),
  FABLE_FORCEINLINE gives 447 relocation-equal. The GFSqr quaternion body and the landed x,y,z,w-copies body give identical bytes
  out of line AND in the row -- out-of-line copies cannot separate them.
- (0x00A4B820) The header SetMultiplyNoTranslation is byte-identical to retail's unreferenced COMDAT; 0x00A4BE10 is
  GetNonLoopingDuration's out-of-line copy (the header comment saying "no out-of-line copy" is stale).

- (0x00A4E300, session 2) The x87 commutative orders of this row follow the amount of IL the whole function carries at x87 lowering,
  not symbol-record ids: 32 constant stores to a global anywhere in the function (even in the else branch or after the last
  statement) give every retail order, 16 give half; a code-neutral header body that reshuffles all record ids (CQuaternion::ToMatrix
  through CMatrix3x4::Set(12 floats)) changes nothing. x87 lowering runs before c2's per-function pipe (0x1077C08E): hooks inside the
  pipe already see fld/fmul/faddp tuples.
- (0x00A4E300, session 2) Dead stores removed only after lowering still count: `m.SetIdentity()` on a matrix that ToMatrix then
  overwrites is code-neutral (1615B) but moves the orders.

- (0x00A4E300, session 3) VC7.1 c2 orders the terms of commutative x87 sums in its tree linearizer (0x1075FC1C, called from 0x10760539):
  each n-ary add's operand list is merge-sorted (0x1076A2F3, comparator 0x1076A394) DESCENDING by key = (SU need << 24) | (operand count
  << 16) | hash16, hash16 = sum(child_hash << (i & 7)) + (opcode - 0x24A), a field-record leaf hashing rotl16(record id, 5). A product of
  two field records hashes 32*max_id + 64*min_id + 48 mod 65536: the record ids of late field records (created in that same pass) decide
  the order, and the 16-bit wrap makes it look chaotic. Any extra c2 records allocated before the pass shift those ids.
- (0x00A4E300, session 3) FableWin.exe.signatures.tsv column 5 (debug instruction count) against VC7.1 /Od of a candidate inline body is a
  free body check: exact for ctors, SetTranslateOnly, TransformVector, PreScale, GetPosition, C3DVector(MMM), +=; VC10 artefacts: +4 per
  float-returning function, +1 per float argument pushed. It proved Transpose = three SWAP<float> calls (22 / 18 exactly).
Review at landing:
- 0x00A4E300 is parked (DIFFER(1615v1619)); its differ source needs this chunk's hdr/ (CMatrix3x4.h body changes listed in
  hdr/README.md). The TransformVector X-row grouping changes a SHARED inline body: regrade 0x00A11AE0, 0x00A11C20, 0x00A22F80 when it
  is folded into rebuild/include (measured RELOCATION_MATCH here).
- FABLE_RETAIL_C3DANIMATIONSTATS_PROTECTED (MovementVector read in place) is still unproven (carried from chunk 2014).
- 0x00A4E300: candidate_g04/ (RELOCATION_MATCH with an unproven SetMultiplyNoTranslation text) needs a decision; hdr/ now has the
  SWAP Transpose body (only this row uses FABLE_RETAIL_CMATRIX3X4_TRANSPOSE).
