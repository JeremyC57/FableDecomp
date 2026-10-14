// CTripleResource_00c1c230::~CTripleResource_00c1c230() -- this class's vtable, then the three resources at +0x10, +0xc and +8 released through their own `__cdecl` free functions and cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1C230, length 70 bytes.
// Callees: the releases @ 0x00C33990, @ 0x00C33820 and @ 0x00C334F0 (`__stdcall`, one pointer each; the manifest labels are propagated _Cons_val / CThingFilter symbols).
// Vtable 0x012B3290 (no RTTI name). Shape: the vtable store is scheduled between the first test and its `je`; each arm `push ; call ; mov [member],0` (the callees pop their argument). The chunk label is propagated; the class is named by address.
void __stdcall ReleaseA_00c33990(void* resource);    // 0x00C33990
void __stdcall ReleaseB_00c33820(void* resource);    // 0x00C33820
void __stdcall ReleaseC_00c334f0(void* resource);    // 0x00C334F0

struct CTripleResource_00c1c230 {                  // vtable 0x012B3290
    virtual void v00();
    unsigned char _pad[4];
    void*         m_pResourceC;                    // +0x08
    void*         m_pResourceB;                    // +0x0c
    void*         m_pResourceA;                    // +0x10
    ~CTripleResource_00c1c230();
};

CTripleResource_00c1c230::~CTripleResource_00c1c230()
{
    if (m_pResourceA) {
        ReleaseA_00c33990(m_pResourceA);
        m_pResourceA = 0;
    }
    if (m_pResourceB) {
        ReleaseB_00c33820(m_pResourceB);
        m_pResourceB = 0;
    }
    if (m_pResourceC) {
        ReleaseC_00c334f0(m_pResourceC);
        m_pResourceC = 0;
    }
}
