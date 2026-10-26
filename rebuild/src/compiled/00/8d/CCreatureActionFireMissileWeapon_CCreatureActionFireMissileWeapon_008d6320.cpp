// CCreatureAction_FireMissileWeapon::CCreatureAction_FireMissileWeapon(const CCreatureAction_FireMissileWeapon&) -- copy constructor: base copy, target pointer re-pointed, shot parameters copied, special-shot def pointer shared (reference count bumped)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D6320, length 146 bytes, `ret 4`. Vtable 0x01285424.
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) @ 0x0062EBC0,
//          CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (CIntelligentPointer<const CThing> vtable 0x01237F1C).
// Shape: the DowseFire copy shape (0x0084C540) for PTarget, then the float/bool/long members copied through integer
//   registers in declaration order, and the CDefPointer's inline copy constructor: the def pointer is loaded into
//   edi, stored, null-tested and its reference count (+4) incremented in place.
// Layout: Ego_r.pdb puts PTarget at +0xb4 and the shot fields at +0xbc..+0xd0 (PDB CCreatureActionBase 0xb4 bytes,
//   retail 0xa8): +0xa8..+0xc4 here; engine/CCreatureAction_FireMissileWeapon.h is the PDB build and is not used.
class CBaseObject;
class CThing;

struct CBaseIntelligentPointer {            // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                   // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    CBaseObject* GetPItem() const;          // 0x00A01B50
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:   // CIntelligentPointer<const CThing>: vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CDefCounted {                        // a game definition: vtable + reference count
    virtual void v00();
    mutable long m_refCount;                // +4 (mutable: const defs are shared too)
};

template <class T>
class CDefPointer { public:                        // shares a def and bumps its count
    T* m_pDef;
    CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) ++m_pDef->m_refCount; }
};

class CMeleeCombatAbilityDef : public CDefCounted { public: };

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(const CCreatureActionBase& other);     // 0x0062EBC0
};

class CCreatureAction_FireMissileWeapon : public CCreatureActionBase { public:   // vtable 0x01285424
    CIntelligentPointer<const CThing> PTarget;              // +0xa8 (PDB +0xb4)
    float ShotAccuracyPercentage;                           // +0xb0
    float ShotDamageMultiplier;                             // +0xb4
    float ShotBuildupTime;                                  // +0xb8
    bool Decapitate;                                        // +0xbc
    long DelayTimer;                                        // +0xc0
    CDefPointer<const CMeleeCombatAbilityDef> PSpecialShotDef;   // +0xc4
    virtual void v00();
    CCreatureAction_FireMissileWeapon(const CCreatureAction_FireMissileWeapon& other);
};

CCreatureAction_FireMissileWeapon::CCreatureAction_FireMissileWeapon(const CCreatureAction_FireMissileWeapon& other)
    : CCreatureActionBase(other)
    , PTarget(other.PTarget)
    , ShotAccuracyPercentage(other.ShotAccuracyPercentage)
    , ShotDamageMultiplier(other.ShotDamageMultiplier)
    , ShotBuildupTime(other.ShotBuildupTime)
    , Decapitate(other.Decapitate)
    , DelayTimer(other.DelayTimer)
    , PSpecialShotDef(other.PSpecialShotDef)
{
}