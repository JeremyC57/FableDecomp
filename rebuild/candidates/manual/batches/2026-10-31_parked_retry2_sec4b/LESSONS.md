# Lessons (wave 2 sec4b)

1. A raw angle used twice (`WrapAngle(desired)` for a setter and again for GFGetSignedAngleDifference) is NOT a named
   local in retail: write `WrapAngle(GetAngleXY(v))` at both sites. VC7.1 CSEs the fpatan and keeps the raw value with
   `fst` beside the wrapped copy (008de080: 213 -> 78).
2. VC7.1 reassociates a single float sum `a*a + b*b + c*c` into its own order whatever the source order or
   parentheses; separate statements (`ls = X*X; ls += Z*Z; ls += Y*Y;`) pin the retail order.
3. A pointer that retail holds in edi/ebx across a call (physics, ParentThing) is a named local declared right before
   its use; declaring it earlier changes the schedule (008de080, 008dcc80).
4. A temporary returned by value in a full expression lives to the end of that statement: when retail releases a
   CDefPointer temp before the next computation, the GetSubDef call is its own statement (008e8060 g3).
5. When the frame is bigger than retail and most diff lines are esp offsets, scope the function's sections in blocks:
   VC7.1 shares slots only between disjoint scopes, and retail's packing (string temps, indices, out-pointers in the
   same low slots) shows which sections were blocks.
6. `CDefClassBase::GetSubDef<T>` defined in the same TU (with the shared CDefPointer header) reproduces retail's
   register-held def and its null-test-free release; declared-only it stays an external call. Never declare explicit
   `template<>` specialisations of it: VC7.1 ICEs (C1001).
7. finish.py reads the first `// Name(` line, so keep the description line first in src/ files, above `// Grade:`.
8. g.sh runs cl from the repo: never pass a bare /FAs (or any output flag) without an absolute z:\ path.
