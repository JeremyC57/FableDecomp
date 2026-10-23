// NGameText::CDataBank::GetPTextEntry(unsigned long,bool) const, retail 0x009C9070.
// Recovered PDB identity egor31452/debug 0x00B17AB0. The old authored assembly
// bootstrap remains untouched; this is the independent pure C++03 candidate.
#define CResource CResourcePdb
#include "engine/CResource.h"
#undef CResource
#define CResourceList CResourceListPdb
#include "engine/CResourceList.h"
#undef CResourceList
#define CBankFile CBankFilePdb
#include "engine/CBankFile.h"
#undef CBankFile
#include <stddef.h>

struct CResource;
struct CResourceList;
struct CResource {
    virtual ~CResource() {}
    mutable long References;                  // +4, original CIV reference count
    CResourceList* ResourceList;              // +8
    CResource* PrevResource;                  // +c
    CResource* NextResource;                  // +10
    unsigned long ResourceSize;                // +14
    unsigned long LastUsedFrame;               // +18
    long Release() const { return --References; }
    void AddRef() const { ++References; }
    virtual void Delete() const { delete const_cast<CResource*>(this); }
    void ListRemove();                        // 0x009FBFB0
    void ListInsert(CResource*, CResource*);  // 0x009FBF90
    __forceinline void SetAsMRU();
};
struct CResourceList {
    void* VTable;                              // +0
    CResource Head;                            // +4, genuine 0x1c-byte resource
    unsigned long ResourceCount;               // +20
    long AllocatedMemory;                      // +24
    long MaximumMemory;                        // +28
    unsigned long CurrentFrame;                // +2c
    unsigned long DebugStatsFrame;             // +30
    unsigned long UnloadDelay;                 // +34
    unsigned long UnloadedThisFrame;           // +38
    bool MakeFreeSpace(unsigned long);         // 0x009FC4F0
};
FABLE_STATIC_ASSERT(sizeof(CResource)==0x1c);
FABLE_STATIC_ASSERT(sizeof(CResourceList)==0x3c);
FABLE_STATIC_ASSERT(offsetof(CResourceList,Head)==0x04);
FABLE_STATIC_ASSERT(offsetof(CResourceList,ResourceCount)==0x20);
FABLE_STATIC_ASSERT(offsetof(CResourceList,CurrentFrame)==0x2c);
FABLE_STATIC_ASSERT(offsetof(CResourceList,UnloadedThisFrame)==0x38);
FABLE_STATIC_ASSERT(sizeof(CResourcePdb)==sizeof(CResource));
FABLE_STATIC_ASSERT(sizeof(CResourceListPdb)==sizeof(CResourceList));
__forceinline void CResource::SetAsMRU()
{
    if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
        ListRemove();
        ListInsert(ResourceList->Head.PrevResource,&ResourceList->Head);
        LastUsedFrame=ResourceList->CurrentFrame;
    }
}
struct CBankFileAsyncData {
    bool IsFinished() const;                 // 0x009D54E0
};
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall *Destroy)(void*);
    void* Object;
};
template<class T> struct CCountedPointer {
    T* Data;
    CCPPointerInfo* Info;
};
namespace NGameText {
struct CTextEntry : CResource {
    unsigned char _pad1c[0x38 - 0x1c];
    CCountedPointer<CBankFileAsyncData> PFileData; // +38, 8-byte holder
    void* Owner;                              // +40
};
}
FABLE_STATIC_ASSERT(offsetof(NGameText::CTextEntry,PFileData)==0x38);
FABLE_STATIC_ASSERT(offsetof(NGameText::CTextEntry,Owner)==0x40);
FABLE_STATIC_ASSERT(sizeof(NGameText::CTextEntry)==0x44);
struct CCountedInfo {
    long RefCount;
    void (__fastcall *Destroy)(void*);
    void* Object;
};
template<class T> struct CIVCountedPointer {
    T* Object;
    CIVCountedPointer() : Object(0) {}
    explicit CIVCountedPointer(T* object) : Object(object)
    { if (Object) ++((CResource*)Object)->References; }
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    { if (Object) ++((CResource*)Object)->References; }
    ~CIVCountedPointer()
    {
        if (Object) {
            CResource* resource=(CResource*)Object;
            if (resource->Release()==0) resource->Delete();
            Object=0;
        }
    }
};
namespace NGameText { struct CDataBank; }
namespace NGameText {
struct CDataBankTextEntry {
    ::CIVCountedPointer<CTextEntry> PText;    // +0
    long NarratorIndex;                       // +4
    void BeginLoading(unsigned long, NGameText::CDataBank&); // 0x009C8340
    void BuildText();                         // 0x009C8FB0
};
}
struct CPointerVector {
    NGameText::CDataBankTextEntry** First;
    NGameText::CDataBankTextEntry** Last;
    NGameText::CDataBankTextEntry** End;
    NGameText::CDataBankTextEntry* operator[](unsigned long i) const { return First[i]; }
};
struct CBankFile {
    virtual ~CBankFile() {}
    unsigned long GetEntryDataType(unsigned long) const; // 0x009CC210
};
struct CBankFileAsync : CBankFile {
    void AsyncUpdatePending();                // 0x009D68B0
};
namespace NGameText {
struct CDataBank : CBankFileAsync {
    // CBankFilePdb is 0x174 in debug; the retail bank base contract places
    // this row's resource list at +0x168, so retain a documented retail
    // overlay instead of inheriting the debug container extent.
    unsigned char _pad04[0x168 - 4];
    CResourceList ResourceList;               // +168
    unsigned char _pad1a4[0x3c];
    CPointerVector Entries;                   // +1e0
    void BeginLoadingText(unsigned long);      // 0x009C8630
    ::CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long, bool) const;
};

};

using NGameText::CDataBankTextEntry;
using NGameText::CTextEntry;
::CIVCountedPointer<const NGameText::CTextEntry>
NGameText::CDataBank::GetPTextEntry(unsigned long id, bool allowPending) const
{
    if (GetEntryDataType(id) == 0) {
        CDataBankTextEntry* entry = Entries[id];
        if (entry) {
            ((CDataBank*)this)->AsyncUpdatePending();
            if ((!entry->PText.Object ||
                 (entry->PText.Object->PFileData.Data &&
                  !entry->PText.Object->PFileData.Data->IsFinished())) && !allowPending) {
                ((CDataBank*)this)->BeginLoadingText(id);
            } else {
                if (!entry->PText.Object) {
                    if (GetEntryDataType(id) == 0) {
                        CDataBankTextEntry* current = Entries[id];
                        if (current)
                            current->BeginLoading(id, *(CDataBank*)this);
                    }
                    entry->BuildText();
                    ((CDataBank*)this)->ResourceList.MakeFreeSpace(0);
                } else if (entry->PText.Object->PFileData.Data) {
                    entry->BuildText();
                }

                entry->PText.Object->SetAsMRU();
                return CIVCountedPointer<const CTextEntry>(entry->PText.Object);
            }
        }
    }
    return CIVCountedPointer<const CTextEntry>();
}