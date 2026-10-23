// CTCPhysicsBase::CTCPhysicsBase(CThing*) -- base ctor, then zero position/old position/velocity, Radius = -1, null def pointer, clear the two moving bits
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
