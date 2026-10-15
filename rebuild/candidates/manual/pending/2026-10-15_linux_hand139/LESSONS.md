# Lessons from chunk 139 (for GOTCHAS)
- A float result computed from two plain locals is sunk past a following destructor call by VC7.1 (lock guard); retail computing it
  before the destructor means the expression reads a MEMBER (the destructor might change it) -- divide by the member, keep the
  quotient in a named float local (009c32c0).
- `if (!f()) return false;` lets VC reuse f's zero al inline (`jne ; pop ; ret`); retail's separate `xor al,al` exit placed after
  the body is the `if (f()) { ... } else return false;` form (009fdc20).
- A bool flag local whose `xor al,al` sits between two header-byte reads, with both bytes spilled to the frame, is the function's
  FIRST declared local (009f1870).
- A `T()` temporary of a POD-like record with an EMPTY user constructor passed by const& lands at frame byte 1 of an 8-byte frame
  (`lea [esp+0Dh]`); a named uninitialised local lands at byte 0 (006b7760).
- A by-value hidden-pointer iterator returned by find() must be a NAMED local when retail re-reads it from the frame slot (00698840).
- A virtual-call object used by two calls in one loop iteration is a named local per iteration (00445c30); a `g->m_p` member chain
  used as test + call wants a named local to get retail's eax/edx roles (00445a60).
- `ecx = (kind == 1) ? reg : [frame]` before a method call = a ternary between the found component and a never-assigned pointer that
  shares the GetInterface key's frame word (0087b6f0).
- C3DVector with a memberwise user copy constructor and NO operator= : the record's implicit assignment block-copies it through
  re-based pointers while its implicit copy constructor copies word by word into the argument area (__pop_heap a462a0 / a701d0).
- The "CFlammableDef::operator=" / "CGameCameraManager::PushCameraMode" manifest rows at 0x00A462A0, 0x00A701D0, 0x00A34420,
  0x00A71B40, 0x00CBBAD0 are SGI `__pop_heap` instantiations: check the callee's `_Adjust_heap<...>` name before trusting a label.
- Script-entity bool queries: the IsDone guard before an assignment may return true OR false depending on the source
  (e81a60 vs e81ad0): read the exit value from the bytes.
- Lane gaps: a local CDefPointer released from its frame slot (`mov ecx,[slot] ; dec [ecx+4]`) -- every release form gives the
  load/`add eax,4`/dec/store sequence (0069a920, 0069ac90); push_back's empty tag over the live value slot (00f37030);
  shared `return 0.0f` block placement between two guarded arms (00d9fe40).
