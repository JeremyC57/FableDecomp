// CRenderManagerCore::`scalar deleting destructor'(unsigned int flags) -- store the core's vtable, shut the system font down if it was initialised, destroy the font and the render-state-manager base, free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0B590, length 71 bytes.
// Leaf: ??_GCRenderManagerCore
// Retail bytes:  push esi ; mov esi,ecx ; push edi ; lea edi,[esi+3A4Ch] ; mov ecx,edi ; mov dword ptr [esi],offset CRenderManagerCore::`vftable' ;
//                call IsInitialised ; test al,al ; je K ; mov ecx,edi ; call Uninitialise ; K: mov ecx,edi ; call ~CInitBaseClass ;
//                mov ecx,esi ; call ~CRenderStateManager ; test byte ptr [esp+0Ch],1 ; je D ; push esi ; call operator delete ;
//                add esp,4 ; D: pop edi ; mov eax,esi ; pop esi ; ret 4
// Vtable 0x0129C9A4 is CRenderManagerCore (rtti-vtables.tsv, slot 0 = this function). Callees: CInitBaseClass
// IsInitialised @ 0x0099A330 / Uninitialise @ 0x00A6C120 (the landed CRenderManagerCore::Uninit 0x00A0A9D0 is exactly
// this test, inlined here as the destructor body), ~CInitBaseClass @ 0x0099A410 on SystemFont (+0x3A4C),
// CRenderStateManager::~CRenderStateManager @ 0x00A044B0 (out of line in this TU), operator delete @ 0x00BFE9BC.
// Model: a local polymorphic model is required (the generated engine/CRenderManagerCore.h flattens the base and the
// vptr into plain bytes); member names and offsets are the PDB ones. SystemFont's own destructor is implicit, so it
// stores no vtable of its own before the base destructor runs. The `new` factory pulls the vtable (and so the
// deleting destructor) into the object; it is not part of the retail body.
void __cdecl operator delete(void* block);          // 0x00BFE9BC

class CDisplayManager;

// engine/CRenderStateManager.h gives the 0x3A3C-byte layout (flattened vptr); only its size matters here.
class CRenderStateManager { public:                         // 0x3A3C bytes, vtable 0x0129C978
    CRenderStateManager();
    virtual ~CRenderStateManager();                  // 0x00A044B0
    unsigned char _state[0x3a38];                    // +0x04 .. +0x3A3B (device pointer, state stacks, device state)
};

class CInitBaseClass { public:                              // vptr + one word
    virtual void v00();
    bool IsInitialised();                            // 0x0099A330
    void Uninitialise_00a6c120();                             // 0x00A6C120
    ~CInitBaseClass();                               // 0x0099A410
    void* m_pInitData;                               // +0x04
};

struct CSystemFont : CInitBaseClass {
};

struct CRenderManagerCore : CRenderStateManager {    // vtable 0x0129C9A4
    CDisplayManager* DisplayManager;                 // +0x3A3C
    float LastRenderEndTime;                         // +0x3A40
    long NoTrisPerSecond;                            // +0x3A44
    long ScreenPixelFormat_FormatIndex;              // +0x3A48
    CSystemFont SystemFont;                          // +0x3A4C
    bool Rendering;                                  // +0x3A54
    bool ResolutionReinit;                           // +0x3A55
    bool ResolutionSet;                              // +0x3A56
    bool TexturesEnabled;                            // +0x3A57
    unsigned char _rest[0x3d00 - 0x3a58];            // +0x3A58 .. +0x3CFF (caps, counters, device pointers)

    void Uninit()
    {
        if (SystemFont.IsInitialised())
            SystemFont.Uninitialise_00a6c120();
    }

    __forceinline virtual ~CRenderManagerCore()
    {
        Uninit();
    }
};

void* MakeOne_00a0b590()
{
    return new CRenderManagerCore;
}