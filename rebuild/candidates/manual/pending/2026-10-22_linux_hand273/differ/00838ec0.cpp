// CThingGameObject::GetShotHitParameters(CThing* attacker, CThing* shot, const C3DVector& position, CHitParameters& params) -- fill a projectile hit: knock-down / blockable / decapitate flags and build-up time from the shot's definition component (interface 0x77), the shot's normalised travel direction, the hitting thing and weapon, the hit position, and the shot's damage with its attribute
// Grade: DIFFER(448v448) -- same length, instruction-for-instruction except (1) the second interface-0x77 lookup key and the damage float are homed in the dead `params` argument slot [esp+0x28] where retail uses the dead `shot` slot [esp+0x20], and (2) the Position/Direction copies rotate eax/ecx/edx one step (retail loads the position pointer into ecx); tried statement order, a pointer `position`, an explicit __forceinline C3DVector::operator= (worse: shot moves to ebp)
// Leaf: GetShotHitParameters
// Callees: LowerBound @ 0x0040F020, the shot component's queries @ 0x007BED70 / 0x007BEE20 / 0x007BED80 and virtual slot 31
// (+0x7c, damage), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CHitParameters @ 0x008685F0 (hitting thing) / SetWeapon
// @ 0x00868CE0 / @ 0x00868590 (attacker) / AddDamageWithAttribute @ 0x00868CA0, the attacker's query @ 0x00815280.
// Layout: CHitParameters from Ego_r.pdb (Localised +0, Directional +1, Position +2, Direction +0xe, KnockDown +0x1a,
// Decapitate +0x1b, Blockable +0x1c, ShotBuildupTime +0x2c: the bools pack the vectors to 2-byte offsets); retail CThing
// interface bitset +0x20 (0x77 = word +0x2c bit 23), CVectorMap +0x44, PhysicsTC +0x60, dead flag +0x91 bit 0; the shot
// component's owner pointer +0x28, its def +0x38 (damage attribute +0x94), build-up time +0x48; the physics positions +0x0c/+0x18.
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
}
extern const float g_NormaliseEpsilon;          // 0x0129BA3C

struct CThing;
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CShotDef {
    unsigned char _pad00[0x94];
    long DamageAttribute;                       // +0x94
};
struct CTCShot {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual float GetDamage() const;            // slot 31 (+0x7c)
    unsigned char _pad04[0x28 - 0x04];
    CBaseIntelligentPointer Owner;              // +0x28
    unsigned char _pad30[0x38 - 0x30];
    CShotDef* PDef;                             // +0x38
    unsigned char _pad3c[0x48 - 0x3c];
    float BuildupTime;                          // +0x48
    bool IsKnockDown() const;                   // 0x007BED70
    bool IsDecapitate() const;                  // 0x007BED80
    bool IsBlockable() const;                   // 0x007BEE20
};
struct CTCPhysics {
    unsigned char _pad00[0x0c];
    C3DVector Position;                         // +0x0c
    C3DVector OldPosition;                      // +0x18
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysics* PhysicsTC;                      // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Dead;                         // +0x91 bit 0
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template <class T> __forceinline T* GetTCIfPresent(long interfaceType)
    {
        T* tc;
        if (HasTC(interfaceType))
            tc = GetTC<T>(interfaceType);
        return tc;
    }
    bool CanDecapitate();                       // 0x00815280
};

#pragma pack(push, 1)
struct CHitParameters {
    bool Localised;                             // +0x00
    bool Directional;                           // +0x01
    C3DVector Position;                         // +0x02
    C3DVector Direction;                        // +0x0e
    bool KnockDown;                             // +0x1a
    bool Decapitate;                            // +0x1b
    bool Blockable;                             // +0x1c
    unsigned char _pad1d[0x2c - 0x1d];
    float ShotBuildupTime;                      // +0x2c
    void SetThingHitting(CThing* thing);        // 0x008685F0
    void SetWeapon(CThing* weapon);             // 0x00868CE0
    void SetAttacker(CThing* attacker);         // 0x00868590
    void AddDamageWithAttribute(long attribute, float damage);   // 0x00868CA0
};
#pragma pack(pop)

class CThingGameObject {
public:
    void GetShotHitParameters(CThing* attacker, CThing* shot, const C3DVector& position, CHitParameters& params);
};

void CThingGameObject::GetShotHitParameters(CThing* attacker, CThing* shot, const C3DVector& position, CHitParameters& params)
{
    CTCShot* shotTC = shot->GetTCIfPresent<CTCShot>(0x77);
    if (shotTC->IsKnockDown())
        params.KnockDown = true;
    params.ShotBuildupTime = shotTC->BuildupTime;
    params.Blockable = shotTC->IsBlockable();
    CTCPhysics* physics = shot->PhysicsTC;
    C3DVector direction = physics->Position - physics->OldPosition;
    float length = direction.X * direction.X + (direction.Y * direction.Y + direction.Z * direction.Z);
    if (length > g_NormaliseEpsilon) {
        length = 1.0f / (float)sqrt(length);
        direction.X *= length;
        direction.Y *= length;
        direction.Z *= length;
    }
    CThing* owner = shotTC->Owner.GetPItem();
    if (owner && !(owner->Dead & 1))
        params.SetThingHitting(owner);
    params.SetWeapon(shot);
    float buildup = 0.0f;
    if (shot->HasTC(0x77))
        buildup = shot->GetTC<CTCShot>(0x77)->BuildupTime;
    params.ShotBuildupTime = buildup;
    if (attacker) {
        if (attacker->CanDecapitate() && shotTC->IsDecapitate())
            params.Decapitate = true;
        params.SetAttacker(attacker);
    }
    params.Position = position;
    params.Direction = direction;
    params.Localised = true;
    params.Directional = true;
    float damage = shotTC->GetDamage();
    params.AddDamageWithAttribute(shotTC->PDef->DamageAttribute, damage);
}
