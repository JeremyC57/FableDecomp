## 0x0070CE10

- A local STLport vector whose out-of-line COMDAT ctor's body is NOT visible is treated as escaped (reloads of _M_start after every call/store, own byte for the allocator temp); retail's register-cached _M_start + allocator temp on an arg slot = body visible, __declspec(noinline).

## 0x006E3580

- (wf10, 0x006E3580) A vector begin() inline argument loaded early (into a spare reg, then moved) before a fastcall __find call = the find body is not known pure: put the real bodies of __find AND its comparison callee (CBasicString::Compare 0x00411570) in the TU under auto_inline(off); then the load sinks into the arg register as retail. /Oa also sinks it but changes register choice.

## 0x00966AF0

- VC7.1 inlines a local's inline dtor at returns outside syntactic loops but calls the COMDAT at returns inside a loop body (GetBar 0x00966AF0: while(true){if(!empty){..return true;} return false;}).

## 0x00641C30 (not landed)

0x00641C30 author pass 2: VC7.1 escape analysis on visible same-TU callee body (DrawLight) decides whether a by-ref local arg is kept in registers and stored once at the merge; a frame-slot residue around a const C3DVector& argument => check callee body visibility
- A 'register-promotion' residue on an argument passed by const& can be callee escape analysis: VC7.1 keeps the object in registers only if an earlier same-TU body proves the callee does not leak the address (0x00641C30 needed DrawLight's real body).
- A caller's RELOCATION_MATCH that depends on a callee body being visible (escape analysis) is only landable once that callee's body is itself byte-verified and not reserved elsewhere; withdraw rather than land a 'faithful but unverified' callee.

