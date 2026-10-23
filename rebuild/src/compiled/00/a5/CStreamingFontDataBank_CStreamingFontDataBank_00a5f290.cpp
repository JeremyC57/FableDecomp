// CStreamingFontDataBank::CStreamingFontDataBank() -- builds the CBankFile base, the font state block, the empty font array, an empty file name and a null streaming entry
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- CArray modelled as the STLport _Vector_base shape (start, finish, alloc-proxy end with its own constructor): the member-object boundary lets `mov ecx,edi` (Filename's this) schedule between the second and third zero store as in retail
// Leaf: ??0CStreamingFontDataBank
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5F290, 89 bytes (+7 int3 padding). Callees: CBankFile::CBankFile @
// 0x009D5F80, CCharString::CCharString() @ 0x0099E4B0, CCharString::operator=(const char*) @ 0x0099EFE0 (with the
// empty literal @ 0x0122D70E). Vtables: CStreamingFontDataBank 0x0129D5CC (+0x0), CStreamingFontBankStateBlock
// 0x0129D594 (the StateBlock member at +0x164, rtti-vtables.tsv).
// Layout: retail CBankFile is 0x164 bytes (PDB 0x174), so the members sit lower than in engine/CStreamingFontDataBank.h
// (a flattened view that cannot emit vtable stores): StateBlock +0x164 (vptr + CStateBlock), Fonts +0x178 (retail
// three-pointer CArray), Filename +0x184, streamingEntry +0x188 (CCountedPointer: Data, Info). Modelled locally with the
// PDB member names.
// Shape: the derived vtable store follows the base constructor, then the member constructors in declaration order
// (StateBlock's inline constructor stores its vtable; the CArray and counted pointer zero from ebx); `Filename = ""` is
// a body statement (the counted-pointer zero stores are scheduled between its push and its call).
namespace N_009d5f80 { class CBankFile {
public:
    CBankFile();                                     // 0x009D5F80
    virtual ~CBankFile();
    unsigned char _body[0x160];                      // +0x04 .. +0x163 (retail size 0x164)
}; } using N_009d5f80::CBankFile;

struct CStateBlock {
    long FontHeight;
    long FontWeight;
    long MinSpacing;
    bool Italics;
};

class CStreamingFontBankStateBlock {
public:
    virtual void Apply();                            // vtable 0x0129D594
    CStateBlock State;                               // +0x4
};

template<class T> struct CArrayAllocProxy {   // STLport _STLP_alloc_proxy: the end-of-storage pointer with its own constructor
    T* p;
    CArrayAllocProxy(T* v) : p(v) { }
};
template<class T> struct CArray {                    // retail CArray is 0x0c with the STLport _Vector_base shape (start, finish, alloc-proxy end)
    T*                  _M_start;
    T*                  _M_finish;
    CArrayAllocProxy<T> _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};

class CCharString {
public:
    CCharString();                                   // 0x0099E4B0
    CCharString& operator=(const char* text);        // 0x0099EFE0
    void* m_pStringData;
};

struct CCPPointerInfo;
template<class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
};

struct CStreamingFontDataBankEntry;
struct CBankFileAsyncEntry;

class CStreamingFontDataBank : public CBankFile {
public:
    CStreamingFontDataBank();
    virtual ~CStreamingFontDataBank();

    CStreamingFontBankStateBlock         StateBlock;       // +0x164
    CArray<CStreamingFontDataBankEntry*> Fonts;            // +0x178
    CCharString                          Filename;         // +0x184
    CCountedPointer<CBankFileAsyncEntry> streamingEntry;   // +0x188
};

CStreamingFontDataBank::CStreamingFontDataBank()
{
    Filename = "";
}