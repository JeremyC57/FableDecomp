// Grade: DIFFER(239v239) -- the params temporary's vtable and Rhs immediate stores are sunk below the physics vtable load and the push (retail stores them first), and the GetSubDef out pointer gets its own frame word (sub esp,2Ch) where retail homes it in the dead position slot; probe 00779d20_v2 (a __forceinline GetAppearanceDef helper) fixes the slot but returns the def through eax (253v239)
// CTCDExperienceOrb::Construct(const C3DVector& position) -- place the orb's physics at the position (default facing), then give its graphic appearance the graphic of the thing's appearance def and flag the appearance for update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00779D20, 239 bytes (`ret 4`, 0x28-byte frame = the physics params object).
// Callees: CBaseClassNonCopyable ctor @ 0x0099A2F0 / dtor ~CBaseClassNonCopyable @ 0x0099A300 (the CTCParamsPhysicsBase temporary,
// vtable 0x0125C818), the physics TC's virtual slot 3 ConstructFromParams (+0x0c), CDefClassBase::GetSubDef<CAppearanceDef>
// @ 0x0042B0A2 (byte-twin label "GetSubDef<CAbilityDef>"), the defs' virtual slot 1 on the last release.
// Layout: CThing PhysicsTC +0x60, PTCGraphicAppearance +0x64, PDef +0x70 (retail); CTCParamsPhysicsBase (Ego_r.pdb, as the landed
// ctor 0x006A06E0): Position +0x04, Rhs +0x10 (Up 0,0,1 / Forward 0,1,0 here); CAppearanceDef Graphic +0x25 (packed);
// retail CTCGraphicAppearance Graphic +0x44 (CEngineGraphic, 14 bytes, pack(2)), the update-flag byte +0x59.
// Shape: the physics TC is read into a named local before the params temporary is built inline (base ctor, vtable, a
// memberwise position copy, immediate Rhs stores) and destroyed through the base dtor (no user dtor); the def lookup is the
// hand-written add-ref/GetSubDef/release on the thing's def with the out pointer zeroed between the load and its null test
// (GOTCHAS chunk 166), the out pointer homed in the dead `position` slot; the graphic is a 3-dword + word struct copy.
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet() : Up(0.0f, 0.0f, 1.0f), Forward(0.0f, 1.0f, 0.0f) { }
};
struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                            // 0x0099A2F0
    ~CBaseClassNonCopyable();                           // 0x0099A300
};
struct CTCParamsBase : CBaseClassNonCopyable { };
struct CTCParamsPhysicsBase : CTCParamsBase {           // vtable 0x0125C818
    C3DVector       Position;                           // +0x04
    CRightHandedSet Rhs;                                // +0x10
    CTCParamsPhysicsBase(const C3DVector& position) : Position(position) { }
    virtual void v00();
};
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void ConstructFromParams(CTCParamsBase* params);   // slot 3 (+0x0c)
};
#pragma pack(push, 2)
struct CEngineGraphic {                                 // Ego_r.pdb: 14 bytes (2-byte packed)
    unsigned long BankIndex;                            // +0x00
    float         AnimStep;                             // +0x04
    float         RenderSizeX;                          // +0x08
    bool          AdditiveAlpha;                        // +0x0c
    unsigned char Type;                                 // +0x0d
};
#pragma pack(pop)
struct CTCGraphicAppearance {
    unsigned char  _pad00[0x44];
    CEngineGraphic Graphic;                             // +0x44
    unsigned char  _pad52[0x59 - 0x52];
    unsigned char  UpdateFlags;                         // +0x59
};
#pragma pack(push, 1)
struct CAppearanceDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1 (+0x04)
    long           m_refCount;                          // +0x04
    unsigned char  _pad08[0x25 - 0x08];
    CEngineGraphic Graphic;                             // +0x25
};
#pragma pack(pop)
template <class T> struct CDefPointer {
    T* Object;
};
struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
    bool GetSubDef(CDefPointer<CAppearanceDef>* out);   // 0x0042B0A2
};
struct CThing {
    unsigned char         _pad00[0x60];
    CTCPhysicsBase*       PhysicsTC;                    // +0x60
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64
    unsigned char         _pad68[0x70 - 0x68];
    CDefPointer<CDefClassBase> PDef;                    // +0x70
};
struct CTCDExperienceOrb {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    void Construct(const C3DVector& position);
};

void CTCDExperienceOrb::Construct(const C3DVector& position)
{
    CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    physics->ConstructFromParams(&CTCParamsPhysicsBase(position));
    CThing* thing = ParentThing;
    CTCGraphicAppearance* appearance = thing->PTCGraphicAppearance;
    CDefClassBase* thingDef = thing->PDef.Object;
    CDefPointer<CAppearanceDef> def;
    def.Object = 0;
    if (thingDef)
        ++thingDef->m_refCount;
    thingDef->GetSubDef(&def);
    if (thingDef && --thingDef->m_refCount == 0)
        thingDef->Destroy();
    CAppearanceDef* appearanceDef = def.Object;
    if (appearance) {
        appearance->Graphic = appearanceDef->Graphic;
        appearance->UpdateFlags |= 2;
    }
    if (appearanceDef && --appearanceDef->m_refCount == 0)
        appearanceDef->Destroy();
}
