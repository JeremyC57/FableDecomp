# Chunk 133 lessons (candidates for docs/pipeline/GOTCHAS.md)

- FP sums: VC7.1 re-associates a single three-term `a*x + b*y + c*z` expression into one canonical order regardless of the source order
  (two different source orders compiled identically); retail's per-row orders reproduce only statement by statement
  (`float x = b*y; x += c*z; x += a*x0;`) -- 00b80ad0, 00b81230.
- An array member of a class with a non-trivial copy is copied by the compiler's counted element loop (`mov ebx,N ... dec ebx ; jne`)
  only from the IMPLICIT copy constructor; a hand-written body loop is unrolled by the lane and arrays cannot be initialiser-list
  entries. Get the implicit constructor out of line with a placement-new copy site and `// Leaf: ??0Class` (00b36c50).
- A by-value small class argument built in its argument slot (`push ecx ; mov eax,esp ; mov [eax+N],..`) needs a USER copy
  constructor; without it the lane builds it in a frame slot and pushes the dword. Byte stores out of declaration order are
  constructor-body assignments (00644526).
- The result pointer returned by a by-value call (`fld [eax]`) is used only when the next operation is a member called ON the returned
  temporary (`GetWindAt(v).ScaleInto(v, 100.0f)`); a named or reference-bound result reads its frame slot. One local reused for the
  argument and the result keeps a single 12-byte slot (00b80fe0).
- STLport `__copy_trivial` in `erase(begin(), end())` reproduces as `if (first == last) return result; return memmove(...) + n;`
  (retail `cmp eax,eax ; jne`), not `n ? memmove(...) + n : result` (00b26010).
- `return x >= 0 ? true : false;` gives a bare `setge al`; `return x >= 0;` goes through `xor edx,edx ; setge dl ; mov al,dl` (006eb9f0).
- A `push_back` of a derived pointer into a vector of base pointers creates a conversion temporary that VC homes in the argument's own
  dead slot (`mov eax,[slot] ; lea edx,[slot] ; mov [slot],eax`) (00c26cd0).
- Label hygiene: 18 of 40 chunk labels here were propagated names; rtti-vtables.tsv (vtable slot 0 = deleting destructor), caller
  context (`callers.py`-style rel32 scans), allocation sizes (`push 34h` before operator new) and the PDB member pattern identified them.
- Lane gaps seen: an LTCG callee that takes an argument in esi (0x00C16CE0); a member load hoisted above a preceding vtable store
  (00b1aeb0); register choice for argument-address temporaries (00c26cd0/00c27130, 00af12a0).
