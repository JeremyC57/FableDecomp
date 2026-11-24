// `return this->Index != 0;` (xor/test/setne). __fastcall this=ecx.
// Leaf: ?IsOpen@CWADFile@@UBE_NXZ
#define FABLE_RETAIL_CWADFILE   // alias route: engine/CWADFile.h declares IsOpen under this class-model switch
#include "engine/CWADFile.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
#pragma pack(pop)
bool CWADFile::IsOpen() const { return this->Index != 0; }
// Was defined on a stand-in alias class (owner rule 2026-11-05); now the real member, through the class-model switch above (alias route, 2026-11-23).
