// Grade: DIFFER(346v348) -- box, ground queries and filter temporaries match; VC7.1 puts the finder below the top vector (retail: top 0x18, finder 0x24, bottom 0x30) and loads 1.0f before adding it to the CSE'd pos.Z+radius (retail fld st0/fxch/fadd [1.0])
// CThingCreatureBase::GetHeightZBelowAt(const C3DVector& pos, bool alternateGround, bool includeThings) const -- the height of whatever lies below `pos` for this creature: the world map's ground height there (one of two ground queries), optionally raised by the solid physical things overlapping the creature's circle in that area (a search from radius+1 above to 50 below)
// Leaf: ?GetHeightZBelowAt@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00666D40, length 346 bytes, `ret 0Ch`, float in st(0).
//   Signature from Ego_r.pdb `?GetHeightZBelowAt@CThingCreatureBase@@QBEMABVC3DVector@@_N1@Z`.
// Callees: _ftol @ 0x00BFEA70 (four box corners), CThing::PeekWorldMap @ 0x004C79E0, the two CWorldMap ground queries
//   @ 0x0063A390 / 0x0063A300 (both labelled GetGroundSizeZAt; which is which is unproven), CThing::GetThingSearchTools
//   @ 0x004C7A60, CThingSearchTools::ForEachInRoughArea<CThingPhysical, CFilter_Bind3<CThingFilter_SolidToThing,
//   CThingFilter_PhysicsFlagsSolidToThing, CThingFilter_OverlappingCircle>, CThingFindHeightZBelowAt> @ 0x0066A1E0
//   (manifest template name). Constants 1.0f @ 0x0122DED8, 50.0f @ 0x0124E7F8.
// Layout (retail): CThing::PhysicsTC +0x60 (PDB +0x70), the physics component's radius +0x34.
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct C2DBoxI { long TLX, TLY, BRX, BRY; };

struct CTCPhysicsBase {
    unsigned char _pad00[0x34];
    float         Radius;                               // +0x34
};

struct CThing;
struct CThingPhysical;
struct CThingFilter_SolidToThing {
    const CThing* Thing;
    long          Ignore;
    CThingFilter_SolidToThing(const CThing* thing, long ignore) : Thing(thing), Ignore(ignore) { }
};
struct CThingFilter_PhysicsFlagsSolidToThing {
    const CThing*         Thing;
    const CTCPhysicsBase* Physics;
    CThingFilter_PhysicsFlagsSolidToThing(const CThing* thing, const CTCPhysicsBase* physics) : Thing(thing), Physics(physics) { }
};
struct CThingFilter_OverlappingCircle {
    C3DVector Centre;
    float     Radius;
    CThingFilter_OverlappingCircle(const C3DVector& centre, float radius) : Centre(centre), Radius(radius) { }
};
template<class A, class B, class C> struct CFilter_Bind3 {
    const A& First;
    const B& Second;
    const C& Third;
    CFilter_Bind3(const A& a, const B& b, const C& c) : First(a), Second(b), Third(c) { }
};
typedef CFilter_Bind3<CThingFilter_SolidToThing, CThingFilter_PhysicsFlagsSolidToThing, CThingFilter_OverlappingCircle> CHeightFilter;
struct CThingFindHeightZBelowAt {
    const C3DVector& Top;
    const C3DVector& Bottom;
    float            Height;
    CThingFindHeightZBelowAt(const C3DVector& top, const C3DVector& bottom, float height) : Top(top), Bottom(bottom), Height(height) { }
};

struct CThingSearchTools {
    bool ForEachInRoughArea(const C2DBoxI& area, const CHeightFilter& filter, CThingFindHeightZBelowAt& finder);   // 0x0066A1E0
};
struct CWorldMap {
    float GetGroundSizeZAt_0063a390(const C3DVector& pos) const;   // 0x0063A390
    float GetGroundSizeZAt(const C3DVector& pos) const;            // 0x0063A300
};

struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    const CWorldMap* PeekWorldMap() const;              // 0x004C79E0
    CThingSearchTools* GetThingSearchTools() const;     // 0x004C7A60
};

struct CThingCreatureBase : CThing {
    float GetHeightZBelowAt(const C3DVector& pos, bool alternateGround, bool includeThings) const;
};

float CThingCreatureBase::GetHeightZBelowAt(const C3DVector& pos, bool alternateGround, bool includeThings) const
{
    float radius = PhysicsTC->Radius;
    C2DBoxI area;
    area.TLX = (long)(pos.X - radius - 1.0f);
    area.TLY = (long)(pos.Y - radius - 1.0f);
    area.BRX = (long)(pos.X + radius + 1.0f);
    area.BRY = (long)(pos.Y + radius + 1.0f);
    float height = alternateGround ? PeekWorldMap()->GetGroundSizeZAt_0063a390(pos) : PeekWorldMap()->GetGroundSizeZAt(pos);
    if (includeThings) {
        C3DVector top(pos.X, pos.Y, pos.Z + radius + 1.0f);
        C3DVector bottom(pos.X, pos.Y, pos.Z + radius - 50.0f);
        CThingFindHeightZBelowAt finder(top, bottom, height);
        GetThingSearchTools()->ForEachInRoughArea(area,
            CHeightFilter(CThingFilter_SolidToThing(this, 0), CThingFilter_PhysicsFlagsSolidToThing(this, PhysicsTC),
                          CThingFilter_OverlappingCircle(pos, radius)), finder);
        height = finder.Height;
    }
    return height;
}
