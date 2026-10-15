// CRenderStateManager::~CRenderStateManager() -- store this class's vtable, clear the global render-state-manager pointer and the device pointer, then run the CBaseClassNonCopyable base destructor (tail jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A044B0, length 21 bytes.
// Leaf: ??1CRenderStateManager
// Retail bytes:  xor eax,eax ; mov dword ptr [ecx],offset CRenderStateManager::`vftable' ; mov [013CB508h],eax ;
//                mov [ecx+4],eax ; jmp CBaseClassNonCopyable::~CBaseClassNonCopyable
// Vtable 0x0129C978 is CRenderStateManager (rtti-vtables.tsv); 0x013CB508 is the render-state-manager singleton pointer
// (landed CRenderStateManager scalar deleting destructor 0x00A07980 inlines exactly this body); +0x04 is PD3DDevice
// (engine/CRenderStateManager.h). Callee: CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300 (tail jump).
// Model: a polymorphic local model (the generated header holds the vptr as a plain word). The destructor is declared
// virtual as in retail; the `Leaf` selects the plain destructor `??1` over the deleting one.
struct IDirect3DDevice9;
struct CRenderStateManager;

extern CRenderStateManager* g_pRenderStateManager;   // 0x013CB508

struct CBaseClassNonCopyable {
    virtual void v00();
    ~CBaseClassNonCopyable();                        // 0x0099A300
};

struct CRenderStateManager : CBaseClassNonCopyable { // vtable 0x0129C978
    IDirect3DDevice9* PD3DDevice;                    // +0x04
    virtual ~CRenderStateManager();
};

CRenderStateManager::~CRenderStateManager()
{
    g_pRenderStateManager = 0;
    PD3DDevice = 0;
}