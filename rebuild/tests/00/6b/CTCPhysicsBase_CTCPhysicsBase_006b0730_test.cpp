#include <cstdio>
#include <cstring>
#include <new>
struct CThing;
struct CDefPointeeBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CTCBase {                                   // polymorphic base: vptr +0, two words
    virtual ~CTCBase();
    long  sub;                                     // +0x04
    void* _base08;                                 // +0x08
    CTCBase(CThing* thing);                        // 0x00686800
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CPhysicsDef;
struct CTCPhysicsBase : public CTCBase {
    C3DVector                       Position;                  // +0x0c
    C3DVector                       OldPosition;               // +0x18
    long                            OldPositionLastFrameSet;   // +0x24
    C3DVector                       Velocity;                  // +0x28
    float                           Radius;                    // +0x34
    CDefPointer<const CPhysicsDef>  PhysicsDef;                // +0x38
    bool                            Moving : 1;                // +0x3c bit 0
    bool                            OldMoving : 1;             // +0x3c bit 1
    CTCPhysicsBase(CThing* thing);
    virtual ~CTCPhysicsBase();
};

CTCPhysicsBase::CTCPhysicsBase(CThing* thing)
    : CTCBase(thing),
      Position(0.0f, 0.0f, 0.0f),
      OldPosition(0.0f, 0.0f, 0.0f),
      OldPositionLastFrameSet(0),
      Velocity(0.0f, 0.0f, 0.0f),
      Radius(-1.0f)
{
    Moving = false;
    OldMoving = false;
}

static CThing* g_arg = 0;
CTCBase::CTCBase(CThing* thing) { g_arg = thing; sub = 0; _base08 = 0; }
CTCBase::~CTCBase() { }
CTCPhysicsBase::~CTCPhysicsBase() { }

int main()
{
    union Storage { double align; unsigned char bytes[sizeof(CTCPhysicsBase)]; } storage;
    std::memset(storage.bytes, 0xff, sizeof(storage.bytes));
    CThing* thing = (CThing*)&storage;
    CTCPhysicsBase* o = new (storage.bytes) CTCPhysicsBase(thing);
    if (g_arg != thing) { std::printf("FAIL arg\n"); return 1; }
    if (o->Position.X != 0.0f || o->Position.Y != 0.0f || o->Position.Z != 0.0f) { std::printf("FAIL position\n"); return 1; }
    if (o->OldPosition.X != 0.0f || o->OldPosition.Z != 0.0f || o->OldPositionLastFrameSet != 0) { std::printf("FAIL old\n"); return 1; }
    if (o->Velocity.X != 0.0f || o->Velocity.Y != 0.0f || o->Velocity.Z != 0.0f) { std::printf("FAIL velocity\n"); return 1; }
    if (o->Radius != -1.0f) { std::printf("FAIL radius\n"); return 1; }
    if (o->PhysicsDef.Object != 0) { std::printf("FAIL def\n"); return 1; }
    if (o->Moving || o->OldMoving) { std::printf("FAIL moving\n"); return 1; }
    o->~CTCPhysicsBase();
    std::printf("CTCPhysicsBase_006b0730_TEST PASS\n");
    return 0;
}
