// CRenderTarget::Uninitialise() -- uninitialise the colour surface and then the depth surface, each only if it holds a surface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A10E30, length 36 bytes (+12 int3 padding).
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+8] ; test eax,eax ; je A ; lea ecx,[esi+4] ; call CSurface::Uninitialise ;
//                A: mov eax,[esi+18h] ; test eax,eax ; je R ; lea ecx,[esi+14h] ; pop esi ; jmp CSurface::Uninitialise ; R: pop esi ; ret
// Model from the landed CRenderTarget constructor 0x009F9CD0: two embedded CSurface members (vtable 0x0122F84C) at +4
// (colour) and +0x14 (depth); the surface pointer is each surface's first word after its vptr. Callee:
// CSurface::Uninitialise @ 0x009F2E20 (the second call is a tail jump).
struct CBase {
    CBase();                                    // 0x0099A2D0
    virtual ~CBase();                           // 0x0099A2E0
};

struct IDirect3DSurface9;

// engine/CSurface.h member names (+4 PD3DSurface, +8 AllocationSource, +0xC PAllocatedMemory); the generated header
// flattens the vptr into a plain word, so a polymorphic local model is used.
struct CSurface {                               // vtable 0x0122F84C, 0x10 bytes
    virtual void v00();
    IDirect3DSurface9* PD3DSurface;             // +0x04
    unsigned long AllocationSource;             // +0x08
    void* PAllocatedMemory;                     // +0x0c
    bool IsInitialised() const { return PD3DSurface != 0; }
    void Uninitialise();                        // 0x009F2E20
    CSurface& operator=(const CSurface& other); // 0x009F2E70
};

struct CRenderTarget : CBase {                  // vtable 0x0129C800
    CSurface m_colour;                          // +0x04
    CSurface m_depth;                           // +0x14
    void Uninitialise();
};

void CRenderTarget::Uninitialise()
{
    if (m_colour.IsInitialised())
        m_colour.Uninitialise();
    if (m_depth.IsInitialised())
        m_depth.Uninitialise();
}