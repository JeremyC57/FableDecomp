// Behaviour test: CTCPhysicsRigidBody::~CTCPhysicsRigidBody (0x0075A480) -- the mesh info is deleted only when present,
// the member buffers are freed (non-null only), and the PhysicsDef release deletes the def when its count reaches 0.
#include <cstdio>
static int g_deletes = 0, g_frees = 0, g_defDeletes = 0, g_baseDtor = 0;
class CThingPhysicsMeshInfo { public: int x; };
static void FreeBuf(void* p) { if (p != 0) ++g_frees; }
struct Buf { int* start; Buf() : start(0) { } ~Buf() { FreeBuf(start); } };
struct CDefPointee { long RefCount; virtual void Delete() { ++g_defDeletes; } virtual ~CDefPointee() { } };
struct CDefPointer {
    CDefPointee* Object; CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { CDefPointee* o = Object; if (--o->RefCount == 0) o->Delete(); Object = 0; } }
};
class CTCBase { public: virtual ~CTCBase() { ++g_baseDtor; } };
class CTCPhysicsBase : public CTCBase { public: CDefPointer PhysicsDef; };
struct CRigidBody { Buf BodyBoundingVertices, Current, Last; };
struct CTCPhysicsRigidBody : CTCPhysicsBase {
    Buf CollisionNormal, CollidingVertexIndex, Collidors;
    CThingPhysicsMeshInfo* PPhysicsMeshInfo;
    CRigidBody Body;
    CTCPhysicsRigidBody() : PPhysicsMeshInfo(0) { }
    virtual ~CTCPhysicsRigidBody() { if (PPhysicsMeshInfo) { delete PPhysicsMeshInfo; ++g_deletes; } }
};
int main()
{
    static int a[1], b[1];
    CDefPointee def; def.RefCount = 1;
    CTCPhysicsRigidBody* body = new CTCPhysicsRigidBody;
    body->PPhysicsMeshInfo = new CThingPhysicsMeshInfo;
    body->Collidors.start = a; body->Body.Last.start = b;
    body->PhysicsDef.Object = &def;
    delete body;
    if (g_deletes != 1 || g_frees != 2 || g_defDeletes != 1 || g_baseDtor != 1) { std::printf("0075a480_TEST FAIL\n"); return 1; }
    CDefPointee def2; def2.RefCount = 2;
    CTCPhysicsRigidBody* empty = new CTCPhysicsRigidBody;
    empty->PhysicsDef.Object = &def2;
    delete empty;
    if (g_deletes != 1 || g_frees != 2 || g_defDeletes != 1 || def2.RefCount != 1 || g_baseDtor != 2)
    { std::printf("0075a480_TEST FAIL\n"); return 1; }
    std::printf("0075a480_TEST PASS\n");
    return 0;
}