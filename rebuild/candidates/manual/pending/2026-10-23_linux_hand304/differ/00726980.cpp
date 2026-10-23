// Grade: DIFFER(609v595) -- 34 difflines: the step copy (retail stores only movement.Y early and reloads X/Z from movement onto x87 after the ftol, keeping length below them for `fld st(2) ; fmul [12.0f]`; the model copies all three words and multiplies 12.0f first) and the do-loop head, which VC aligns here (`lea esp,[esp]`) but retail leaves unaligned (the same loop-alignment residue as 007243c0). The collision loop, landscape test, single-step retry and ground update match.
// CTCPhysicsNavigator::PerformForcedMovement(const vector<CThing*>& things, const C3DVector& movement, C3DVector& position) -- move the position by the movement in steps of at most 1/12 unit, sweeping the body against every nearby thing's physics mesh (responding to collisions that should stop it, retrying once in three steps when the very first single step is blocked) and against the landscape, then update the ground interactions when the landscape was hit
// Leaf: PerformForcedMovement
// Retail: Fable.exe (TLC, VC7.1) @ 0x00726980, 595 bytes, `ret 0Ch`.
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    __forceinline float Length() const { return sqrtf(X * X + Y * Y + Z * Z); }
};
struct CPhysicsCollideResponse {
    C3DVector CollidePos;                               // +0x00
    C3DVector CollideNormal;                            // +0x0c
    float     Friction;                                 // +0x18
    CPhysicsCollideResponse() : Friction(1.0f) { }
};
struct CTCPhysicsBase;
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
};
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
};
extern float g_epsilon_0129ba3c;                        // 0x0129BA3C (1e-4f)
#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2) FILL10(3) FILL10(4)
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual bool CollidesWithLandscape();               // slot 59 (+0xec) (name descriptive)
    FILL10(6) FILL10(7) FILL10(8)
    virtual bool HasSphereMeshCollidedFromTo(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to,
                                             CPhysicsCollideResponse& response);   // slot 90 (+0x168)
    virtual void v91();
    virtual bool HasCollidedWithLandscape(const C3DVector& to, CPhysicsCollideResponse& response);   // slot 92 (+0x170)
    bool IsPhysicsMeshTypeCylinder();                   // 0x006B0170
    bool Movement_CheckVelocityAgainstPlane(const CPhysicsCollideResponse& response, C3DVector& velocity);   // 0x006AFA40
    void ModifyResponseForNearVerticalSlopes(CPhysicsCollideResponse* response);   // 0x006B0610
    void ImpulseRespondToCollision(const CPhysicsCollideResponse& response, C3DVector& velocity);   // 0x006AF7D0
    void UpdateCollisionGroundIneractions(const C3DVector& position);   // 0x006AFF00
};
struct CTCPhysicsStandard : CTCPhysicsBase {
    void UpdateVelocityForImpassability(const C3DVector& position, C3DVector& velocity);   // 0x007243C0
    bool IsCollisionWithThingToCauseResponse(CTCPhysicsBase* other);   // 0x008E5750
    void UpdateCollisionGameInteractions(CThing* thing, const CPhysicsCollideResponse& response);   // 0x00723AC0
};
struct CTCPhysicsNavigator : CTCPhysicsStandard {
    void PerformForcedMovement(const vector<CThing*>& things, const C3DVector& movement, C3DVector& position);
};

void CTCPhysicsNavigator::PerformForcedMovement(const vector<CThing*>& things, const C3DVector& movement, C3DVector& position)
{
    bool hitLandscape = false;
    if (things.empty() && !CollidesWithLandscape())
        return;
    C3DVector step = movement;
    CPhysicsCollideResponse response;
    long i = 0;
    float length = movement.Length();
    long steps = (long)(length * 12.0f);
    if (steps < 1)
        steps = 1;
    float scale = 1.0f / steps;
    step.X *= scale;
    step.Y *= scale;
    step.Z *= scale;
    if (!(length > g_epsilon_0129ba3c))
        return;
    do {
        UpdateVelocityForImpassability(position, step);
        C3DVector newPos = position + step;
        bool blocked = false;
        for (CThing** it = things.begin(); it != things.end(); ++it) {
            CThing* thing = *it;
            CTCPhysicsBase* other = thing->PhysicsTC;
            if (HasSphereMeshCollidedFromTo(*other, position, newPos, response)) {
                if (IsCollisionWithThingToCauseResponse(other)) {
                    if (other->IsPhysicsMeshTypeCylinder()) {
                        blocked = Movement_CheckVelocityAgainstPlane(response, step);
                    } else {
                        blocked = true;
                        ModifyResponseForNearVerticalSlopes(&response);
                        ImpulseRespondToCollision(response, step);
                    }
                }
                UpdateCollisionGameInteractions(thing, response);
            }
        }
        if (CollidesWithLandscape() && HasCollidedWithLandscape(newPos, response)) {
            newPos = response.CollidePos;
            ImpulseRespondToCollision(response, step);
            hitLandscape = true;
        }
        ++i;
        if (blocked) {
            if (i == 1 && steps == 1) {
                steps = 3;
                continue;
            }
        } else {
            position = newPos;
        }
    } while (i < steps);
    if (hitLandscape)
        UpdateCollisionGroundIneractions(position);
}
