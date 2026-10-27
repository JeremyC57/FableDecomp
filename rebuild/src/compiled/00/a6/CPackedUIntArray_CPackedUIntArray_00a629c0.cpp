// CPackedUIntArray::CPackedUIntArray() -- empty packed array: no storage, no entries, zero bit width and bias
// Leaf: ??0CPackedUIntArray@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A629C0, 16 bytes. Ego_r.pdb `??0CPackedUIntArray@@QAE@XZ`; called from the four
//   member constructors at 0x009CD4B8..0x009CD4D0 (landed caller 0x009CD480 binds this name).
// Layout: Ego_r.pdb CPackedUIntArray (16 bytes): PackedInts +0, Size +4, Bits +8 (byte), Bias +0xC; the init list
//   shares one zero register (`xor ecx,ecx`) after `mov eax,ecx` keeps `this` for the return.
class CPackedUIntArray {
public:
    unsigned long* PackedInts;                             // +0x00
    unsigned long  Size;                                   // +0x04
    unsigned char  Bits;                                   // +0x08
    unsigned long  Bias;                                   // +0x0C
    CPackedUIntArray();
};

CPackedUIntArray::CPackedUIntArray() : PackedInts(0), Size(0), Bits(0), Bias(0)
{
}