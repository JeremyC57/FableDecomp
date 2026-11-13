- (0x0060EEB0) `if (A) v.push_back(x); else if (B) v.push_back(x);` (identical blocks cross-jumped) is not the same as `if (A || B)`: it
  moved a hoisted `mov ecx,this` and push order AND the function-wide constant-0 register choice; try it when one `||` condition's
  arms both end in the same call.
- (0x0060EEB0) A SIB base/index flip that no local spelling moved closed with a dead-local ruler (5..8 dead bools anywhere upstream
  fixed it) and then natural names: one reused `clickable` local replaced by six named FindChild results.
- (0x0060EEB0) q.py masks call targets: a PeekTC (const LowerBound 0x004365B0) and a GetTC (non-const 0x0040F020) can give identical
  bytes; vland's RELOC_CONFLICT showed it. Write GetTC where retail calls 0x0040F020.
- (0x0060EEB0) Frame-packer ties: tools/pk (copied from pending/2026-11-08_linux_hand885/tools/pk) + tools/pkslots.py list slot
  weights and the sorted order; sim.layout what-ifs tell whether a weight or a creation-order change is needed.
- (0x005FF190) An inline split of one helper over many sites that no frequency spelling moves can be the helper's TEXT: the c2 trace
  gives each site's f, and the cost c1 that reproduces retail's whole split can be solved for (Reset<CComponent>: c1 108 v 95 ->
  `... if (data) Info = new CCPPointerInfo(1, DeleteData, (void*)data); else Info = 0;`). Trace tool: pending/2026-11-08_linux_hand883/tools/inl*.
- (0x005FF190) c2 compiles every inline callee of a function before the function (trace "pipe" order), so a helper is "already
  compiled" (cost rule) even for the first function of a TU; a helper retail CALLS where the cost rule expands may simply have had
  no visible body in that row's TU (Compare declared only).
- (0x005FF190) An implicit STL destructor at a function's end may never reach c2's plain-inline scoring (no event in the trace): an
  explicit FABLE_FORCEINLINE ~map() {} opt-in expands it like retail.
