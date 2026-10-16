// NGameText::CTextEntry::CTextEntry(const CCountedPointer<CBankFileAsyncData>&, CDataBankTextEntry*) -- empty strings and tags, shared file data, owner; register its size
// Leaf: ??0CTextEntry@NGameText@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C8250, 106 bytes (+6 int3). __fastcall, `ret 8`.
// Callees: CResource::CResource() 0x009FBEC0; CCharString::CCharString() 0x0099E4B0 (x3); CWideString::CWideString() 0x0099AED0;
//          NGameText::CTextEntry::Sizeof() const 0x009C7D90 (called directly); CResource::SetResourceSize(unsigned long)
//          0x009FBF20. Vtable 0x0129B4A4 (.?AVCTextEntry@NGameText@@).
// Shape: members in declaration order in the init list (the tag vector's three zero words share one `xor ecx,ecx`, the
//        counted pointer copy adds a reference when Info is set); the body is SetResourceSize(CTextEntry::Sizeof()).
// Layout: retail 0x44 bytes: SpeechBank +0x1C, Narrator +0x20, SoundName +0x24, Text +0x28, Tags +0x2C, PFileData +0x38,
//         Owner +0x40.
struct CCharString {
    void* PStringData;
    CCharString();                               // 0x0099E4B0
    unsigned long Sizeof() const;                // 0x0099E500
};
struct CWideString {
    void* PStringData;
    CWideString();                               // 0x0099AED0
    unsigned long Sizeof() const;                // 0x0099B2A0
};
struct CCPPointerInfo { long Count; };
struct CBankFileAsyncData {
    unsigned long Sizeof() const;                // 0x009D5620 (catalog "CVertexBufferWin32::DoSizeof" is a byte twin)
};
template <class T> struct CCountedPointer {
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
struct CResource {                               // 0x1C bytes
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
struct CTextTag { unsigned long a, b; };         // 8 bytes
struct CTextEntry : CResource {                  // vtable 0x0129B4A4 (.?AVCTextEntry@NGameText@@), retail 0x44 bytes
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
CTextEntry::CTextEntry(const CCountedPointer<CBankFileAsyncData>& data, CDataBankTextEntry* owner)
    : PFileData(data), Owner(owner)
{
    SetResourceSize(CTextEntry::Sizeof());
}
}