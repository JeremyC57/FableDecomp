// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CBankFile_Close
extern "C" void ForwardTarget_009cd850();
extern "C" void CBankFile_Close() { ForwardTarget_009cd850(); }
// Stand-in (owner rule 2026-11-05): CBankFile_Close for CBankFile::Close; RTTI vtable slot ?Close@CBankFile@@UAEXXZ + PDB name set (egor/fablewin).
