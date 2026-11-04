// NGameText::CDataBank::Validate() -- walk every bank entry from index 1: for each group entry, touch each member text entry (its data type and its counted text-entry pointer, copying the entry's text) -- the checks themselves are compiled out of the release build
// Grade: DIFFER(273v268) -- dl=52: with the landed GetPGroupEntry 0x009C8890 / GetPTextEntry 0x009C9070 bodies in the TU the frame (0x10, outer index homed) and the text pointer kept across the CWideString calls match; left: this/index ebp/ebx swapped (ours ebx/ebp) and the group release reloads the temporary
// Leaf: ?Validate@CDataBank@NGameText@@QAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include <stddef.h>
class CResourceList;
class CResource { public:
    virtual ~CResource();
    virtual void Delete() const;                      // slot 1
    mutable unsigned long IVRefCount;                 // +0x04
    CResourceList* ResourceList;                      // +0x08
    CResource* PrevResource;                          // +0x0c
    CResource* NextResource;                          // +0x10
    unsigned long ResourceSize;                       // +0x14
    unsigned long LastUsedFrame;                      // +0x18
    void AddRef() const { ++IVRefCount; }
    long Release() const { return --IVRefCount; }
    void TouchResource();
private:
    void ListRemove();                                // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next);// 0x009FBF90
};
class CResourceList { public:
    void* __vftable;
    CResource Head;                                   // +0x04
    unsigned long ResourceCount;
    long AllocatedMemory, MaximumMemory;
    unsigned long CurrentFrame, DebugStatsFrame, UnloadDelay, UnloadedThisFrame;
    bool MakeFreeSpace(unsigned long);                // 0x009FC4F0
};
inline void CResource::TouchResource()
{
    if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
        ListRemove();
        ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
        LastUsedFrame = ResourceList->CurrentFrame;
    }
}
template <class T> class CIVCountedPointer { public:
    CIVCountedPointer() : Object(0) { }
    ~CIVCountedPointer() { Release(); }
    void Reset(T* object) { Release(); Object = object; if (Object) Object->AddRef(); }
    void Release() {
        if (Object) {
            if (Object->Release() == 0) Object->Delete();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
private:
    T* Object;
};
template <class T> struct CArray {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    T& operator[](unsigned long i) const { return m_pFirst[i]; }
};
class CBankFileAsyncData { public: bool IsFinished() const; };   // 0x009D54E0
class CBankFile { public:
    virtual ~CBankFile();
    unsigned long GetEntryDataType(unsigned long) const;           // 0x009CC210
};
class CBankFileAsync : public CBankFile { public:
    void AsyncUpdatePending();                                     // 0x009D68B0
};
namespace NGameText {
class CDataBank;
class CGroupEntry : public CResource { public:
    CArray<long> Entries;                       // +0x1c
    CBankFileAsyncData* PFileData;              // +0x28 (CCountedPointer<CBankFileAsyncData> Data)
    void* PFileDataInfo;                        // +0x2c
};
class CTextEntry : public CResource { public:
    CCharString SpeechBank, Narrator, SoundName;// +0x1c
    CWideString Text;                           // +0x28
    CArray<long> Tags;                          // +0x2c (CArray<CTextTag>)
    CBankFileAsyncData* PFileData;              // +0x38
    void* PFileDataInfo;                        // +0x3c
};
class CDataBankGroupEntry { public:
    CGroupEntry* PGroup;                        // CIVCountedPointer<CGroupEntry>
    void BeginLoading(unsigned long, CDataBank&) const;   // 0x009C80F0
    void BuildGroup() const;                    // 0x009C8700
};
class CDataBankTextEntry { public:
    CTextEntry* PText;                          // CIVCountedPointer<CTextEntry>
    long NarratorIndex;
    void BeginLoading(unsigned long, CDataBank&) const;   // 0x009C8340
    void BuildText() const;                     // 0x009C8FB0
};
class CDataBank : public CBankFileAsync { public:
    unsigned char _pad04[0x168 - 4];
    CResourceList ResourceList;                 // +0x168
    unsigned char _pad1a4[0x3c];
    CArray<void*> Entries;                      // +0x1e0
    void BeginLoadingGroup(unsigned long) const;      // 0x009C8220
    void BeginLoadingText(unsigned long) const;       // 0x009C8630
    CIVCountedPointer<const CGroupEntry> GetPGroupEntry(unsigned long index, bool allowPending) const;
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long index, bool allowPending) const;
    void Validate();
};

CIVCountedPointer<const CGroupEntry> CDataBank::GetPGroupEntry(unsigned long index, bool allowPending) const
{
    CIVCountedPointer<const CGroupEntry> result;
    if (GetEntryDataType(index) == 1) {
        CDataBankGroupEntry* entry = (CDataBankGroupEntry*)Entries[index];
        if (entry) {
            const_cast<CDataBank*>(this)->AsyncUpdatePending();
            if ((!entry->PGroup || (entry->PGroup->PFileData && !entry->PGroup->PFileData->IsFinished())) && !allowPending) {
                BeginLoadingGroup(index);
            } else {
                if (!entry->PGroup) {
                    if (GetEntryDataType(index) == 1 && Entries[index])
                        ((CDataBankGroupEntry*)Entries[index])->BeginLoading(index, *const_cast<CDataBank*>(this));
                    entry->BuildGroup();
                    const_cast<CResourceList&>(ResourceList).MakeFreeSpace(0);
                } else if (entry->PGroup->PFileData) {
                    entry->BuildGroup();
                }
                entry->PGroup->TouchResource();
                result.Reset(entry->PGroup);
            }
        }
    }
    return result;
}

CIVCountedPointer<const CTextEntry> CDataBank::GetPTextEntry(unsigned long index, bool allowPending) const
{
    CIVCountedPointer<const CTextEntry> result;
    if (GetEntryDataType(index) == 0) {
        CDataBankTextEntry* entry = (CDataBankTextEntry*)Entries[index];
        if (entry) {
            const_cast<CDataBank*>(this)->AsyncUpdatePending();
            if ((!entry->PText || (entry->PText->PFileData && !entry->PText->PFileData->IsFinished())) && !allowPending) {
                BeginLoadingText(index);
            } else {
                if (!entry->PText) {
                    if (GetEntryDataType(index) == 0 && Entries[index])
                        ((CDataBankTextEntry*)Entries[index])->BeginLoading(index, *const_cast<CDataBank*>(this));
                    entry->BuildText();
                    const_cast<CResourceList&>(ResourceList).MakeFreeSpace(0);
                } else if (entry->PText->PFileData) {
                    entry->BuildText();
                }
                entry->PText->TouchResource();
                result.Reset(entry->PText);
            }
        }
    }
    return result;
}

void CDataBank::Validate()
{
    for (unsigned long i = 1; i < Entries.size(); ++i) {
        if (GetEntryDataType(i) == 1) {
            const CArray<long>& entries = GetPGroupEntry(i, true)->Entries;
            for (unsigned long j = 0; j < entries.size(); ++j) {
                GetEntryDataType(entries[j]);
                CIVCountedPointer<const CTextEntry> text = GetPTextEntry(entries[j], true);
                CWideString copy(text->Text);
            }
        }
    }
}
}
