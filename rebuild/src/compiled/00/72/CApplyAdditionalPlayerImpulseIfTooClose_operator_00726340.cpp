// CApplyAdditionalPlayerImpulseIfTooClose::operator()(CThingCreatureBase* other) -- when the other thing overlaps the player vertically, is within the combined radius plus 0.1 in XY, both collide with things, it is pushable by the hero and the player's collision filter lets them collide: push it away from the player (harder the deeper the overlap) or, when on the same spot, along the player's velocity, turned by a random angle; a moving player also tells a creature it is being pushed
// Grade: RELOCATION_MATCH (retry A 11a) -- the unit scaling is written per component (X = X / dist ...), not as offset *= 1/dist, and GetXYLengthSquared is sequenced (s = X*X; s += Y*Y)
// Leaf: ??RCApplyAdditionalPlayerImpulseIfTooClose
#include <math.h>
#pragma intrinsic(fabs, sqrt)

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0 (the const copy)
};
class CThing;
class CTCSolidToThing { public:                     // the interface-0x4D component
public:
    bool IsSolidToThing(const CThing*) const;   // 0x00770DE0
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline C3DVector& operator*=(float s) { X = s * X; Y = s * Y; Z = s * Z; return *this; }
    float GetXYLengthSquared() const { float s = X * X; s += Y * Y; return s; }
public:
    void RotateXY(float);   // 0x00A14040
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
    __forceinline unsigned long HasTC(long interfaceType) const
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
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class C2DVector { public:
    float X, Y;
    float GetLengthSquared() const { return X * X + Y * Y; }
};
class CThingCreatureBase : public CThing {
public:
};
// Layout: retail CThing type byte +0x10 (1 = creature); the player's flag byte +0x6c (bit 3) and 2D move vector +0x10c.
struct CThingView {
    unsigned char _pad00[0x10];
    unsigned char Type;                         // +0x10
    unsigned char _pad11[0x6c - 0x11];
    unsigned char Flags6c;                      // +0x6c
    unsigned char _pad6d[0x10c - 0x6d];
    C2DVector MoveVector;                       // +0x10c
};
class CThingAICreature : public CThingCreatureBase {
public:
    void OnHeroPush(CThing* pusher, float distance, const C3DVector& direction);   // 0x008328D0
};
namespace NCreatureMode {
bool __fastcall IsPhysicsPushableByHero(const CThing& thing);   // 0x0085FD70
}

class CWorld { public:
    unsigned long* GetPRandomSeed();            // 0x0049D810

public:
    static long __fastcall GetFrame();   // 0x0049D870
};
class CMainGameComponent { public:
    unsigned char _pad[0x24];
    CWorld* World;                              // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
        // 0x0049D870
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0


class CApplyAdditionalPlayerImpulseIfTooClose { public:   // 0x14 bytes
    C3DVector       Position;                   // +0x00
    float           Radius;                     // +0x0c
    CTCPhysicsBase* Owner;                      // +0x10
    void operator()(CThingCreatureBase* other);
};

void CApplyAdditionalPlayerImpulseIfTooClose::operator()(CThingCreatureBase* other)
{
    const C3DVector& pos = other->GetPos();
    float dz = pos.Z - Position.Z;
    C3DVector offset(pos.X - Position.X, pos.Y - Position.Y, 0.0f);
    float radii = other->PhysicsTC->Radius + Radius;
    if (fabs(dz) < radii) {
        float distSq = offset.GetXYLengthSquared();
        float range = radii + 0.1f;
        if (Owner->GetCollidesWithThings() && other->PhysicsTC->GetCollidesWithThings() && range * range > distSq
            && other->PhysicsTC->IsPushableByHero() && NCreatureMode::IsPhysicsPushableByHero(*other)) {
            CThing* owner = Owner->ParentThing;
            if (owner->HasTC(0x4d)) {
                if (!((CTCSolidToThing*)owner->GetTC(0x4d))->IsSolidToThing(other))
                    return;
            }
            float penetration = 0.0f;
            if (distSq > 0.0001f) {
                float scale = 1.0f;
                float dist = sqrt(distSq);
                penetration = dist - radii;
                if (penetration > 0.0f)
                    scale = (0.1f - penetration) * 10.0f;
                offset.X = offset.X / dist; offset.Y = offset.Y / dist; offset.Z = offset.Z / dist;
                offset *= scale * 0.2f;
            } else {
                offset = Owner->GetVelocity();
                offset *= 0.1f;
            }
            CWorld* world = g_pMainGameComponent->World;
            float sign = (CWorld::GetFrame() % 2 == 0) ? -1.0f : 1.0f;
            float angle = GFFloatRandom(45.0f, world->GetPRandomSeed()) * sign;
            float turns = angle * (1.0f / 360.0f);
            offset.RotateXY(turns * sign);
            other->PhysicsTC->AddAdditionalImpulse(offset);
            if (((CThingView*)other)->Type == 1 && (((CThingView*)Owner->ParentThing)->Flags6c & 8)) {
                CThing* player = Owner->ParentThing;
                if (((CThingView*)player)->MoveVector.GetLengthSquared() > 0.0001f)
                    ((CThingAICreature*)other)->OnHeroPush(player, 0.1f - penetration, offset);
            }
        }
    }
}