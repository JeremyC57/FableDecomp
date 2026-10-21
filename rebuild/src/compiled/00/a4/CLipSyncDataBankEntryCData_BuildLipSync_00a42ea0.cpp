// CLipSyncDataBankEntry::CData::BuildLipSync() -- once the async bank read is finished, parse it into a new CLipSyncSampleData, drop the async data and register the resource with its new size
// Leaf: BuildLipSync
// Retry 10b (getsw sweep, FIXES B-15): the size is read through an inline getter `GetSize()`; the plain member read gives the parked register/scheduling difference.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A42EA0, 268 bytes (272 with padding).
// Callees: CBankFileAsyncData::IsFinished 0x009D54E0 / WaitUntilFinished 0x009D5B60 / GetData 0x009D5530 / Sizeof
//   0x009D5620, operator new 0x00BFEA1A (0x10 bytes), CCountedPointer<CLipSyncSampleData>::Reset 0x00A44170,
//   CMemoryDataInputStream(const void*, long) 0x00994930 / ~CMemoryDataInputStream 0x00993B10, CLipSyncSampleData::
//   ReadFromFile 0x00A495F0 / Sizeof 0x00A43960, operator delete 0x00BFE9BC, CResource::SetResourceSize 0x009FBF20 /
//   AddToResourceList 0x009FC440, CResourceList::MakeFreeSpace 0x009FC4F0 / AddResource 0x009FBFE0.
// Layout: CResource spans +0..+0x1c; the sample data counted pointer at +0x1c (object, count block), the async bank data
//   counted pointer at +0x24/+0x28, the owning bank at +0x30 (its resource list at +0x170, list object +4). The CData
//   ctor (0x00A43580, landed) zeroes these four words.
// Shape: the new sample data's inline ctor zeroes three words (`mov [eax],ebx` x3, the ebx zero register); the async
//   pointer is cleared with the inline counted-pointer release (count block re-read, destroy + delete at zero) and both
//   words zeroed; each Sizeof term is `p ? p->Sizeof() + 8 : 8` (VC folds the +8 into both arms) and the size is
//   `a + b + 0x34` (`lea edi,[eax+edi+34h]`); the stream is a function-scope local destroyed at the end.
struct CLipSyncSampleData;
class CMemoryDataInputStream {
public:
    CMemoryDataInputStream(const void* data, long size);   // 0x00994930
    ~CMemoryDataInputStream();                             // 0x00993B10
private:
    void* m_vtable;
    long m_words[6];                                       // sizeof 0x1c
};
struct CLipSyncSampleData {                                // 0x10 bytes
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    long m_count;
    CLipSyncSampleData() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    void ReadFromFile(CMemoryDataInputStream* stream);     // 0x00A495F0
    unsigned long Sizeof() const;                          // 0x00A43960
};
struct CBankFileAsyncData {
    unsigned char _pad00[0x10];
    long m_size;                                           // +0x10
    long GetSize() const { return m_size; }
    bool IsFinished();                                     // 0x009D54E0
    bool WaitUntilFinished();                              // 0x009D5B60
    void* GetData();                                       // 0x009D5530
    unsigned long Sizeof() const;                          // 0x009D5620
};
struct SCountBlock {
    long references;
    void (__fastcall* destroy)(void*);
    void* object;
};
template<class T> struct CCountedPointer {
    T* object;
    SCountBlock* count;
    void Reset(T* value);                                  // 0x00A44170 (sample data instantiation)
    T* operator->() const { return object; }
    void Clear()
    {
        if (count) {
            --count->references;
            if (count->references == 0) {
                count->destroy(count->object);
                delete count;
            }
        }
        count = 0;
        object = 0;
    }
};
struct CResourceList {
    bool MakeFreeSpace(unsigned long size);                // 0x009FC4F0
    void AddResource(struct CResource* resource);          // 0x009FBFE0
};
struct CLipSyncDataBank {
    unsigned char _pad00[0x170];
    struct SResourceOwner { long _pad; CResourceList List; }* m_pResources;   // +0x170 (list object at +4)
};
struct CResource {
    virtual void v00();
    unsigned char _body[0x18];                             // +0x04..+0x1c
    void SetResourceSize(unsigned long size);              // 0x009FBF20
    void AddToResourceList();                              // 0x009FC440
};
struct CLipSyncDataBankEntry {
    struct CData : CResource {
        CCountedPointer<CLipSyncSampleData> m_pSampleData; // +0x1c
        CCountedPointer<CBankFileAsyncData> m_pAsyncData;  // +0x24
        unsigned char _pad2c[4];
        CLipSyncDataBank* m_pBank;                         // +0x30
        void BuildLipSync();
    };
};

void CLipSyncDataBankEntry::CData::BuildLipSync()
{
    if (m_pSampleData.object == 0) {
        if (!m_pAsyncData.object->IsFinished())
            m_pAsyncData.object->WaitUntilFinished();
        m_pSampleData.Reset(new CLipSyncSampleData);
        CMemoryDataInputStream stream(m_pAsyncData.object->GetData(), m_pAsyncData.object->GetSize());
        m_pSampleData->ReadFromFile(&stream);
        m_pAsyncData.Clear();
        unsigned long sampleSize = m_pSampleData.object ? m_pSampleData.object->Sizeof() + 8 : 8;
        unsigned long asyncSize = m_pAsyncData.object ? m_pAsyncData.object->Sizeof() + 8 : 8;
        unsigned long size = asyncSize + sampleSize + 0x34;
        SetResourceSize(size);
        AddToResourceList();
        m_pBank->m_pResources->List.MakeFreeSpace(size);
        m_pBank->m_pResources->List.AddResource(this);
    }
}