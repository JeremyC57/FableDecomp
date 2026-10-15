// NCameraModes::CProjectileWeapon::CProjectileWeapon(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&, float, float) -- construct the projectile-weapon camera mode: no shot fired yet, not finished, no attractors, the zoom-in FOV limits
// Leaf: ??0CProjectileWeapon
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00879620, length 88 bytes.
// Callees: NCameraModes::CBase::CBase(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&) @ 0x0084E420
// (Ego_r.pdb ??0CBase@NCameraModes@@...; the manifest calls it CActionParam_Constructor). Vtable 0x01277804.
// Ego_r.pdb ??0CProjectileWeapon@NCameraModes@@QAE@AAVCGameCameraManager@@ABV?$CDefPointer@$$CBVCCameraModeDef@@@@MM@Z.
// Layout (Ego_r.pdb - 0x24 for the early members, - 0x28 after the PDB 16-byte Attractors vector):
// ShotFiredTimer +0xa4, Finished +0xa8, Attractors +0xf0, MaxZoomInFOV +0xfc, MinZoomInFOV +0x100.
// Shape: the Finished byte is scheduled before the vtable store, ShotFiredTimer = -1 and the vector's inline
// zeroing constructor after it; the float arguments move through edx/eax.
struct CGameCameraManager;
struct CCameraModeDef;
template <class T> struct CDefPointer {
    T* m_pDef;
};

struct C3DVector {
    float X, Y, Z;
};

struct C3DVectorVector {                                    // retail std::vector<C3DVector> (12 bytes)
    C3DVector* m_pFirst;
    C3DVector* m_pLast;
    C3DVector* m_pEnd;
    C3DVectorVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

namespace NCameraModes {

struct CCameraModeBase {
    CCameraModeBase(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def);   // 0x0084E420
    virtual void v00();
    unsigned char _body[0xa0];                              // +0x04 .. +0xa4
};

struct CProjectileWeapon : CCameraModeBase {                // vtable 0x01277804
    long            ShotFiredTimer;                         // +0xa4
    bool            Finished;                               // +0xa8
    C3DVector       XAxis;                                  // +0xac
    C3DVector       YAxis;                                  // +0xb8
    C3DVector       ZAxis;                                  // +0xc4
    float           Phi;                                    // +0xd0
    float           HeightParam;                            // +0xd4
    float           DeltaPhi;                               // +0xd8
    float           DeltaHeightParam;                       // +0xdc
    float           LockDeltaPhi;                           // +0xe0
    float           LockDeltaHeightParam;                   // +0xe4
    float           DeltaLockDeltaPhi;                      // +0xe8
    float           DeltaLockDeltaHeightParam;              // +0xec
    C3DVectorVector Attractors;                             // +0xf0
    float           MaxZoomInFOV;                           // +0xfc
    float           MinZoomInFOV;                           // +0x100
    CProjectileWeapon(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def, float maxZoomInFOV, float minZoomInFOV);
    virtual void v00();
};

CProjectileWeapon::CProjectileWeapon(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def, float maxZoomInFOV, float minZoomInFOV)
    : CCameraModeBase(manager, def), ShotFiredTimer(-1), Finished(false),
      MaxZoomInFOV(maxZoomInFOV), MinZoomInFOV(minZoomInFOV)
{
}

}