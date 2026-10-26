// CRenderTarget::AttachTarget(const CSurface& target) -- attach a colour surface: uninitialise the current one if it holds a surface, then copy the new one in
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A10E60, length 35 bytes (+13 int3 padding).
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+8] ; test eax,eax ; je A ; lea ecx,[esi+4] ; call CSurface::Uninitialise ;
//                A: mov eax,[esp+8] ; push eax ; lea ecx,[esi+4] ; call CSurface::operator= ; pop esi ; ret 4
// Retail has two AttachTarget overloads: this one works on the colour surface (+4), its twin 0x00A10E90 on the depth
// surface (+0x14); the argument types of the two overloads are not recoverable from the bytes (both are a pointer to a
// surface), so this TU declares only the colour one. Callees: CSurface::Uninitialise @ 0x009F2E20,
// CSurface::operator= @ 0x009F2E70 (its returned reference is dropped).
struct CBase {
    CBase();                                    // 0x0099A2D0
    virtual ~CBase();                           // 0x0099A2E0
};

struct IDirect3DSurface9;

// engine/CSurface.h member names (+4 PD3DSurface, +8 AllocationSource, +0xC PAllocatedMemory); the generated header
// flattens the vptr into a plain word, so a polymorphic local model is used.
class CSurface { public:                               // vtable 0x0122F84C, 0x10 bytes
    virtual void v00();
    IDirect3DSurface9* PD3DSurface;             // +0x04
    unsigned long AllocationSource;             // +0x08
    void* PAllocatedMemory;                     // +0x0c
    bool IsInitialised() const { return PD3DSurface != 0; }
    void Uninitialise();                        // 0x009F2E20
    CSurface& operator=(const CSurface& other); // 0x009F2E70
};

class CRenderTarget : public CBase { public:                  // vtable 0x0129C800
    CSurface m_colour;                          // +0x04
    CSurface m_depth;                           // +0x14
    void AttachTarget(const CSurface& target);
};

void CRenderTarget::AttachTarget(const CSurface& target)
{
    if (m_colour.IsInitialised())
        m_colour.Uninitialise();
    m_colour = target;
}