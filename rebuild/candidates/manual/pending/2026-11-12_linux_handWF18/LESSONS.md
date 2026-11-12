## 0x008A7A70

- VC7.1 frame: a C2DVector temporary of operator- keeps its own 8-byte slot when its X is spilled; float locals spill into a shared 4-byte slot (row 0x008A7A70: sub esp,38h -> 30h).

## 0x00724630

- A Min/Max operand order that only flips with real code removals (not parens) can hinge on how a call is spelled: `delete p` vs `operator delete(p)` emit identical bytes but change VC7.1's x87 operand choice elsewhere (row 0x00724630).

## 0x00AC53F0

- (0x00AC53F0) A register/layout residue in a ctor that calls a same-TU private helper vanished once that helper's REAL body was visible under #pragma auto_inline(off).

