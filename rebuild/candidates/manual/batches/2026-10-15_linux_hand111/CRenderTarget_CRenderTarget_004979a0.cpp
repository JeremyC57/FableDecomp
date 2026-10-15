// CRenderTarget::~CRenderTarget() -- destroys the depth surface (+0x14) then the colour surface (+4) -- each stores the CSurface vtable 0x0122F84C and calls Uninitialise -- then tail-calls CBase::~CBase
// Leaf: ??1CRenderTarget
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004979A0, length 39 bytes (no code references).
// Callees: CSurface::Uninitialise @ 0x009F2E20 (x2), CBase::~CBase @ 0x0099A2E0 (tail jump).
// The catalog label "CThingNoiseDef::~CThingNoiseDef" is propagated (the PDB CThingNoiseDef holds a
// CTCNoiseDef, nothing like this): the layout is the landed CRenderTarget model (0x00A10E30,
// constructor 0x009F9CD0) -- CBase vptr, colour CSurface at +4, depth CSurface at +0x14.
// Shape: no vtable store of the class itself = a `__declspec(novtable)` class over CBase with an
// empty user destructor; CSurface's inline virtual destructor stores its vtable and calls
// Uninitialise(); members destroyed in reverse declaration order; the base destructor is the
// sibling-call `jmp`.
struct IDirect3DSurface9;
struct CSurface {                                  // vtable 0x0122F84C, 0x10 bytes
    IDirect3DSurface9* PD3DSurface;                // +0x04
    unsigned long      AllocationSource;           // +0x08
    void*              PAllocatedMemory;           // +0x0c
    void Uninitialise();                           // 0x009F2E20
    virtual ~CSurface() { Uninitialise(); }
};
struct CBase {
    virtual void v00();
    ~CBase();                                      // 0x0099A2E0
};
struct __declspec(novtable) CRenderTarget : CBase {
    CSurface ColourSurface;                        // +0x04
    CSurface DepthSurface;                         // +0x14
    ~CRenderTarget();
};

CRenderTarget::~CRenderTarget()
{
}
