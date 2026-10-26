// C3DAnimationInfo::BeginLoading(bool) const -- if the animation data is not loaded, create it and start an asynchronous read of the whole bank entry, then register it as a bank resource
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A26730, 247 bytes (`ret 4`; the bool is unused in this build).
// Signature: Ego_r.pdb `QBEX_N` (const, bool), as declared by the landed caller 0x00693030.
// Callees: CBankFile::GetEntryDataSize(ulong) 0x009CC240, CBankFileAsync::CreateEntry(ulong) 0x009D58D0
// (CCountedPointer<CBankFileAsyncEntry> by hidden pointer; catalog "CEngineWorldMap::CreateNewBankMap"),
// operator new 0x00BFEA1A (0x3C bytes), C3DAnimationInfo::CAnimationData::CAnimationData(const C3DAnimationInfo*)
// 0x00A2D5F0 (the landed ctor calls its argument `size`: it is this owner pointer), the counted data's virtual
// destroy (slot 1), CBankFileAsyncEntry::BeginReadDataAutoAlloc(ulong, ulong, EThreadedFilePriority) 0x009D6100,
// CCountedPointer<CBankFileAsyncData>::operator= 0x009A4440, CResource::SetResourceSize(ulong) 0x009FBF20,
// CResourceBank::AddResource(CResource*) 0x009FC3D0, operator delete 0x00BFE9BC; read priority global 0x01397244.
// Shape: as the landed NParticleEngine::CEmitterData::BeginLoading 0x00AF1360: the entry is a counted-pointer local,
// the read result an unnamed temporary assigned through the out-of-line operator= and released (long form) right
// after; the CIVCountedPointer assignment from `new` is the inline Reset (dead zero store kept) then the store.
// Bank and entry index (an unnamed base subobject in the PDB, +4/+8) are read through inline const accessors, which puts
// the bank load after the hidden-return push of CreateEntry and the size straight into ebx.
// Layout: Ego_r.pdb C3DAnimationInfo: bank +4, entry index +8, Data +0x0C (CIVCountedPointer<CAnimationData>);
// CAnimationData = CResource (intrusive count +4) .. FileData +0x30; the bank's resource bank is at +0x268.
// Leaf: BeginLoading@C3DAnimationInfo

struct SCountBlock {
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
template <class T> class CCountedPointer { public:
    T* Object;                                     // +0
    SCountBlock* Info;                             // +4
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x009A4440
    T* operator->() const { return Object; }
};

template <class T> class CIVCountedPointer { public:
    T* Data;
    void Reset()
    {
        T* p = Data;
        if (p) {
            if (--p->m_references == 0)
                p->Destroy();
            Data = 0;
        }
    }
    CIVCountedPointer& operator=(T* p)
    {
        Reset();
        Data = p;
        return *this;
    }
    T* operator->() const { return Data; }
};

enum EThreadedFilePriority { };
extern EThreadedFilePriority g_AnimationReadPriority;   // 0x01397244
class CBankFileAsyncData;
class CBankFileAsyncEntry { public:
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long offset, unsigned long size, EThreadedFilePriority priority);   // 0x009D6100
};

class CResource { public:
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+4): last reference
    long m_references;                             // +0x04
    unsigned char _body[0x14];                     // to 0x1c
    void SetResourceSize(unsigned long size);      // 0x009FBF20
};
class CResourceBank { public:
    void AddResource(CResource* resource);         // 0x009FC3D0
};
class CBankFileAsync { public:
    unsigned char _pad[0x268];
    CResourceBank ResourceBank;                    // +0x268
    unsigned long GetEntryDataSize(unsigned long index);   // 0x009CC240
    CCountedPointer<CBankFileAsyncEntry> CreateEntry(unsigned long index);   // 0x009D58D0
};

class C3DAnimationInfo { public:
    class CAnimationData : public CResource { public:
        unsigned char _1c[0x14];                   // track map +0x1c, +0x24, +0x28
        CCountedPointer<CBankFileAsyncData> FileData;   // +0x30
        const C3DAnimationInfo* Owner;             // +0x38
        CAnimationData(const C3DAnimationInfo* owner);  // 0x00A2D5F0
    };
    void* __vftable;                               // +0x00
    CBankFileAsync* Bank;                          // +0x04
    unsigned long EntryIndex;                      // +0x08
    mutable CIVCountedPointer<CAnimationData> Data;    // +0x0c
    CBankFileAsync* GetBank() const { return Bank; }
    unsigned long GetEntryIndex() const { return EntryIndex; }
    void BeginLoading(bool wait) const;
};

void C3DAnimationInfo::BeginLoading(bool wait) const
{
    if (Data.Data)
        return;
    unsigned long size = GetBank()->GetEntryDataSize(GetEntryIndex());
    CCountedPointer<CBankFileAsyncEntry> entry = GetBank()->CreateEntry(GetEntryIndex());
    Data = new CAnimationData(this);
    Data->FileData = entry->BeginReadDataAutoAlloc(0, size, g_AnimationReadPriority);
    Data->SetResourceSize(size);
    Bank->ResourceBank.AddResource(Data.Data);
}