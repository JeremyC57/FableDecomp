// CResourceHolder_00642a60::~CResourceHolder_00642a60() -- free the buffer at +0x30, destroy the member at +0x24 out of line, destroy the polymorphic member at +0x14 (its vtable 0x0122F84C, then its base destructor out of line), then free the buffer at +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00642A60, length 58 bytes.
// Callees: free @ 0x00BFEA14 twice, the member destructor @ 0x009F9F70, the polymorphic member's base destructor @ 0x009F2E20. Shape: members destroyed in reverse
// declaration order; the inline derived destructor of the +0x14 member stores its vtable then tail-calls nothing (the base destructor is a plain call). The chunk
// label is propagated; the class is named by address.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct CTexture {
    unsigned char _body[0xc];
    void Uninitialise();                           // 0x009F9F70 ?Uninitialise@CTexture@@QAEXXZ
    ~CTexture() { Uninitialise(); }
};

struct CHolderBase_009f2e20 {
    virtual void v00();
    unsigned char _pad[0xc];                       // +0x04 .. +0x10
    ~CHolderBase_009f2e20();                       // 0x009F2E20
};

struct CHolder_00642a60 : CHolderBase_009f2e20 {   // vtable 0x0122F84C
    virtual void v00();
    ~CHolder_00642a60() { }
};

struct CBuffer_00642a60 { void* m_pBlock; ~CBuffer_00642a60() { if (m_pBlock) free(m_pBlock); } };

struct CResourceHolder_00642a60 {
    unsigned char    _pad[8];
    CBuffer_00642a60 m_bufferA;                    // +0x08
    unsigned char    _pad0c[8];
    CHolder_00642a60 m_holder;                     // +0x14
    CTexture m_member;                     // +0x24
    CBuffer_00642a60 m_bufferB;                    // +0x30
    ~CResourceHolder_00642a60();
};

CResourceHolder_00642a60::~CResourceHolder_00642a60()
{
}