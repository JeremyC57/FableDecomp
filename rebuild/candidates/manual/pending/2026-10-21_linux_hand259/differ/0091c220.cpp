// CAIStateGroup_NymphAttack::IsTargetInLineOfSight(CThing* target) -- line-of-sight ray test from the nymph to the target with the target and the nymph excluded by IsNot filters
// Leaf: IsTargetInLineOfSight
// Grade: DIFFER(416v419) -- the two IsNot filter targets: retail homes the creature pointer in the dead `target` argument slot and copies target to a frame local (edi kept from entry); every C++ shape tried homes them the other way round or drops the stores
// Retail: Fable.exe (TLC, VC7.1) @ 0x0091C220; best of the probes graded under VC7.1 RTM (probe/0091c220f.cpp).
#include <stdlib.h>
#include <math.h>
struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(x + o.x, y + o.y, z + o.z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(x - o.x, y - o.y, z - o.z); }
    C3DVector operator*(float f) const { return C3DVector(x * f, y * f, z * f); }
    float Get2DLength() const { return sqrtf(x * x + y * y); }
};
struct CThing {
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CThingCreature : CThing { };
struct C2DVector {
    float x, y;
    C2DVector(float ax, float ay) : x(ax), y(ay) { }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(x + o.x, y + o.y); }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(x - o.x, y - o.y); }
    C2DVector operator*(float f) const { return C2DVector(x * f, y * f); }
    float GetLength() const { return sqrtf(x * x + y * y); }
};
struct C2DBoxI {
    long x0, y0, x1, y1;
    C2DBoxI(long ax0, long ay0, long ax1, long ay1) : x0(ax0), y0(ay0), x1(ax1), y1(ay1) { }
};
template <class T> struct vector {              // STLport vector<T> (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
};
struct CThingFilter_IsNot {
    const CThing* const& Thing;
    CThingFilter_IsNot(const CThing* const& thing) : Thing(thing) { }
};
template <class A, class B> struct CFilter_Bind2 {
    A First;
    B Second;
    CFilter_Bind2(const A& first, const B& second) : First(first), Second(second) { }
};
typedef CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsNot> CNotEitherFilter;
struct CThingSearchTools {
    void GetAllThingsInRoughArea(const C2DBoxI& box, const CNotEitherFilter& filter, vector<const CThing*>& things);   // 0x0091C9C0
};
struct CTools {
    bool IsLineOfSight(const vector<const CThing*>& ignore, const C3DVector& from, const C3DVector& to,
        C3DVector* hitPos, C3DVector* hitNormal, float* hitDistance, CThing** hitThing, long test, bool flag);   // 0x00877840
};
struct CToolsOwner {
    unsigned char _pad[0x78];
    CTools Tools;                               // +0x78
};
struct CToolsManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void GetToolsOwner(CToolsOwner** out);      // slot 10 (+0x28)
};
struct CWorld {
    unsigned char _pad[0x20];
    CThingSearchTools* PSearchTools;            // +0x20
    unsigned char _pad24[0x34 - 0x24];
    CToolsManager* PToolsManager;               // +0x34
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreature* PCreature;                  // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    CWorld* GetPWorld() const;                  // 0x008FCFC0
};
struct CAIStateGroup_NymphAttack : CAIStateGroupBase {
    bool IsTargetInLineOfSight(CThing* target);
};

bool CAIStateGroup_NymphAttack::IsTargetInLineOfSight(CThing* target)
{
    if (target == 0 || (target->m_flags91 & 1))
        return false;
    C3DVector targetPos = target->GetPosition();
    vector<const CThing*> things;
    const C3DVector& myPos = PBrain->PCreature->GetPosition();
    const CThing* const& self = PBrain->PCreature;
    const CThing* const& other = target;
    CNotEitherFilter filter((CThingFilter_IsNot(self)), CThingFilter_IsNot(other));
    C3DVector mid = (targetPos + myPos) * 0.5f;
    C3DVector half = targetPos - mid;
    long centreX = (long)mid.x;
    long centreY = (long)mid.y;
    long radius = (long)(half.Get2DLength() * 1.1f);
    C2DBoxI box(centreX - radius, centreY - radius, centreX + radius, centreY + radius);
    GetPWorld()->PSearchTools->GetAllThingsInRoughArea(box, filter, things);
    CToolsOwner* owner;
    GetPWorld()->PToolsManager->GetToolsOwner(&owner);
    targetPos.z += 1.0f;
    C3DVector hitPos;
    C3DVector hitNormal;
    float hitDistance;
    CThing* hitThing;
    return owner->Tools.IsLineOfSight(things, PBrain->PCreature->GetPosition(), targetPos, &hitPos, &hitNormal,
        &hitDistance, &hitThing, 7, false);
}
