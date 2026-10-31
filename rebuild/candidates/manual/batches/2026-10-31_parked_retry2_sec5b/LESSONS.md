# Lessons (section 5b sweep)
1. VC7.1 reuses the dead stack-argument slot for temporaries of inlined helpers (value-returning ReadULong), not for
   named address-taken locals: write stream reads as inline helpers returning the value.
2. A virtual that returns a struct by value reads the result through eax; retail code that reads the hidden slot
   directly means an out-parameter virtual (`void GetPosition(C2DVector&)`).
3. Inline bool helpers with two `return` statements materialise the bool (sete/test); a single short-circuit
   expression lets VC branch straight to the target, as retail does (CCharString::operator==(const char*)).
4. Inline strcmp against a literal uses the FIRST operand's length (repe cmpsb ecx = strlen+1): retail
   `strcmp(text, "")` vs `strcmp("", text)` is visible in ecx.
5. A failure branch that retail places at the function tail comes from `if (ok) {...} else Error();`, not an early return.
6. Empty STLport tag objects (__true_type/__false_type) get addresses; retail aliases them with a live slot. With a
   user ctor no zero store is emitted; where they land is still uncontrolled (residue in 2 rows).
7. Force-inline container helpers when the retail body has them inline even if the diff count rises: it restores the
   right code size and the remaining residue is honest (9afb90 1314 -> 1354 v 1344).
8. Retail float epsilon tests compare in float (`fcomp dword`): wrap fabs in a float-returning helper.
9. Propagated labels on callees are often wrong (0x00A20E10 is list<CWall>::clear, 0x009D5060 constructs a CDefString);
   decode the call site before trusting a landed callee name.
