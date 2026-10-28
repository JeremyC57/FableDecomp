// CParticleEnvironmentInteraction::IsParticleCollidingAt(const C3DVector& pos, const C3DVector& velocity, bool checkMeshes, C3DVector& hitPos, C3DVector& hitNormal, bool& hitGround) const -- collide a particle step with the world: optionally cast pos -> pos + velocity against the physics mesh of every thing with a physics mesh in a 4x4 box (keeping the nearest hit, normal reversed); otherwise (or with no mesh hit) test the ground under the start and the end of the step; with no world the ground is the plane Z = 0
// Grade: DIFFER(806v808) -- register allocation only: retail keeps hitPos in edi for the whole body, the thing iterator in ebx and the per-thing hit flag spilled to the filter temporary's byte; VC7.1 here gives the iterator edi, the flag bl and re-reads hitPos from its argument slot
// Leaf: ?IsParticleCollidingAt@CParticleEnvironmentInteraction@@UBE_NABVC3DVector@@0_NAAV2@2AA_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049BE90, 808 bytes (+ int3 padding), `ret 18h`.
// Callees: __ftol2 0x00BFEA70 (x2), CThingSearchTools::GetAllThingsInRoughArea<const CThing, vector<const CThing*>,
//   CThingFilter_HasPhysicsMesh> 0x0049C490 (its body calls CThingFilter_HasPhysicsMesh::operator() 0x006A6F50),
//   CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850, CPhysicsMeshObject::GetPointIntersectionFromTo 0x00A12450,
//   ~CPhysicsMeshObject 0x00A127D0, free 0x00BFEA14, CWorld::PeekWorldMap (vtable 0x012390F0 slot 13, +0x34),
//   CWorldMap::GetGroundSizeZAt 0x0063A300, CWorldMap::GetGroundFaceNormalAt 0x0063AB10. FLT_MAX immediate, 0.0 0x0122DEDC.
// Tried: 5 variants (filter temp, empty filter ctor, temp in the condition) (806v808); chunk 537 (RTM): real PDB names (virtual const, CWorld::PeekWorldMap slot 13, the HasPhysicsMesh rough-area template, reference args) -- unchanged; named outPos / thing locals, outer iterator, inlined hit test: no change or worse. | next: lane gap: regalloc -- no source form moves hitPos into edi.
// Layout: Ego_r.pdb CParticleEnvironmentInteraction (base NParticleEngine::CAEnvironmentInteraction, CWorld* PWorld +0x04);
//   CMainGameComponent::PWorld +0x24 (scoped_ptr<CWorld>), CWorld::PThingSearchTools +0x20 (scoped_ptr<CThingSearchTools>),
//   CThing::PhysicsTC +0x60 (retail), CMeshIntersectionData {IntersectionPosition, Normal, DistanceSquared}. engine/CWorld.h
//   holds the vptr as data, so CWorld's two virtual slots are declared here.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
};
class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};
class CMeshIntersectionData {
public:
    C3DVector IntersectionPosition;                // +0x00
    C3DVector Normal;                              // +0x0c
    float     DistanceSquared;                     // +0x18
};
class CPhysicsMeshObject {
public:
    virtual ~CPhysicsMeshObject();                 // 0x00A127D0
    void*         Mesh;                            // +0x04 CIVCountedPointer<const CPhysicsMesh>
    float         PositionMatrix[12];              // +0x08 CMatrix3x4
    float         Scale;                           // +0x38
    float         OO_Scale;                        // +0x3c
    bool GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;   // 0x00A12450
};
class CTCPhysicsBase {
public:
    CPhysicsMeshObject GetPhysicsMeshObject() const;   // 0x006B0850
};
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                     // +0x60
};
extern "C" void __cdecl free(void*);
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc> class vector {  // STLport 4.6 (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
};
}
class CThingFilter_HasPhysicsMesh {
public:
    CThingFilter_HasPhysicsMesh() { }
    bool operator()(const CThing& thing) const;    // 0x006A6F50
};
class CThingSearchTools {
public:
    template <class TThing, class TContainer, class TFilter>
    void GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TContainer& things) const;   // 0x0049C490
};
class CWorldMap {
public:
    float GetGroundSizeZAt(const C3DVector& pos) const;            // 0x0063A300
    C3DVector GetGroundFaceNormalAt(const C3DVector& pos) const;   // 0x0063AB10
};
class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap& GetWorldMap();                      // slot 12 (+0x30) 0x004AE8B0
    virtual const CWorldMap& PeekWorldMap() const;         // slot 13 (+0x34) 0x004AE8C0
    unsigned char      _pad04[0x1C];
    CThingSearchTools* PThingSearchTools;                  // +0x20 scoped_ptr<CThingSearchTools>
};
template <class T> class scoped_ptr {
public:
    T* ptr;
    T* operator->() const { return ptr; }
};
class CMainGameComponent {
public:
    unsigned char      _pad00[0x24];
    scoped_ptr<CWorld> PWorld;                             // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;           // 0x013B86A0
namespace NParticleEngine {
class CAEnvironmentInteraction {
public:
    virtual bool IsParticleCollidingAt(const C3DVector& pos, const C3DVector& velocity, bool checkMeshes, C3DVector& hitPos, C3DVector& hitNormal, bool& hitGround) const = 0;
};
}
class CParticleEnvironmentInteraction : public NParticleEngine::CAEnvironmentInteraction {
public:
    CWorld* PWorld;                                        // +0x04
    virtual bool IsParticleCollidingAt(const C3DVector& pos, const C3DVector& velocity, bool checkMeshes, C3DVector& hitPos, C3DVector& hitNormal, bool& hitGround) const;
};

bool CParticleEnvironmentInteraction::IsParticleCollidingAt(const C3DVector& pos, const C3DVector& velocity, bool checkMeshes, C3DVector& hitPos, C3DVector& hitNormal, bool& hitGround) const
{
    if (PWorld) {
        C3DVector end = pos + velocity;
        if (checkMeshes) {
            std::vector<const CThing*, std::allocator<const CThing*> > things;
            long x = (long)pos.X;
            long y = (long)pos.Y;
            g_pMainGameComponent->PWorld->PThingSearchTools->GetAllThingsInRoughArea<const CThing>(C2DBoxI(x - 2, y - 2, x + 2, y + 2), CThingFilter_HasPhysicsMesh(), things);
            float nearest = 3.402823466e+38F;
            bool found = false;
            for (const CThing** it = things.begin(); it != things.end(); ++it) {
                CMeshIntersectionData data;
                bool hit = (*it)->PhysicsTC->GetPhysicsMeshObject().GetPointIntersectionFromTo(pos, end, data);
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
        float groundZ = PWorld->PeekWorldMap().GetGroundSizeZAt(pos);
        float endGroundZ = PWorld->PeekWorldMap().GetGroundSizeZAt(end);
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
        hitNormal = PWorld->PeekWorldMap().GetGroundFaceNormalAt(hitPos);
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
