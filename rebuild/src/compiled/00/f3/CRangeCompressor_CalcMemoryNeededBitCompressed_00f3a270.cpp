// CRangeCompressor::CalcMemoryNeededBitCompressed(const CArray<unsigned long>& values) -- bytes needed to store every value with the bit width of the widest one, rounded up to whole dwords
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3A270, length 99 bytes (`ret 4`; `this` is unused,
// CRangeCompressor is an empty class in the PDB). No callees.
// Layout: retail CArray<T> is three pointers (begin +0, end +4, capacity +8; see the landed 0x007003A0).
// Shape: the OR mask starts from element 0 and ORs every element (`mov edx,[ecx]` before the count test), the
// unsigned count gives `jbe`; the bit scan is a count-down `for` from bit 31 that breaks on the first set bit with
// the width `bit + 1`, and the size is the unsigned `(Size() * bits + 31) / 32 * 4`.
// Size() is called at the loop test and again in the result (a count local changes the register allocation).
// Leaf: CalcMemoryNeededBitCompressed
template<class T> struct CArray {                       // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
    const T& operator[](unsigned long i) const { return m_begin[i]; }
};

struct CRangeCompressor {
    long CalcMemoryNeededBitCompressed(const CArray<unsigned long>& values);
};

long CRangeCompressor::CalcMemoryNeededBitCompressed(const CArray<unsigned long>& values)
{
    unsigned long mask = values[0];
    unsigned long bits = 0;
    for (unsigned long i = 0; i < values.Size(); ++i)
        mask |= values[i];
    for (long bit = 31; bit >= 0; --bit) {
        if (mask & (1 << bit)) {
            bits = bit + 1;
            break;
        }
    }
    return (values.Size() * bits + 31) / 32 * 4;
}