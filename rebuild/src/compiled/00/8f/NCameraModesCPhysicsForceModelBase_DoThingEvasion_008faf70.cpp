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