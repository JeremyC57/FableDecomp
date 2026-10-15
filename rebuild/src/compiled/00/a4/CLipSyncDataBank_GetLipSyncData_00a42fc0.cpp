// CLipSyncDataBank::GetLipSyncData(long index) -- make sure entry `index` is decoded (start loading it when the bank holds it, build it), touch it in its resource list and return its counted data pointer
// Leaf: GetLipSyncData
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A42FC0, 155 bytes. Callees: CBankFile::EntryExists 0x009CC190 (bool),
// CLipSyncDataBankEntry::BeginLoading 0x00A42980 (index, bank), CLipSyncDataBankEntry::CData::BuildLipSync
// 0x00A42EA0, CResource::ListRemove 0x009FBFB0, CResource::ListInsert 0x009FBF90.
// The catalog name "C3DAnimationInfo::GetAnimationStats" is propagated from the byte-similar 0x00A269C0; this
// body indexes the bank's CArray<CLipSyncDataBankEntry> (8-byte entries {CIVCountedPointer<CData> Data,
// float Duration}) at retail +0x164 (the PDB's CLipSyncDataBank puts Entries at +0x1D8) and `ret 8`
// (hidden result + index). CData derives from CResource (list +8, LastUsedFrame +0x18) and holds the
// result counted pointer at +0x1C. Shape: the entry address is a named local computed before the bank call
// (`lea ebx,[ebp*8]` kept for the re-indexed null test), the build test reads the data through the entry,
// the touch block is the inline resource-list move-to-tail, the result the add-ref copy constructor.
// The re-indexed slot is a REFERENCE local inside the `if` (`mov eax,[ecx+ebx] ; add ecx,ebx ; test eax,eax`);
// `Entries[index].Data == 0` in the condition gives `lea ecx,[ebx+eax] ; cmp [ecx],0`.
struct CResourceList;

struct CResource {
    virtual void v00();
    unsigned long m_pad4;                  // +4
    CResourceList* ResourceList;           // +8
    CResource* PrevResource;               // +0xc
    CResource* NextResource;               // +0x10
    unsigned long ResourceSize;            // +0x14
    unsigned long LastUsedFrame;           // +0x18
    void ListRemove();                     // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next);   // 0x009FBF90
    void AddToResourceList();              // 0x009FC440
};

struct CResourceList {
    long m_pad0;                           // +0
    CResource Head;                        // +4
    unsigned long ResourceCount;           // +0x20
    long AllocatedMemory;                  // +0x24
    long MaximumMemory;                    // +0x28
    unsigned long CurrentFrame;            // +0x2c
};

struct SCountBlock { long m_references; };

template <class T>
struct CCountedPointer {
    T* m_pObject;                          // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount) ++m_pCount->m_references;
    }
};

struct CLipSyncData;
struct CLipSyncDataBank;

struct CLipSyncEntryData : CResource {
    CCountedPointer<const CLipSyncData> LipSync;       // +0x1c
    void BuildLipSync();                               // 0x00A42EA0
    void Touch()
    {
        if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
            ListRemove();
            ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
            LastUsedFrame = ResourceList->CurrentFrame;
        }
    }
};

struct CLipSyncDataBankEntry {
    CLipSyncEntryData* Data;               // +0 CIVCountedPointer<CData>
    float Duration;                        // +4
    void BeginLoading(long index, CLipSyncDataBank* bank);   // 0x00A42980
};

template <class T>
struct CArray {                            // retail CArray: begin/end/capacity
    T* m_pBegin;
    T* m_pEnd;
    T* m_pEndOfStorage;
    T* begin() { return m_pBegin; }
    T& operator[](long i) { return *(begin() + i); }
};

struct CLipSyncDataBank {
    virtual void v00();
    unsigned char _body[0x160];            // +0x004 .. +0x163 (CBankFile base and bank state)
    CArray<CLipSyncDataBankEntry> Entries; // +0x164
    bool EntryExists(long index);          // 0x009CC190 (CBankFile)
    CCountedPointer<const CLipSyncData> GetLipSyncData(long index);
};

CCountedPointer<const CLipSyncData> CLipSyncDataBank::GetLipSyncData(long index)
{
    CLipSyncDataBankEntry* entry = &Entries[index];
    if (EntryExists(index)) {
        CLipSyncDataBankEntry& slot = Entries[index];
        if (slot.Data == 0)
            slot.BeginLoading(index, this);
    }
    if (entry->Data->LipSync.m_pObject == 0)
        entry->Data->BuildLipSync();
    entry->Data->Touch();
    return entry->Data->LipSync;
}