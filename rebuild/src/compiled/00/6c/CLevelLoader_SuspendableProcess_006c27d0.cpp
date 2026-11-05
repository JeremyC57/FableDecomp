// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CLevelLoader_SuspendableProcess
extern "C" void ForwardTarget_006c2710();
extern "C" void CLevelLoader_SuspendableProcess() { ForwardTarget_006c2710(); }
// Stand-in (owner rule 2026-11-05): CLevelLoader_SuspendableProcess for CLevelLoader::SuspendableProcess; RTTI vtable slot ?SuspendableProcess@CLevelLoader@@MAEXXZ + PDB name set (egor/fablewin).
