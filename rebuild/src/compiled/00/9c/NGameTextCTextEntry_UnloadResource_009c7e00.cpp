// NGameText::CTextEntry::UnloadResource() -- unless the entry's asynchronous bank read is still pending, drop the owner's counted resource pointer (the first word of Owner); always reports true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7E00, length 64 bytes.
// Callees: CBankFileAsyncData::IsFinished() @ 0x009D54E0 (on PFileData's object), the resource's virtual slot 1 (Destroy) when its count at +4 reaches zero.
// Shape: as the landed CEmitterData::UnloadResource @ 0x00AF1440 but the pending case ALSO answers true (`je` to the shared `mov al,1`): the release is guarded by
// `!PFileData || PFileData->IsFinished()`; the release through the owner pointer with the null inside the branch and again after it. Layout: Ego_r.pdb CTextEntry (72 bytes): three CCharStrings, a CWideString, the Tags CArray (16 in the PDB, 12 in retail), PFileData at retail +0x38 (PDB +0x3c), Owner at +0x40; the chunk label "CGroupEntry" is the byte-identical twin at 0x009C7EB0.
class CBankFileAsyncData { public:
    bool IsFinished() const;                       // 0x009D54E0
};
struct CResourceObject_Retail {                    // counted object: vptr, then the reference count
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long m_refCount;                               // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};
struct CIVCountedPointer_Resource {                // the owner's counted resource pointer (one word)
    CResourceObject_Retail* m_p;
    CIVCountedPointer_Resource() : m_p(0) { }
    ~CIVCountedPointer_Resource() { Release(); }
    void Release()
    {
        if (m_p) {
            m_p->Release();
            m_p = 0;
        }
    }
    CIVCountedPointer_Resource& operator=(const CIVCountedPointer_Resource& other)
    {
        Release();
        m_p = other.m_p;
        if (m_p)
            ++m_p->m_refCount;
        return *this;
    }
};
struct CDataBankEntryOwner_Retail {                // Owner: its resource pointer is its first word
    CIVCountedPointer_Resource m_resource;         // +0x00
};

struct CTextEntry {
    unsigned char               _pad[0x38];
    CBankFileAsyncData*         PFileData;         // +0x38 (CCountedPointer<CBankFileAsyncData>::Data)
    unsigned char               _padCount[4];     // the counted pointer's count block
    CDataBankEntryOwner_Retail* Owner;             // +0x40
    bool UnloadResource();
};

bool CTextEntry::UnloadResource()
{
    if (PFileData == 0 || PFileData->IsFinished())
        Owner->m_resource = CIVCountedPointer_Resource();
    return true;
}