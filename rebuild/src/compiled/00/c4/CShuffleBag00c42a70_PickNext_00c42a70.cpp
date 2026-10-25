// CShuffleBag_00c42a70::PickNext() -- with more than one entry, swap a randomly chosen entry from slots 1..count-1 into slot 0 (so the previous pick is never repeated); return slot 0
// Leaf: ?PickNext@CShuffleBag_00c42a70@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C42A70, 49 bytes (+ int3 padding), __fastcall, no arguments.
// Library: libpk audio (no PDB/RTTI); descriptive names. The random helper 0x00C42A20 is `__fastcall`
//   (n in ecx): `(unsigned long)(rand() * (double)n * (1.0 / 32768))` through MSVCR71 rand / _ftol. Reached from
//   0x00C34A83 (the bag at +4 of a sound variation set) and 0x00C34E4A (an entry of the array at +4). The
//   manifest's "Std_Erase_Adjust_Pair" is propagated.
// Shape: `index = Random(count - 1) + 1;` used twice gives the folded `+4` load then `inc eax` for the store
//   (writing `index + 1` at both uses folds both displacements).
unsigned long __fastcall RandomBelow_00c42a70(unsigned long n);   // 0x00C42A20

struct CShuffleBag_00c42a70 {
    long* m_pEntries;                                   // +0
    unsigned long m_capacity;                           // +4
    unsigned long m_count;                              // +8
    long PickNext();
};

long CShuffleBag_00c42a70::PickNext()
{
    if (m_count > 1) {
        unsigned long index = RandomBelow_00c42a70(m_count - 1) + 1;
        long picked = m_pEntries[index];
        m_pEntries[index] = m_pEntries[0];
        m_pEntries[0] = picked;
    }
    return m_pEntries[0];
}