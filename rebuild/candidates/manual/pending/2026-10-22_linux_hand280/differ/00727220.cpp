// CApplyAdditionalImpulseIfInCombatAndTooClose::operator()(CThingCreatureBase* other) -- when the owner and the other thing may collide (both ways through their interface-0x4d filters), the feature is on and the owner creature is busy, pushable and pushable by self-avoidance, and the other thing overlaps the owner vertically and is within the combined radius plus a margin in XY: push the owner away from it (scaled by the penetration), or, when they are on the same spot, along its own velocity turned by a random angle
// Grade: DIFFER(732v732) -- structure and frame right; left: x87 operand order on the XY offset's Y (VC7.1 keeps the dy CSE copy at +0x10 and reads it first in the length and the two scalings) and the owner-creature load through ecx v eax
// Leaf: ??RCApplyAdditionalImpulseIfInCombatAndTooClose
#include <math.h>
#pragma intrinsic(fabs, sqrt)

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing;
struct CTCCollisionFilter {                     // the interface-0x4D component
    bool CanCollideWith(CThing* other);         // 0x00770DE0
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline C3DVector& operator*=(const float& s) { X *= s; Y *= s; Z *= s; return *this; }
    __forceinline C3DVector& operator/=(float s) { float inv = 1.0f / s; X *= inv; Y *= inv; Z *= inv; return *this; }
    void RotateAboutZ(float turns);             // 0x00A14040
};

class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56();
    virtual bool GetCollidesWithThings();       // slot 57 (+0xe4)
    virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetVelocity();            // slot 70 (+0x118)
    virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76();
    virtual void v77(); virtual void v78(); virtual void v79(); virtual void v80();
    virtual void AddAdditionalImpulse(const C3DVector& impulse);   // slot 81 (+0x144)
    virtual void v82();
    virtual bool IsPushableByHero();            // slot 83 (+0x14c)
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x34 - 0x08];
    float Radius;                               // +0x34
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
class CThingCreatureBase : public CThing {
public:
    bool IsPerformingAnyAction() const;         // 0x006622A0
};
namespace NCreatureMode {
bool __fastcall IsPhysicsPushableBySelfAvoidance(const CThing& thing);   // 0x0085FE60
}

struct CWorld {
    unsigned long* GetPRandomSeed();            // 0x0049D810
};
struct CMainGameComponent {
    unsigned char _pad[0x24];
    CWorld* World;                              // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
extern long __fastcall GFGetCurrentFrame();        // 0x0049D870
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

extern bool g_AdditionalImpulseEnabled;         // 0x0137574A
extern float g_AdditionalImpulseMargin;         // 0x01381198 (0.1)

__forceinline bool CanCollide(CThing* thing, CThing* other)
{
    if (thing->HasTC(0x4d))
        return ((CTCCollisionFilter*)thing->GetTC(0x4d))->CanCollideWith(other);
    return true;
}

struct CApplyAdditionalImpulseIfInCombatAndTooClose {   // 0x18 bytes
    C3DVector       Position;                   // +0x00
    float           Radius;                     // +0x0c
    CTCPhysicsBase* Owner;                      // +0x10
    CThingCreatureBase* OwnerCreature;          // +0x14
    CThingCreatureBase* GetOwnerCreature() const { return (CThingCreatureBase*)Owner->ParentThing; }
    void operator()(CThingCreatureBase* other);
};

void CApplyAdditionalImpulseIfInCombatAndTooClose::operator()(CThingCreatureBase* other)
{
    bool canCollide = true;
    CThing* owner = Owner->ParentThing;
    if (owner->HasTC(0x4d)) {
        canCollide = ((CTCCollisionFilter*)owner->GetTC(0x4d))->CanCollideWith(other);
        if (!canCollide)
            return;
    }
    if (other->HasTC(0x4d))
        canCollide = ((CTCCollisionFilter*)other->GetTC(0x4d))->CanCollideWith(Owner->ParentThing);
    if (canCollide && g_AdditionalImpulseEnabled) {
        CThingCreatureBase* creature = GetOwnerCreature();
        if (creature->IsPerformingAnyAction() && creature->PhysicsTC->IsPushableByHero()
            && NCreatureMode::IsPhysicsPushableBySelfAvoidance(*creature)) {
            const C3DVector& pos = other->GetPosition();
            float dz = pos.Z - Position.Z;
            C3DVector offset(pos.X - Position.X, pos.Y - Position.Y, 0.0f);
            float radii = other->PhysicsTC->Radius + Radius;
            if (fabs(dz) < radii) {
                float margin = g_AdditionalImpulseMargin;
                float distSq = offset.X * offset.X + offset.Y * offset.Y;
                float range = margin + radii;
                if (Owner->GetCollidesWithThings() && other->PhysicsTC->GetCollidesWithThings() && range * range > distSq) {
                    float scale = 1.0f;
                    float dist = sqrt(distSq);
                    if (dist > 0.0001f) {
                        float penetration = dist - radii;
                        if (penetration > 0.0f)
                            scale = (margin - penetration) / margin;
                        offset /= dist;
                        offset *= scale * -0.2f;
                        Owner->AddAdditionalImpulse(offset);
                    } else {
                        C3DVector velocity = Owner->GetVelocity();
                        velocity.X *= 0.1f;
                        velocity.Y *= 0.1f;
                        velocity.Z *= 0.1f;
                        CWorld* world = g_pMainGameComponent->World;
                        float sign = (GFGetCurrentFrame() % 2 == 0) ? -1.0f : 1.0f;
                        float angle = GFFloatRandom(45.0f, world->GetPRandomSeed()) * sign;
                        float turns = angle * (1.0f / 360.0f);
                        velocity.RotateAboutZ(turns * sign);
                        Owner->AddAdditionalImpulse(velocity);
                    }
                }
            }
        }
    }
}
