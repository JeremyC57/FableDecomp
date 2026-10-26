// CRecordSetOwner004ae450::`scalar deleting destructor'(unsigned int) -- destroy the record vector at +0x1C, release the counted pointer at +0x18, free the object when bit 0 of the flags is set
// Leaf: ??_GCRecordSetOwner004ae450
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AE450, length 57 bytes (+ int3 padding).
// Callees: the vector destructor @ 0x004AE040 (walks 0x18-byte records destroying two CCharStrings at +4/+8 each; the
// manifest's "CCombatWheel::ResetRings" label is propagated), the pointee's virtual slot 1 (`call [eax+4]`),
// operator delete @ 0x00BFE9BC.
// Label correction: the manifest's "CThingHolySite::`vector_deleting_destructor'" is propagated over byte-identical
// bodies; nothing references this copy, so the class is named by address.
// Layout: local model -- 0x18 bytes of plain members, a CIVCountedPointer at +0x18, a vector of records at +0x1C.
// Shape: a NON-polymorphic class deleted with `delete` (non-virtual `??_G`, no vtable store); members are destroyed in
// reverse order (the vector first, out of line; then the forceinlined counted-pointer release).
struct CCountedObject {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long m_refCount;                                // +0x04
};

template <class T> class CIVCountedPointer { public:
    T* m_p;
    __forceinline ~CIVCountedPointer()
    {
        T* p = m_p;
        if (p) {
            if (--p->m_refCount == 0)
                p->Destroy();
            m_p = 0;
        }
    }
};

struct CRecordVector {                              // vector<0x18-byte record>
    void* m_first;
    void* m_last;
    void* m_end;
    ~CRecordVector();                               // 0x004AE040
};

struct CRecordSetOwner004ae450 {
    long                              m_words[6];   // +0x00
    CIVCountedPointer<CCountedObject> m_object;     // +0x18
    CRecordVector                     m_records;    // +0x1c
};

void DeleteCRecordSetOwner004ae450(CRecordSetOwner004ae450* p)
{
    delete p;
}