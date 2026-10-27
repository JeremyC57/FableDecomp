// CTCLook::GetLookAngleRelToBodyXY() const -- the XY look angle relative to the body: head angle plus eye angle
// Leaf: ?GetLookAngleRelToBodyXY@CTCLook@@QBEMXZ
// Retail: Fable.exe @ 0x00751A80, 7 bytes: `fld dword ptr [ecx+34h] ; fadd dword ptr [ecx+2Ch] ; ret`.
// Identity (proven): Ego_r.pdb names ?GetLookAngleRelToBodyXY@CTCLook@@QBEMXZ (public const float getter) and lays
// HeadAngleRelToBodyXY at +0x2C and EyeAnglesRelToHeadXY at +0x34; the row sits among the landed CTCLook members
// (0x00751A90 OnAppearanceClear, 0x00751AD0 UpdateLookTimer); its callers CThingCreatureBase::IsThingInViewAngle /
// IsPosInFOV area (0x00664F3E, 0x0066536F) take interface 0x43 from the thing's interface map and add the result to a
// body angle. The manifest label "CMap::DrawGetSeaLevel" is propagated (that one is a virtual taking a float, `ret 4`).
// Shape: VC7.1 evaluates the right operand of a float `+` first, so the source reads head + eye for `fld eye ; fadd head`.
class CThing;
class CThingCreatureBase;
template<class T> class CDefPointer { public: T* Object; };
class CLookDef;
class CAppearanceDef;
class CTCBase { public:                             // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
};
class CTCLook : public CTCBase { public:
    unsigned char       _pad_0x0c[4];               // +0x0c (unnamed in Ego_r)
    CThingCreatureBase* PParentCreature;            // +0x10
    CDefPointer<const CLookDef>       PDef;         // +0x14
    CDefPointer<const CAppearanceDef> PAppearanceDef;   // +0x18
    float RecoveryAngleToLookAtXY;                  // +0x1c
    float RecoveryAngleToLookAtYZ;                  // +0x20
    float BodyAngleXY;                              // +0x24
    float BodyAngleYZ;                              // +0x28
    float HeadAngleRelToBodyXY;                     // +0x2c
    float HeadAngleRelToBodyYZ;                     // +0x30
    float EyeAnglesRelToHeadXY;                     // +0x34
    float EyeAnglesRelToHeadYZ;                     // +0x38
    float GetLookAngleRelToBodyXY() const;
};

float CTCLook::GetLookAngleRelToBodyXY() const
{
    return HeadAngleRelToBodyXY + EyeAnglesRelToHeadXY;
}