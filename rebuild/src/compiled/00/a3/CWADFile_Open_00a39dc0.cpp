// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CWADFile_Open
extern "C" void ForwardTarget_00a39cc0();
extern "C" void CWADFile_Open() { ForwardTarget_00a39cc0(); }
// Stand-in (owner rule 2026-11-05): CWADFile_Open for CWADFile::Open; RTTI vtable slot ?Open@CWADFile@@UAEXABVCWideString@@W4FileAccessType@@K@Z + PDB name set (egor/fablewin).
