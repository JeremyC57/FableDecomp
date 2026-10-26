// CTCVillage::OnInitialActivate() -- a village restored in limbo re-applies its limbo state on activation
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065CE88, length 19 bytes. CTCVillage vtable 0x0125A9AC slot 6, the CTCBase
// OnInitialActivate slot (PDB `?OnInitialActivate@CTCVillage@@UAEXXZ`); the catalog's "vfunc_6" is the slot name.
// Callees: CTCVillage::SetVillageLimbo(bool) @ 0x0065C36E.
// Shape: the flag is read once into a zero-extended register (`xor eax,eax ; mov al,[ecx+0B2h]`) that serves both the
// test and the pushed argument; the call keeps `call ; ret` (the callee pops an argument this function does not have).
// Layout: CTCVillage.h is the PDB layout (Limbo +0x13E); with the 4-byte retail lists / 12-byte set it is +0xB2.
class CTCVillage { public:
    void*         __vftable;                // +0x00
    unsigned char _pad_0x04[0xAE];
    bool          Limbo;                    // +0xB2
    void SetVillageLimbo(bool limbo);       // 0x0065C36E
    void OnInitialActivate();
};

void CTCVillage::OnInitialActivate()
{
    if (Limbo)
        SetVillageLimbo(Limbo);
}