// CPtrArray::Remove(CPlaybackEntry* value) -- finds the last slot holding the value, shifts the later entries down one and drops the count
// Leaf: Remove
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C35080, 75 bytes (`ret 4`). Descriptive name (no PDB covers libpk): the
//   catalog's `IntArray_RemoveByValue` is a placeholder. Byte-identical to chunk 434's 0x00C1EBF0 / 0x00C1E900: the same
//   pointer-array template {data, capacity, count} instantiated over the element list at +4 of the owning set
//   (vtable 0x012B501C); caller that set's slot-8 remover 0x00C34FF0 (`m_entries.Remove(entry)` before deleting it).
// Shape: Find inlined (backward scan `for (i = count - 1; i >= 0 && idx < 0; --i)`), the shift loop compares unsigned
//   against `count - 1` re-read every pass (the stores may alias the count); an absent value still decrements the
//   count, as retail does.
class CPlaybackEntry;

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

template void CPtrArray<CPlaybackEntry*>::Remove(CPlaybackEntry*);