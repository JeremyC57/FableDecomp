// CThingGameObject::GetShotHitParameters(NHitLocations::CHitLocation* hitLocation, CThing& shot, const C3DVector& position, CHitParameters& params) -- fill a projectile hit: knock-down / blockable / decapitate flags and build-up time from the shot's definition component (interface 0x77), the shot's normalised travel direction, the hitting thing and weapon, the hit position, and the shot's damage with its attribute
// Leaf: ?GetShotHitParameters@CThingGameObject@@QBEXPAVCHitLocation@NHitLocations@@AAVCThing@@ABVC3DVector@@AAVCHitParameters@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00838EC0, 448 bytes; Ego_r.pdb ?GetShotHitParameters@CThingGameObject@@QBEXPAVCHitLocation@
//   NHitLocations@@AAVCThing@@ABVC3DVector@@AAVCHitParameters@@@Z (the first argument is the hit location, not an attacker; the shot
//   is a CThing&). PDB names: NThingShot::CTCShotDefinitionBase (TCI_SHOT_DEFINITION 119) IsKnockDown / IsDecapitate / IsBlockable
//   const 0x007BED70 / 0x007BED80 / 0x007BEE20, PShooter / PShotDef / ShotBuildupTime (Ego_r +0x2c / +0x3c / +0x4c, 4 less in retail),
//   GetShotDamage (slot 31); CHitParameters SetHitLocation 0x00868590, SetThingHitting(CThing&) 0x008685F0, SetWeapon(CThing&)
//   0x00868CE0, AddDamageWithAttribute(EDamageAttribute, float) 0x00868CA0; NHitLocations::CHitLocation::IsToCauseDecapitionOnHit
//   const 0x00815280 (the bool query on the hit location that gates Decapitate; the only Ego_r CHitLocation bool of that meaning).
// Fix (chunk 759): both TCI_SHOT_DEFINITION lookups are PeekTC over the real engine/CVectorMap.h + std::bitset bits (the old local
//   map model homed the second key in the dead `params` slot: 25 diff lines -> 2), and `params.Localised = true` sits between the
//   Position and Direction copies (VC7.1 then schedules the byte store between Direction.Y and .Z as retail does).
// Layout: engine/CHitParameters.h keeps its members private (byte arrays), so the writes need a local model of the PDB layout.
// Callees: LowerBound @ 0x0040F020, the shot component's queries @ 0x007BED70 / 0x007BEE20 / 0x007BED80 and virtual slot 31
// (+0x7c, damage), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CHitParameters @ 0x008685F0 (hitting thing) / SetWeapon
// @ 0x00868CE0 / @ 0x00868590 (attacker) / AddDamageWithAttribute @ 0x00868CA0, the attacker's query @ 0x00815280.
// Layout: CHitParameters from Ego_r.pdb (Localised +0, Directional +1, Position +2, Direction +0xe, KnockDown +0x1a,
// Decapitate +0x1b, Blockable +0x1c, ShotBuildupTime +0x2c: the bools pack the vectors to 2-byte offsets); retail CThing
// interface bitset +0x20 (0x77 = word +0x2c bit 23), CVectorMap +0x44, PhysicsTC +0x60, dead flag +0x91 bit 0; the shot
// component's owner pointer +0x28, its def +0x38 (damage attribute +0x94), build-up time +0x48; the physics positions +0x0c/+0x18.
#include "fable_compiler.h"
#include <math.h>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
FABLE_FORCEINLINE C3DVector operator-(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
}
extern const float g_NormaliseEpsilon;          // 0x0129BA3C

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThing;
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
class CTCBase { public: virtual ~CTCBase(); };
enum EDamageAttribute { };
class CShotDef { public:
    unsigned char _pad00[0x94];
    EDamageAttribute DamageAttribute;           // +0x94
};
namespace NThingShot {
class CTCShotDefinitionBase : public CTCBase { public:   // TCI_SHOT_DEFINITION (119)
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual float GetShotDamage() const;        // slot 31 (+0x7c)
    unsigned char _pad04[0x28 - 0x04];
    CIntelligentPointer<CThing> PShooter;       // +0x28 (Ego_r +0x2c)
    unsigned char _pad30[0x38 - 0x30];
    CShotDef* PShotDef;                         // +0x38 (Ego_r +0x3c)
    unsigned char _pad3c[0x48 - 0x3c];
    float ShotBuildupTime;                      // +0x48 (Ego_r +0x4c)
    bool IsKnockDown() const;                   // 0x007BED70
    bool IsDecapitate() const;                  // 0x007BED80
    bool IsBlockable() const;                   // 0x007BEE20
};
}
namespace NHitLocations {
class CHitLocation { public:
    bool IsToCauseDecapitionOnHit() const;      // 0x00815280
};
}
class CTCPhysics { public:
    unsigned char _pad00[0x0c];
    C3DVector Position;                         // +0x0c
    C3DVector OldPosition;                      // +0x18
};
class CThing : public CBaseObject { public:
    unsigned char _pad08[0x20 - 0x08];
    std::bitset<274> UsingComponents;           // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char _pad54[0x60 - 0x54];
    CTCPhysics* PhysicsTC;                      // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Dead;                         // +0x91 bit 0
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template<class T> FABLE_FORCEINLINE bool PeekTC(ETCInterfaceType id, T** ptc)
    {
        if (HasTC(id)) {
            ETCInterfaceType type = id;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(type);
            if (it == map->end() || id < it->first)
                it = map->end();
            *ptc = static_cast<T*>(it->second);
            return true;
        }
        return false;
    }
};

