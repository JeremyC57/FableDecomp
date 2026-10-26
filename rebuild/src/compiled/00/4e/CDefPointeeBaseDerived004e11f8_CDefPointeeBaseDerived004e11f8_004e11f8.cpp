// CDefPointeeBase_Derived_004e11f8::~CDefPointeeBase_Derived_004e11f8() -- free the two owned buffers (+0x44, then +0x38), then the inlined CDefPointeeBase destructor stores its vtable and tail-calls ~CResource
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E11F8, length 45 bytes.
// Callees: free @ 0x00BFEA14 (x2, each under an explicit null test; the result dropped with `pop ecx`: a size-optimised TU), CResource::~CResource
// @ 0x009FC550 (tail call; the manifest labels it "~CMemoryBlock", a propagated name). Vtable
// 0x01230BA0 = CDefPointeeBase (rtti-vtables.tsv): the inlined empty intermediate destructor's store.
// The class cannot be named from the body (the only caller is its own deleting destructor @ 0x004E11DF); the manifest label "~CCompressedMemoryDataInputStream" is propagated.
// Same shape as the landed 0x004D9861 / 0x004E27E0: the most-derived class is `__declspec(novtable)`
// (abstract, its own vtable store never appears), its two buffer members are destroyed in reverse
// declaration order by an inline `if (p) free(p)` destructor, and a non-virtual destructor beside a
// virtual function keeps the lander on the plain `??1` body. The def-class layout is byte packed
// (the second buffer sits at the unaligned +0x38, as every CParentDefClassBase-derived def).
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

struct CFreeableBuffer {                                    // a malloc'd block owned by the def
    void* m_pData;
    ~CFreeableBuffer()
    {
        if (m_pData != 0)
            free(m_pData);
    }
};

struct CResource_Calls {                                    // engine/CResource.h: the 0x1c-byte root
    virtual void v00();
    ~CResource_Calls();                                     // 0x009FC550
};

class CDefPointeeBase : public CResource_Calls { public:                  // vtable 0x01230BA0
    virtual void v00();
    ~CDefPointeeBase() { }                                  // inline, empty: inlined into every derived destructor
};

#pragma pack(push, 1)                                       // def classes are byte packed in retail (Ego_r.pdb: CParentDefClassBase is 0x39 bytes)
struct __declspec(novtable) CDefPointeeBase_Derived_004e11f8 : CDefPointeeBase {
    unsigned char   _pad0[0x38 - 0x04];
    CFreeableBuffer m_buffer0;                              // +0x38 (destroyed second)
    unsigned char   _pad1[0x44 - 0x38 - 4];
    CFreeableBuffer m_buffer1;                              // +0x44 (destroyed first)
    virtual void v00();
    ~CDefPointeeBase_Derived_004e11f8();
};
#pragma pack(pop)

CDefPointeeBase_Derived_004e11f8::~CDefPointeeBase_Derived_004e11f8()
{
}