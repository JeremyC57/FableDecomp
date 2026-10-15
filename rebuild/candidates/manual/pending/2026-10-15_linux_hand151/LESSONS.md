# Chunk 151 lessons (for GOTCHAS)
- `!(a <= b)` on floats gives `fcomp ; test ah,1 ; je` with the left operand computed first; `a > b` gives `test ah,5 ; jp`;
  `b < a` computes b first. `x <= 0.0f` gives `test ah,41h ; jp`, `x >= 1.0f` gives `test ah,1 ; jne` (0x00AECF90, 0x00AEB8C0).
- An inline Find over LowerBound that keeps the key in a register while passing an address takes the key BY VALUE and
  passes `&copy` (0x009AD4A0); a key compared from its own argument slot is passed as `&param` (0x00AE89E0).
- A whole-word `xor eax,eax ; mov ax,[m] ; shr eax,N ; test al,1` read of a ushort bit-field = inline `bool IsX()` accessor;
  the raw field gives `test byte ptr [m+1],bit`.
- Copying a polymorphic member by its IMPLICIT operator= (vptr untouched) keeps the lea'd copy pointers local; assigning
  the inner vectors one by one keeps them alive for later reads (0x00AEDDC0).
- A record copy ctor used by placement new in push_back must be `__forceinline` when it contains out-of-line member copies
  (implicit and plain inline ones are emitted out of line) (0x00CB7210); same for a string-returning getter inlined on
  another object (0x00AB0BF0).
- A call made between the base ctor and the derived vtable store with ARGUMENT words: inline `novtable` intermediate base
  (0x00E299F0, reconfirms chunk 147).
- `MAX(MAX(x,y),z)` macro re-evaluating the inner MAX on the taken arm explains a duplicated compare (0x00AF8F00).
- A frame iterator passed by reference while the loop keeps the node in a register = a SEPARATE iterator variable assigned
  before the loop and at its bottom (0x00AF56E0).
- A call result needed before a counted-pointer local's zero stores = a named local declared first (0x00AE89E0).
- Dinkumware `_Ufill` passes an empty `_Scalar_ptr_iterator_tag` by value; the lane DOES place it in the dead argument slot
  (0x00C29C30), unlike the by-reference tag sharing a live argument slot (0x0054E980, parked).
- Lane gaps seen: shared dead slot for key + tag (0x009B0170); tag at the live `child` slot (0x0054E980); callee-saved push
  delayed into a branch (0x00AF5010); vtable cached across a nested virtual call vs float spill (0x00AE2CB0/0x00AE2D20);
  single-bit byte or/and merged (0x00AEE8C0).
- Rows whose jump table is followed directly (no padding) by a 16-aligned stub hold two functions (0x009B98B0).
