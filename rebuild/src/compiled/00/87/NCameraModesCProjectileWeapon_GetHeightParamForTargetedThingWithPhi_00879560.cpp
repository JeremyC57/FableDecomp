// NCameraModes::CProjectileWeapon::GetHeightParamForTargetedThingWithPhi(const CThing&, float) const -- the camera height parameter for the thing's targeting position (full height) at the given phi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00879560, length 56 bytes.
// Callees: NCameraModes::CBase::GetWorld() const @ 0x0084D050 (the camera manager's World; manifest label
// CPixelShader::CountReferences is propagated), NTargeting::GetTargetingPositionOnThing(const CWorld&, const
// CThing&, float) @ 0x007EA2F0 (`__fastcall`, hidden C3DVector result in ecx, the world in edx, `ret 8`),
// GetHeightParamForPostionWithPhi(const C3DVector&, float) const @ 0x00879070. Names from Ego_r.pdb.
// Shape: the position is a NAMED local -- the temporary form pushes the returned eax and evaluates phi before
// the call, the named local is re-addressed with `lea` after it; C3DVector needs a constructor for the
// `__fastcall` hidden-pointer return.
class CWorld;
class CThing;
class CGameCameraManager;

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
};

namespace NTargeting {
C3DVector __fastcall GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float height);   // 0x007EA2F0
}

namespace NCameraModes {

class CBase { public:
    virtual void v00();
    const CWorld& GetWorld() const;                         // 0x0084D050
};

class CProjectileWeapon : public CBase { public:
    float GetHeightParamForPostionWithPhi(const C3DVector& position, float phi) const;   // 0x00879070
    float GetHeightParamForTargetedThingWithPhi(const CThing& thing, float phi) const;
};

float CProjectileWeapon::GetHeightParamForTargetedThingWithPhi(const CThing& thing, float phi) const
{
    C3DVector position = NTargeting::GetTargetingPositionOnThing(GetWorld(), thing, 1.0f);
    return GetHeightParamForPostionWithPhi(position, phi);
}

}