// NUISystem::CScrollingViewport::`scalar deleting destructor'(unsigned int flags) -- the three vtables of this class, the viewport removed from the front-end manager's observers, the active-scrolling-viewport global cleared if it is this one, CViewport::~CViewport, then operator delete for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053FC30, 87 bytes (vtable 0x012468D4 slot 0 = NUISystem::CScrollingViewport).
// Callees: CManager::GetInstance @ 0x0041E5F2 (`__fastcall` singleton), the manager's RemoveObserver
// (slot 5, +0x14) with the CObserver sub-object at +4, CViewport::~CViewport @ 0x0054EAC0 (manifest
// "CMesh::~CMesh" is a propagated name), operator delete @ 0x00BFE9BC. Global 0x013B8AAC = the active
// scrolling viewport (only CScrollingViewport bodies 0x0053E922..0x0053FC61 touch it).
//
// Shape: the inlined virtual destructor of the CMouseCursor family (landed ??_G 0x0055C8D0): the three
// vtable stores (the CObserver base pointer `lea edi,[esi+4]` is formed once and used for its vtable store
// and the RemoveObserver argument), the manager call, the `if (global == this) global = 0;` test, the
// out-of-line base destructor, the flag-tested delete. The `new` factory below pulls the vtable (and so
// this deleting destructor) into the object with the constructor inline (0x0053F120 in retail).
// Retail layout (Ego_r.pdb minus 0x34): CViewport 0x188 bytes, PScrollBar +0x188, PScrollBarOutside +0x18C,
// Changed +0x190, OffsetToUseAbove +0x194, OffsetToUseUnder +0x198 (same model as the landed constructor).
// Leaf: ??_GCScrollingViewport@NUISystem@@UAEPAXI@Z

void __cdecl operator delete(void* block);       // 0x00BFE9BC

struct IViewportPrimary {                        // the primary base (vptr at +0)
    virtual void v00();
};

struct CObserver {                               // the secondary base at +4 (vtable + 0x10 bytes)
    virtual void a00();
    unsigned char _pad04[0x10];
};

struct IViewportSecondaryB {                     // the secondary base at +0x18
    virtual void b00();
};

struct CViewport : IViewportPrimary, CObserver, IViewportSecondaryB {   // retail 0x188 bytes
    unsigned char _pad1c[0x188 - 0x1c];
    CViewport(void* def);                        // 0x0054EA00
    virtual ~CViewport();                        // 0x0054EAC0
};

namespace NUISystem { class CManager { public:
    virtual void v000(); virtual void v001();
    virtual void AddObserver(CObserver* observer);       // slot 2 (+0x08)
    virtual void v003(); virtual void v004();
    virtual void RemoveObserver(CObserver* observer);    // slot 5 (+0x14)
    static CManager* __fastcall GetInstance();   // 0x0041E5F2
}; }

namespace NUISystem {

class CComponent;
class CScrollingViewport;

extern CScrollingViewport* g_pActiveScrollingViewport;  // 0x013B8AAC

class CScrollingViewport : public CViewport { public:          // vtables 0x012468D4 / 0x012468AC / 0x012468A4
    virtual void v00();
    virtual void a00();
    virtual void b00();
    CComponent* PScrollBar;                      // +0x188
    CComponent* PScrollBarOutside;               // +0x18C
    bool        Changed;                         // +0x190
    float       OffsetToUseAbove;                // +0x194
    float       OffsetToUseUnder;                // +0x198
    CScrollingViewport(void* def)                // 0x0053F120 (inline here so the factory pulls the vtables in)
        : CViewport(def), PScrollBar(0), PScrollBarOutside(0), Changed(true), OffsetToUseAbove(0), OffsetToUseUnder(0)
    {
        GetDataFromDef();
    }
    __forceinline virtual ~CScrollingViewport()
    {
        CManager::GetInstance()->RemoveObserver(this);
        if (g_pActiveScrollingViewport == this)
            g_pActiveScrollingViewport = 0;
    }
    void GetDataFromDef();                       // 0x0053ED40
};

}

void* MakeOne_0053fc30(void* def)                // forces the vtables (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new NUISystem::CScrollingViewport(def);
}