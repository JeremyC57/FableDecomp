// CPackedUIntArray::Sizeof() -- byte size of the array object plus its packed storage: 0x10 + ceil(count * bitsPerEntry / 32) words
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A62A60, length 22 bytes. No callees.
// Layout as the landed constructor/destructor (0x00A629D0) use it: +0 buffer, +4 entry count,
// +8 bits per entry (a byte: `movzx`), +0xc capacity; sizeof == 0x10, which is the `lea [eax*4+10h]`
// constant. Unsigned arithmetic gives `add 1Fh ; shr 5` for the word count (a signed count would
// round with `cdq`/`sar`).
struct CPackedUIntArray {
    unsigned long* m_pData;                 // +0x00
    unsigned long  m_count;                 // +0x04
    unsigned char  m_bitsPerEntry;          // +0x08
    unsigned long  m_capacity;              // +0x0c
    unsigned long Sizeof() const;
};

unsigned long CPackedUIntArray::Sizeof() const
{
    return sizeof(CPackedUIntArray) + ((m_bitsPerEntry * m_count + 31) >> 5) * sizeof(unsigned long);
}
