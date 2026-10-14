// NUISystem::CUIStateDef::~CUIStateDef() -- free the two owned buffers (+0x6c, then +0x29), then the inlined CDefPointeeBase destructor stores its vtable and tail-calls ~CResource
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00624DC0, length 49 bytes.
// Callees: free @ 0x00BFEA14 (x2, each under an explicit null test; the result dropped with `add esp,4`), CResource::~CResource
// @ 0x009FC550 (tail call; the manifest labels it "~CMemoryBlock", a propagated name). Vtable
// 0x01230BA0 = CDefPointeeBase (rtti-vtables.tsv): the inlined empty intermediate destructor's store.
// The class is NUISystem::CUIStateDef: its scalar deleting destructor @ 0x00625160 (slot 0 of vtable 0x0125871C) inlines this exact body (+0x6c then +0x29), and this plain destructor is called on a stack object @ 0x00632EDD; the manifest label "~CCompressedMemoryDataInputStream" is propagated.
// Same shape as the landed 0x004D9861 / 0x004E27E0: the most-derived class is `__declspec(novtable)`
// (abstract, its own vtable store never appears), its two buffer members are destroyed in reverse
// declaration order by an inline `if (p) free(p)` destructor, and a non-virtual destructor beside a
// virtual function keeps the lander on the plain `??1` body. The def-class layout is byte packed
// (the second buffer sits at the unaligned +0x29, as every CParentDefClassBase-derived def).

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

struct CDefPointeeBase : CResource_Calls {                  // vtable 0x01230BA0
    virtual void v00();
    ~CDefPointeeBase() { }                                  // inline, empty: inlined into every derived destructor
};

#pragma pack(push, 1)                                       // def classes are byte packed in retail (Ego_r.pdb: CParentDefClassBase is 0x39 bytes)
struct __declspec(novtable) CUIStateDef : CDefPointeeBase {
    unsigned char   _pad0[0x29 - 0x04];
    CFreeableBuffer m_buffer0;                              // +0x29 (destroyed second)
    unsigned char   _pad1[0x6c - 0x29 - 4];
    CFreeableBuffer m_buffer1;                              // +0x6c (destroyed first)
    virtual void v00();
    ~CUIStateDef();
};
#pragma pack(pop)

CUIStateDef::~CUIStateDef()
{
}