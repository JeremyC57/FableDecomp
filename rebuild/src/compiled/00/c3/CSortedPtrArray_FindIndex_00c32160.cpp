// CSortedPtrArray::FindIndex(const CKeyComparator* comparator, const void* key, unsigned long* found) -- binary search of the sorted pointer array through the comparator's first equal/less pair; returns the match index or the insertion point
// Leaf: FindIndex
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C32160, 191 bytes, __fastcall `ret 0Ch`, eax = index.
//   Descriptive name (no PDB covers this library): the catalog's `BinarySearch_FindInsertionPoint` is kept as the meaning.
//   Twin 0x00C32220 is the same body through comparator slots 2/3.
// Shape: `this` is spilled into the push-ecx slot; *found = 0 on the empty array stores the zero count register;
//   after the loop the last midpoint is recomputed and compared once more (less: insert at mid, else mid + 1).
struct CKeyComparator {
    virtual long IsEqual(const void* key, void* item) const;      // slot 0
    virtual long IsLess(const void* key, void* item) const;       // slot 1
    virtual long IsEqualAlt(const void* key, void* item) const;   // slot 2
    virtual long IsLessAlt(const void* key, void* item) const;    // slot 3
};

struct CSortedPtrArray {
    void* m_owner;                          // +0x00
    void** m_data;                          // +0x04
    unsigned long m_capacity;               // +0x08
    unsigned long m_count;                  // +0x0C
    long FindIndex(const CKeyComparator* comparator, const void* key, unsigned long* found);
};

long CSortedPtrArray::FindIndex(const CKeyComparator* comparator, const void* key, unsigned long* found)
{
    if (m_count == 0) {
        *found = 0;
        return 0;
    }
    long high = m_count - 1;
    long low = 0;
    long mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (comparator->IsEqual(key, m_data[mid])) {
            *found = 1;
            return mid;
        }
        if (comparator->IsLess(key, m_data[mid]))
            high = mid - 1;
        else
            low = mid + 1;
    }
    *found = 0;
    mid = (low + high) / 2;
    if (comparator->IsLess(key, m_data[mid]))
        return mid;
    return mid + 1;
}