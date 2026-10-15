// CRangeCompressor::CalcShuffleOperations(unsigned long mask, unsigned long* runMasks, unsigned long* shifts) -- split a bit mask into runs of set bits: each run's mask and the right shift that packs it down; returns the run count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F39E50, length 122 bytes (`ret 0Ch`; `this` is unused,
// CRangeCompressor is an empty class in the PDB). No callees.
// Shape: the outer bit index runs 0..31; on a set bit an inner `for` extends the run while the bit stays set
// (`end < 32 && (mask & (1 << end))`, the shifted bit CSE'd into esi), stores the run mask and the shift
// (`bit - removed`, removed = bits squeezed out by earlier runs), then resumes after the run. VC strength-reduces
// the two output arrays into one walking pointer plus the pointer difference homed in the dead argument slot;
// the count lives in the `push ecx` frame word.
// Leaf: CalcShuffleOperations
struct CRangeCompressor {
    long CalcShuffleOperations(unsigned long mask, unsigned long* runMasks, unsigned long* shifts);
};

long CRangeCompressor::CalcShuffleOperations(unsigned long mask, unsigned long* runMasks, unsigned long* shifts)
{
    long count = 0;
    long removed = 0;
    for (long bit = 0; bit < 32; ++bit) {
        if (mask & (1 << bit)) {
            unsigned long run = 0;
            long end;
            for (end = bit; end < 32 && (mask & (1 << end)); ++end)
                run |= 1 << end;
            runMasks[count] = run;
            shifts[count] = bit - removed;
            ++count;
            removed += end - bit;
            bit = end;
        }
    }
    return count;
}