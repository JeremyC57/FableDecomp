// CNavigatorFlyer::MoveToPosProximity(const C3DVector& pos, const C3DVector& destination, float proximity, float speed, C3DVector& move, bool, bool, bool, bool, bool, bool, float) -- straight-line flyer step: revalidate a changed destination (node under it, above the ground, same map as the flyer), report arrival inside radius+proximity, otherwise return a move of at most `speed` that keeps the flyer a fixed clearance above the terrain
// Grade: DIFFER(1123v1125) -- revalidation, arrival returns (tail-merged), squared-distance CSE and the clearance logic match; left: frame 0x34 vs 0x30 (one extra temporary slot shifts every local), this in ebp (retail edi), and x87 order in the speed-scaled Normalise and the probe offset add
// Leaf: MoveToPosProximity
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A18FC0, 1123 bytes (+ int3 padding), `ret 0x30`; CNavigatorFlyer vtable slot 4.
// Callees: CNavigatorManager::GetNodeAt 0x00A14F70 (2D temporary, layer 0), CNavigatorManager::IsInSameMap 0x00A14FD0 (two
//   2D temporaries; the landed body only reads X/Y), CNavigatorExternalInterface (+0x14) slots 3 (+0x0c ground height at a
//   position), 5 (+0x14 height at a position, bool) and 6 (+0x18 radius). Globals: 1e-4f 0x0129BA3C, the flying clearance
//   0x0129CB48, the probe offset C3DVector 0x01397130, 3.0f 0x0122DED4, 0.5f 0x0122F59C, rsqrt table 0x013CE558.
// Layout: Ego_r.pdb CNavigatorBase {PInterface +0x14, OnRoute +0x18, Interrupted +0x19, NavigationManager +0x1c},
//   CNavigatorFlyer DestinationPos +0x24 (the engine header only names DestinationPos, so the class is modelled locally).
// Returns ENavigatorReturn: 0 arrived, 1 failed, 2 moving.
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
extern const float g_fNavigationEpsilon;            // 0x0129BA3C
extern const float g_fFlyerClearance;               // 0x0129CB48

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    __forceinline C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    __forceinline void Normalise()
    {
        float lengthSquared = Z * Z;
        lengthSquared += Y * Y;
        lengthSquared += X * X;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r;
        r *= 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
extern const C3DVector g_FlyerProbeOffset;          // 0x01397130
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
};
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
inline float fabs(float x) { return (float)fabs((double)x); }   // <math.h> C++ overload

struct CANavQuadTreeNode;
class CNavigatorManager {
public:
    CANavQuadTreeNode* GetNodeAt(const C2DVector& position, bool layer);           // 0x00A14F70
    bool IsInSameMap(const C2DVector& a, const C2DVector& b);                      // 0x00A14FD0
};
class CNavigatorExternalInterface {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual float GetGroundHeightAt(const C3DVector& pos) const;                   // slot 3 (+0x0c)
    virtual float FindPosZAt(const C3DVector& pos, bool useNavigation) const;      // slot 4 (+0x10)
    virtual float GetFlyingHeightAt(const C3DVector& pos, bool flag) const;        // slot 5 (+0x14)
    virtual float GetRadius(bool flag) const;                                      // slot 6 (+0x18)
};
enum ENavigatorReturn { NAVIGATOR_ARRIVED, NAVIGATOR_FAILED, NAVIGATOR_MOVING };

class CNavigatorFlyer {
public:
    virtual void v00();
    unsigned char                _pad04[0x10];
    CNavigatorExternalInterface* PInterface;           // +0x14
    bool                         OnRoute;              // +0x18
    bool                         Interrupted;          // +0x19
    CNavigatorManager*           NavigationManager;    // +0x1c
    void*                        SteeringAgent;        // +0x20
    C3DVector                    DestinationPos;       // +0x24
    virtual ENavigatorReturn MoveToPosProximity(const C3DVector& pos, const C3DVector& destination, float proximity,
                                                float speed, C3DVector& move, bool, bool, bool, bool, bool, bool, float);
};

ENavigatorReturn CNavigatorFlyer::MoveToPosProximity(const C3DVector& pos, const C3DVector& destination, float proximity,
                                                     float speed, C3DVector& move, bool, bool, bool, bool, bool, bool, float)
{
    if (Interrupted || !OnRoute ||
        fabs(destination.X - DestinationPos.X) > g_fNavigationEpsilon ||
        fabs(destination.Y - DestinationPos.Y) > g_fNavigationEpsilon ||
        fabs(destination.Z - DestinationPos.Z) > g_fNavigationEpsilon) {
        Interrupted = false;
        DestinationPos = destination;
        if (!NavigationManager->GetNodeAt(destination, false) ||
            PInterface->GetGroundHeightAt(destination) - g_fNavigationEpsilon > destination.Z ||
            !NavigationManager->IsInSameMap(pos, destination)) {
            OnRoute = false;
            return NAVIGATOR_FAILED;
        }
        OnRoute = true;
    }
    float range = PInterface->GetRadius(false) + proximity;
    float rangeSquared = range * range;
    C3DVector delta = destination - pos;
    float distanceSquared = delta.X * delta.X + delta.Y * delta.Y + delta.Z * delta.Z;
    if (distanceSquared <= rangeSquared) {
        move.X = 0.0f;
        move.Y = 0.0f;
        move.Z = 0.0f;
        OnRoute = false;
        return NAVIGATOR_ARRIVED;
    }
    if (delta.X * delta.X + delta.Y * delta.Y <= rangeSquared &&
        PInterface->GetGroundHeightAt(destination) + g_fNavigationEpsilon < destination.Z) {
        move.X = 0.0f;
        move.Y = 0.0f;
        move.Z = 0.0f;
        OnRoute = false;
        return NAVIGATOR_ARRIVED;
    }
    if (distanceSquared > speed * speed) {
        delta.Normalise();
        delta *= speed;
    }
    C3DVector next = delta + pos;
    C3DVector probe = next + g_FlyerProbeOffset;
    float height = PInterface->GetFlyingHeightAt(probe, true);
    C2DVector flat = C2DVector(destination) - C2DVector(pos);
    if (flat.X * flat.X + flat.Y * flat.Y > speed * speed) {
        if (next.Z - height < g_fFlyerClearance) {
            next.Z = g_fFlyerClearance + height;
            delta = next - pos;
            delta.Normalise();
            delta *= speed;
        }
    }
    if (next.Z < height) {
        next.Z = height + g_fFlyerClearance;
        delta = next - pos;
    }
    move = delta;
    return NAVIGATOR_MOVING;
}
