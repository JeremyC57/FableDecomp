// NGameText::CTextEntry::~CTextEntry() -- release the text entry's file data, destroy its tag array, text and the three name strings, then the CResource base
// Leaf: ??1CTextEntry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CB350, length 137 bytes.
// Label correction: the catalog's "CTCDParticleEmitter::~CTCDParticleEmitter" is propagated and wrong: the only caller is
// 0x009CB330, slot 0 (the deleting dtor) of vtable 0x0129B4A4 = NGameText::CTextEntry (rtti-vtables.tsv), and the members fit
// Ego_r.pdb CTextEntry with a retail 12-byte array.
// Callees: the count block's destroy function (+4, object at +8 in ecx), operator delete @ 0x00BFE9BC, CCharString::~CCharString
// @ 0x0099EAE0 (x4), free @ 0x00BFEA14 (the tag storage), CWideString::~CWideString @ 0x0099B510, the base dtor @ 0x009FC550
// (tail jump; it stores the CResource vtable 0x012354A4, so the manifest's "~CMemoryBlock" names its byte twin).
// Layout: CResource base 0x1C; SpeechBank +0x1C, Narrator +0x20, SoundName +0x24, Text +0x28 (CWideString), Tags +0x2C
// (CArray<NGameText::CTextTag>: STLport vector of 8-byte tags, a CCharString at +4), PFileData +0x38 (CCountedPointer<
// CBankFileAsyncData>: object +0x38, count block +0x3C), Owner +0x40.
// Shape: members destroyed in reverse order; the counted pointer's inline dtor re-reads the block after the decrement and nulls
// object then block; the array dtor is STLport `_Destroy(start, finish)` (the bounds passed by value, so finish stays in ebx) then
// frees start when set; no vtable store of its own: CTextEntry is `__declspec(novtable)` here (the out-of-line base dtor restores
// CResource's vtable).
extern "C" void __cdecl free(void* block);          // 0x00BFEA14
void __cdecl operator delete(void* block);          // 0x00BFE9BC

class CCharString { public:
    void* PStringData;
    ~CCharString();                                 // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    ~CWideString();                                 // 0x0099B510
};
struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4 per-type deleter, object in ecx
    void* m_pObject;                                // +8
};
template <class T>
class CCountedPointer { public:
    T* Data;                                        // +0
    SCountBlock* Info;                              // +4
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
namespace NGameText {
class CTextTag { public:                                   // 8 bytes
    long        Position;                           // +0
    CCharString Tag;                                // +4
};
}
inline void _Destroy(NGameText::CTextTag* first, NGameText::CTextTag* last)   // STLport range destroy (bounds by value)
{
    for (; first != last; ++first)
        first->~CTextTag();
}
struct CTextTagArray {                              // CArray<NGameText::CTextTag> (STLport vector, 12 bytes)
    NGameText::CTextTag* _M_start;
    NGameText::CTextTag* _M_finish;
    NGameText::CTextTag* _M_end_of_storage;
    ~CTextTagArray()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
class CResource { public:                                  // 0x1c bytes
    virtual ~CResource();                           // 0x009FC550
    unsigned char _body[0x18];
};
class CBankFileAsyncData;
struct CDataBankTextEntry;
namespace NGameText {
struct __declspec(novtable) CTextEntry : CResource {
    CCharString SpeechBank;                         // +0x1c
    CCharString Narrator;                           // +0x20
    CCharString SoundName;                          // +0x24
    CWideString Text;                               // +0x28
    CTextTagArray Tags;                             // +0x2c
    CCountedPointer<CBankFileAsyncData> PFileData;  // +0x38
    CDataBankTextEntry* Owner;                      // +0x40
    virtual ~CTextEntry();
};

CTextEntry::~CTextEntry()
{
}
}