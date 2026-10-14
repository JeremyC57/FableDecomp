# Batch `2026-10-14_linux_hand04` — hand-authored C3DMeshObject .. C3DVector functions: 40 bodies (36 landed)

Delegated hand-authoring pass (compiler in the loop, Wine lane) over the fifth chunk of
class-labelled open functions of 17–96 bytes: mesh object / partition element / cached file
constructors and deleting destructors, `C3DMeshStats` helper-point lookups (SGI-style
`CPreAllocVectorMap` with `vector::erase` inlined as `__copy` + trailing destructor), camera-mode
script module constructors, `CSimVoice::HasVoice`, `CTCCarrying::HasCarrySlot`, `C3DMeshVolume`
and `C3DPrimitive2::UploadStaticBuffers`. Most manifest labels were propagated and were corrected
from the stored vtables and PDB layouts; four rows landed `_unattributed` with descriptive leaves.

Lessons recorded by the pass: a vtable store before two zero pointer stores is a `CCountedPointer`
sub-object with an inline zeroing constructor; a deleting destructor that inlines a member's
`~CCountedPointer` needs the class's virtual destructor declared in-class and defined out of line;
three straight dword moves are a user memberwise `C3DVector` copy constructor; an empty class
passed by const reference is built in a dead argument slot and needs `S() { }` so no zero byte is
stored; a bare `setne al` epilogue is an `unsigned char` result. Four bodies differ only in the
x87 load order of the three vector components (`Normalise` shapes) or a register choice.

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
