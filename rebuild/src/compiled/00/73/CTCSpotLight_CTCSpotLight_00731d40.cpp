// CTCSpotLight::CTCSpotLight(CThing* thing) -- CTCBase(thing) + the graphic-appearance component base, light info/def/primitive cleared, flicker factors 1.0, then register with the owner's graphic appearance
// Leaf: ??0CTCSpotLight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00731D40, length 108 bytes (`ret 4`). Vtables (rtti-vtables.tsv): CTCSpotLight primary
// 0x01266FBC, CTCSpotLight's NGraphicAppearance::CAGraphicAppearanceComponent side 0x01266FA4 at +0xC; the component base's own
// vtable 0x0125A170 is stored first by its inline ctor (same as the landed CTCLight ctor 0x00640F00).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (catalog "CEngineSubPrimitive2DClockSprite" is propagated),
// NGraphicAppearance::CTCGraphicAppearance::RegisterComponent @ 0x004BD390 (owner +0x64, passed the +0xC subobject; catalog
// "DoubleLinkedListInsertNode" is propagated); the def release is the inline CDefPointer reset (pointee vtable slot 1 = Destroy).
// Layout (retail = PDB CTCSpotLight 0x2C, engine/CTCSpotLight.h): CAGraphicAppearanceComponent +0x0C, PLightInfo (CCountedPointer
// <CSpotLightInfo>) +0x10/+0x14, PLightDef (CDefPointer<const CSpotLightDef>) +0x18, Overridden +0x1C, LightPrimitive +0x20,
// AccumilativeFlickerFactor +0x24, OldAccumilativeFlickerFactor +0x28. The header's flattened pads cannot carry the two bases, so this
// is a local model with the same offsets.
// Shape: PLightInfo/PLightDef in the init list; the rest are body assignments in retail store order (`Accum = Old = 1.0f` stores
// +0x28 first), then `PLightDef = CDefPointer()` -- the inline operator= releases the old def through a pointer local (null test, `dec [ecx+4] ; jne`, slot 1; the member form re-reads),
// zeroes it inside the test and stores the new (null) object again.
struct CThing;
struct CCPPointerInfo;
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
struct CThing {
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
};
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long RefCount;                                  // +0x04
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (T* old = Object) {
            if (--old->RefCount == 0)
                old->Destroy();
            Object = 0;
        }
        Object = other.Object;
        return *this;
    }
};
struct CSpotLightDef : CDefPointeeBase { };
struct CSpotLightInfo;
template<class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
struct CTCBase {
    virtual ~CTCBase();
    CThing* ParentThing;                            // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
    CTCBase(CThing* thing);                         // 0x00686800
};
struct CTCSpotLight : CTCBase, NGraphicAppearance::CAGraphicAppearanceComponent {   // vtables 0x01266FBC / 0x01266FA4
    CCountedPointer<CSpotLightInfo>  PLightInfo;            // +0x10
    CDefPointer<CSpotLightDef>       PLightDef;             // +0x18
    bool  Overridden;                                       // +0x1c
    NGraphicAppearance::CPrimitive* LightPrimitive;         // +0x20
    float AccumilativeFlickerFactor;                        // +0x24
    float OldAccumilativeFlickerFactor;                     // +0x28
    CTCSpotLight(CThing* thing);
    virtual ~CTCSpotLight();
    virtual void OnAppearanceDraw();
};

CTCSpotLight::CTCSpotLight(CThing* thing)
    : CTCBase(thing)
{
    AccumilativeFlickerFactor = OldAccumilativeFlickerFactor = 1.0f;
    LightPrimitive = 0;
    Overridden = false;
    PLightDef = CDefPointer<CSpotLightDef>();
    ParentThing->PTCGraphicAppearance->RegisterComponent(this);
}