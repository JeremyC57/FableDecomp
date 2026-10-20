// Grade: DIFFER(306v314) -- register allocation: retail keeps the sizes reference in edx (reloaded after each call) with its begin in edi and spills the available-slot pointer into the dead `start` slot; ours keeps sizes in edi and re-indexes the slot
// CVertexGroupBlockOptimiser::CalcMinimumBlocks(const CArray<unsigned long>&, unsigned long&, CArray<bool>&, unsigned long, unsigned long) -- recursive search for the minimum number of blocks: try every still-available size from `start` that fits the current block (MaxN), recurse, keep the minimum; when nothing fits, open a new block with the first available size
// Leaf: CalcMinimumBlocks
template <class T> struct CArray {                 // retail 12-byte STLport-style vector
    T* m_first;
    T* m_last;
    T* m_end;
    T* begin() const { return m_first; }
    unsigned long size() const { return m_last - m_first; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
template <class T> inline const T& min_of(const T& a, const T& b) { return b < a ? b : a; }   // STLport std::min
struct CVertexGroupBlockOptimiser {                // retail layout (see the landed InsertBlock 0x00AD92E0)
    unsigned char _pad00[0x44];
    unsigned long MaxN;                            // +0x44
    unsigned long CalcMinimumBlocks(const CArray<unsigned long>& sizes, unsigned long& current, CArray<bool>& available,
                                    unsigned long remaining, unsigned long start);
};
unsigned long CVertexGroupBlockOptimiser::CalcMinimumBlocks(const CArray<unsigned long>& sizes, unsigned long& current,
                                                            CArray<bool>& available, unsigned long remaining, unsigned long start)
{
    if (remaining == 0)
        return 0;
    bool noneFitted = true;
    unsigned long best = sizes.size();
    for (unsigned long i = start; i < sizes.size(); ++i) {
        bool& slot = available[i];
        if (slot) {
            unsigned long size = sizes[i];
            if (current + size <= MaxN) {
                slot = false;
                current += size;
                remaining -= size;
                noneFitted = false;
                unsigned long blocks = CalcMinimumBlocks(sizes, current, available, remaining - 1, i + 1);
                best = min_of(best, blocks);
                current -= size;
                available[i] = true;
                remaining += size;
            }
        }
    }
    if (noneFitted) {
        for (unsigned long i = 0; i < sizes.size(); ++i) {
            if (available[i]) {
                unsigned long size = sizes[i];
                available[i] = false;
                remaining -= size;
                unsigned long fresh = size;
                best = CalcMinimumBlocks(sizes, fresh, available, remaining - 1, i + 1) + 1;
                available[i] = true;
                return best;
            }
        }
    }
    return best;
}
