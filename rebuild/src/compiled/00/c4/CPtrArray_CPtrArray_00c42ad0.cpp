// CPtrArray::~CPtrArray() -- empties the pointer array by removing its last entry until the count is zero (the entries are not owned), then frees the data block
// Leaf: ??1?$CPtrArray@PAUCTableEntry@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C42AD0, 122 bytes (+ int3 padding), __fastcall `ret`, no EH frame.
// Library: libpk (no PDB/RTTI): descriptive names. Callers: the table destructor 0x00C349C0 (vtable 0x012B4FBC,
//   one of the four CItemTable tables of agent 437's 0x00C348F0; the array is its member at +4) and the
//   array's own scalar deleting destructor 0x00C34CD0. The catalog's `Container_RemoveLastElement` is a placeholder.
// Layout: {data +0, capacity +4, count +8} -- the CPtrArray of agent 434 (Remove 0x00C1E900 / 0x00C1EBF0) and
//   agent 437 (0x00C34B80, which inlines this same loop into the owner's destructor).
// Shape: `for (count = m_count; count; count = m_count) Remove(m_data[count - 1])` -- the tested count register is
//   the index (a plain `while (m_count)` re-reads the count first and swaps edx/edi); Remove (find-last + shift +
//   `--count`) inlined; then `if (m_data) { delete[] m_data; m_data = 0; }` (operator delete[] @ 0x00BFEB1C).
void operator delete[](void* p) throw();

struct CTableEntry;

template <class T>
struct CPtrArray {
    T* m_data;                              // +0
    unsigned long m_capacity;               // +4
    unsigned long m_count;                  // +8
    ~CPtrArray();
    long Find(T value)
    {
        long index = -1;
        for (long i = m_count - 1; i >= 0 && index < 0; --i)
            if (m_data[i] == value)
                index = i;
        return index;
    }
    void Remove(T value)
    {
        for (unsigned long i = Find(value); i < m_count - 1; ++i)
            m_data[i] = m_data[i + 1];
        --m_count;
    }
};

template <class T>
CPtrArray<T>::~CPtrArray()
{
    for (unsigned long count = m_count; count != 0; count = m_count)
        Remove(m_data[count - 1]);
    if (m_data) {
        delete[] m_data;
        m_data = 0;
    }
}

template struct CPtrArray<CTableEntry*>;