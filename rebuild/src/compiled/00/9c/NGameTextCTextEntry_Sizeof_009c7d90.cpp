// NGameText::CTextEntry::Sizeof() const -- bytes used by a text entry: the object plus its strings, tag storage and file data
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C7D90, 99 bytes (+13 int3). __fastcall virtual (vtable 0x0129B4A4, RTTI
//         .?AVCTextEntry@NGameText@@), returns unsigned long. Called directly by the ctor 0x009C8250.
// Callees: CCharString::Sizeof() 0x0099E500 (x3); CWideString::Sizeof() 0x0099B2A0; CBankFileAsyncData::Sizeof() 0x009D5620
//          (catalog "CVertexBufferWin32::DoSizeof" is a byte twin).
// Shape: a running `size +=` sum in member order (sizeof(*this) = 0x44 folded into the first `add edi,40h`), each member
//        contributing Sizeof() - sizeof(member); the tag vector adds capacity()*8 (`(eos - start) >> 3` then `*8`); the
//        counted file-data pointer adds `(p ? p->Sizeof() + 8 : 8) - 8` (both +8 arms kept, `lea eax,[eax+edi-8]`).
// Layout: retail NGameText::CTextEntry 0x44 bytes (PDB 0x48 with a 16-byte CArray): CResource base 0x1C, SpeechBank +0x1C,
//         Narrator +0x20, SoundName +0x24, Text +0x28, Tags +0x2C (STLport vector), PFileData +0x38, Owner +0x40.
class CCharString { public:
    void* PStringData;
    CCharString();                               // 0x0099E4B0
    unsigned long Sizeof() const;                // 0x0099E500
};
class CWideString { public:
    void* PStringData;
    CWideString();                               // 0x0099AED0
    unsigned long Sizeof() const;                // 0x0099B2A0
};
class CCPPointerInfo { public: long Count; };
class CBankFileAsyncData { public:
    unsigned long Sizeof() const;                // 0x009D5620 (catalog "CVertexBufferWin32::DoSizeof" is a byte twin)
};
template <class T> class CCountedPointer { public:
    T* Data;                                     // +0
    CCPPointerInfo* Info;                        // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->Count; }
};
template <class T> struct CRetailVector {        // STLport vector (12 bytes; PDB CArray is 16)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    unsigned int capacity() const { return _M_end_of_storage - _M_start; }
};
class CResourceList;
class CResource { public:                               // 0x1C bytes
    virtual unsigned long Sizeof() const;
    void* _pad04;
    CResourceList* ResourceList;                 // +0x08
    CResource* PrevResource;                     // +0x0c
    CResource* NextResource;                     // +0x10
    unsigned long ResourceSize;                  // +0x14
    unsigned long LastUsedFrame;                 // +0x18
    CResource();                                 // 0x009FBEC0
    void SetResourceSize(unsigned long size);    // 0x009FBF20
};
struct CDataBankTextEntry;
namespace NGameText {
class CTextTag { public: unsigned long a, b; };         // 8 bytes
class CTextEntry : public CResource { public:                  // vtable 0x0129B4A4 (.?AVCTextEntry@NGameText@@), retail 0x44 bytes
    CCharString SpeechBank;                      // +0x1c
    CCharString Narrator;                        // +0x20
    CCharString SoundName;                       // +0x24
    CWideString Text;                            // +0x28
    CRetailVector<CTextTag> Tags;                // +0x2c
    CCountedPointer<CBankFileAsyncData> PFileData;   // +0x38
    CDataBankTextEntry* Owner;                   // +0x40
    CTextEntry(const CCountedPointer<CBankFileAsyncData>& data, CDataBankTextEntry* owner);
    virtual unsigned long Sizeof() const;
};
}
namespace NGameText {
unsigned long CTextEntry::Sizeof() const
{
    unsigned long size = sizeof(CTextEntry);
    size += SpeechBank.Sizeof() - sizeof(CCharString);
    size += Narrator.Sizeof() - sizeof(CCharString);
    size += SoundName.Sizeof() - sizeof(CCharString);
    size += Text.Sizeof() - sizeof(CWideString);
    size += Tags.capacity() * sizeof(CTextTag);
    size += (PFileData.Data ? PFileData.Data->Sizeof() + sizeof(PFileData) : sizeof(PFileData)) - sizeof(PFileData);
    return size;
}
}