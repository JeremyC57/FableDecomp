// CThingAICreature::WorldUpdateBulletTime() -- bullet-time world update of an AI creature: update its action, run the CThing bullet-time update, then update the animation with the creature's distance from the camera
// Leaf: WorldUpdateBulletTime
// Grade: RELOCATION_MATCH -- GetLength accumulates `s = Z*Z; s += Y*Y; s += X*X` (retail x87 order z, y, x); the single-expression sum is reassociated to z, x, y whatever the written order
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082FD90, length 108 bytes. Catalog label "CTCLook::UpdateHeadPos" is propagated: the
// address is slot 20 of CThingAICreature's vtable 0x0127293C (rtti-vtables.tsv), the CThing slot of WorldUpdateBulletTime (0x004C8BA0).
// Callees: CThingCreatureBase::UpdateAction @ 0x006643C0, CThing::WorldUpdateBulletTime @ 0x004C8BA0 (direct base call), the camera
// accessor @ 0x004C7AA0 (`mov eax,[0x013B8A1C] ; mov eax,[eax+24h]`: the world's current camera), the camera's virtual slot 8 (+0x20,
// position returned by value through a hidden pointer), CThing::GetPosition @ 0x006A4D00, CThingCreatureBase::UpdateAnimation(float)
// @ 0x00665860.
// Shape: camera and position are named locals fetched in that order; the distance is `(cameraPos - pos).GetLength()` inlined.
#include <math.h>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLength() const { float s = Z * Z; s += Y * Y; s += X * X; return (float)sqrt(s); }
};
class CCamera { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual C3DVector GetPosition() const;      // slot 8 (+0x20)
};
class CThingPhysical { public:  // retail: CThingPhysical derives from CThing; modelled as its base here (empty insertion, layout unchanged) so CThing-typed callers resolve the call
    virtual void v00();  // moved from CThing: the inserted base owns the vptr (vtable order unchanged)
    const C3DVector& PeekPos() const;       // 0x006A4D00
};
class CThing : public CThingPhysical { public:
    CCamera* GetCamera() const;                 // 0x004C7AA0
    void WorldUpdateBulletTime();               // 0x004C8BA0 (slot 20)
};
class CThingCreatureBase : public CThing { public:
    void UpdateAction();                        // 0x006643C0
    void UpdateAnimation(float distanceToCamera);   // 0x00665860
};
class CThingAICreature : public CThingCreatureBase { public:  // vtable 0x0127293C
    void WorldUpdateBulletTime();
};

void CThingAICreature::WorldUpdateBulletTime()
{
    UpdateAction();
    CThing::WorldUpdateBulletTime();
    CCamera* camera = GetCamera();
    const C3DVector& pos = PeekPos();
    UpdateAnimation((camera->GetPosition() - pos).GetLength());
}