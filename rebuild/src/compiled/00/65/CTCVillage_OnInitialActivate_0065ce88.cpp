// CTCVillage::OnInitialActivate() -- a village restored in limbo re-applies its limbo state on activation
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065CE88, length 19 bytes. CTCVillage vtable 0x0125A9AC slot 6, the CTCBase
// OnInitialActivate slot (PDB `?OnInitialActivate@CTCVillage@@UAEXXZ`); the catalog's "vfunc_6" is the slot name.
// Callees: CTCVillage::SetVillageLimbo(bool) @ 0x0065C36E.
// Shape: the flag is read once into a zero-extended register (`xor eax,eax ; mov al,[ecx+0B2h]`) that serves both the
// test and the pushed argument; the call keeps `call ; ret` (the callee pops an argument this function does not have).
// Layout: CTCVillage.h is the PDB layout (Limbo +0x13E); with the 4-byte retail lists / 12-byte set it is +0xB2.
// Layout (supersedes any local-model note above): engine/CTCVillage.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCVillage CTCVillageDonor
#include "engine/CTCVillage.h"
#undef CTCVillage
class CTCVillage : public CTCVillageDonor {
 public:
    void SetVillageLimbo(bool limbo);       // 0x0065C36E
    void OnInitialActivate();
};

void CTCVillage::OnInitialActivate()
{
    if (Limbo)
        SetVillageLimbo(Limbo);
}