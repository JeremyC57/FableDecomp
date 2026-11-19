#ifndef FABLE_ENGINE_CPACKEDUINTARRAY_H
#define FABLE_ENGINE_CPACKEDUINTARRAY_H
#ifdef FABLE_RETAIL_CPACKEDUINTARRAY_CALCULATEBIAS
#include <string.h>
#endif
// CPackedUIntArray (new, chunk 849, row 0x009CF3A0 CBankFile::WriteSymbolHeaderFile): Ego_r.pdb layout (struct_layouts_egor.tsv, 16
// bytes, no containers so the offsets are retail's): PackedInts +0, Size +4, Bits +8, Bias +0xC. Retail reads Size in place
// (`cmp dword ptr [ebx+5Ch],0` = CBankFile::PackedDataType +0x58 +4) and calls the out-of-line const index operator
// ??ACPackedUIntArray@@QBEKK@Z 0x00A629F0 (Ego_r.pdb / FableWin.pdb name sets; bound in reloc_bindings.tsv). GetSize is the inline
// Ego_r ?GetSize@CPackedUIntArray@@QBEKXZ.
class CPackedUIntArray {
public:
    unsigned long operator[](unsigned long index) const;            // 0x00A629F0
    unsigned long GetSize() const { return Size; }
#ifdef FABLE_RETAIL_CPACKEDUINTARRAY_PACK
    // NF72 opt-in (row 0x009CD740 CBankFile::PackRuntimeData): Ego_r.pdb / FableWin.pdb ?Initialise@CPackedUIntArray@@QAEXPBXKJK@Z
    // (retail 0x00A62D80), ?Sizeof@CPackedUIntArray@@QBEKXZ (0x00A62A60, landed CPackedUIntArray::Sizeof) and ?Clear@CPackedUIntArray@@QAEXXZ
    // (0x00A629D0), all out of line. Declarations only: no layout change.
    void Initialise(const void* data, unsigned long bits, long stride, unsigned long count);   // 0x00A62D80
    unsigned long Sizeof() const;                                   // 0x00A62A60
    void Clear();                                                   // 0x00A629D0
#endif
#ifdef FABLE_RETAIL_CPACKEDUINTARRAY_CALCULATEBIAS
    // hand2c opt-in (row 0x00A62AE0): Ego_r.pdb ?CalculateBias@CPackedUIntArray@@ABEKPBXKJK@Z (private const), the private static
    // __fastcall ?ReadNBytes@CPackedUIntArray@@CIXAAKPBEK@Z (inline in retail: 0x00A62B15 `cmp edx,4 ; mov ecx,[ebx]` else the
    // memcpy intrinsic `rep movsd ; rep movsb`; Ego_r 0x00C03430 calls memcpy there, FableWin 0x033AC5D0 is its n == 4 test) and the
    // private static table ?BitMasks@CPackedUIntArray@@0PAKA (0x013979E8; Ego_d.pdb field list: BitMasks private static).
    // ReadNBytes is written without its __fastcall (inline at its only retail site, so the convention emits nothing; keeps g++ builds).
private:
    unsigned long CalculateBias(const void* src_array, unsigned long src_bits, long src_stride, unsigned long count) const;   // 0x00A62AE0
    static void ReadNBytes(unsigned long& dest, const unsigned char* src, unsigned long n)
    {
        if (n == 4)
            dest = *(const unsigned long*)src;
        else if (n > 0)
            memcpy(&dest, src, n);
    }
    static unsigned long BitMasks[];                                 // 0x013979E8
#endif
private:
    unsigned long* PackedInts;                                      // +0x00
    unsigned long Size;                                             // +0x04
    unsigned char Bits;                                             // +0x08
    unsigned long Bias;                                             // +0x0C
};
#endif
