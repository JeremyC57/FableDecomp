// Behaviour test for 008faf70: src/008faf70.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// NCameraModes::CPhysicsForceModelBase::DoThingEvasion()
#include <cstdio>
#include <cmath>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NCameraModes::CPhysicsForceModelBase::DoThingEvasion() -- gathers the standard things within 50 units of the camera that have a collision mesh, rescans their mesh points and returns the 5/distance potential gradient around the camera
// Leaf: ?DoThingEvasion@CPhysicsForceModelBase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FAF70, 510 bytes, `ret 4` (C3DVector by hidden pointer).
// Callees: malloc 0x00BFEA0E (list headers 0x0C and 0x48), CTools::GetStandardThings 0x00877720 (camera manager +0xA8,
//   tools +0x78), list<const CThing*>::remove_if<unary_negate<CGetThingsWithCollisionMesh> > 0x008FBDD0 (the empty
//   predicate's word is pushed from the hidden-result slot), transform<..., back_insert_iterator<list<CPhysicsMeshObject> >,
//   CGetMeshObject> 0x008FCA40 (__fastcall, hidden result in ecx), UpdateScan<list<CPhysicsMeshObject> > 0x008FC050,
//   CPhysicsCollisionPointsObject::Finalise 0x008FA9F0 on Things (+0x08), list<CPhysicsMeshObject>::clear 0x008FBAB0, free
//   0x00BFEA14 and list<const CThing*>::~list 0x00615540. Constants as 0x008FADB0.
// Shape: STLport lists with the header pointer in an alloc proxy initialised to 0 (retail stores the zero before malloc);
//   the mesh list's destructor is inline (clear + null-tested free), the thing list's is out of line; the camera manager is
//   read through an inline GetCameraManager() accessor (its load then follows the header store and the tools address is a
//   `lea ecx,[eax+78h]`); the evasion loop is the landed 0x008FAA80 form with the C3DVector user copy ctor; the result
//   comes from a `float r[3]` aggregate (left-to-right: X, Y, Z computed, then `fxch st(2)` and three stores).
// Layout: retail Things +0x08, CameraManager +0xA8 (PDB +0xF0).
extern "C" void* __cdecl malloc(unsigned int size); // 0x00BFEA0E
extern "C" void __cdecl free(void* block);          // 0x00BFEA14
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double);
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const float* v) : X(v[0]), Y(v[1]), Z(v[2]) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
inline float Magnitude(const C3DVector& v) { return (float)sqrt(v.Z * v.Z + v.Y * v.Y + v.X * v.X); }
struct SVectorNode { SVectorNode* _M_next; SVectorNode* _M_prev; C3DVector _M_data; };
extern const C3DVector g_EvasionSampleOffsets[4];  // 0x013893AC: 0, X, Y, Z

struct CThing;
struct SThingNode { SThingNode* _M_next; SThingNode* _M_prev; const CThing* _M_data; };
struct SThingListIterator {
    SThingNode* _M_node;
    SThingListIterator(SThingNode* n) : _M_node(n) { }
    SThingListIterator(const SThingListIterator& o) : _M_node(o._M_node) { }
};
namespace NCameraModes {
struct CGetThingsWithCollisionMesh { CGetThingsWithCollisionMesh() { } bool operator()(const CThing* t) const; };
struct CGetMeshObject { CGetMeshObject() { } };
}
template <class P> struct unary_negate {
    P pred;
    unary_negate(const P& p) : pred(p) { }
    bool operator()(const CThing* t) const { return !pred(t); }
};
template <class P> inline unary_negate<P> not1(const P& p) { return unary_negate<P>(p); }
template <class T> struct SListProxy {
    T* _M_data;
    SListProxy(T* p) : _M_data(p) { }
};
struct CThingList {                                 // list<const CThing*> (header node 0x0C)
    SListProxy<SThingNode> _M_node;
    CThingList() : _M_node(0)
    {
        SThingNode* head = (SThingNode*)malloc(sizeof(SThingNode));
        head->_M_next = head;
        head->_M_prev = head;
        _M_node._M_data = head;
    }
    ~CThingList();                                 // 0x00615540
    SThingListIterator begin() { return SThingListIterator(_M_node._M_data->_M_next); }
    SThingListIterator end() { return SThingListIterator(_M_node._M_data); }
    void remove_if(unary_negate<NCameraModes::CGetThingsWithCollisionMesh> pred);   // 0x008FBDD0
};
struct CPhysicsMeshObject { virtual ~CPhysicsMeshObject(); unsigned char _data[0x3c]; };
struct SMeshNode { SMeshNode* _M_next; SMeshNode* _M_prev; CPhysicsMeshObject _M_data; };
struct CMeshList {                                  // list<CPhysicsMeshObject> (header node 0x48)
    SListProxy<SMeshNode> _M_node;
    CMeshList() : _M_node(0)
    {
        SMeshNode* head = (SMeshNode*)malloc(sizeof(SMeshNode));
        head->_M_next = head;
        head->_M_prev = head;
        _M_node._M_data = head;
    }
    ~CMeshList()
    {
        clear();
        if (_M_node._M_data)
            free(_M_node._M_data);
    }
    void clear();                                  // 0x008FBAB0
};
struct SMeshBackInserter {
    CMeshList* container;
    explicit SMeshBackInserter(CMeshList& c) : container(&c) { }
};
inline SMeshBackInserter back_inserter(CMeshList& c) { return SMeshBackInserter(c); }
SMeshBackInserter __fastcall transform(SThingListIterator first, SThingListIterator last, SMeshBackInserter result,
                                       NCameraModes::CGetMeshObject op);   // 0x008FCA40
