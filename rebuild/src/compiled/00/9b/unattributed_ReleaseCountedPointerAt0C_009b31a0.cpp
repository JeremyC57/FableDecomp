// _unattributed::ReleaseCountedPointerAt0C(SCountedRecord10* record) -- destroy a 16-byte record whose only non-trivial member is an intrusive counted pointer at +0x0c (release it when set, then null it)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B31A0, length 29 bytes.
// Leaf: ReleaseCountedPointerAt0C
// Label correction: the catalog "_Dest_val<..., std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase>>>" is
// propagated (that pair would hold its pointer at +4). The body is the inline CIVCountedPointer destructor applied to
// the member at +0x0c: if set, drop one reference (count at the pointee's +4, virtual slot 1 at zero) and null the
// member -- the shape of the landed counted-pointer release 0x007CA090. No call or address reference to the row was found
// and its neighbours are unrelated template instantiations, so the owning class is not identified; it is modelled as an
// out-of-line element destroy (`p->~T()`, element in ecx, plain `ret`) of a 16-byte record.
// Callees: the pointee's virtual slot 1 (+4) at zero references.
// Shape: the pointer is loaded straight into ecx (the inline Release() member runs on it), the zero store sits inside
// the null test.
struct CIVCountedPointeeBase {
    virtual ~CIVCountedPointeeBase();       // slot 0
    virtual void OnLastReference();         // slot 1 (+0x04)
    unsigned long IVRefCount;               // +0x04
    void Release()
    {
        if (--IVRefCount == 0)
            OnLastReference();
    }
};
template <class T> struct CIVCountedPointer {
    T* m_p;
    ~CIVCountedPointer()
    {
        if (m_p) {
            m_p->Release();
            m_p = 0;
        }
    }
};
struct SCountedRecord10 {                   // 16-byte record (class not identified)
    long m_words[3];                        // +0x00
    CIVCountedPointer<CIVCountedPointeeBase> m_pObject;   // +0x0c
};

void __fastcall ReleaseCountedPointerAt0C(SCountedRecord10* record)
{
    record->~SCountedRecord10();
}