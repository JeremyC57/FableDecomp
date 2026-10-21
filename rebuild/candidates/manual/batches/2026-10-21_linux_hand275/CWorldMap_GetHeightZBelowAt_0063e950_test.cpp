// Behaviour test for retail 0x0063e950: candidate source embedded verbatim, then stubs and a main.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef _MSC_VER
#define __cdecl
#endif
#define malloc Spy_malloc
#define free Spy_free
// CWorldMap::GetHeightZBelowAt(const C3DVector& pos, float radius, bool useTheme, const CThingFilterBase& filter) const -- the highest surface below pos: starts from the ground (or theme) height, gathers the things whose rough area overlaps a circle of `radius` round pos and pass `filter`, and raises the height to any physics-mesh hit (+0.007) on the 200-unit drop segment below pos that lies under pos
// Leaf: GetHeightZBelowAt
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063E950, 463 bytes, `ret 10h`.
//   PDB ?GetHeightZBelowAt@CWorldMap@@QBEMABVC3DVector@@M_NABVCThingFilterBase@1@@Z.
// Callees: __ftol2 0x00BFEA70 (x4), C2DBoxI::ValidateOrientationAndFlip 0x00A3A150, malloc 0x00BFEA0E / free 0x00BFEA14
//   (the STLport list<CThing*> sentinel and nodes), CThingSearchTools::ForEachInRoughArea<...> 0x0063FD40 (this TU's
//   template instance; same-TU stand-in below), GetThemeSizeZAt 0x0063A390 / GetGroundSizeZAt 0x0063A300,
//   CTCPhysicsBase virtual slot 93 (+0x174), GetPhysicsMeshObject 0x006B0850, CPhysicsMeshObject::
//   GetPointIntersectionFromTo 0x00A12450, ~CPhysicsMeshObject 0x00A127D0; floats 1.0f @ 0x0122DED8, 200.0f @
//   0x0125A168, 0.007f @ 0x0125A164.
// Layout: CWorldMap World +0x08 (as the PDB header); CWorld ThingSearchTools +0x20; CThing PhysicsTC +0x60; the
//   CPhysicsMeshObject / CMeshIntersectionData models of the landed CThingFindHeightZBelowAt::operator() 0x00668AB0.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long x0, long y0, long x1, long y1) : TLX(x0), TLY(y0), BRX(x1), BRY(y1) { }
    void ValidateOrientationAndFlip();                  // 0x00A3A150
};
struct CMeshIntersectionData {
    C3DVector IntersectionPosition;             // +0x00
    C3DVector Normal;                           // +0x0c
    float     DistanceSquared;                  // +0x18
};
struct CPhysicsMeshObject {
    void*         __vftable;
    void*         m_pMesh;                      // +0x04 counted mesh
    unsigned char _pad08[0x38];
    ~CPhysicsMeshObject();                      // 0x00A127D0
    bool GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData* out);   // 0x00A12450
};
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092();
    virtual void* GetPhysicsMeshParameters();   // slot 93 (+0x174)
    CPhysicsMeshObject GetPhysicsMeshObject();  // 0x006B0850
};
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
};
struct CThingFilterBase {
    virtual bool operator()(const CThing* thing) const;
};

extern "C" void* __cdecl malloc(unsigned int size);      // 0x00BFEA0E (MSVCR71)
extern "C" void __cdecl free(void* p);                   // 0x00BFEA14 (MSVCR71)

struct SThingNode {                                      // STLport _List_node<CThing*>
    SThingNode* _M_next;
    SThingNode* _M_prev;
    CThing*     _M_data;
};
struct CThingList {                                      // STLport list<CThing*>: one sentinel pointer
    SThingNode* _M_node;
    CThingList()
    {
        SThingNode* head = (SThingNode*)malloc(sizeof(SThingNode));
        head->_M_next = head;
        head->_M_prev = head;
        _M_node = head;
    }
    ~CThingList()
    {
        SThingNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SThingNode* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
        free(_M_node);
    }
    void push_back(CThing* thing)                        // STLport insert(end(), thing)
    {
        SThingNode* pos = _M_node;
        SThingNode* node = (SThingNode*)malloc(sizeof(SThingNode));
        node->_M_data = thing;
        node->_M_next = pos;
        node->_M_prev = pos->_M_prev;
        pos->_M_prev->_M_next = node;
        pos->_M_prev = node;
    }
};

