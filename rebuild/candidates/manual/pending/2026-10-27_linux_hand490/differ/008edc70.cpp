// CUnprovenSequence_008edc70::Step_008edc70() -- advance a float-table cursor one entry and store the entry truncated to long
// Grade: DIFFER(83v85) -- retail loads Index into ecx before the bool test and copies it to eax (mov eax,ecx) before the spill/compare; built tests the bool first and spills/compares ecx directly. 4 grades: post-increment, ++Index member, local+Table[Index-1], two locals.
// Leaf: ?Step_008edc70@CUnprovenSequence_008edc70@@QAEXXZ
// Retail: Fable.exe @ 0x008EDC70, 85 bytes, thiscall plain `ret`, no arguments.
// Identity (descriptive): no PDB name. The only caller, 0x007C85E2 inside 0x007C85B0 (unreferenced; sits beside
// CTCFireballSpell methods), stores `true` at +0x2E and a count at +0x30 of a sub-object at this+0xC, exactly what the
// neighbour setter 0x007C85A0 does, then calls this function count times. Owning class not proven, so the type and
// the method keep the address suffix.
// Callees: _ftol2 (0x00BFEA70, compiler helper for the float->long casts).
// Layout: local model; +0x00 long result, +0x04 long index, +0x18 float table, +0x28 float limit, +0x2E bool
// "use the integer limit", +0x30 long integer limit. Nothing else is touched.
// Shape: the integer branch compares the index against +0x30; the float branch `fild` of the spilled index against
// +0x28 (`fcomp ; fnstsw ; test ah,5 ; jp` is `<`). Both store Index+1 before loading table[Index].
class CUnprovenSequence_008edc70 {
public:
    void Step_008edc70();

    long Value;                 // +0x00
    long Index;                 // +0x04
    char Unknown08[0x10];       // +0x08
    float* Table;               // +0x18
    char Unknown1C[0xC];        // +0x1C
    float Limit;                // +0x28
    char Unknown2C[2];          // +0x2C
    bool UseCountLimit;         // +0x2E
    long CountLimit;            // +0x30
};

void CUnprovenSequence_008edc70::Step_008edc70()
{
    long index = Index;
    long position = index;
    if (UseCountLimit) {
        if (position < CountLimit) {
            Index = index + 1;
            Value = (long)Table[Index - 1];
        }
    } else if ((float)position < Limit) {
        Index = index + 1;
        Value = (long)Table[Index - 1];
    }
}
