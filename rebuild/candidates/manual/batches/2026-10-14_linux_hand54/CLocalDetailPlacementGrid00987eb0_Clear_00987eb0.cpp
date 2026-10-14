// CLocalDetailPlacementGrid_00987eb0::Clear() -- release the shared grid data at +4: its reference count (+0x8) decremented and the data destroyed through its virtual slot 0 (with 1) at zero; the pointer then cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00987EB0, length 30 bytes.
// Leaf: Clear
// The chunk label is propagated; named by address. Shape: `this` in esi, the count decremented in memory (`dec ; jne`), `ret`.
struct CGridData_00987eb0 {                           // reference-counted, Destroy(flags) in slot 0
    virtual void Destroy(long flags);
    long m_4;
    long m_references;                             // +0x8
};

struct CLocalDetailPlacementGrid_00987eb0 {
    long            m_0;
    CGridData_00987eb0* m_pData;                       // +0x04
    void Clear();
};

void CLocalDetailPlacementGrid_00987eb0::Clear()
{
    CGridData_00987eb0* data = m_pData;
    if (data) {
        if (--data->m_references == 0)
            data->Destroy(1);
    }
    m_pData = 0;
}
