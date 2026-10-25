// CThingFilter_IsInFOV::operator()(const NPlayerTargeting::CTargetPoint& point) const -- test the target point's targeting position against the field of view
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7380, 16 bytes: mov eax,[esp+4] ; add eax,0Ch ; mov [esp+4],eax ;
//   jmp 0x006A7270 (the reference argument is rebased onto the member in place and the call is a tail jump).
// Callees: CThingFilter_IsInFOV::operator()(const C3DVector&) const @ 0x006A7270 (landed, ??RCThingFilter_IsInFOV@@QBE_NABVC3DVector@@@Z).
// Naming: Ego_r.pdb / FableWin.pdb define four CThingFilter_IsInFOV::operator() overloads (const C3DVector&,
//   const NPlayerTargeting::CTargetPoint&, const CThing&, const CThing*); this one reads its argument's +0x0C, which is
//   CTargetPoint::TargetingPosition (Ego_r.pdb: PThing +0x00 (8-byte CIntelligentPointer), Index +0x08,
//   TargetingPosition +0x0C, 28 bytes), and its callers are the landed std::__find_if over CTargetPoint* 0x008060C0
//   (35 calls, 28-byte stride, the /28 magic 0x92492493) -- FINDINGS chunk 398. Was landed as a generic `Cons_val`
//   free-function stub under the propagated label "std::_Cons_val<...pair<EHeroMorphType,CParticleMorphs::CEntry>...>".
// Layout: Ego_r.pdb CThingFilter_IsInFOV (36 bytes): FromPos +0x00, Direction +0x0C, CosFOV +0x18, FOV +0x1C,
//   AngleTest3D +0x20 (not read here).
// Leaf: ??RCThingFilter_IsInFOV@@QBE_NABVCTargetPoint@NPlayerTargeting@@@Z
class C3DVector {
public:
    float X, Y, Z;
};

namespace NPlayerTargeting {
class CTargetPoint {
public:
    unsigned char PThing[0x8];              // +0x00 CIntelligentPointer<CThing>
    long          Index;                    // +0x08
    C3DVector     TargetingPosition;        // +0x0C
    long          FrameRejected;            // +0x18
};
}

class CThingFilter_IsInFOV {
public:
    C3DVector FromPos;                      // +0x00
    C3DVector Direction;                    // +0x0C
    float     CosFOV;                       // +0x18
    float     FOV;                          // +0x1C
    bool      AngleTest3D;                  // +0x20
    bool operator()(const C3DVector& pos) const;                          // 0x006A7270
    bool operator()(const NPlayerTargeting::CTargetPoint& point) const;
};

bool CThingFilter_IsInFOV::operator()(const NPlayerTargeting::CTargetPoint& point) const
{
    return (*this)(point.TargetingPosition);
}
