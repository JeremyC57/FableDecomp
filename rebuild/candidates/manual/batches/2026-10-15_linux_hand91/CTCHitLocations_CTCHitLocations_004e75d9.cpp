#pragma optimize("s", on)
// CTCHitLocations::CTCHitLocations(CThing* parent) -- CTCBase(parent), own vtable, an empty hit-location vector, no default hit location or definition, a zero bounding sphere not yet cached
// Leaf: ??0CTCHitLocations
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E75D9, length 64 bytes.
// Callee: CTCBase::CTCBase(CThing*) 0x00686800 (manifest label "CEngineSubPrimitive2DClockSprite" is
//         propagated; the body stores ParentThing +4, the two flag bytes +8/+9 and the CTCBase vtable).
// Vtable 0x012434E4 (RTTI: CTCHitLocations). PDB CTCHitLocations (56 bytes) with the retail 12-byte
// std::vector: HitLocations +0x0c, PDefaultHitLocation +0x18, PDef (CDefPointer) +0x1c,
// BoundingSphereOffsetFromPos (C3DVector) +0x20, BoundingSphereRadius +0x2c, CachedBoundingSphere +0x30.
// Shape: a size-optimised TU (odd address, `push dword ptr [esp+8]` pass-through): every float zero is
// an x87 `fldz ; fstp` pair, the integer zeros and the bool share eax; the vtable store precedes all
// member stores because the vector, the def pointer and the vector3 are member objects with inline
// constructors.
struct CThing;
struct CHitLocation;
struct CHitLocationsDef;

struct CTCBase {
    virtual void v00();
    CThing*       ParentThing;                 // +0x04
    bool          ToBeRemoved;                 // +0x08
    bool          IsInGlobalUpdateTCs;         // +0x09
    CTCBase(CThing* parent);                   // 0x00686800
};

struct CHitLocationVector {                    // retail std::vector<pair<CHitLocation, long> >: three words
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CHitLocationVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CHitLocationsDefPointer {               // CDefPointer<const CHitLocationsDef>
    const CHitLocationsDef* Object;
    CHitLocationsDefPointer() : Object(0) { }
};

struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct CTCHitLocations : CTCBase {             // vtable 0x012434E4
    virtual void v00();
    CHitLocationVector      HitLocations;                  // +0x0c
    CHitLocation*           PDefaultHitLocation;           // +0x18
    CHitLocationsDefPointer PDef;                          // +0x1c
    C3DVector               BoundingSphereOffsetFromPos;   // +0x20
    float                   BoundingSphereRadius;          // +0x2c
    bool                    CachedBoundingSphere;          // +0x30
    CTCHitLocations(CThing* parent);
};

CTCHitLocations::CTCHitLocations(CThing* parent)
    : CTCBase(parent),
      PDefaultHitLocation(0),
      BoundingSphereOffsetFromPos(0.0f, 0.0f, 0.0f),
      BoundingSphereRadius(0.0f),
      CachedBoundingSphere(false)
{
}