struct CTools {
    void GetStandardThings(CThingList& things, const C3DVector& centre, float radius);   // 0x00877720
};
struct CGameCameraManager {
    unsigned char _pad00[0x78];
    CTools        Tools;                           // +0x78
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
    CPhysicsCollisionPointsObject Things;          // +0x08
    unsigned char                 _pad2c[0xa8 - 0x2c];
    CGameCameraManager*           CameraManager;   // +0xa8 (PDB +0xF0)
    void UpdateScan(CPhysicsCollisionPointsObject& points, CMeshList& meshes, const C3DVector& centre, long flags, float radius);   // 0x008FC050
    C3DVector DoThingEvasion();
    CGameCameraManager* GetCameraManager() const { return CameraManager; }
};

C3DVector CPhysicsForceModelBase::DoThingEvasion()
{
    C3DVector position = LinkedState->Position;
    CThingList things;
    GetCameraManager()->Tools.GetStandardThings(things, position, 50.0f);
    things.remove_if(not1(CGetThingsWithCollisionMesh()));
    CMeshList meshes;
    transform(things.begin(), things.end(), back_inserter(meshes), CGetMeshObject());
    UpdateScan(Things, meshes, position, 1, 50.0f);
    Things.Finalise();
    SVectorNode* head = Things.Points;
    float potential[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    for (SVectorNode* it = head->_M_next; it != head; it = it->_M_next) {
        for (long i = 0; i < 4; ++i) {
            C3DVector sample = g_EvasionSampleOffsets[i] * 0.1f + position;
            potential[i] += 5.0f / Magnitude(sample - it->_M_data);
        }
    }
    float r[3] = { (potential[1] - potential[0]) * 10.0f, (potential[2] - potential[0]) * 10.0f, (potential[3] - potential[0]) * 10.0f };
    return C3DVector(r);
}
}

// ---- stubs and driver ----
const C3DVector g_EvasionSampleOffsets[4] = { C3DVector(0.0f, 0.0f, 0.0f), C3DVector(1.0f, 0.0f, 0.0f),
                                              C3DVector(0.0f, 1.0f, 0.0f), C3DVector(0.0f, 0.0f, 1.0f) };
