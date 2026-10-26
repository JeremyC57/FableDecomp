// CTCSpotLight::GetDirection() const -- the owner's facing flattened onto the XY plane and normalised, tilted down about its horizontal perpendicular by (0.25 - angle) where the angle is the overridden light info's or the def's
// Leaf: GetDirection
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00731CA0, length 147 bytes (`ret 4`, C3DVector returned through the hidden pointer).
// Callees: CTCPhysicsBase virtual slot 70 (+0x118, returns a stored C3DVector by value; CTCPhysicsStandard's override 0x00723C00 copies
// its +0x5C vector) on CThing::PhysicsTC, C3DVector::AccurateNormalise @ 0x00A14440 (in place; manifest label "GetScaled" is wrong: it divides
// all three components by the length and returns nothing), C3DVector::Rotate(const C3DVector& axis, float angle) @ 0x00A13C60.
// Constant: 0.25f @ 0x0123078C.
// Layout: CThing::PhysicsTC at retail +0x60 (PDB +0x70; CThing's STLport list UpdateComponentsList is one pointer). CTCSpotLight (retail =
// PDB, engine/CTCSpotLight.h): PLightInfo.Data +0x10 (CSpotLightInfo::Angle +0), PLightDef.Object +0x18 (CSpotLightDef::Angle +0x40),
// Overridden +0x1C.
// Shape: the facing is constructed straight into the result local; the axis is C3DVector(dir.Y, -dir.X, 0); the angle is a named float
// local (retail keeps it on the x87 stack until after the argument store, `fstp st(0)`); the user copy ctor gives the copy-out
// `fld X ; fstp [eax]` with Y/Z as dword moves.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector& AccurateNormalise();                                           // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
    void Rotate(const C3DVector& axis, float angle);            // 0x00A13C60
};
class CTCPhysicsBase { public:
    virtual void v00();  virtual void v01();  virtual void v02();  virtual void v03();  virtual void v04();
    virtual void v05();  virtual void v06();  virtual void v07();  virtual void v08();  virtual void v09();
    virtual void v10();  virtual void v11();  virtual void v12();  virtual void v13();  virtual void v14();
    virtual void v15();  virtual void v16();  virtual void v17();  virtual void v18();  virtual void v19();
    virtual void v20();  virtual void v21();  virtual void v22();  virtual void v23();  virtual void v24();
    virtual void v25();  virtual void v26();  virtual void v27();  virtual void v28();  virtual void v29();
    virtual void v30();  virtual void v31();  virtual void v32();  virtual void v33();  virtual void v34();
    virtual void v35();  virtual void v36();  virtual void v37();  virtual void v38();  virtual void v39();
    virtual void v40();  virtual void v41();  virtual void v42();  virtual void v43();  virtual void v44();
    virtual void v45();  virtual void v46();  virtual void v47();  virtual void v48();  virtual void v49();
    virtual void v50();  virtual void v51();  virtual void v52();  virtual void v53();  virtual void v54();
    virtual void v55();  virtual void v56();  virtual void v57();  virtual void v58();  virtual void v59();
    virtual void v60();  virtual void v61();  virtual void v62();  virtual void v63();  virtual void v64();
    virtual void v65();  virtual void v66();  virtual void v67();  virtual void v68();  virtual void v69();
    virtual C3DVector GetFacingVector() const;                  // slot 70 (+0x118)
};
class CThing { public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                                  // +0x60 (PDB +0x70)
};
struct CSpotLightInfo { float Angle; };                         // CTCSpotLight::CSpotLightInfo, Angle +0
class CSpotLightDef { public:
    unsigned char _pad00[0x40];
    float Angle;                                                // +0x40
};
class CTCSpotLight { public:
    virtual void v00();
    CThing* ParentThing;                                        // +0x04 (CTCBase)
    unsigned char _base08[4];
    void* _vfptrComponent;                                      // +0x0c CAGraphicAppearanceComponent (vtable 0x01266FA4)
    CSpotLightInfo* PLightInfo_Data;                            // +0x10 CCountedPointer<CSpotLightInfo>::Data
    void* PLightInfo_Info;                                      // +0x14
    const CSpotLightDef* PLightDef_Object;                      // +0x18 CDefPointer<const CSpotLightDef>::Object
    bool Overridden;                                            // +0x1c
    C3DVector GetDirection() const;
};

C3DVector CTCSpotLight::GetDirection() const
{
    C3DVector dir = ParentThing->PhysicsTC->GetFacingVector();
    dir.Z = 0.0f;
    dir.AccurateNormalise();
    C3DVector axis(dir.Y, -dir.X, 0.0f);
    float angle = Overridden ? PLightInfo_Data->Angle : PLightDef_Object->Angle;
    dir.Rotate(axis, 0.25f - angle);
    return dir;
}