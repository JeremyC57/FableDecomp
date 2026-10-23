// Grade: DIFFER(607v593) -- 163 difflines, coarse pass: frame 0x68 v 0x58 (retail reuses the parent-copy slot for the vertex index whose address the index push_back takes, and keeps the result in the frame where the model holds it in ebp), loop-head alignment, and the x87 plane/point-velocity scheduling. The search set-up (exclude filter of {&parent, &self}, slot 97 key, GetAllThingsInRoughArea into a freed vector), the -0.001/0.001 tests and the two push_backs follow retail.
// CTCPhysicsRigidBody::CheckForCollisions(const CRigidBodyState& state) -- gather the things roughly around the body (excluding the parent and this body), then test every physics-mesh vertex of the state against the ground plane under it: a vertex sunk past -0.001 means interpenetration (0); a vertex within 0.001 of the plane that moves into it records a contact normal and the vertex index (1); otherwise no collision (2)
// Leaf: CheckForCollisions
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075AAF0, 591 bytes, `ret 4`.
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    float operator*(const C3DVector& o) const { return Z * o.Z + Y * o.Y + X * o.X; }
};
struct C2DBoxI { long MinX, MinY, MaxX, MaxY; };
class CThing;
struct CWorldMap {
    float GetGroundSizeZAt(const C3DVector& pos);     // 0x0063A300
    C3DVector GetGroundFaceNormalAt(const C3DVector& pos);   // 0x0063AB10
};
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CC3DVectorVector : vector<C3DVector> { void push_back(const C3DVector& v); };   // 0x0066E630
struct CLongVector : vector<long> { void push_back(const long& v); };                 // 0x00485FF3
struct CRigidBodyFilter {
    CThing* const* PParent;
    const void* const* PSelf;
    CRigidBodyFilter(CThing* const* parent, const void* const* self) : PParent(parent), PSelf(self) { }
};
struct CThingSearchTools {
    void GetAllThingsInRoughArea(const C2DBoxI& area, const CRigidBodyFilter& filter, vector<CThing*>& things);   // 0x0075BC40
};
struct CRigidBodyState {
    C3DVector Position;                               // +0x00
    unsigned char _pad0c[0x3c - 0x0c];
    C3DVector Velocity;                               // +0x3c
    unsigned char _pad48[0x9c - 0x48];
    C3DVector AngularVelocity;                        // +0x9c
    vector<C3DVector> Vertices;                       // +0xa8
};
struct CTCPhysicsRigidBody {
    virtual void v00();
    virtual void s01();
    virtual void s02();
    virtual void s03();
    virtual void s04();
    virtual void s05();
    virtual void s06();
    virtual void s07();
    virtual void s08();
    virtual void s09();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53();
    virtual void s54();
    virtual void s55();
    virtual void s56();
    virtual void s57();
    virtual void s58();
    virtual void s59();
    virtual void s60();
    virtual void s61();
    virtual void s62();
    virtual void s63();
    virtual void s64();
    virtual void s65();
    virtual void s66();
    virtual void s67();
    virtual void s68();
    virtual void s69();
    virtual void s70();
    virtual void s71();
    virtual void s72();
    virtual void s73();
    virtual void s74();
    virtual void s75();
    virtual void s76();
    virtual void s77();
    virtual void s78();
    virtual void s79();
    virtual void s80();
    virtual void s81();
    virtual void s82();
    virtual void s83();
    virtual void s84();
    virtual void s85();
    virtual void s86();
    virtual void s87();
    virtual void s88();
    virtual void s89();
    virtual void s90();
    virtual void s91();
    virtual void s92();
    virtual void s93();
    virtual void s94();
    virtual void s95();
    virtual void s96();
    CThing* ParentThing;                              // +0x04
    unsigned char _pad08[0x40 - 0x08];
    CC3DVectorVector CollisionNormal;                 // +0x40
    CLongVector      CollidingVertexIndex;            // +0x4c
    unsigned char _pad58[0xe4 - 0x58];
    vector<C3DVector> MeshVertices;                   // +0xe4
    CWorldMap* GetWorldMap();                         // 0x006AF620
    CThingSearchTools* GetSearchTools(const void* key);   // 0x006AFD40
    virtual const void* GetSearchKey();               // slot 97 (+0x184) -- placeholder slot
    long CheckForCollisions(const CRigidBodyState& state);
};

long CTCPhysicsRigidBody::CheckForCollisions(const CRigidBodyState& state)
{
    long vertexCount = (long)MeshVertices.size();
    CThing* parent = ParentThing;
    const void* self = this;
    long result = 2;
    vector<CThing*> things;
    C2DBoxI area;
    GetSearchTools(GetSearchKey())->GetAllThingsInRoughArea(area, CRigidBodyFilter(&parent, &self), things);
    for (long i = 0; i < vertexCount; ++i) {
        C3DVector vertex = state.Vertices[i];
        C3DVector rel = vertex - state.Position;
        C3DVector pointVelocity(state.AngularVelocity.Y * rel.Z - state.AngularVelocity.Z * rel.Y,
                                state.AngularVelocity.Z * rel.X - state.AngularVelocity.X * rel.Z,
                                state.AngularVelocity.X * rel.Y - state.AngularVelocity.Y * rel.X);
        pointVelocity = pointVelocity + state.Velocity;
        float groundZ = GetWorldMap()->GetGroundSizeZAt(vertex);
        C3DVector normal = GetWorldMap()->GetGroundFaceNormalAt(vertex);
        C3DVector contact = normal;
        float d = -(normal.Z * groundZ + normal.X * vertex.X + normal.Y * vertex.Y);
        float distance = normal * vertex + d;
        if (distance < -0.001f)
            return 0;
        if (distance < 0.001f && normal * pointVelocity < 0.0f) {
            result = 1;
            CollisionNormal.push_back(contact);
            CollidingVertexIndex.push_back(i);
        }
    }
    return result;
}