struct CThing { int hasMesh; };
static CThing g_world[5];
static int g_seq = 0, g_atGet = 0, g_atRemove = 0, g_atTransform = 0, g_atScan = 0, g_atFinal = 0, g_atClear = 0, g_atThingDtor = 0;
static int g_liveNodes = 0, g_meshDtors = 0, g_scanMeshes = 0;
static const CThing* g_scanned[5];
static float g_cx, g_cy, g_cz, g_gx, g_gy, g_gz, g_radius, g_scanRadius; static long g_scanFlags;
static const void* g_toolsSeen; static const void* g_pointsSeen;
bool NCameraModes::CGetThingsWithCollisionMesh::operator()(const CThing* t) const { return t->hasMesh != 0; }
void CTools::GetStandardThings(CThingList& things, const C3DVector& centre, float radius)
{
    g_atGet = ++g_seq; g_toolsSeen = this; g_gx = centre.X; g_gy = centre.Y; g_gz = centre.Z; g_radius = radius;
    SThingNode* h = things._M_node._M_data;
    for (int i = 0; i < 5; ++i) {
        SThingNode* n = (SThingNode*)malloc(sizeof(SThingNode)); ++g_liveNodes;
        n->_M_data = &g_world[i]; n->_M_prev = h->_M_prev; n->_M_next = h; h->_M_prev->_M_next = n; h->_M_prev = n;
    }
}
void CThingList::remove_if(unary_negate<NCameraModes::CGetThingsWithCollisionMesh> pred)
{
    g_atRemove = ++g_seq;
    SThingNode* h = _M_node._M_data;
    for (SThingNode* n = h->_M_next; n != h; ) {
        SThingNode* next = n->_M_next;
        if (pred(n->_M_data)) { n->_M_prev->_M_next = next; next->_M_prev = n->_M_prev; free(n); --g_liveNodes; }
        n = next;
    }
}
CThingList::~CThingList()
{
    g_atThingDtor = ++g_seq;
    SThingNode* h = _M_node._M_data;
    for (SThingNode* n = h->_M_next; n != h; ) { SThingNode* next = n->_M_next; free(n); --g_liveNodes; n = next; }
    free(h);
}
CPhysicsMeshObject::~CPhysicsMeshObject() { ++g_meshDtors; }
SMeshBackInserter __fastcall transform(SThingListIterator first, SThingListIterator last, SMeshBackInserter result,
                                       NCameraModes::CGetMeshObject op)
{
    g_atTransform = ++g_seq;
    SMeshNode* h = result.container->_M_node._M_data;
    for (; first._M_node != last._M_node; first._M_node = first._M_node->_M_next) {
        SMeshNode* n = (SMeshNode*)malloc(sizeof(SMeshNode)); ++g_liveNodes;
        new (&n->_M_data) CPhysicsMeshObject();
        memcpy(n->_M_data._data, &first._M_node->_M_data, sizeof(const CThing*));
        n->_M_prev = h->_M_prev; n->_M_next = h; h->_M_prev->_M_next = n; h->_M_prev = n;
    }
    return result;
}
void CMeshList::clear()
{
    g_atClear = ++g_seq;
    SMeshNode* h = _M_node._M_data;
    for (SMeshNode* n = h->_M_next; n != h; ) { SMeshNode* next = n->_M_next; n->_M_data.~CPhysicsMeshObject(); free(n); --g_liveNodes; n = next; }
    h->_M_next = h->_M_prev = h;
}
static SVectorNode g_ground[2];
void NCameraModes::CPhysicsForceModelBase::UpdateScan(CPhysicsCollisionPointsObject& points, CMeshList& meshes, const C3DVector& centre, long flags, float radius)
{
    g_atScan = ++g_seq; g_pointsSeen = &points;
    g_cx = centre.X; g_cy = centre.Y; g_cz = centre.Z; g_scanFlags = flags; g_scanRadius = radius;
    g_scanMeshes = 0;
    SMeshNode* h = meshes._M_node._M_data;
    for (SMeshNode* n = h->_M_next; n != h; n = n->_M_next) memcpy(&g_scanned[g_scanMeshes++], n->_M_data._data, sizeof(const CThing*));
}
void NCameraModes::CPhysicsCollisionPointsObject::Finalise() { g_atFinal = ++g_seq; }
void NCameraModes::CPhysicsForceModelBase::v00() {}
static bool Near(float a, float b) { return fabs(a - b) <= 1e-3 * (fabs(b) + 1.0); }
static float Pot(const C3DVector& pos, int i, const C3DVector& c)
{
    const C3DVector& o = g_EvasionSampleOffsets[i];
    double x = pos.X + o.X * 0.1 - c.X, y = pos.Y + o.Y * 0.1 - c.Y, z = pos.Z + o.Z * 0.1 - c.Z;
    return (float)(5.0 / sqrt(x * x + y * y + z * z));
}
int main()
{
    for (int i = 0; i < 5; ++i) g_world[i].hasMesh = (i == 1 || i == 3 || i == 4);
    C3DVector pos(1.0f, 2.0f, 3.0f);
    NCameraModes::CPhysicsState state; state.Position = pos;
    CGameCameraManager mgr;
    NCameraModes::CPhysicsForceModelBase model;
    model.LinkedState = &state; model.CameraManager = &mgr;
    // ground points list: one point at (2, 2, 2)
    SVectorNode head; head._M_next = head._M_prev = &head;
    g_ground[0]._M_data = C3DVector(2.0f, 2.0f, 2.0f);
    g_ground[0]._M_next = &head; g_ground[0]._M_prev = &head; head._M_next = head._M_prev = &g_ground[0];
    model.Things.Points = &head;
    C3DVector r = model.DoThingEvasion();
    CHECK(g_atGet == 1 && g_atRemove == 2 && g_atTransform == 3 && g_atScan == 4 && g_atFinal == 5 && g_atClear == 6 && g_atThingDtor == 7);
    CHECK(g_toolsSeen == &mgr.Tools && g_gx == 1.0f && g_gy == 2.0f && g_gz == 3.0f && g_radius == 50.0f);
    CHECK(g_pointsSeen == &model.Things && g_cx == 1.0f && g_cy == 2.0f && g_cz == 3.0f && g_scanFlags == 1 && g_scanRadius == 50.0f);
    CHECK(g_scanMeshes == 3 && g_scanned[0] == &g_world[1] && g_scanned[1] == &g_world[3] && g_scanned[2] == &g_world[4]);
    CHECK(g_meshDtors == 3 && g_liveNodes == 0);
    float p0 = Pot(pos, 0, g_ground[0]._M_data), p1 = Pot(pos, 1, g_ground[0]._M_data), p2 = Pot(pos, 2, g_ground[0]._M_data), p3 = Pot(pos, 3, g_ground[0]._M_data);
    CHECK(Near(r.X, (p1 - p0) * 10.0f) && Near(r.Y, (p2 - p0) * 10.0f) && Near(r.Z, (p3 - p0) * 10.0f));
    // no ground points: zero gradient
    head._M_next = head._M_prev = &head; g_seq = 0;
    r = model.DoThingEvasion();
    CHECK(r.X == 0.0f && r.Y == 0.0f && r.Z == 0.0f && g_liveNodes == 0 && g_atThingDtor == 7);
    if (fails == 0) printf("008faf70_TEST PASS\n");
    return fails;
}