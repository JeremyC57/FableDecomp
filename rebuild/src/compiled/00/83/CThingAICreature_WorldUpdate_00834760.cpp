// CThingAICreature::WorldUpdate() -- per-frame update of an AI creature: the optional combat stress test, the action, the creature-base update, the movement vector from the last position, then (alive) movement and animation by distance from the camera and the weapon trails; always false
// Leaf: WorldUpdate
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
#include <math.h>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const { float s = Z * Z; s += Y * Y; s += X * X; return (float)sqrt(s); }
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                      // 0x0040F020
};
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
class CThingCreatureBase {
public:
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                            // +0x20
    CInterfaceMap m_interfaces;                                // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
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
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    const C3DVector& GetPosition() const;                      // 0x006A4D00
    const C3DVector& GetLastPosition() const;                  // 0x006A4D10
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
        MovementVector = GetPosition() - GetLastPosition();
    m_updateFlags |= 2;
    CCamera* camera = GetCamera();
    const C3DVector& pos = GetPosition();
    float distance = (camera->GetPosition() - pos).GetLength();
    if (!IsDead()) {
        UpdateMovement(distance);
        UpdateAnimation(distance);
        if (HasTC(0x4c))
            ((CTCWeaponTrail*)GetTC(0x4c))->UpdateWeaponTrails();
    }
    CurrentlyFrameUpdating = false;
    return false;
}