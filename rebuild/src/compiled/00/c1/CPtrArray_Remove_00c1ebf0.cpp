// CPtrArray::Remove(CDisplayElement* value) -- finds the last slot holding the value, shifts the later entries down one and drops the count
// Leaf: Remove
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1EBF0, 75 bytes (`ret 4`). Descriptive name (no PDB covers this library): the
//   manifest's "Vector_RemoveByValue" is a generic placeholder. Byte-identical to 0x00C1E900: the same pointer-array
//   template {data, capacity, count} instantiated over the owner's display-element list (+0x2C); caller the display
//   element's destructor 0x00C1EB10 (`m_pOwner->m_elements.Remove(this)`), the counterpart of the constructor
//   0x00C1EAA0 that adds it.
// Shape: Find inlined (backward scan `for (i = count - 1; i >= 0 && idx < 0; --i)`), the shift loop compares unsigned
//   against `count - 1` re-read every pass (the stores may alias the count); an absent value still decrements the
//   count, as retail does.
struct CDisplayElement;

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

template void CPtrArray<CDisplayElement*>::Remove(CDisplayElement*);