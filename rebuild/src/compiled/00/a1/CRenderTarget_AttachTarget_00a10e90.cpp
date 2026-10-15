// CRenderTarget::AttachTarget(const CSurface& target) -- attach a depth surface: uninitialise the current one if it holds a surface, then copy the new one in
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A10E90, length 35 bytes.
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+18h] ; test eax,eax ; je A ; lea ecx,[esi+14h] ; call CSurface::Uninitialise ;
//                A: mov eax,[esp+8] ; push eax ; lea ecx,[esi+14h] ; call CSurface::operator= ; pop esi ; ret 4
// The depth-surface overload (+0x14); its twin 0x00A10E60 attaches the colour surface (+4). The two overloads differ only
// in their (unrecoverable) argument types, so this TU declares only this one. Callees: CSurface::Uninitialise @ 0x009F2E20,
// CSurface::operator= @ 0x009F2E70 (its returned reference is dropped).
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
    void AttachTarget(const CSurface& target);
};

void CRenderTarget::AttachTarget(const CSurface& target)
{
    if (m_depth.IsInitialised())
        m_depth.Uninitialise();
    m_depth = target;
}