// CFlagOverride::ApplyTo(CFlagState& target) -- apply the override's two (mask, value) word pairs to a flag state: group A from +0x08/+0x0A, then group B from +0x04/+0x06; returns the target
// Leaf: ApplyTo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2C670, length 64 bytes (57 + int3 padding), __fastcall, `ret 4`.
// Callers: the libpk object at 0x00C14A80 (overrides at +0x78 and +0x84 / +0x90 applied to its argument) and 0x00C2C7CF
//   (an override at +0x38), 0x00C383EF; all drop the result.
// Callees: the flag state's masked setters SetGroupA(mask, value) @ 0x00C27D40 and SetGroupB(mask, value) @ 0x00C27D80
//   (each ORs its lock word into the mask and merges `(current & locked) | (mask & value)` through the +0x08 member).
// Name: no PDB covers libpk; the names are descriptive (the catalog's `CClothingItem_ValidateGeometry` is propagated).
// Shape: `unsigned short` arguments are pushed zero-extended (`xor eax,eax ; mov ax,[esi+0Ah]`), the value before the mask
//   (right to left); the target reference is loaded into edi after the first two field loads and returned in eax.
class CFlagState {
public:
    void SetGroupA(unsigned short mask, unsigned short value);   // 0x00C27D40
    void SetGroupB(unsigned short mask, unsigned short value);   // 0x00C27D80
};

class CFlagOverride {
public:
    CFlagState& ApplyTo(CFlagState& target) const;
private:
    unsigned char  _pad_0x00[4];            // +0x00 the vtable pointer (0x012B4630, base 0x012B2828, stored by the ctor 0x00C2C6B0);
                                            //   left opaque: ApplyTo is not virtual and calls none
    unsigned short m_maskB;                 // +0x04
    unsigned short m_valueB;                // +0x06
    unsigned short m_maskA;                 // +0x08
    unsigned short m_valueA;                // +0x0a
};

CFlagState& CFlagOverride::ApplyTo(CFlagState& target) const
{
    target.SetGroupA(m_maskA, m_valueA);
    target.SetGroupB(m_maskB, m_valueB);
    return target;
}