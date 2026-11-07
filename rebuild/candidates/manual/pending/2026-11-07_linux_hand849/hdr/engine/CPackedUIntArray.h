#ifndef FABLE_ENGINE_CPACKEDUINTARRAY_H
#define FABLE_ENGINE_CPACKEDUINTARRAY_H
// CPackedUIntArray (new, chunk 849, row 0x009CF3A0 CBankFile::WriteSymbolHeaderFile): Ego_r.pdb layout (struct_layouts_egor.tsv, 16
// bytes, no containers so the offsets are retail's): PackedInts +0, Size +4, Bits +8, Bias +0xC. Retail reads Size in place
// (`cmp dword ptr [ebx+5Ch],0` = CBankFile::PackedDataType +0x58 +4) and calls the out-of-line const index operator
// ??ACPackedUIntArray@@QBEKK@Z 0x00A629F0 (Ego_r.pdb / FableWin.pdb name sets; bound in reloc_bindings.tsv). GetSize is the inline
// Ego_r ?GetSize@CPackedUIntArray@@QBEKXZ.
class CPackedUIntArray {
public:
    unsigned long operator[](unsigned long index) const;            // 0x00A629F0
    unsigned long GetSize() const { return Size; }
private:
    unsigned long* PackedInts;                                      // +0x00
    unsigned long Size;                                             // +0x04
    unsigned char Bits;                                             // +0x08
    unsigned long Bias;                                             // +0x0C
};
#endif