struct CThingFilter_OverlappingCircle {
    C3DVector Center;
    float     Radius;
    CThingFilter_OverlappingCircle(const C3DVector& center, float radius) : Center(center), Radius(radius) { }
};
struct CBoundThingFilter {                               // the circle test and the caller's filter, by reference
    const CThingFilter_OverlappingCircle& Circle;
    const CThingFilterBase&               Filter;
    CBoundThingFilter(const CThingFilter_OverlappingCircle& circle, const CThingFilterBase& filter) : Circle(circle), Filter(filter) { }
};

CThing* NextThingInRoughArea(const C2DBoxI& box, CThing* previous);

struct CThingSearchTools {
    void ForEachInRoughArea(const C2DBoxI& box, const CBoundThingFilter& filter, CThingList& things);   // 0x0063FD40
};
// Same-TU stand-in for the 0x0063FD40 template instance (retail emits it in this TU after the caller): with its body in
// view VC7.1 knows the walk only appends nodes and keeps the list sentinel in a register across the call.
#pragma auto_inline(off)
void CThingSearchTools::ForEachInRoughArea(const C2DBoxI& box, const CBoundThingFilter& filter, CThingList& things)
{
    for (CThing* thing = NextThingInRoughArea(box, 0); thing; thing = NextThingInRoughArea(box, thing))
        if (filter.Filter(thing))
            things.push_back(thing);
}
#pragma auto_inline(on)

struct CWorld {
    unsigned char      _pad00[0x20];
    CThingSearchTools* ThingSearchTools;         // +0x20
};

class CWorldMap {
public:
    void*   __vftable;
    long    f4;                                  // +0x04
    CWorld* World;                               // +0x08
    float GetGroundSizeZAt(const C3DVector& pos) const;      // 0x0063A300
    float GetThemeSizeZAt(const C3DVector& pos) const;       // 0x0063A390
    float GetHeightZBelowAt(const C3DVector& pos, float radius, bool useTheme, const CThingFilterBase& filter) const;
};

float CWorldMap::GetHeightZBelowAt(const C3DVector& pos, float radius, bool useTheme, const CThingFilterBase& filter) const
{
    float size = radius + radius;
    C2DBoxI box((long)(pos.X - size - 1.0f), (long)(pos.Y - size - 1.0f), (long)(size + pos.X + 1.0f), (long)(size + pos.Y + 1.0f));
    box.ValidateOrientationAndFlip();
    CThingList things;
    CThingFilter_OverlappingCircle circle(pos, radius);
    World->ThingSearchTools->ForEachInRoughArea(box, CBoundThingFilter(circle, filter), things);
    float height = useTheme ? GetThemeSizeZAt(pos) : GetGroundSizeZAt(pos);
    C3DVector from = pos;
    C3DVector to(from.X, from.Y, from.Z - 200.0f);
    for (SThingNode* node = things._M_node->_M_next; node != things._M_node; node = node->_M_next) {
        CTCPhysicsBase* physics = node->_M_data->PhysicsTC;
        if (physics && physics->GetPhysicsMeshParameters()) {
            CPhysicsMeshObject mesh = physics->GetPhysicsMeshObject();
            CMeshIntersectionData data;
            if (mesh.GetPointIntersectionFromTo(from, to, &data)) {
                float z = data.IntersectionPosition.Z + 0.007f;
                if (z > height && z < from.Z)
                    height = z;
            }
        }
    }
    return height;
}

