// CTCSpotLight::OnCreate() -- fetch the spotlight sub-def; unless overridden, create the light info on demand and copy colour, radii, flicker, angle and width from the def
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00731E20, length 121 bytes (padded row 128); CTCSpotLight vtable 0x01266FBC slot 4
// ("vfunc_4" on CTC classes is OnCreate).
// Callees: CDefClassBase::GetSubDef<CSpotLightDef>(CDefPointer<const CSpotLightDef>*) @ 0x007325D0 (manifest label
// GetSubDef<CAICreatureWillPowerIndicatorDef> is a byte twin; owner def +0x70), operator new @ 0x00BFEA1A (0x18 bytes, POD info, no ctor),
// CCountedPointer<CTCSpotLight::CSpotLightInfo>::Reset(CSpotLightInfo*) @ 0x007323D0 (manifest "CCountedPointer<CIPhysicsFacesSegmentBase>
// ::Reset" is a byte twin).
// Layout (retail = PDB, engine/CTCSpotLight.h): PLightInfo.Data +0x10, PLightDef.Object +0x18, Overridden +0x1C. CSpotLightDef (PDB 0x44):
// Colour +0x28, InnerRadius +0x2C, OuterRadius +0x30, Width +0x34, Flicker +0x38, Angle +0x40. CSpotLightInfo (0x18, no PDB layout;
// offsets from these stores and the landed SetAngle 0x00731BE0): Angle +0, Width +4, InnerRadius +8, OuterRadius +0xC, Flicker +0x10,
// Colour +0x14.
// Shape: every copy re-reads both pointers (the stores may alias); `PLightInfo` address kept in esi, `&PLightDef` in edi.
struct CCPPointerInfo;
struct CRGBColour {
    unsigned char B, G, R, A;
};
struct CSpotLightDef {
    unsigned char _pad00[0x28];
    CRGBColour Colour;                              // +0x28
    float InnerRadius;                              // +0x2c
    float OuterRadius;                              // +0x30
    float Width;                                    // +0x34
    float Flicker;                                  // +0x38
    float FlickerSpeed;                             // +0x3c
    float Angle;                                    // +0x40
};
template<class T> struct CDefPointer {
    T* Object;
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<const CSpotLightDef>* out) const;   // 0x007325D0
};
struct CThing {
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                            // +0x70
};
struct CSpotLightInfo {                             // CTCSpotLight::CSpotLightInfo, 0x18 bytes
    float Angle;                                    // +0x00
    float Width;                                    // +0x04
    float InnerRadius;                              // +0x08
    float OuterRadius;                              // +0x0c
    float Flicker;                                  // +0x10
    CRGBColour Colour;                              // +0x14
};
template<class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    void Reset(T* p);                               // 0x007323D0
};
void* operator new(unsigned int size);              // 0x00BFEA1A
struct CTCSpotLight {
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _base08[4];
    void* _vfptrComponent;                          // +0x0c CAGraphicAppearanceComponent (vtable 0x01266FA4)
    CCountedPointer<CSpotLightInfo>  PLightInfo;    // +0x10
    CDefPointer<const CSpotLightDef> PLightDef;     // +0x18
    bool  Overridden;                               // +0x1c
    void OnCreate();
};

void CTCSpotLight::OnCreate()
{
    ParentThing->PDef->GetSubDef(&PLightDef);
    if (!Overridden && PLightDef.Object) {
        if (!PLightInfo.Data)
            PLightInfo.Reset((CSpotLightInfo*)operator new(sizeof(CSpotLightInfo)));
        PLightInfo.Data->Colour = PLightDef.Object->Colour;
        PLightInfo.Data->InnerRadius = PLightDef.Object->InnerRadius;
        PLightInfo.Data->OuterRadius = PLightDef.Object->OuterRadius;
        PLightInfo.Data->Flicker = PLightDef.Object->Flicker;
        PLightInfo.Data->Angle = PLightDef.Object->Angle;
        PLightInfo.Data->Width = PLightDef.Object->Width;
    }
}
