// NGameText::CDataBankTextEntry::BuildText() const -- touch the text entry's resource (move it to the most-recently-used end of its list), and if its file data was loaded asynchronously, wait for it, parse the text entry from a memory stream over it, and drop the file data
// Leaf: ?BuildText@CDataBankTextEntry@NGameText@@ABEXXZ
// NF174 (q11869, access-only): BuildText is private const in Ego_r.pdb 0x00B17870 + FableWin.pdb 0x02F747F0
//   (?BuildText@CDataBankTextEntry@NGameText@@ABEXXZ, as its byte-twin 0x009C8700 ?BuildGroup@CDataBankGroupEntry@NGameText@@ABEXXZ); the local
//   declaration moves under private: (was the public const QBE spelling). Bytes unchanged.
// NF131 (2026-11-20, q6182): the row-local CMemoryDataInputStream ctor declaration respelled in place to the PDB (const void*, unsigned long):
//   ??0CMemoryDataInputStream@@QAE@PBXK@Z (Ego_r.pdb 0x00AFCA20 + FableWin.pdb 0x030849C0), the spelling the other landed callers of 0x00994930 bind.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C8FB0, length 190 bytes (+ int3 padding), `ret`.
// Callees: CResource::ListRemove @ 0x009FBFB0, CResource::ListInsert(prev, next) @ 0x009FBF90,
// CBankFileAsyncData::IsFinished @ 0x009D54E0, ::WaitUntilFinished @ 0x009D5B60, ::GetData @ 0x009D5530 (Ego_r / FableWin order GetData, PeekData; byte-twin of the const PeekData 0x009D5580),
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
// Name (NF143, q4942 / q4943): ListRemove 0x009FBFB0 / ListInsert 0x009FBF90 are the private CResource members
//   ?ListRemove@CResource@@AAEXXZ / ?ListInsert@CResource@@AAEXPAV1@0@Z (Ego_r.pdb / FableWin.pdb), reached through the inline
//   CResource::TouchResource (engine/CResourceList.h FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE), as the byte-twin BuildGroup 0x009C8700
//   spells it; the local CResourceMethods view bound them as public CResourceMethods members (its MarkAsUsed was this touch). The
//   header-less CResourceMethods token stays as the base of the row's local NGameText::CTextEntry (base list unchanged).
// NF188 (2026-11-20, q4251): the row-local CMemoryDataInputStream stand-in (a user-declared destructor that bound a
//   CMemoryDataInputStream dtor spelling at 0x00993B10) is replaced by the shared engine/CMemoryDataInputStream.h class, whose
//   compiler-generated destructor calls ~CDataStream 0x00993B10 directly (??1CDataStream@@UAE@XZ, Ego_r.pdb 0x00AFBBB0 +
//   FableWin.pdb 0x03080440; engine/CDataInputStream.h retry11a note).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#include "engine/CResource.h"
#include "engine/CResourceList.h"

struct CResourceMethods : CResource {
};

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
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#include "engine/CMemoryDataInputStream.h"

namespace NGameText {
class CTextEntry : public CResourceMethods { public:
    void* SpeechBank;                                // +0x1c CCharString
    void* Narrator;                                  // +0x20 CCharString
    void* SoundName;                                 // +0x24 CCharString
    void* Text;                                      // +0x28 CWideString
    void* Tags[3];                                   // +0x2c retail CArray<CTextTag> (12 bytes)
    CCountedPointer<CBankFileAsyncData> PFileData;   // +0x38
    void* Owner;                                     // +0x40
    void ReadFromDataStream(CMemoryDataInputStream* stream);   // 0x009C8DE0
};

class CDataBankTextEntry { public:
    CTextEntry* PText;                   // +0 CIVCountedPointer<NGameText::CTextEntry>
    long NarratorIndex;                  // +4
private:
    void BuildText() const;              // ?BuildText@CDataBankTextEntry@NGameText@@ABEXXZ (private in the PDB)
};
}

void NGameText::CDataBankTextEntry::BuildText() const
{
    PText->TouchResource();
    if (PText->PFileData.Object) {
        if (!PText->PFileData->IsFinished())
            PText->PFileData->WaitUntilFinished();
        CMemoryDataInputStream stream(PText->PFileData->GetData(), PText->PFileData->GetDataSize());
        PText->ReadFromDataStream(&stream);
        PText->PFileData.Clear();
    }
}