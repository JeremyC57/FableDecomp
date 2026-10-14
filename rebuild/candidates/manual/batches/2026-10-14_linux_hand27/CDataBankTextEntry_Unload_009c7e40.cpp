// CDataBankTextEntry::Unload() -- drop the entry's counted pointer: release the object (virtual slot 1 destroys it at zero) and null the pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7E40, length 33 bytes.
// Callees: the counted object's virtual slot 1 (Destroy) when its count at +4 reaches zero.
// Shape: the inlined CIVCountedPointer release -- `test ecx,ecx ; je`, `dec [ecx+4] ; jne`, `call [eax+4]`, then the
// pointer nulled inside the branch -- followed by a second unconditional null store: the assignment of a
// default (null) counted pointer, whose "add a reference" arm is folded away because the source is known null.
// PDB: CDataBankTextEntry { CIVCountedPointer<NGameText::CTextEntry> PText +0 }. The body is byte-identical to the
// sibling entry type's Unload (0x009C7D60 / 0x009C7E40); the class is taken from the manifest label.
struct CTextEntry {                            // counted object: vptr, then the reference count
    virtual void v00();
    virtual void Destroy();              // slot 1
    long m_refCount;                     // +0x04
    void Release()                       // `this` = the object (ecx) for the virtual call
    {
        if (--m_refCount == 0)
            Destroy();
    }
};

struct CIVCountedPointer_CTextEntry {
    CTextEntry* m_p;
    CIVCountedPointer_CTextEntry() : m_p(0) { }
    ~CIVCountedPointer_CTextEntry() { Release(); }
    void Release()
    {
        if (m_p) {
            m_p->Release();
            m_p = 0;
        }
    }
    CIVCountedPointer_CTextEntry& operator=(const CIVCountedPointer_CTextEntry& other)
    {
        Release();
        m_p = other.m_p;
        if (m_p)
            ++m_p->m_refCount;
        return *this;
    }
};

struct CDataBankTextEntry {
    CIVCountedPointer_CTextEntry PText;        // +0x00
    void Unload();
};

void CDataBankTextEntry::Unload()
{
    PText = CIVCountedPointer_CTextEntry();
}