#undef malloc
#undef free
static int g_allocs = 0;
extern "C" void* __cdecl Spy_malloc(unsigned int size) { ++g_allocs; return malloc(size); }
extern "C" void __cdecl Spy_free(void* p) { --g_allocs; free(p); }
void CTCPhysicsBase::v000() { }
void CTCPhysicsBase::v001() { }
void CTCPhysicsBase::v002() { }
void CTCPhysicsBase::v003() { }
void CTCPhysicsBase::v004() { }
void CTCPhysicsBase::v005() { }
void CTCPhysicsBase::v006() { }
void CTCPhysicsBase::v007() { }
void CTCPhysicsBase::v008() { }
void CTCPhysicsBase::v009() { }
void CTCPhysicsBase::v010() { }
void CTCPhysicsBase::v011() { }
void CTCPhysicsBase::v012() { }
void CTCPhysicsBase::v013() { }
void CTCPhysicsBase::v014() { }
void CTCPhysicsBase::v015() { }
void CTCPhysicsBase::v016() { }
void CTCPhysicsBase::v017() { }
void CTCPhysicsBase::v018() { }
void CTCPhysicsBase::v019() { }
void CTCPhysicsBase::v020() { }
void CTCPhysicsBase::v021() { }
void CTCPhysicsBase::v022() { }
void CTCPhysicsBase::v023() { }
void CTCPhysicsBase::v024() { }
void CTCPhysicsBase::v025() { }
void CTCPhysicsBase::v026() { }
void CTCPhysicsBase::v027() { }
void CTCPhysicsBase::v028() { }
void CTCPhysicsBase::v029() { }
void CTCPhysicsBase::v030() { }
void CTCPhysicsBase::v031() { }
void CTCPhysicsBase::v032() { }
void CTCPhysicsBase::v033() { }
void CTCPhysicsBase::v034() { }
void CTCPhysicsBase::v035() { }
void CTCPhysicsBase::v036() { }
void CTCPhysicsBase::v037() { }
void CTCPhysicsBase::v038() { }
void CTCPhysicsBase::v039() { }
void CTCPhysicsBase::v040() { }
void CTCPhysicsBase::v041() { }
void CTCPhysicsBase::v042() { }
void CTCPhysicsBase::v043() { }
void CTCPhysicsBase::v044() { }
void CTCPhysicsBase::v045() { }
void CTCPhysicsBase::v046() { }
void CTCPhysicsBase::v047() { }
void CTCPhysicsBase::v048() { }
void CTCPhysicsBase::v049() { }
void CTCPhysicsBase::v050() { }
void CTCPhysicsBase::v051() { }
void CTCPhysicsBase::v052() { }
void CTCPhysicsBase::v053() { }
void CTCPhysicsBase::v054() { }
void CTCPhysicsBase::v055() { }
void CTCPhysicsBase::v056() { }
void CTCPhysicsBase::v057() { }
void CTCPhysicsBase::v058() { }
void CTCPhysicsBase::v059() { }
void CTCPhysicsBase::v060() { }
void CTCPhysicsBase::v061() { }
void CTCPhysicsBase::v062() { }
void CTCPhysicsBase::v063() { }
void CTCPhysicsBase::v064() { }
void CTCPhysicsBase::v065() { }
void CTCPhysicsBase::v066() { }
void CTCPhysicsBase::v067() { }
void CTCPhysicsBase::v068() { }
void CTCPhysicsBase::v069() { }
void CTCPhysicsBase::v070() { }
void CTCPhysicsBase::v071() { }
void CTCPhysicsBase::v072() { }
void CTCPhysicsBase::v073() { }
void CTCPhysicsBase::v074() { }
void CTCPhysicsBase::v075() { }
void CTCPhysicsBase::v076() { }
void CTCPhysicsBase::v077() { }
void CTCPhysicsBase::v078() { }
void CTCPhysicsBase::v079() { }
void CTCPhysicsBase::v080() { }
void CTCPhysicsBase::v081() { }
void CTCPhysicsBase::v082() { }
void CTCPhysicsBase::v083() { }
void CTCPhysicsBase::v084() { }
void CTCPhysicsBase::v085() { }
void CTCPhysicsBase::v086() { }
void CTCPhysicsBase::v087() { }
void CTCPhysicsBase::v088() { }
void CTCPhysicsBase::v089() { }
void CTCPhysicsBase::v090() { }
void CTCPhysicsBase::v091() { }
void CTCPhysicsBase::v092() { }
static C2DBoxI g_box(0, 0, 0, 0);
void C2DBoxI::ValidateOrientationAndFlip() { g_box = *this; }

