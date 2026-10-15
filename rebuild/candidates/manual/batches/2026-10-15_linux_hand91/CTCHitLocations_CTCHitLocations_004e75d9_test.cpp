#include <cstdio>
// Behaviour test for CTCHitLocations::CTCHitLocations(CThing*) (retail 0x004E75D9).
struct CThing { int dummy; };
struct CHitLocation;
struct CHitLocationsDef;

struct CTCBase {
    virtual void v00() { }
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase(CThing* parent) : ParentThing(parent), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
    virtual ~CTCBase() { }
};

struct CHitLocationVector {
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CHitLocationVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CHitLocationsDefPointer {
    const CHitLocationsDef* Object;
    CHitLocationsDefPointer() : Object(0) { }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct CTCHitLocations : CTCBase {
    virtual void v00() { }
    CHitLocationVector      HitLocations;
    CHitLocation*           PDefaultHitLocation;
    CHitLocationsDefPointer PDef;
    C3DVector               BoundingSphereOffsetFromPos;
    float                   BoundingSphereRadius;
    bool                    CachedBoundingSphere;
    CTCHitLocations(CThing* parent);
};

CTCHitLocations::CTCHitLocations(CThing* parent)
    : CTCBase(parent), PDefaultHitLocation(0), BoundingSphereOffsetFromPos(0.0f, 0.0f, 0.0f),
      BoundingSphereRadius(0.0f), CachedBoundingSphere(false)
{
}

int main()
{
    CThing thing;
    CTCHitLocations* h = new CTCHitLocations(&thing);
    bool ok = h->ParentThing == &thing && !h->ToBeRemoved && !h->IsInGlobalUpdateTCs
        && h->HitLocations.m_pFirst == 0 && h->HitLocations.m_pLast == 0 && h->HitLocations.m_pEnd == 0
        && h->PDefaultHitLocation == 0 && h->PDef.Object == 0
        && h->BoundingSphereOffsetFromPos.X == 0.0f && h->BoundingSphereOffsetFromPos.Y == 0.0f
        && h->BoundingSphereOffsetFromPos.Z == 0.0f && h->BoundingSphereRadius == 0.0f
        && !h->CachedBoundingSphere;
    delete h;
    if (!ok) { std::printf("004e75d9_TEST FAIL\n"); return 1; }
    std::printf("004e75d9_TEST PASS\n");
    return 0;
}
