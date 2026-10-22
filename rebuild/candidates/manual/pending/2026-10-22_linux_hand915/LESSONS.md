# Chunk 915 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Self tail recursion: VC7.1 turns a `return F(next, count);` written directly in F into a loop (`jne` back-edge,
  constants hoisted into a register: `mov dl,1 ; cmp [ecx+1Ch],dl`). Retail's tail `jmp F` to its own entry
  (0x00A644A0 GFCheckBlackHeight) comes from putting the null test + recursive call in an inline helper
  (`inline long Below(T* left, long c) { if (left) return F(left, c); return c; }` ; `return Below(node->left, count);`).
  Pragmas "s"/"t"/"g" and a template do not change it.
- STLport by-value-iterator results under __fastcall: a non-POD result (back_insert_iterator with a user ctor) comes
  back through a hidden pointer in ecx, first argument in edx (0x007B5530/0x007B5560 copy/__copy_aux, callee
  0x007B5400). The empty tags need user constructors, then the tag temporary takes the dead by-value `result` slot.
- 0x0080AA60 (__linear_insert, 219 B): an extern __copy_backward gives the tag its own `push ecx` frame word; the real
  same-TU noinline body (GOTCHAS 24l reachability) homes it in the LIVE `__val` slot. A struct holding an intelligent
  pointer is only copied inline when its copy ctor / operator= and the pointer's are `__forceinline`; a C3DVector
  user copy ctor makes the copy constructor store directly (`mov [esi+0Ch],ecx`) while the implicit C3DVector
  assignment keeps `lea ecx,[esi+0Ch]` in the assignment.
- An `int` result `return 1/0` on two exits gives `mov eax,1 ; ret 4 / xor eax,eax ; ret 4` even where the PDB says bool
  (0x00A64480 IsNodeBlack, `?IsNodeBlack@...@@IBE_NPBV...`): the retail TU returns a 32-bit value.
- DECLARE_IUNKNOWN delegations (`GetOwner()->AddRef()`) compile to `mov eax,[esp+4] ; mov eax,[eax-8] ; mov ecx,[eax] ;
  mov [esp+4],eax ; jmp [ecx+4]` with the release-build CBaseObject empty (owner at CUnknown+4, IPin at +0xC).
- Two byte-identical instantiations (0x0092B270/0x0092B290 __distance) must be given distinct template arguments
  (a per-row opaque element type) so one symbol does not bind to both retail copies.
- When a Leaf names a template class member, use the `?$` spelling (`??0?$_List_iterator`); `??0_List_iterator`
  matches no symbol and the grader silently falls back to the largest function.

Review at landing:
- 007b5560 / 00a644a0 / 0080aa60 TUs emit extra COMDATs (an inline __copy_aux, the inline recursion helper, the
  CIntelligentPointer vtable/??_G and a same-TU __copy_backward body); the leaves pick the right symbol, but check the
  lander does not object to the extra code.
- 007b5530 defines `?__copy_aux@@YI...` strongly while 007b5560's TU also emits the same symbol as an inline COMDAT
  (same types): confirm the lander keeps them apart (or rename one model's element type).
- 00ca53f0 models the DirectShow classes locally (no engine header); 0080aa60 models CTargetPoint /
  CThingCompare_BestForTargeting2 locally (no engine headers exist for them).
- No row needed more than 4 grades except 00a644a0 (6 probes across strategies) and 0080aa60 (5 probes).
