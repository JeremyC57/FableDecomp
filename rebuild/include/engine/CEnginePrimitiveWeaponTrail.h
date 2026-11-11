#ifndef FABLE_ENGINE_CENGINEPRIMITIVEWEAPONTRAIL_H
#define FABLE_ENGINE_CENGINEPRIMITIVEWEAPONTRAIL_H
// Retail opt-in (Codex pass18): full original Ego_r/FableWin member set; native STLport layout.
#include "engine/CEnginePrimitive.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
class CEnginePrimitiveWeaponTrail : public CEnginePrimitive {
public:
    enum EWeaponTrailType { WEAPON_TRAIL_TYPE_WEAPON, WEAPON_TRAIL_TYPE_PROJECTILE, WEAPON_TRAIL_TYPE_CREATURE, WEAPON_TRAIL_TYPE_TRAP };
    class CWeaponTrailEntry {
    public:
#ifdef FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE
        // Original constructor: native0076de20, both PDB signatures and member order.
        CWeaponTrailEntry(const C3DVector& top,const C3DVector& bottom,float time):Top(top),Bottom(bottom),CreationTime(time){}
        CWeaponTrailEntry(){}
#endif
        C3DVector Top;
        C3DVector Bottom;
        float CreationTime;
    };
    std::list<CWeaponTrailEntry> Trail;
    unsigned long GraphicIndex;
    unsigned long GraphicFrame;
    EWeaponTrailType TrailType;
    float SkipPoints;
    float FadeStart;
    unsigned char Alpha;
    float CurrentTime;
    float FallOffTime;
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveWeaponTrail::CWeaponTrailEntry)==28);
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveWeaponTrail)==48);
#endif
