// NCameraModes::CProjectileWeapon::GetHeightParamForTargetedThingWithPhi(const CThing&, float) const -- the camera height parameter for the thing's targeting position (full height) at the given phi
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00879560, length 56 bytes.
// Callees: NCameraModes::CBase::GetWorld() const @ 0x0084D050 (the camera manager's World; manifest label
// CPixelShader::CountReferences is propagated), NTargeting::GetTargetingPositionOnThing(const CWorld&, const
// CThing&, float) @ 0x007EA2F0 (`__fastcall`, hidden C3DVector result in ecx, the world in edx, `ret 8`),
// GetHeightParamForPostionWithPhi(const C3DVector&, float) const @ 0x00879070. Names from Ego_r.pdb.
// Shape: the position is a NAMED local -- the temporary form pushes the returned eax and evaluates phi before
// the call, the named local is re-addressed with `lea` after it; C3DVector needs a constructor for the
// `__fastcall` hidden-pointer return.
// Leaf: ?GetHeightParamForTargetedThingWithPhi@CProjectileWeapon@NCameraModes@@ABEMABVCThing@@M@Z
// Naming (NF157, 2026-11-20; PRIORITY q4539): in place in the row's own local CProjectileWeapon: both members private as both PDBs spell them,
//   so 0x00879070 binds Ego_r.pdb 0x00863420 / FableWin.pdb 0x029C9E90 ?GetHeightParamForPostionWithPhi@CProjectileWeapon@NCameraModes@@ABEMABVC3DVector@@M@Z
//   (was QBE) and this row's own leaf is Ego_r.pdb 0x00863AD0 / FableWin.pdb 0x029C9E40
//   ?GetHeightParamForTargetedThingWithPhi@CProjectileWeapon@NCameraModes@@ABEMABVCThing@@M@Z (was QBE; no landed row references 0x00879560).
class CWorld;
class CThing;
class CGameCameraManager;

#include "engine/C3DVector.h"

namespace NTargeting {
C3DVector __fastcall GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float height);   // 0x007EA2F0
}

namespace NCameraModes {

class CBase { public:
    virtual void v00();
protected:  // NF34: Ego_r.pdb + FableWin.pdb ?GetWorld@CBase@NCameraModes@@IBEABVCWorld@@XZ (protected)
    const CWorld& GetWorld() const;                         // 0x0084D050
public:
};

class CProjectileWeapon : public CBase { private:          // Ego_r.pdb / FableWin.pdb: both ABE (private const)
    float GetHeightParamForPostionWithPhi(const C3DVector& position, float phi) const;   // 0x00879070
    float GetHeightParamForTargetedThingWithPhi(const CThing& thing, float phi) const;
};

float CProjectileWeapon::GetHeightParamForTargetedThingWithPhi(const CThing& thing, float phi) const
{
    C3DVector position = NTargeting::GetTargetingPositionOnThing(GetWorld(), thing, 1.0f);
    return GetHeightParamForPostionWithPhi(position, phi);
}

}