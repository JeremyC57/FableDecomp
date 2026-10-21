// Grade: DIFFER(482v510) -- first-pass sketch: the shared evasion loop's x87 schedule (008faa80) plus the thing/mesh list construction and teardown order are not reproduced
// NCameraModes::CPhysicsForceModelBase::DoThingEvasion() -- gathers the standard things within 50 units of the camera that have a collision mesh, rescans their mesh points and returns the 5/distance potential gradient around the camera
// Leaf: ?DoThingEvasion@CPhysicsForceModelBase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FAF70, 510 bytes, `ret 4` (C3DVector by hidden pointer).
// Callees: malloc 0x00BFEA0E (list headers 0x0C and 0x48), CTools::GetStandardThings 0x00877720 (camera manager +0xA8,
//   tools +0x78), list::remove_if<unary_negate<CGetThingsWithCollisionMesh>> 0x008FBDD0 (the empty predicate's word is pushed
//   uninitialised), 0x008FCA40 (builds the mesh list from the thing range), UpdateScan<list<CPhysicsMeshObject>> 0x008FC050,
//   0x008FA9F0 on Things (+0x08), the list teardowns 0x008FBAB0 / 0x00615540 and free 0x00BFEA14. Constants as 008fadb0.
// First-pass sketch: the list helpers' exact shapes (the 0x8FCA40 call passes two by-value iterators and the list) are unverified.
extern "C" void* __cdecl malloc(unsigned int size); // 0x00BFEA0E
extern "C" void __cdecl free(void* block);          // 0x00BFEA14
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double);
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float Length() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
struct SVectorNode { SVectorNode* _M_next; SVectorNode* _M_prev; C3DVector _M_data; };
extern const C3DVector g_EvasionSampleOffsets[4];  // 0x013893AC: 0, X, Y, Z

struct CThing;
struct SThingNode { SThingNode* _M_next; SThingNode* _M_prev; const CThing* _M_data; };
struct SThingListIterator { SThingNode* _M_node; };
namespace NCameraModes { struct CGetThingsWithCollisionMesh { }; }
template <class P> struct unary_negate { P pred; };
struct CThingList {                                 // list<const CThing*> (header node 0x0C)
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
        Clear();                                   // 0x008FBAB0 (manifest "SortTreeRecursively")
        free(_M_node);
    }
    void Clear();
    void remove_if(unary_negate<NCameraModes::CGetThingsWithCollisionMesh> pred);   // 0x008FBDD0
};
struct SMeshNode { SMeshNode* _M_next; SMeshNode* _M_prev; unsigned char _data[0x40]; };
struct CMeshList {                                  // list<CPhysicsMeshObject> (header node 0x48)
    SMeshNode* _M_node;
    CMeshList()
    {
        SMeshNode* head = (SMeshNode*)malloc(sizeof(SMeshNode));
        head->_M_next = head;
        head->_M_prev = head;
        _M_node = head;
    }
    ~CMeshList();                                  // 0x00615540
    void Build(SThingListIterator first, SThingListIterator last, const CThingList& things);   // 0x008FCA40
};
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
};

C3DVector CPhysicsForceModelBase::DoThingEvasion()
{
    C3DVector position = LinkedState->Position;
    CThingList things;
    CameraManager->Tools.GetStandardThings(things, position, 50.0f);
    things.remove_if(unary_negate<CGetThingsWithCollisionMesh>());
    CMeshList meshes;
    SThingListIterator first = { things._M_node->_M_next };
    SThingListIterator last = { things._M_node };
    meshes.Build(first, last, things);
    UpdateScan(Things, meshes, position, 1, 50.0f);
    Things.Finalise();
    SVectorNode* head = Things.Points;
    float potential[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    for (SVectorNode* it = head->_M_next; it != head; it = it->_M_next) {
        for (long i = 0; i < 4; ++i)
            potential[i] += 5.0f / (g_EvasionSampleOffsets[i] * 0.1f + position - it->_M_data).Length();
    }
    return C3DVector((potential[1] - potential[0]) * 10.0f, (potential[2] - potential[0]) * 10.0f, (potential[3] - potential[0]) * 10.0f);
}
}
