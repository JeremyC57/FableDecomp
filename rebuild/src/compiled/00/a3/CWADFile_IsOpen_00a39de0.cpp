// `return this->Index != 0;` (xor/test/setne). __fastcall this=ecx.
// Leaf: ?IsOpen@CWADFile_Methods@@QAE_NXZ
#include "engine/CWADFile.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
struct CWADFile_Methods : CWADFile {
    bool IsOpen();
};
#pragma pack(pop)
bool CWADFile_Methods::IsOpen() { return this->Index != 0; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsOpen@CWADFile@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