struct CTestPhysics : CTCPhysicsBase {
    float MeshZ;
    bool  HasParams;
};
static int g_meshes = 0;
void* CTCPhysicsBase::GetPhysicsMeshParameters() { return static_cast<CTestPhysics*>(this)->HasParams ? this : 0; }
CPhysicsMeshObject CTCPhysicsBase::GetPhysicsMeshObject()
{
    CPhysicsMeshObject mesh;
    mesh.__vftable = 0;
    mesh.m_pMesh = &static_cast<CTestPhysics*>(this)->MeshZ;
    return mesh;
}
CPhysicsMeshObject::~CPhysicsMeshObject() { ++g_meshes; }
bool CPhysicsMeshObject::GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData* out)
{
    float z = *(float*)m_pMesh;
    if (z > from.Z || z < to.Z)
        return false;
    out->IntersectionPosition = C3DVector(from.X, from.Y, z);
    return true;
}

static CThing g_things[6];
static CTestPhysics g_physics[6];
CThing* NextThingInRoughArea(const C2DBoxI&, CThing* previous)
{
    int next = previous ? (int)(previous - g_things) + 1 : 0;
    return next < 6 ? &g_things[next] : 0;
}
struct CRejectThing : CThingFilterBase {
    const CThing* Rejected;
    virtual bool operator()(const CThing* thing) const { return thing != Rejected; }
};
bool CThingFilterBase::operator()(const CThing*) const { return true; }

float CWorldMap::GetGroundSizeZAt(const C3DVector&) const { return 0.0f; }
float CWorldMap::GetThemeSizeZAt(const C3DVector&) const { return 8.0f; }

int main()
{
    float meshZ[6] = { 5.0f, 0.0f, 7.0f, 12.0f, 6.0f, 3.0f };
    for (int k = 0; k < 6; ++k) {
        g_physics[k].MeshZ = meshZ[k];
        g_physics[k].HasParams = (k != 2);
        g_things[k].PhysicsTC = (k == 1) ? 0 : &g_physics[k];
    }
    CThingSearchTools tools;
    CWorld world;
    world.ThingSearchTools = &tools;
    CWorldMap map;
    map.World = &world;
    CRejectThing filter;
    filter.Rejected = &g_things[4];
    C3DVector pos(10.5f, 20.5f, 10.0f);

    bool ok = true;
    float h = map.GetHeightZBelowAt(pos, 1.0f, false, filter);
    if (fabs(h - 5.007f) > 1e-4f) ok = false;
    if (g_box.TLX != 7 || g_box.TLY != 17 || g_box.BRX != 13 || g_box.BRY != 23) ok = false;
    float t = map.GetHeightZBelowAt(pos, 1.0f, true, filter);
    if (fabs(t - 8.0f) > 1e-4f) ok = false;
    filter.Rejected = &g_things[0];
    float r = map.GetHeightZBelowAt(pos, 1.0f, false, filter);
    if (fabs(r - 6.007f) > 1e-4f) ok = false;
    if (g_allocs != 0 || g_meshes < 9) ok = false;   // every gathered mesh object is destroyed (copies may add more)
    if (!ok) {
        printf("0063e950_TEST FAIL\n");
        return 1;
    }
    printf("0063e950_TEST PASS\n");
    return 0;
}
