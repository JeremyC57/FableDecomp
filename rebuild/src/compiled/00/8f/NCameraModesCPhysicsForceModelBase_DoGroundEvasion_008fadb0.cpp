// NCameraModes::CPhysicsForceModelBase::DoGroundEvasion() -- rescans the ground points within 50 units of the camera, sums a 5/distance potential at four samples and returns the gradient, flipped to point down when it points up and with its height change clamped to [-1000, 0]
// Leaf: ?DoGroundEvasion@CPhysicsForceModelBase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FADB0, 446 bytes, `ret 4` (C3DVector by hidden pointer).
// Callees: the camera manager's (+0xA8) world (+0x74) slot 13 (+0x34, the world map), UpdateScan<const CWorldMap> 0x008FC430,
//   0x008FA9F0 on Ground (manifest "LinkedList_ClearAndReinit"). Constants: offsets 0x013893AC, 0.1f 0x0122EE58, 5.0f
//   0x012462A8, 10.0f 0x01237F0C, 0.0f 0x0122DEDC, -1.0f 0x0122DEE0, -1000.0f 0x012448F8.
// Shape: C3DVector has a user member-wise copy ctor (the evasion loop then spills the sample to frame temporaries and
//   adds the position from memory, as the landed 0x008FAA80); the world map is fetched into a named reference before the
//   scan call; the flip is three in-place `*= -1.0f` (retail keeps X/Y in the result's frame words and Z on the x87 stack);
//   UpdateScan<const CWorldMap> (a template instantiation in retail's TU) has a same-TU noinline body taking the centre by
//   value, so VC7.1 knows &position does not escape and sinks the position stores past the world-map loads.
// Layout: retail Ground +0x50 (PDB +0x80), CameraManager +0xA8 (PDB +0xF0).
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double);
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float Length() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
inline float Magnitude(const C3DVector& v) { return (float)sqrt(v.Z * v.Z + v.Y * v.Y + v.X * v.X); }
struct SVectorNode { SVectorNode* _M_next; SVectorNode* _M_prev; C3DVector _M_data; };
extern const C3DVector g_EvasionSampleOffsets[4];  // 0x013893AC: 0, X, Y, Z

struct CWorldMap;
struct CWorldMapOwner {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual const CWorldMap& GetWorldMap() const;  // slot 13 (+0x34)
};
struct CGameCameraManager {
    unsigned char   _pad00[0x74];
    CWorldMapOwner* World;                         // +0x74
};
namespace NCameraModes {
struct CPhysicsState {
    C3DVector Position;                            // +0x00
};
struct CPhysicsCollisionPointsObject {             // retail 0x24 bytes
    SVectorNode* Points;                           // +0x00 list<C3DVector> header
    unsigned char _pad04[0x20];
    void Finalise();                               // 0x008FA9F0
};
struct CPhysicsForceModelBase {
    virtual void v00();
    CPhysicsState*                LinkedState;     // +0x04
    unsigned char                 _pad08[0x50 - 0x08];
    CPhysicsCollisionPointsObject Ground;          // +0x50 (PDB +0x80)
    unsigned char                 _pad74[0xa8 - 0x74];
    CGameCameraManager*           CameraManager;   // +0xa8 (PDB +0xF0)
    void UpdateScan(CPhysicsCollisionPointsObject& points, const CWorldMap& map, const C3DVector& centre, long flags, float radius);   // 0x008FC430
    C3DVector DoGroundEvasion();
};

void ScanGroundPoints_X(CPhysicsForceModelBase* model, CPhysicsCollisionPointsObject& points, const CWorldMap& map,
                        float x, float y, float z, long flags, float radius);
#pragma auto_inline(off)
__declspec(noinline) void CPhysicsForceModelBase::UpdateScan(CPhysicsCollisionPointsObject& points, const CWorldMap& map,
                                                            const C3DVector& centre, long flags, float radius)
{
    ScanGroundPoints_X(this, points, map, centre.X, centre.Y, centre.Z, flags, radius);
}
#pragma auto_inline(on)

C3DVector CPhysicsForceModelBase::DoGroundEvasion()
{
    C3DVector position = LinkedState->Position;
    const CWorldMap& map = CameraManager->World->GetWorldMap();
    UpdateScan(Ground, map, position, 0, 50.0f);
    Ground.Finalise();
    SVectorNode* head = Ground.Points;
    float potential[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    for (SVectorNode* it = head->_M_next; it != head; it = it->_M_next) {
        for (long i = 0; i < 4; ++i) {
            C3DVector sample = g_EvasionSampleOffsets[i] * 0.1f + position;
            potential[i] += 5.0f / Magnitude(sample - it->_M_data);
        }
    }
    C3DVector result((potential[1] - potential[0]) * 10.0f, (potential[2] - potential[0]) * 10.0f, (potential[3] - potential[0]) * 10.0f);
    if (result.Z > 0.0f) {
        result.X *= -1.0f;
        result.Y *= -1.0f;
        result.Z *= -1.0f;
    }
    if (result.Z < -1000.0f)
        result.Z = -1000.0f;
    else if (result.Z > 0.0f)
        result.Z = 0.0f;
    return result;
}
}