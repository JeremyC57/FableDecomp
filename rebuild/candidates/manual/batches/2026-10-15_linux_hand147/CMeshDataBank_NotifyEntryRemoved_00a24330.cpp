// CMeshDataBank::NotifyEntryRemoved(unsigned long) -- an entry left the bank: drop its dynamic-info counted pointer (when in range) and clear its slot in the entry table
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A24330, 128 bytes (padding included); vtable 0x0129CE94 slot 14.
// Leaf: ?NotifyEntryRemoved@CMeshDataBank@@MAEXK@Z
// Shape: `index < DynamicInfoBuffer.size()` (+0x3b0, 8-byte CCountedPointer<CMeshDataBankEntry>
// elements, unsigned) selects the element through STLport's `*(begin() + n)` and assigns null to it:
// inline release (count re-read after the decrement; deleter +4 on the object +8, block freed by
// operator delete 0x00BFE9BC) then block and object zeroed. Either way MeshesAndAnimations[index]
// (+0x380) = 0; VC duplicates that store into both arms. Retail CArray is 12 bytes (PDB 16), so the
// arrays sit 0x74 + 4n below the PDB offsets.
struct CMeshDataBankEntry;

struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4
    void* m_pObject;                                // +8
};

template <class T> struct CCountedPointer {
    T*           Data;                              // +0
    SCountBlock* m_pCount;                          // +4
    CCountedPointer& operator=(T* object)           // used with null
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pCount = 0;
        Data = object;
        return *this;
    }
};

template <class T> struct CArray {                  // STLport vector
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T* begin() { return _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

struct CMeshDataBank {
    virtual void v00();
    unsigned char _pad004[0x380 - 4];
    CArray<CMeshDataBankEntry*> MeshesAndAnimations;                 // +0x380
    unsigned char _pad38c[0x3b0 - 0x38c];                             // MeshInfoBuffer, PhysicsMeshInfoBuffer, AnimationInfoBuffer
    CArray<CCountedPointer<CMeshDataBankEntry> > DynamicInfoBuffer;   // +0x3b0
protected:
    virtual void NotifyEntryRemoved(unsigned long index);   // protected virtual (PDB symbol ...@@MAEXK@Z)
};

void CMeshDataBank::NotifyEntryRemoved(unsigned long index)
{
    if (index < DynamicInfoBuffer.size())
        DynamicInfoBuffer[index] = 0;
    MeshesAndAnimations[index] = 0;
}