#pragma pack(push, 1)
class CHitParameters { public:
    bool Localised;                             // +0x00
    bool Directional;                           // +0x01
    C3DVector Position;                         // +0x02
    C3DVector Direction;                        // +0x0e
    bool KnockDown;                             // +0x1a
    bool Decapitate;                            // +0x1b
    bool Blockable;                             // +0x1c
    unsigned char _pad1d[0x2c - 0x1d];
    float ShotBuildupTime;                      // +0x2c
    void SetThingHitting(CThing& thing);        // 0x008685F0
    void SetWeapon(CThing& weapon);             // 0x00868CE0
    void SetHitLocation(NHitLocations::CHitLocation* location);   // 0x00868590
    void AddDamageWithAttribute(EDamageAttribute attribute, float damage);   // 0x00868CA0
};
#pragma pack(pop)

class CThingGameObject {
public:
    void GetShotHitParameters(NHitLocations::CHitLocation* hitLocation, CThing& shot, const C3DVector& position, CHitParameters& params) const;
};

void CThingGameObject::GetShotHitParameters(NHitLocations::CHitLocation* hitLocation, CThing& shot, const C3DVector& position, CHitParameters& params) const
{
    NThingShot::CTCShotDefinitionBase* shotTC;
    shot.PeekTC(TCI_SHOT_DEFINITION, &shotTC);
    if (shotTC->IsKnockDown())
        params.KnockDown = true;
    params.ShotBuildupTime = shotTC->ShotBuildupTime;
    params.Blockable = shotTC->IsBlockable();
    CTCPhysics* physics = shot.PhysicsTC;
    C3DVector direction = physics->Position - physics->OldPosition;
    float length = direction.X * direction.X + (direction.Y * direction.Y + direction.Z * direction.Z);
    if (length > g_NormaliseEpsilon) {
        length = 1.0f / (float)sqrt(length);
        direction.X *= length;
        direction.Y *= length;
        direction.Z *= length;
    }
    CThing* owner = shotTC->PShooter.Get();
    if (owner && !(owner->Dead & 1))
        params.SetThingHitting(*owner);
    params.SetWeapon(shot);
    float buildup = 0.0f;
    NThingShot::CTCShotDefinitionBase* shotDef;
    if (shot.PeekTC(TCI_SHOT_DEFINITION, &shotDef))
        buildup = shotDef->ShotBuildupTime;
    params.ShotBuildupTime = buildup;
    if (hitLocation) {
        if (hitLocation->IsToCauseDecapitionOnHit() && shotTC->IsDecapitate())
            params.Decapitate = true;
        params.SetHitLocation(hitLocation);
    }
    params.Position = position;
    params.Localised = true;
    params.Direction = direction;
    params.Directional = true;
    float damage = shotTC->GetShotDamage();
    params.AddDamageWithAttribute(shotTC->PShotDef->DamageAttribute, damage);
}