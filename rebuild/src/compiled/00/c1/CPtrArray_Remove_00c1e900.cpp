// CPtrArray::Remove(CBlendEntry* value) -- finds the last slot holding the value, shifts the later entries down one and drops the count
// Leaf: Remove
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1E900, 75 bytes (`ret 4`). Descriptive name (no PDB covers this library): the
//   manifest's "EHeroMorphType_FindAndRemoveEntry" is propagated. A pointer-array template {data, capacity, count}
//   whose Grow (0x00C1E640: new[] of capacity + n, rep movsd, delete[]) and Find (0x00C1E6A0) are out of line elsewhere;
//   this instantiation serves the blend lists (caller RemoveBlend 0x00C1E7D0). 0x00C1EBF0 is the byte-identical
//   instantiation over the display-element list.
// Shape: Find inlined (backward scan `for (i = count - 1; i >= 0 && idx < 0; --i)`), the shift loop compares unsigned
//   against `count - 1` re-read every pass (the stores may alias the count); an absent value still decrements the
//   count, as retail does.
struct CBlendEntry;

template <class T>
struct CPtrArray {
    T* m_data;                              // +0
    unsigned long m_capacity;               // +4
    unsigned long m_count;                  // +8
    long Find(T value)
    {
        long index = -1;
        for (long i = m_count - 1; i >= 0 && index < 0; --i)
            if (m_data[i] == value)
                index = i;
        return index;
    }
    void Remove(T value);
};

template <class T>
void CPtrArray<T>::Remove(T value)
{
    for (unsigned long i = Find(value); i < m_count - 1; ++i)
        m_data[i] = m_data[i + 1];
    --m_count;
}

template void CPtrArray<CBlendEntry*>::Remove(CBlendEntry*);