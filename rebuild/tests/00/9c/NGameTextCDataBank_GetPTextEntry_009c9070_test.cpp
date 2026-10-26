#include <stdio.h>
#include <stddef.h>
// BEGIN exact source snapshot SHA256 51ad231dbec1d44ed08139169bd9983a05f9f62cc945921f0c9f7e4b385822e5
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

class CResource;
class CResourceList;
class CResource { public:
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
class CResourceList { public:
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
class CBankFileAsyncData { public:
    bool IsFinished() const;                 // 0x009D54E0
};
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall *Destroy)(void*);
    void* Object;
};
template<class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
};
namespace NGameText {
class CTextEntry : public CResource { public:
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
template<class T> class CIVCountedPointer { public:
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
namespace NGameText { class CDataBank; }
namespace NGameText {
class CDataBankTextEntry { public:
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
class CBankFile { public:
    virtual ~CBankFile() {}
    unsigned long GetEntryDataType(unsigned long) const; // 0x009CC210
};
class CBankFileAsync : public CBankFile { public:
    void AsyncUpdatePending();                // 0x009D68B0
};
namespace NGameText {
class CDataBank : public CBankFileAsync { public:
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

// END exact source snapshot
using namespace NGameText;
typedef char ResourceListOffsetCheck[(offsetof(CDataBank, ResourceList)==0x168)?1:-1];
typedef char EntriesOffsetCheck[(offsetof(CDataBank, Entries)==0x1e0)?1:-1];

static int asyncCalls, pendingCalls, beginCalls, buildCalls, makeSpaceCalls;
static int removeCalls, insertCalls;
static CResource* insertedBefore;
static CResource* insertedHead;
static unsigned long lastIndex;
static bool typeIsText = true;
static CTextEntry builtText;
static CTextEntry replacementText;
static CDataBankTextEntry* activeEntry;
static bool replaceOnInsert;
static CBankFileAsyncData fileData;
static bool fileFinished;
static int lastReferenceDeletes;
struct LastRefTextEntry : CTextEntry {
    virtual ~LastRefTextEntry() { ++lastReferenceDeletes; }
};

bool CBankFileAsyncData::IsFinished() const { return fileFinished; }
unsigned long CBankFile::GetEntryDataType(unsigned long i) const
{ lastIndex = i; return typeIsText ? 0 : 1; }
void CBankFileAsync::AsyncUpdatePending() { ++asyncCalls; }
void CDataBank::BeginLoadingText(unsigned long i)
{ ++pendingCalls; lastIndex = i; }
void CDataBankTextEntry::BeginLoading(unsigned long i, NGameText::CDataBank&)
{
    ++beginCalls; lastIndex = i; PText.Object = &builtText;
    ++builtText.References;
}
void CDataBankTextEntry::BuildText() { ++buildCalls; }
bool CResourceList::MakeFreeSpace(unsigned long n)
{ ++makeSpaceCalls; (void)n; return true; }
void CResource::ListRemove() { ++removeCalls; }
void CResource::ListInsert(CResource* before, CResource* head)
{
    ++insertCalls; (void)before; (void)head;
    insertedBefore = before; insertedHead = head;
    if (replaceOnInsert) {
        if (activeEntry->PText.Object)
            activeEntry->PText.Object->Release();
        activeEntry->PText.Object = &replacementText;
        replacementText.AddRef();
    }
}

int main()
{
    static CDataBank bank;
    static CDataBankTextEntry entry;
    static CDataBankTextEntry* slots[2];
    bank.Entries.First = slots; bank.Entries.Last = slots + 2;
    bank.Entries.End = slots + 2; slots[0] = &entry; slots[1] = 0;
    activeEntry = &entry;
    bank.ResourceList.Head.PrevResource = &bank.ResourceList.Head;
    builtText.References = 1;       // fixture-owned baseline references
    replacementText.References = 1;
    builtText.ResourceList = &bank.ResourceList;
    builtText.PFileData.Data = &fileData;

    typeIsText = false;
    {
        CIVCountedPointer<const CTextEntry> miss = bank.GetPTextEntry(0, true);
        if (miss.Object || asyncCalls || beginCalls) return 1;
    }
    typeIsText = true;
    slots[0] = 0;
    {
        CIVCountedPointer<const CTextEntry> miss = bank.GetPTextEntry(0, true);
        if (miss.Object || asyncCalls) return 2;
    }
    slots[0] = &entry;

    {
        CIVCountedPointer<const CTextEntry> miss = bank.GetPTextEntry(0, false);
        if (miss.Object || pendingCalls != 1 || asyncCalls != 1) return 3;
    }

    bank.ResourceList.CurrentFrame = 7;
    fileFinished = false;
    replaceOnInsert = true;
    {
        CIVCountedPointer<const CTextEntry> ready = bank.GetPTextEntry(0, true);
        if (ready.Object != &replacementText || beginCalls != 1 || buildCalls != 1 ||
            makeSpaceCalls != 1 || builtText.References != 1 || removeCalls != 1 ||
            insertCalls != 1 || replacementText.References != 3 ||
            insertedBefore != &bank.ResourceList.Head ||
            insertedHead != &bank.ResourceList.Head) return 4;
    }
    if (replacementText.References != 2) return 5;
    replaceOnInsert = false;
    replacementText.Release();
    entry.PText.Object = &builtText;
    builtText.AddRef();
    builtText.LastUsedFrame = 0;
    fileFinished = true;
    {
        CIVCountedPointer<const CTextEntry> ready = bank.GetPTextEntry(0, false);
        if (ready.Object != &builtText || buildCalls != 2 || removeCalls != 2 ||
            insertCalls != 2 || builtText.LastUsedFrame != 7) return 6;
    }
    {
        LastRefTextEntry* object = new LastRefTextEntry;
        object->References = 0;
        {
            CIVCountedPointer<const CTextEntry> owner(object);
            if (object->References != 1) return 7;
            {
                CIVCountedPointer<const CTextEntry> copy(owner);
                if (object->References != 2) return 8;
            }
            if (object->References != 1 || lastReferenceDeletes) return 9;
        }
        if (lastReferenceDeletes != 1) return 10;
    }
    puts("009c9070_TEST PASS");
    return 0;
}