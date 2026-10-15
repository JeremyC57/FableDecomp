// NGraphicAppearance::CTCGraphicAppearanceStaticMeshFadeOutOnProximity::`scalar deleting destructor'(unsigned int) -- this class's two vtables (primary 0x0126AE04, CAGraphicAppearanceComponent-side 0x0126ADEC at +0xc); unless the owner thing is dead (bit 0 of +0x91) the component unregisters itself from the owner's CTCGraphicAppearance (+0x64); the CAGraphicAppearanceComponent vtable, ~CTCBase, and operator delete for flag bit 0
// Leaf: ??_GCTCGraphicAppearanceStaticMeshFadeOutOnProximity@NGraphicAppearance@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077C2A0, length 76 bytes (manifest label "CTCGraphicAppearanceStaticMesh::`vector_deleting_destructor'"
// is propagated across the three byte-identical graphic-appearance component bodies 0x0077BD20 / 0x0077C2A0 / 0x0077C7E0; the vtable stores name the class).
// Callees: NGraphicAppearance::CTCGraphicAppearance::UnregisterComponent @ 0x004C0000 (takes the CAGraphicAppearanceComponent
// subobject: `push edi` = this+0xc), CTCBase::~CTCBase @ 0x00686830 (manifest "~CChunkedFileChunk" is propagated), operator delete @ 0x00BFE9BC.
// Shape: CTCBase (vptr, ParentThing +4, two bools +8/+9) is the primary base, CAGraphicAppearanceComponent (vptr only, vtable
// 0x0125A170) the secondary one at +0xc. The derived destructor must be `__forceinline virtual` and the TU must instantiate the
// class through `new T(thing)` (an inline constructor), else VC7.1 emits the ??_G as `call ??1 ; delete`. The secondary base's
// destructor is inline and empty, so only its vtable store survives (after the unregister call, before ~CTCBase).
// Local models: no engine headers exist for these classes; CThing is reduced to the two members the body reads;
// engine/CTCBase.h is a flattened POD (vptr as a data member), so the polymorphic CTCBase is modelled locally.
void __cdecl operator delete(void* block);                  // 0x00BFE9BC
struct CPrimitive;

namespace NGraphicAppearance {
struct CAGraphicAppearanceComponent;
struct CTCGraphicAppearance {
    void UnregisterComponent(CAGraphicAppearanceComponent* component);   // 0x004C0000
};
}

struct CThing {
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;      // +0x64
    unsigned char _pad68[0x29];
    unsigned char Dead : 1;                                 // +0x91 bit 0
};

struct CTCBase {
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    CTCBase(CThing* thing);                                 // 0x00686800
    virtual ~CTCBase();                                     // 0x00686830
};

namespace NGraphicAppearance {
struct CAGraphicAppearanceComponent {                       // vtable 0x0125A170
    virtual void OnAppearanceDraw();
    ~CAGraphicAppearanceComponent() { }
};

struct CTCGraphicAppearanceStaticMeshFadeOutOnProximity : CTCBase, CAGraphicAppearanceComponent {   // vtables 0x0126AE04 / 0x0126ADEC
    CPrimitive* PrimHandle;                                 // +0x10
    CTCGraphicAppearanceStaticMeshFadeOutOnProximity(CThing* thing) : CTCBase(thing), PrimHandle(0) { }
    __forceinline virtual ~CTCGraphicAppearanceStaticMeshFadeOutOnProximity()
    {
        if (!ParentThing->Dead)
            ParentThing->PTCGraphicAppearance->UnregisterComponent(this);
    }
    virtual void OnAppearanceDraw();
};
}

// The factory only exists to instantiate the class (and with it the deleting destructor) in this TU.
CTCBase* MakeFadeOutMesh(CThing* thing)
{
    return new NGraphicAppearance::CTCGraphicAppearanceStaticMeshFadeOutOnProximity(thing);
}
