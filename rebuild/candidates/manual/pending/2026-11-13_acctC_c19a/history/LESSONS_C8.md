- (agentC8, 0x00BB30A0) Two record-keyed residues can need OPPOSITE kinds of change: the test/or operand order of an early SetRenderState
  expansion moved with the WHOLE function's record count (late or early rulers alike, steps of 32), the fog expansion's load order only with
  records created BEFORE it (code after it moves O only). Meter both separately (tools/om.py binary-searches the O step; meter.sh the L window).
- (agentC8) Big O-only lever: an inlined helper with constant arguments whose dead branch folds (SelectVertexBuffer(0, 0) inside FlushRender)
  creates ~17 records per expansion that vanish; writing the folded call directly (SetStreamSource(0, 0, 0, 0)) removes them with the same bytes.
- (agentC8) L-units ~ inline-call parameters / named values: Initialise-forwarding constructors (+4 for four float params), a C4DVector(colour)
  instead of C4DVector(R, G, B, A) (-4), a named single-use value +1 (a named pointer used only for a member read: 0). Dead FABLE_RETAIL_ASSERT
  arms weighed 0 in 0x00BB30A0 but one full 32-record O step each in 0x00BB3E80.
- (agentC8) Same-TU siblings: when a shared helper text must change for one row, run a joint subset search (tools/combo.py: every subset of
  candidate items, 2-3 s per full grade) over shared + row-local items for BOTH rows and intersect on the shared choice.
- (agentC8) FableWin.exe was built with VC8/VC9 (Dinkumware _Iterator012 / _Container_base12): its byte counts differ for char pushes
  (movzx) and conversions; instruction counts of plain accessors still match VC7.1 /Od exactly.
- (agentC8) purity.classify grades a row naked_asm when ANY helper text in it names an asm helper (GFFloatToLongFloor) without a Retail-asm
  evidence line -- even an uninstantiated FORCEINLINE body; drop unused sibling helpers from the row.
