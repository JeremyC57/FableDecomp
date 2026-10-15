// CFontDataBank::`scalar deleting destructor'(unsigned int) -- stores the CFontDataBank vtable, closes the fonts, destroys the string at +0x12c, frees the array block at +0x120, the inline destructor of the CBankStateBlock member at +0x110 stores its vtable, then CBankFile::~CBankFile and a free on bit 0
// Leaf: ??_GCFontDataBank
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A63B30, length 83 bytes (vtable 0x0129DA84 slot 0).
// Callees: CFontDataBank::CloseFonts @ 0x00A633A0, CCharString::~CCharString @ 0x0099EAE0 (manifest
// "_Cons_val<...>", propagated), free @ 0x00BFEA14, CBankFile::~CBankFile @ 0x009CE180 (manifest
// "NHeroInformationScreens::CBase::~CBase", propagated), operator delete @ 0x00BFE9BC.
// The catalog label "CThingTrackNode::vector_deleting_destructor" is propagated: the only reference is
// slot 0 of the CFontDataBank vtable 0x0129DA84.
// Layout: engine/CFontDataBank.h is opaque at these offsets; modelled locally: the CBankFile base
// (0x110 bytes, vptr at +0), a polymorphic CBankStateBlock member at +0x110 (vtable 0x0129C82C, 0x10
// bytes), a 0x0c-byte CArray at +0x120 and a CCharString at +0x12c.
// Shape: the destructor body (CloseFonts), then the members in reverse order: the string, the CArray's
// inline `if (data) free(data)`, the state block's inline destructor (its vtable store survives because
// the base destructor call follows), then the base.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CCharString {
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
};

struct CBankFile {                                 // polymorphic base, 0x110 bytes
    virtual ~CBankFile();                          // 0x009CE180
    unsigned char _body[0x10c];
    CBankFile();
};

struct CBankStateBlock {                           // vtable 0x0129C82C
    virtual void v00();
    unsigned char _body[0x0c];
    CBankStateBlock() { }
    ~CBankStateBlock() { }
};

struct CFontArray {                                // CArray<...>, 0x0c bytes
    void* m_pData;
    long  m_size;
    long  m_capacity;
    CFontArray() : m_pData(0), m_size(0), m_capacity(0) { }
    ~CFontArray() { if (m_pData) free(m_pData); }
};

struct CFontDataBank : CBankFile {                 // vtable 0x0129DA84
    CBankStateBlock m_stateBlock;                  // +0x110
    CFontArray      m_fonts;                       // +0x120
    CCharString     m_name;                        // +0x12c
    void CloseFonts();                             // 0x00A633A0
    CFontDataBank() { }
    __forceinline virtual ~CFontDataBank() { CloseFonts(); }
};

CFontDataBank* Create_CFontDataBank()
{
    return new CFontDataBank;
}
