// NGameText::CDataBankTextEntry::BuildText() const -- touch the text entry's resource (move it to the most-recently-used end of its list), and if its file data was loaded asynchronously, wait for it, parse the text entry from a memory stream over it, and drop the file data
// Leaf: BuildText
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C8FB0, length 190 bytes (+ int3 padding), `ret`.
// Callees: CResource::ListRemove @ 0x009FBFB0, CResource::ListInsert(prev, next) @ 0x009FBF90,
// CBankFileAsyncData::IsFinished @ 0x009D54E0, ::WaitUntilFinished @ 0x009D5B60, ::PeekData @ 0x009D5530 (byte-twin of GetData 0x009D5580, named apart for relocation identity),
// CMemoryDataInputStream::CMemoryDataInputStream(void*, long) @ 0x00994930, its destructor @ 0x00993B10,
// NGameText::CTextEntry::ReadFromDataStream(CDataInputStream*) @ 0x009C8DE0, operator delete @ 0x00BFE9BC
// (the count block; the object is destroyed through the block's stored deleter).
// Catalog label "CDataBankGroupEntry::BuildGroup" is propagated from the byte-twin 0x009C8700: this body reads
// PFileData at +0x38 and calls CTextEntry::ReadFromDataStream, i.e. ?BuildText@CDataBankTextEntry@NGameText@@ABEXXZ.
// Layout: CDataBankTextEntry = { CIVCountedPointer<NGameText::CTextEntry> PText +0, long NarratorIndex +4 } (PDB).
// NGameText::CTextEntry derives from CResource (engine/CResource.h: ResourceList +8, Prev/Next +0xc/+0x10,
// LastUsedFrame +0x18); SpeechBank/Narrator/SoundName/Text strings +0x1c..+0x28, Tags is a retail 12-byte CArray at
// +0x2c and PFileData (CCountedPointer<CBankFileAsyncData>: object +0x38, count block +0x3c) sits 4 bytes below the
// PDB offset 0x3c. CResourceList per engine/CResourceList.h
// (Head +4, CurrentFrame +0x2c). The async record's data size is at +0x10.
// Shape: the touch is an inline CResource member; the stream arguments evaluate right to left (size pushed before the
// GetData call); the file-data release is an inline CCountedPointer Clear() on the member (block re-read after the
// decrement, deleter through the block, then the block and object words zeroed).
#include "engine/CResource.h"
#include "engine/CResourceList.h"

struct CResourceMethods : CResource {
    static CResource* GetHead(CResourceList* list) { return reinterpret_cast<CResource*>(list->Head); }
    void ListRemove();                                   // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next);   // 0x009FBF90
    void MarkAsUsed()
    {
        if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
            ListRemove();
            ListInsert(GetHead(ResourceList)->PrevResource, GetHead(ResourceList));
            LastUsedFrame = ResourceList->CurrentFrame;
        }
    }
};

struct CBankFileAsyncData {
    unsigned char _pad[0x10];
    long DataSize;                     // +0x10
    long GetDataSize() const { return DataSize; }
    bool IsFinished();                 // 0x009D54E0
    bool WaitUntilFinished();          // 0x009D5B60
    const void* PeekData() const;      // 0x009D5530 (byte-identical to GetData 0x009D5580)
};

void __fastcall DestroyBankFileAsyncData(void* object);   // the deleter stored in the count block

struct SCountBlock {
    long m_references;                       // +0
    void (__fastcall* m_pfnDestroy)(void*);  // +4
    void* m_pObject;                         // +8
};

template <class T> struct CCountedPointer {
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

struct CDataInputStream;
struct CMemoryDataInputStream {
    void* __vftable;
    unsigned char _body[0x18];
    CMemoryDataInputStream(const void* data, long size);   // 0x00994930
    ~CMemoryDataInputStream();                       // 0x00993B10
};

namespace NGameText {
struct CTextEntry : CResourceMethods {
    void* SpeechBank;                                // +0x1c CCharString
    void* Narrator;                                  // +0x20 CCharString
    void* SoundName;                                 // +0x24 CCharString
    void* Text;                                      // +0x28 CWideString
    void* Tags[3];                                   // +0x2c retail CArray<CTextTag> (12 bytes)
    CCountedPointer<CBankFileAsyncData> PFileData;   // +0x38
    void* Owner;                                     // +0x40
    void ReadFromDataStream(CMemoryDataInputStream* stream);   // 0x009C8DE0
};

struct CDataBankTextEntry {
    CTextEntry* PText;                   // +0 CIVCountedPointer<NGameText::CTextEntry>
    long NarratorIndex;                  // +4
    void BuildText() const;              // ?BuildText@CDataBankTextEntry@NGameText@@ABEXXZ (private in the PDB)
};
}

void NGameText::CDataBankTextEntry::BuildText() const
{
    PText->MarkAsUsed();
    if (PText->PFileData.Object) {
        if (!PText->PFileData->IsFinished())
            PText->PFileData->WaitUntilFinished();
        CMemoryDataInputStream stream(PText->PFileData->PeekData(), PText->PFileData->GetDataSize());
        PText->ReadFromDataStream(&stream);
        PText->PFileData.Clear();
    }
}
