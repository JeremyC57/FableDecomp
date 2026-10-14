// CManagedTextureData::CManagedTextureData() -- the CResource base constructor, two zero shorts at +0x1c/+0x1e, this class's vtable, then a zeroed 10-word record at +0x20 and three cleared flag bytes at +0x48
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A69980, length 80 bytes.
// Callees: CResource::CResource @ 0x009FBEC0. Vtable 0x0129DC24 = CManagedTextureData (rtti-vtables.tsv; the chunk label "CGPUScratchBufferTextureManager" is
// propagated). Shape: the shorts stored BEFORE the vtable (plain initialisers), the words and bytes after it (member objects with inline constructors), one
// zero register; `this` is returned.
struct CResource {
    virtual void v00();
    unsigned char _body[0x1c - 4];                 // sizeof = 0x1c
    CResource();                                   // 0x009FBEC0
};

struct STextureDims_00a69980 {                     // ten words, all cleared
    long m_0, m_4, m_8, m_c, m_10, m_14, m_18, m_1c, m_20, m_24;
    STextureDims_00a69980() : m_0(0), m_4(0), m_8(0), m_c(0), m_10(0), m_14(0), m_18(0), m_1c(0), m_20(0), m_24(0) { }
};
struct STextureFlags_00a69980 {                    // three flag bytes, all cleared
    bool m_a, m_b, m_c;
    STextureFlags_00a69980() : m_a(false), m_b(false), m_c(false) { }
};

struct CManagedTextureData : CResource {           // vtable 0x0129DC24
    short                  m_width;                // +0x1c
    short                  m_height;               // +0x1e
    STextureDims_00a69980  m_dims;                 // +0x20
    STextureFlags_00a69980 m_flags;                // +0x48
    virtual void v00();
    CManagedTextureData();
};

CManagedTextureData::CManagedTextureData()
    : CResource()
    , m_width(0), m_height(0)
{
}