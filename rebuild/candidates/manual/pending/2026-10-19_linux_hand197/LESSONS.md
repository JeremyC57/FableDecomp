# Lessons from chunk 197
- STLport set/map iterator ++ is `node = _Rb_global::_M_increment(node)` (a static helper taking and returning the node);
  writing it as a member that updates `m_pNode` in place gives different registers (0x009D5A90).
- An out-of-line tree find reached through an inline `map::find(k) { return _M_t.find(k); }` makes VC re-read the returned
  iterator from its frame slot (no use of eax) - the shape of every `table.find(i)->second` in the nav loaders (0x00A77CA0).
- A temporary homed in a dead argument slot tells you WHICH parameter is dead: 0x00B1DB00's hidden-result temporary sat in
  the third slot, so the bool& out-parameter is third (signature (system, float, bool&)), not second.
- STLport `find` takes its iterator-category tag by const reference: `lea eax,[esp+N] ; push eax` (the landed 0x00A7D050
  by-value declaration is a different symbol and pushes a byte).
- int -> float through a register copy (`mov [esp+x],reg ; fild`) means the ints were a by-value temporary first: a
  C2DCoordI(GetTopLeft()) converted with a C2DVector(const C2DCoordI&) ctor reproduces the frame (0x00A16050, 0x00A156D0).
- `fcom 0 ; test ah,1 ; jne neg` = `(x >= 0.0f) ? pos : neg`; `n ? n : 1` into a long local reproduces the store-to-slot-then-fild
  (0x00681680).
- In a /Os TU (`optimize("y",off)` + `("s",on)`) small vector helpers must be `__forceinline`, and a counted release must be
  a member `if (--RefCount == 0) Destroy();` (dec flags) rather than accessor calls (0x0065F056, 0x0042EBB6).
- A ctor that writes bytes in R,G,B,A order on a B,G,R,A struct uses body assignments (initialiser lists follow declaration order).
- Compiler-generated COptimisedVertex: the engine C3DVector needs a user copy ctor (flat dword copies) but implicit assignment
  (shared `lea` destination) - both reproduce from one model (0x00AD7050 / 0x00AD77F0).
- Lane gaps seen repeatedly (park on sight): x87 float copies (`fld X ; fld Y ; fstp`) of way-point/centre coordinates (three
  CNavigatorAStar rows), the z,x,y squared-sum order, an empty allocator/tag temporary overlaid on a live or dead parameter slot,
  tail duplication of an epilogue+call, shrink-wrapped callee-saved pushes, bitfield merges that re-read the destination byte.
