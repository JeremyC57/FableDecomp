// NGameText::CDataBankGroupEntry::BuildGroup() const -- touch the group's resource (move it to the most-recently-used end of its list), and if its file data was loaded asynchronously, wait for it, parse the group from a memory stream over it, and drop the file data
// Leaf: ?BuildGroup@CDataBankGroupEntry@NGameText@@ABEXXZ
// NF131 (2026-11-20, q6182): the row-local CMemoryDataInputStream ctor declaration respelled in place to the PDB (const void*, unsigned long):
//   ??0CMemoryDataInputStream@@QAE@PBXK@Z (Ego_r.pdb 0x00AFCA20 + FableWin.pdb 0x030849C0), the spelling the other landed callers of 0x00994930 bind.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C8700, length 190 bytes (+ int3 padding), `ret`.
// Callees: CResource::ListRemove @ 0x009FBFB0, CResource::ListInsert(prev, next) @ 0x009FBF90,
// CBankFileAsyncData::IsFinished @ 0x009D54E0, ::WaitUntilFinished @ 0x009D5B60, ::GetData @ 0x009D5530 (Ego_r / FableWin order GetData, PeekData; byte-twin of the const PeekData 0x009D5580),
// CMemoryDataInputStream::CMemoryDataInputStream(void*, long) @ 0x00994930, its destructor @ 0x00993B10,
// NGameText::CGroupEntry::ReadFromDataStream(CDataInputStream*) @ 0x009C8470, operator delete @ 0x00BFE9BC
// (the count block; the object is destroyed through the block's stored deleter).
// Layout: CDataBankGroupEntry = { CIVCountedPointer<NGameText::CGroupEntry> PGroup } (PDB, 4 bytes, the object pointer).
// NGameText::CGroupEntry derives from CResource (engine/CResource.h: ResourceList +8, Prev/Next +0xc/+0x10,
// LastUsedFrame +0x18); Entries is a retail 12-byte CArray at +0x1c and PFileData (CCountedPointer<CBankFileAsyncData>:
// object +0x28, count block +0x2c) sits 4 bytes below the PDB offset 0x2c. CResourceList per engine/CResourceList.h
// (Head +4, CurrentFrame +0x2c). The async record's data size is at +0x10.
// Shape: the touch is the inline CResource::TouchResource; the stream arguments evaluate right to left (size pushed before the
// GetData call); the file-data release is an inline CCountedPointer Clear() on the member (block re-read after the
// decrement, deleter through the block, then the block and object words zeroed).
// Name (round 7, NF53): ListRemove 0x009FBFB0 / ListInsert 0x009FBF90 are the private CResource members
//   ?ListRemove@CResource@@AAEXXZ / ?ListInsert@CResource@@AAEXPAV1@0@Z (Ego_r.pdb / FableWin.pdb) reached through the inline
//   CResource::TouchResource (engine/CResourceList.h FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE); was a local CResourceMethods view
//   (MarkAsUsed) that bound them as public CResourceMethods members.
// NF72 (round 7, q10907; access-only): BuildGroup is private const in Ego_r.pdb 0x00B16030 + FableWin.pdb 0x02F75620
//   (?BuildGroup@CDataBankGroupEntry@NGameText@@ABEXXZ; CDataBank calls it as a friend, agentNF69's 0x009C8890); the local declaration moves
//   under private: (was the public QAE spelling). Bytes unchanged.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#include "engine/CResource.h"
#include "engine/CResourceList.h"

class CBankFileAsyncData { public:
    unsigned char _pad[0x10];
    long DataSize;                     // +0x10
    long GetDataSize() const { return DataSize; }
    bool WaitUntilFinished();          // 0x009D5B60
    void* GetData();                   // 0x009D5530 (byte-identical to the const PeekData 0x009D5580)
public:
    bool IsFinished() const;   // 0x009D54E0
};

void __fastcall DestroyBankFileAsyncData(void* object);   // the deleter stored in the count block

struct SCountBlock {
    long m_references;                       // +0
    void (__fastcall* m_pfnDestroy)(void*);  // +4
    void* m_pObject;                         // +8
};

template <class T> class CCountedPointer { public:
    T* Object;                               // +0
    SCountBlock* Info;                       // +4
    T* operator->() const { return Object; }
    void Clear()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
        Info = 0;
        Object = 0;
    }
};

class CDataInputStream;
class CMemoryDataInputStream { public:
    unsigned char _body[0x18];
    CMemoryDataInputStream(const void* data, unsigned long size);   // 0x00994930
    virtual ~CMemoryDataInputStream();                       // 0x00993B10
};

namespace NGameText {
class CGroupEntry : public CResource { public:
    void* Entries[3];                                // +0x1c retail CArray<long> (12 bytes)
    CCountedPointer<CBankFileAsyncData> PFileData;   // +0x28
    void* Owner;                                     // +0x30
    void ReadFromDataStream(CMemoryDataInputStream* stream);   // 0x009C8470
};
}

namespace NGameText {
class CDataBankGroupEntry { public:
    CGroupEntry* PGroup;                 // +0 CIVCountedPointer<NGameText::CGroupEntry>
private:
    void BuildGroup() const;             // ?BuildGroup@CDataBankGroupEntry@NGameText@@ABEXXZ (private in the PDB)
};
}

void NGameText::CDataBankGroupEntry::BuildGroup() const
{
    PGroup->TouchResource();
    if (PGroup->PFileData.Object) {
        if (!PGroup->PFileData->IsFinished())
            PGroup->PFileData->WaitUntilFinished();
        CMemoryDataInputStream stream(PGroup->PFileData->GetData(), PGroup->PFileData->GetDataSize());
        PGroup->ReadFromDataStream(&stream);
        PGroup->PFileData.Clear();
    }
}