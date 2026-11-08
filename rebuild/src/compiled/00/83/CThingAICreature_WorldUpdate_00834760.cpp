// CThingAICreature::WorldUpdate() -- per-frame update of an AI creature: the optional combat stress test, the action, the creature-base update, the movement vector from the last position, then (alive) movement and animation by distance from the camera and the weapon trails; always false
// Leaf: WorldUpdate
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLength respelled GetMagnitude (the header / FableWin.pdb method, same computation).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00834760, length 328 bytes (plain `ret`, no frame pointer; Ego_r.pdb
//   `?WorldUpdate@CThingAICreature@@UAE_NXZ`; catalog label propagated).
// Callees: (anonymous)::CombatStressTestDebug(CThingAICreature&, const CThingSearchTools&, unsigned long*) 0x00834530
//   (Ego_r.pdb `?CombatStressTestDebug@?A0x4ed42fe4@@YIXAAVCThingAICreature@@ABVCThingSearchTools@@PAK@Z`, __fastcall;
//   manifest "AnimationBlend_UpdateCombatants"); CThing::PeekThingSearchTools() const 0x004C7A60 (this TU's copy; the
//   world +0x24 -> +0x20 read); CThing::GetRandomSeed() 0x004C7AE0 (as landed); CThingCreatureBase::UpdateAction
//   0x006643C0 and WorldUpdate 0x006666A0 (direct base call); the last-position getter 0x006A4D10 and
//   CThing::GetPosition() const 0x006A4D00 (as landed); the camera accessor 0x004C7AA0 and the camera's virtual slot 8
//   (+0x20, position by value) as the landed WorldUpdateBulletTime 0x0082FD90; CThingAICreature::UpdateMovement(float)
//   0x008326A0; CThingCreatureBase::UpdateAnimation(float) 0x00665860; CVectorMap LowerBound 0x0040F020 (interface 0x4C =
//   CTCWeaponTrail, bit 12 of +0x28); CTCWeaponTrail::UpdateWeaponTrails 0x0076EFA0.
// Data: the stress-test switch byte 0x013B86D5 (descriptive name).
// Layout: retail CThingCreatureBase as the landed CThingPlayerCreature::WorldUpdate 0x006AD9D0 (byte-packed; interface
//   bitset +0x20, map +0x44, MovementVector +0x10c, the update flags +0x15d bit 1); CurrentlyFrameUpdating +0x15c bit 0
//   (Ego_r.pdb +0x188 - 0x2c, the same shift as +0x15d); CThing Dead +0x91 bit 0 (as landed).
// Shape: the stress test's arguments evaluate right to left (seed first, then the search tools into edx); the movement
//   vector and the camera distance follow the landed player / bullet-time bodies; the weapon trail comes through the
//   plain entry pick (component loaded straight into ecx).
#include "fable_compiler.h"
#include <math.h>
#include "engine/C3DVector.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CCamera { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual C3DVector GetPosition() const;                     // slot 8 (+0x20)
};
class CTCWeaponTrail {
public:
    void UpdateWeaponTrails();                                 // 0x0076EFA0
};
class CThingSearchTools;
class CThingAICreature;
namespace {
void __fastcall CombatStressTestDebug(CThingAICreature& creature, const CThingSearchTools& tools, unsigned long* seed);   // 0x00834530
}
extern bool g_CombatStressTestEnabled;                         // 0x013B86D5

#pragma pack(push, 1)
class CThingPhysical { public:
    virtual void v00();  // moved from CThingCreatureBase: the base owns the vptr (vtable order unchanged)
    const C3DVector& PeekPos() const;                          // 0x006A4D00 ?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ (Ego_r donor order Kill/PeekPos/PeekLastPos/OnCreate)
    const C3DVector& PeekLastPos() const;                      // 0x006A4D10 ?PeekLastPos@CThingPhysical@@QBEABVC3DVector@@XZ
};
class CThingCreatureBase : public CThingPhysical {
public:
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                            // +0x20
    CInterfaceMap m_interfaces;                                // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_thingFlags;                                // +0x91 bit 0: dead
    unsigned char _pad92[0x10c - 0x92];
    C3DVector     MovementVector;                              // +0x10c
    unsigned char _pad118[0x15c - 0x118];
    bool          CurrentlyFrameUpdating : 1;                  // +0x15c bit 0
    unsigned char m_updateFlags;                               // +0x15d bit 1: last position valid
    unsigned char _pad15e[0x1cc - 0x15e];
    bool IsDead() const { return (m_thingFlags & 1) != 0; }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    FABLE_FORCEINLINE CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return ((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
    }
    CCamera* GetCamera() const;                                // 0x004C7AA0
    const CThingSearchTools& PeekThingSearchTools() const;     // 0x004C7A60
    unsigned long* GetRandomSeed();                            // 0x004C7AE0
    void UpdateAction();                                       // 0x006643C0
    bool WorldUpdate();                                        // 0x006666A0
    void UpdateAnimation(float distanceToCamera);              // 0x00665860
};
class CThingAICreature : public CThingCreatureBase {
public:
    void UpdateMovement(float distanceToCamera);               // 0x008326A0
    bool WorldUpdate();
};
#pragma pack(pop)

bool CThingAICreature::WorldUpdate()
{
    if (g_CombatStressTestEnabled)
        CombatStressTestDebug(*this, PeekThingSearchTools(), GetRandomSeed());
    CurrentlyFrameUpdating = true;
    UpdateAction();
    CThingCreatureBase::WorldUpdate();
    if (m_updateFlags & 2)
        MovementVector = PeekPos() - PeekLastPos();
    m_updateFlags |= 2;
    CCamera* camera = GetCamera();
    const C3DVector& pos = PeekPos();
    float distance = (camera->GetPosition() - pos).GetMagnitude();
    if (!IsDead()) {
        UpdateMovement(distance);
        UpdateAnimation(distance);
        if (HasTC(0x4c))
            ((CTCWeaponTrail*)GetTC(0x4c))->UpdateWeaponTrails();
    }
    CurrentlyFrameUpdating = false;
    return false;
}