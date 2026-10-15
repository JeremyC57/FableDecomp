// CRenderTarget::`scalar deleting destructor'(unsigned int flags) -- destroy the depth then the colour surface (each stores the CSurface vtable and uninitialises itself), run the CBase destructor, free for flag bit 0
// Grade: DIFFER(ours v retail) -- 66v60: an extra `mov dword ptr [esi],offset CRenderTarget::`vftable'` (the user-declared empty destructor stores the vptr; the implicit one is not inlined by the lane)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FBD00, length 60 bytes.
// Leaf: ??_GCRenderTarget
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+14h] ; mov dword ptr [ecx],offset CSurface::`vftable' ; call CSurface::Uninitialise ;
//                lea ecx,[esi+4] ; mov dword ptr [ecx],offset CSurface::`vftable' ; call CSurface::Uninitialise ; mov ecx,esi ;
//                call CBase::~CBase ; test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ;
//                pop esi ; ret 4
// Manifest label says `vector deleting destructor'; the body only tests flag bit 0, so it is the scalar one.
// Model from the landed CRenderTarget constructor 0x009F9CD0: CBase root, two embedded CSurface members (vtable
// 0x0122F84C) at +4 (colour) and +0x14 (depth). Callees: CSurface::Uninitialise @ 0x009F2E20, CBase::~CBase @ 0x0099A2E0,
// operator delete @ 0x00BFE9BC.
// Shape: no CRenderTarget vtable store in retail, while CSurface's user-written inline destructor stores its vtable
// before calling Uninitialise. Tried: implicit destructor (the lane calls ??1 out of line, 30v60, also with /Ob2 and a
// __forceinline CSurface destructor), novtable (no ??_G emitted). This best form inlines everything but keeps the
// CRenderTarget vtable store (66v60).
// The `new` factory pulls the vtable (and so the deleting destructor) into the object; it is not part of the retail body.
void __cdecl operator delete(void* block);          // 0x00BFE9BC

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
    CSurface() : PD3DSurface(0), AllocationSource(0) { }
    void Uninitialise();                        // 0x009F2E20
    ~CSurface() { Uninitialise(); }
};

struct CRenderTarget : CBase {                  // vtable 0x0129C800
    CSurface m_colour;                          // +0x04
    CSurface m_depth;                           // +0x14
    __forceinline virtual ~CRenderTarget() { }
};

void* MakeOne_009fbd00()
{
    return new CRenderTarget;
}
