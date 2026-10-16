// CTCLight::CTCLight(CThing* thing) -- CTCBase(thing) + the graphic-appearance component base, everything cleared (flicker factors 1.0), then register with the owner's graphic appearance and fetch the light sub-def
// Leaf: ??0CTCLight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00640F00, length 120 bytes (`ret 4`). Vtables: CTCLight primary 0x0125A194, the
// NGraphicAppearance::CAGraphicAppearanceComponent base 0x0125A170 (stored first by its inline ctor) and CTCLight's side 0x0125A208 at +0xC
// (as the landed ~CTCLight 0x00640F80).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (catalog "CEngineSubPrimitive2DClockSprite" is propagated),
// NGraphicAppearance::CTCGraphicAppearance::RegisterComponent @ 0x004BD390 (owner +0x64, the +0xC subobject; catalog
// "DoubleLinkedListInsertNode" is propagated), CDefClassBase::GetSubDef<CLightDef> @ 0x0059346A (owner def +0x70; the catalog's
// <CAbilityDef> instantiation is a byte twin).
// Layout (retail = PDB CTCLight 0x4C with 12-byte STLport vectors, 0x44): PLightInfo +0x10, PLightDef +0x18, Active/Overridden
// +0x1C/+0x1D, AccumilativeFlickerFactor/OldAccumilativeFlickerFactor +0x20/+0x24, Initialised +0x28, FlareSprites +0x2C, Lights +0x38.
// Shape: init-list in declaration order; the vectors as the STLport _Vector_base + _STLP_alloc_proxy model (places `lea edi`); the zeroes share eax, the 1.0f pair edx; the body registers then calls GetSubDef(&PLightDef).
struct CThing;
struct CLightInfo;
struct CCPPointerInfo;
struct CDefClassBase;
struct CLightDef;
namespace NGraphicAppearance {
struct CPrimitive;
struct CAGraphicAppearanceComponent;
struct CTCGraphicAppearance {
    void RegisterComponent(CAGraphicAppearanceComponent* component);   // 0x004BD390
};
struct CAGraphicAppearanceComponent {               // vtable 0x0125A170
    virtual void OnAppearanceDraw();
    CAGraphicAppearanceComponent() { }
};
}
struct CThingDefHolder;
struct CThing {
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    unsigned char _pad68[0x70 - 0x68];
    CDefClassBase* PDef;                            // +0x70
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<const CLightDef>* out) const;   // 0x0059346A (GetSubDef<CLightDef>)
};
template<class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
template <class T> struct CAllocator {              // STLport allocator<T>: empty
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> {    // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {             // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template<class T> struct CPtrVector : CVectorBase<T*> {   // STLport vector<T*> (12 bytes)
    CPtrVector() : CVectorBase<T*>(CAllocator<T*>()) { }
};
struct CTCBase {
    virtual ~CTCBase();
    CThing* ParentThing;                            // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
    CTCBase(CThing* thing);                         // 0x00686800
};
struct CTCLight : CTCBase, NGraphicAppearance::CAGraphicAppearanceComponent {   // vtables 0x0125A194 / 0x0125A208
    CCountedPointer<CLightInfo>     PLightInfo;             // +0x10
    CDefPointer<const CLightDef>    PLightDef;              // +0x18
    bool  Active;                                           // +0x1c
    bool  Overridden;                                       // +0x1d
    float AccumilativeFlickerFactor;                        // +0x20
    float OldAccumilativeFlickerFactor;                     // +0x24
    bool  Initialised;                                      // +0x28
    CPtrVector<NGraphicAppearance::CPrimitive> FlareSprites;   // +0x2c
    CPtrVector<NGraphicAppearance::CPrimitive> Lights;         // +0x38
    CTCLight(CThing* thing);
    virtual ~CTCLight();
    virtual void OnAppearanceDraw();
};

CTCLight::CTCLight(CThing* thing)
    : CTCBase(thing)
    , Active(false)
    , Overridden(false)
    , AccumilativeFlickerFactor(1.0f)
    , OldAccumilativeFlickerFactor(1.0f)
    , Initialised(false)
{
    ParentThing->PTCGraphicAppearance->RegisterComponent(this);
    ParentThing->PDef->GetSubDef(&PLightDef);
}