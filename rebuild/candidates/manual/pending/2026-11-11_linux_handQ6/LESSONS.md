# Q6 lessons

1. Same-TU STL body lever (4 matches: 0x00912840, 0x007DDA30, 0x00A66FD0, 0x00542780). When the row passes a local's
   address to an out-of-line template callee (vector push_back / _M_insert_overflow, deque _M_push_back_aux_v, list
   insert/erase, __copy), retail's TU had that template body visible (COMDAT bodies are in every TU that instantiates them,
   whatever address the linker kept). Give it a same-TU body under `#pragma auto_inline(off)` ... `(on)`: c2 then knows the
   local is not captured, which changes slot sharing, copy pipelining and scheduling. The WHOLE chain must be visible:
   push_back alone did nothing for 0x00542780; push_back + _M_insert_overflow matched.
2. The lever only applies to template/inline callees. A huge grid-search template (GetAllThingsInRoughArea 0x008E3C50,
   GetPFirstThingInRoughArea 0x007FD110) needs a faithful body; a stub moves the code but is not a reconstruction.
3. It did not help where the residue is register allocation or a reload order (0x00747C70 erase body, 0x0087A7C0 real
   _M_insert_overflow, 0x00753E90 LowerBound body, 0x00972F30 full non-POD insert chain): try it first only on
   frame-slot / copy-schedule residues around an address-taking STL call.
4. PAREN-tuple scheduling: in 0x007DDA30 the last residue went away by parenthesising the X term of the local C3DVector
   operator- (`C3DVector((a.X - b.X), a.Y - b.Y, a.Z - b.Z)`) and leaving GetSquaredMagnitude unparenthesised.
5. Tooling: gg.sh/q.py with an empty Leaf grade the largest function -- a same-TU helper body can then become the graded
   function, or c2 can fold the row (a stub that wrote nothing to the out-vector deleted the loop: 234v744). Always give
   the row a full mangled `// Leaf:` and set FABLE_HDR_OVERLAY before gall.sh.
6. Shared-header rework: the same-TU body lever maps onto the shared STLport header as "visible but out of line" opt-ins
   (FABLE_STLP_NOINLINE on the inline member, or a template body under auto_inline(off)); 0x00A66FD0 and 0x00912840 matched on the
   first compiling grade once the shared retail forms were selected. A new opt-in that duplicates a declaration-only one must be
   independent of it (defining both gives C2535).
7. The graphics-bank stack (CGraphicInfoFrame::GetFrame and its refcount/touch helpers) has no shared inline bodies yet; rows that
   expand GetFrame (0x00542780, landed 0x0041AC20 / 0x0041C260) all carry local models until those opt-ins exist.
