// Grade: DIFFER(806v808) -- register allocation only: retail keeps hitPos in edi for the whole body, the thing iterator in ebx and the per-thing hit flag spilled to the filter temporary's byte; VC7.1 here gives the iterator edi, the flag bl and re-reads hitPos from its argument slot
// CParticleEnvironmentInteraction::IsParticleCollidingAt(const C3DVector& pos, const C3DVector& velocity, bool checkMeshes, C3DVector& hitPos, C3DVector& hitNormal, bool& hitGround) -- collide a particle step with the world: optionally cast pos -> pos + velocity against the physics mesh of every thing in a 4x4 box (keeping the nearest hit, normal reversed); otherwise (or with no mesh hit) test the ground under the start and the end of the step; with no environment the ground is the plane Z = 0
// Leaf: IsParticleCollidingAt
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049BE90, 806 bytes (+ int3 padding), `ret 18h`.
// Callees: __ftol2 0x00BFEA70 (x2), the thing-search-tools rough-area query 0x0049C490 (label
//   "GetPBestThingInRoughArea<...>"), CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850, CPhysicsMeshObject::
//   GetPointIntersectionFromTo 0x00A12450, ~CPhysicsMeshObject 0x00A127D0, free 0x00BFEA14, the environment's virtual
//   slot 13 (+0x34, the world map), CWorldMap::GetGroundSizeZAt 0x0063A300, CWorldMap::GetGroundFaceNormalAt 0x0063AB10.
//   FLT_MAX immediate, 0.0 0x0122DEDC.
// Layout: the environment interface pointer at +4; the thing search, physics-mesh and intersection models are the
//   landed CWorldMap::GetHeightZBelowPos 0x0063E480 ones (g_pMainGameComponent 0x013B86A0 -> +0x24 world -> +0x20 search
//   tools, CThing physics TC +0x60, CMeshIntersectionData {position, normal, distance squared}).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
};
struct C2DBoxI {
    long x0, y0, x1, y1;
    C2DBoxI(long ax0, long ay0, long ax1, long ay1) : x0(ax0), y0(ay0), x1(ax1), y1(ay1) { }
};
struct CMeshIntersectionData {
    C3DVector IntersectionPosition;                // +0x00
    C3DVector Normal;                              // +0x0c
    float     DistanceSquared;                     // +0x18
};
struct CPhysicsMeshObject {
    virtual void v00();
    void*         Mesh;
    float         PositionMatrix[12];
    float         Scale;
    float         OO_Scale;
    ~CPhysicsMeshObject();                         // 0x00A127D0
    bool GetPointIntersectionFromTo(const C3DVector* from, const C3DVector* to, CMeshIntersectionData* out) const;   // 0x00A12450
};
struct CTCPhysicsBase {
    CPhysicsMeshObject GetPhysicsMeshObject() const;   // 0x006B0850
};
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                     // +0x60
};
extern "C" void free(void*);
struct CThingPtrVector {                           // STLport vector<CThing*>
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingPtrVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingPtrVector() { if (_M_start) free(_M_start); }
    CThing** begin() { return _M_start; }
    CThing** end() { return _M_finish; }
};
struct CThingFilter_All { CThingFilter_All() { } };
struct CThingSearchTools {
    void GetAllThingsInRoughArea(const C2DBoxI& box, const CThingFilter_All& filter, CThingPtrVector& out);   // 0x0049C490
};
struct CWorld {
    unsigned char       _pad00[0x20];
    CThingSearchTools*  ThingSearchTools;          // +0x20
};
struct CMainGameComponent {
    unsigned char _pad00[0x24];
    CWorld*       World;                           // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
class CWorldMap {
public:
    float GetGroundSizeZAt(const C3DVector& pos) const;        // 0x0063A300
    C3DVector GetGroundFaceNormalAt(const C3DVector& pos) const;   // 0x0063AB10
};
struct CParticleEnvironment {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual CWorldMap* GetWorldMap();              // slot 13 (+0x34)
};
class CParticleEnvironmentInteraction {
public:
    void*                 __vftable;               // +0x00
    CParticleEnvironment* PEnvironment;            // +0x04
    bool IsParticleCollidingAt(const C3DVector& pos, const C3DVector& velocity, bool checkMeshes, C3DVector& hitPos, C3DVector& hitNormal, bool& hitGround);
};

bool CParticleEnvironmentInteraction::IsParticleCollidingAt(const C3DVector& pos, const C3DVector& velocity, bool checkMeshes, C3DVector& hitPos, C3DVector& hitNormal, bool& hitGround)
{
    if (PEnvironment) {
        C3DVector end = pos + velocity;
        if (checkMeshes) {
            CThingPtrVector things;
            long x = (long)pos.X;
            long y = (long)pos.Y;
            g_pMainGameComponent->World->ThingSearchTools->GetAllThingsInRoughArea(C2DBoxI(x - 2, y - 2, x + 2, y + 2), CThingFilter_All(), things);
            float nearest = 3.402823466e+38F;
            bool found = false;
            for (CThing** it = things.begin(); it != things.end(); ++it) {
                CMeshIntersectionData data;
                bool hit = (*it)->PhysicsTC->GetPhysicsMeshObject().GetPointIntersectionFromTo(&pos, &end, &data);
                if (hit && data.DistanceSquared < nearest) {
                    nearest = data.DistanceSquared;
                    hitPos = data.IntersectionPosition;
                    hitNormal = -data.Normal;
                    found = true;
                }
            }
            if (found) {
                hitGround = false;
                return true;
            }
        }
        float groundZ = PEnvironment->GetWorldMap()->GetGroundSizeZAt(pos);
        float endGroundZ = PEnvironment->GetWorldMap()->GetGroundSizeZAt(end);
        if (groundZ >= pos.Z) {
            hitGround = true;
            hitPos = C3DVector(pos.X, pos.Y, groundZ);
        } else if (endGroundZ >= end.Z) {
            hitGround = true;
            hitPos = C3DVector(end.X, end.Y, groundZ);
        } else {
            hitGround = false;
            return false;
        }
        hitNormal = PEnvironment->GetWorldMap()->GetGroundFaceNormalAt(hitPos);
        return true;
    }
    if (0.0f > pos.Z) {
        hitPos = pos;
        hitPos.Z = 0.0f;
        hitNormal = C3DVector(0.0f, 0.0f, 1.0f);
        hitGround = true;
        return true;
    }
    hitGround = false;
    return false;
}
