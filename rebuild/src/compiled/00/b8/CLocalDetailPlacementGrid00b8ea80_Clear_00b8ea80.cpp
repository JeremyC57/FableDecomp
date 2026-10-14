// CLocalDetailPlacementGrid_00b8ea80::Clear() -- release the shared grid data at +4: its reference count (+0x10) decremented and the data destroyed through its virtual slot 0 (with 1) at zero; the pointer then cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8EA80, length 30 bytes.
// Leaf: Clear
// The chunk label is propagated; named by address. Shape: `this` in esi, the count decremented in memory (`dec ; jne`), `ret`.
struct CGridData_00b8ea80 {                           // reference-counted, Destroy(flags) in slot 0
    virtual void Destroy(long flags);
    long m_4, m_8, m_c;
    long m_references;                             // +0x10
};

struct CLocalDetailPlacementGrid_00b8ea80 {
    long            m_0;
    CGridData_00b8ea80* m_pData;                       // +0x04
    void Clear();
};

void CLocalDetailPlacementGrid_00b8ea80::Clear()
{
    CGridData_00b8ea80* data = m_pData;
    if (data) {
        if (--data->m_references == 0)
            data->Destroy(1);
    }
    m_pData = 0;
}