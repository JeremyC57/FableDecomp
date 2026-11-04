// CWorldMap::GetHeightZBelowPos(const C3DVector& pos) const -- the highest physics-mesh surface between pos and the
// ground below it: starts from the ground height, gathers every thing in a 4x4 box round the position and casts the
// segment pos -> (pos.X, pos.Y, ground) against each thing's physics mesh, keeping the highest intersection Z
// Leaf: GetHeightZBelowPos
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063E480, 272 bytes, `ret 4`. PDB ?GetHeightZBelowPos@CWorldMap@@QBEMABVC3DVector@@@Z.
// Callees: GetGroundSizeZAt 0x0063A300, __ftol2 0x00BFEA70 (x2), the thing-search-tools all-things-in-rough-area
//   query 0x0049C490 (`ret 0Ch`; labelled GetPBestThingInRoughArea<HasTrackName,...>), CTCPhysicsBase::
//   GetPhysicsMeshObject 0x006B0850, CPhysicsMeshObject::GetPointIntersectionFromTo 0x00A12450,
//   ~CPhysicsMeshObject 0x00A127D0, free thunk 0x00BFEA14.
// Layout: CMainGameComponent::Get() 0x013B86A0 -> +0x24 world -> +0x20 thing search tools; CThing::PhysicsTC +0x60 and the
//   CPhysicsMeshObject / CMeshIntersectionData models of the landed CTools::GetCollisionThing 0x00875E60;
//   local STLport vector<CThing*> (12 bytes, freed through free()). No CWorldMap data members are read.
#include "engine/C3DVector.h"
class C2DBoxI { public:
    long x0, y0, x1, y1;
    C2DBoxI(long ax0, long ay0, long ax1, long ay1) : x0(ax0), y0(ay0), x1(ax1), y1(ay1) { }
};
class CMeshIntersectionData { public:
    C3DVector IntersectionPosition;                // +0x00
    C3DVector Normal;                              // +0x0c
    float     DistanceSquared;                     // +0x18
};
class CPhysicsMeshObject { public:
    virtual void v00();
    void*         Mesh;
    float         PositionMatrix[12];
    float         Scale;
    float         OO_Scale;
    virtual ~CPhysicsMeshObject();                         // 0x00A127D0
    bool GetPointIntersectionFromTo(const C3DVector* from, const C3DVector* to, CMeshIntersectionData* out) const;   // 0x00A12450
};
class CTCPhysicsBase { public:
    CPhysicsMeshObject GetPhysicsMeshObject() const;   // 0x006B0850
};
class CThing { public:
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
struct CThingFilter_All { };
class CThingSearchTools { public:
    void GetAllThingsInRoughArea(const C2DBoxI& box, const CThingFilter_All& filter, CThingPtrVector& out);   // 0x0049C490
};
class CWorld { public:
    unsigned char       _pad00[0x20];
    CThingSearchTools*  ThingSearchTools;          // +0x20
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad00[0x24];
    CWorld*       World;                           // +0x24
};
CThing** CollectThingsInRoughArea(const C2DBoxI& box, CThing** first, CThing** last);
// Same-TU stand-in for the 0x0049C490 template instantiation (retail emits it in this TU): with its body visible VC
// knows `things` does not escape and keeps the vector's start in ebp across the loop's calls.
#pragma auto_inline(off)
void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& box, const CThingFilter_All&, CThingPtrVector& out)
{
    out._M_finish = CollectThingsInRoughArea(box, out._M_start, out._M_finish);
}
#pragma auto_inline(on)
class CWorldMap {
public:
    float GetGroundSizeZAt(const C3DVector& pos) const;      // 0x0063A300
    float GetHeightZBelowPos(const C3DVector& pos) const;
};
float CWorldMap::GetHeightZBelowPos(const C3DVector& pos) const
{
    float ground = GetGroundSizeZAt(pos);
    float best = ground;
    CThingPtrVector things;
    CThingFilter_All filter;
    long x = (long)pos.X;
    long y = (long)pos.Y;
    CMainGameComponent::Get()->World->ThingSearchTools->GetAllThingsInRoughArea(C2DBoxI(x - 2, y - 2, x + 2, y + 2), filter, things);
    for (CThing** it = things.begin(); it != things.end(); ++it) {
        CTCPhysicsBase* physics = (*it)->PhysicsTC;
        C3DVector below(pos.X, pos.Y, ground);
        CMeshIntersectionData data;
        bool hit = physics->GetPhysicsMeshObject().GetPointIntersectionFromTo(&pos, &below, &data);
        if (hit && data.IntersectionPosition.Z > best)
            best = data.IntersectionPosition.Z;
    }
    return best;
}