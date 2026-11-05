// Forwarder to a member of the Bank-object pointer at this+0x4. VC7.1 tail-calls
// Leaf: ?GetAlignment@CWADFile_Methods@@QAEXXZ
// a void member->void member forward, so retail is `mov ecx,[ecx+d]; jmp rel32`.
#include "engine/CWADFile.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
class CBankFileAsync { public: void Run(); };
struct CWADFile_Methods : CWADFile {
    void GetAlignment();
};
#pragma pack(pop)
void CWADFile_Methods::GetAlignment() { this->Bank->Run(); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetAlignment@CWADFile@@UBEJXZ); evidence: RTTI vtable slot + PDB name set.
