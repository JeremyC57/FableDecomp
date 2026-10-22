#include <stdio.h>
#include <string.h>
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
template <class T> struct CCountedPointer {
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

template <class T> struct CIVCountedPointer {
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
struct CBankFileAsyncData;
struct CBankFileAsyncEntry {
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long offset, unsigned long size, EThreadedFilePriority priority);   // 0x009D6100
};

struct CResource {
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+4): last reference
    long m_references;                             // +0x04
    unsigned char _body[0x14];                     // to 0x1c
    void SetResourceSize(unsigned long size);      // 0x009FBF20
};
struct CResourceBank {
    void AddResource(CResource* resource);         // 0x009FC3D0
};
struct CBankFileAsync {
    unsigned char _pad[0x268];
    CResourceBank ResourceBank;                    // +0x268
    unsigned long GetEntryDataSize(unsigned long index);   // 0x009CC240
    CCountedPointer<CBankFileAsyncEntry> CreateEntry(unsigned long index);   // 0x009D58D0
};

struct C3DAnimationInfo {
    struct CAnimationData : CResource {
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

// ---- test stubs ----
EThreadedFilePriority g_AnimationReadPriority = (EThreadedFilePriority)3;
static int g_destroyed[4];
static void __fastcall DestroyEntry(void* p) { (void)p; g_destroyed[0]++; }
static void __fastcall DestroyData(void* p) { (void)p; g_destroyed[1]++; }
static CBankFileAsyncEntry g_entry;
static int g_dataObj;
static SCountBlock* g_entryBlock; static SCountBlock* g_dataBlock;
static unsigned long g_sizeIndex = 99, g_createIndex = 99, g_readOffset = 99, g_readSize, g_resSize; static int g_readPrio;
static int g_nullRead, g_hijack, g_resDestroyed, g_order[8], g_n;
static CResource* g_added;
struct CFakeResource : CResource { };
void CResource::v00() { }
void CResource::Destroy() { g_resDestroyed++; }
static double g_resStore[8];
unsigned long CBankFileAsync::GetEntryDataSize(unsigned long index) { g_order[g_n++] = 1; g_sizeIndex = index; return 0x1234 + index; }
CCountedPointer<CBankFileAsyncEntry> CBankFileAsync::CreateEntry(unsigned long index)
{
    g_order[g_n++] = 2; g_createIndex = index;
    g_entryBlock = new SCountBlock; g_entryBlock->m_references = 2;   // VC7.1 has no NRVO: the local below is released once on return
    g_entryBlock->m_pfnDestroy = DestroyEntry; g_entryBlock->m_pObject = &g_entry;
    CCountedPointer<CBankFileAsyncEntry> r = { &g_entry, g_entryBlock };
    return r;
}
CCountedPointer<CBankFileAsyncData> CBankFileAsyncEntry::BeginReadDataAutoAlloc(unsigned long offset, unsigned long size, EThreadedFilePriority priority)
{
    g_order[g_n++] = 4; g_readOffset = offset; g_readSize = size; g_readPrio = (int)priority;
    if (g_nullRead) { CCountedPointer<CBankFileAsyncData> z = { 0, 0 }; return z; }
    g_dataBlock = new SCountBlock; g_dataBlock->m_references = 2;
    g_dataBlock->m_pfnDestroy = DestroyData; g_dataBlock->m_pObject = &g_dataObj;
    CCountedPointer<CBankFileAsyncData> r = { (CBankFileAsyncData*)&g_dataObj, g_dataBlock };
    return r;
}
template <class T> CCountedPointer<T>& CCountedPointer<T>::operator=(const CCountedPointer<T>& other)
{
    if (other.Info) ++other.Info->m_references;
    if (Info && --Info->m_references == 0) { Info->m_pfnDestroy(Info->m_pObject); delete Info; }
    Object = other.Object; Info = other.Info;
    return *this;
}
C3DAnimationInfo::CAnimationData::CAnimationData(const C3DAnimationInfo* owner)
{
    g_order[g_n++] = 3;
    m_references = 1; FileData.Object = 0; FileData.Info = 0; Owner = owner;
    if (g_hijack) {   // the owner's Data is mutable: occupy it so the assignment's release path runs
        CResource* other = (CResource*)g_resStore;
        memset(g_resStore, 0, sizeof g_resStore);
        *(void**)other = *(void**)this;   // share this object's vtable
        other->m_references = g_hijack;
        owner->Data.Data = (CAnimationData*)other;
    }
}
void CResource::SetResourceSize(unsigned long size) { g_order[g_n++] = 5; g_resSize = size; }
void CResourceBank::AddResource(CResource* resource) { g_order[g_n++] = 6; g_added = resource; }

static int fails;
static void check(int ok, const char* what) { if (!ok) { char b[160]; _snprintf(b, sizeof b, "FAIL %s", what); puts(b); fails++; } }
static double g_bankStore[0x280 / 8];
int main()
{
    CBankFileAsync* bank = (CBankFileAsync*)g_bankStore;
    C3DAnimationInfo info; memset(&info, 0, sizeof info);
    info.Bank = bank; info.EntryIndex = 7;
    // 1: not loaded -> create, read, size, register
    info.BeginLoading(false);
    C3DAnimationInfo::CAnimationData* d = info.Data.Data;
    check(d != 0 && d->Owner == &info, "data created");
    check(g_n == 6 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 3 && g_order[3] == 4 && g_order[4] == 5 && g_order[5] == 6, "order");
    check(g_sizeIndex == 7 && g_createIndex == 7, "index");
    check(g_readOffset == 0 && g_readSize == 0x123b && g_readPrio == 3, "read args");
    check(d->FileData.Object == (CBankFileAsyncData*)&g_dataObj && d->FileData.Info == g_dataBlock && g_dataBlock->m_references == 1, "file data kept, temp released");
    check(g_resSize == 0x123b && g_added == d, "size + registered");
    check(g_destroyed[0] == 1 && g_destroyed[1] == 0, "entry released at exit");
    // 2: already loaded -> nothing
    g_n = 0; info.BeginLoading(true);
    check(g_n == 0 && info.Data.Data == d, "loaded: early out");
    // 3: release path in the assignment (last reference) + null read result
    info.Data.Data = 0; g_n = 0; g_hijack = 1; g_nullRead = 1; g_resDestroyed = 0;
    info.BeginLoading(false);
    check(g_resDestroyed == 1 && info.Data.Data != 0 && info.Data.Data != (C3DAnimationInfo::CAnimationData*)g_resStore, "old data destroyed, replaced");
    check(info.Data.Data->FileData.Object == 0 && info.Data.Data->FileData.Info == 0, "null read");
    check(g_destroyed[0] == 2, "entry released 2");
    // 4: release path, not the last reference
    info.Data.Data = 0; g_n = 0; g_hijack = 3; g_resDestroyed = 0;
    info.BeginLoading(false);
    check(g_resDestroyed == 0 && ((CResource*)g_resStore)->m_references == 2 && info.Data.Data != (C3DAnimationInfo::CAnimationData*)g_resStore, "old data kept alive");
    if (fails) { puts("00a26730_TEST FAIL"); return 1; }
    puts("00a26730_TEST PASS");
    return 0;
}