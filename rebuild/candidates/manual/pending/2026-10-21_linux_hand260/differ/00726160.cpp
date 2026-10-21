// Grade: DIFFER(473v469) -- frame now matches; remaining: 2nd Contains loads ecx=component before reloading the owner thing, Y*Y/X*X operand slots, push vs GetVelocity temp slot swap, sign=-1 store scheduled before the modulo
// CApplyAdditionalImpulseIfInCombatAndTooClose::operator()(CThing*) -- push the owner away from an overlapping combat opponent
// Layout: functor holds only the owner's physics object; CThing modelled locally like the landed 0x008337B0 (no header names these offsets).
#include <math.h>

struct CTCBase;
struct CThing;
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    void RotateInPlace(float angle);                 // 0xA14040
};
struct C2DVector { float X, Y; C2DVector(float x, float y) : X(x), Y(y) {} };
struct CRandomSeed { float RandFloat(float range); };   // 0x4978A0
struct CWorld {
    CRandomSeed* GetPRandomSeed();                   // 0x49D810
    long GetWorldFrame();                            // 0x49D870
};
struct CMainGameComponent { char _pad[0x24]; CWorld* PWorld; };
extern CMainGameComponent* g_PMainGameComponent;     // 0x13B86A0

struct CTCCombatTargets { bool Contains(CThing* thing); };   // 0x770DE0

struct CPhysicsObject {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual bool IsCollidable();                                  // slot 57 (+0xE4)
    virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual C3DVector GetVelocity();                              // slot 70 (+0x118)
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080();
    virtual void ApplyImpulse(const C3DVector& impulse);         // slot 81 (+0x144)
    virtual void v082();
    virtual bool IsStatic();                                      // slot 83 (+0x14C)
    CThing* PThing;          // +0x04
    char _pad08[0xc - 0x8];
    C3DVector Position;      // +0x0C
    char _pad18[0x34 - 0x18];
    float Radius;            // +0x34
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

struct CThing {
    void*               __vftable;                   // +0x00
    unsigned char       _pad04[0x20 - 0x04];
    unsigned long       UsingComponents[9];          // +0x20 bitset<274>
    CInterfaceMap       Components;                  // +0x44
    unsigned char       _pad4c[0x60 - 0x4c];
    CPhysicsObject*     PPhysics;                    // +0x60
    const C3DVector& GetPosition() const;            // 0x4C73D0
    bool HasInterface(long interfaceType) const
    {
        return (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CApplyAdditionalImpulseIfInCombatAndTooClose
{
    CPhysicsObject* POwnerPhysics;
    void operator()(CThing* other);
};

void CApplyAdditionalImpulseIfInCombatAndTooClose::operator()(CThing* other)
{
    CTCCombatTargets* targets;
    if (POwnerPhysics->PThing->GetInterface(&targets, 0x4d) && !targets->Contains(other))
        return;
    if (other->GetInterface(&targets, 0x4d) && !targets->Contains(POwnerPhysics->PThing))
        return;
    if (other->PPhysics->IsStatic())
        return;
    if (!(other->UsingComponents[0xad >> 5] & (1UL << (0xad & 31))))
        return;
    {
        float radii;
        const C3DVector& ownerPos = POwnerPhysics->Position;
        const C3DVector& otherPos = other->GetPosition();
        C2DVector delta(otherPos.X - ownerPos.X, otherPos.Y - ownerPos.Y);
        radii = POwnerPhysics->Radius + other->PPhysics->Radius;
        if (!(fabs(otherPos.Z - ownerPos.Z) < radii))
            return;
        if (!POwnerPhysics->IsCollidable())
            return;
        if (!other->PPhysics->IsCollidable())
            return;
        if (!(delta.Y * delta.Y + delta.X * delta.X < radii))
            return;
    }
    C3DVector push = -POwnerPhysics->GetVelocity();
    CWorld* world = g_PMainGameComponent->PWorld;
    float sign = -1.0f;
    if (world->GetWorldFrame() % 2)
        sign = 1.0f;
    push.RotateInPlace(world->GetPRandomSeed()->RandFloat(30.0f) * sign * (1.0f / 360.0f) * sign);
    POwnerPhysics->ApplyImpulse(push);
}
