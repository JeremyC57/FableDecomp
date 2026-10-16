// CCreatureAction_DragonFlight::PerformAction() -- with a target, fetch the owner's ability def and fire its shot at the target (accuracy from the owner's shot-accuracy percentage)
// Grade: DIFFER(219v212) -- retail caches the ability def in ebx right after GetSubDef (index read and release through ebx, def slot reused: 8-byte frame), ours re-reads the def slot (0xc frame); the PhysicsTC load / ecx order also differs | tried: function-scope CDefPointer + inline owner conversion | next: lane gap: CDefPointer kept in a register across calls (GOTCHAS wave 25 chunk 163)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F3020, length 212 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CDefClassBase::GetSubDef<CAbilityDef>(CDefPointer*) 0x0042B1A6 (on the
// owner's def-class base at +0x70), CThingCreatureBase::GetShotAccuracyPercentage 0x00663B10, CreateShot(long, CThing*, CThing*,
// CThing*, float, float, float, CDefPointer<CMeleeCombatAbilityDef>*, C3DVector*, CRightHandedSet*) 0x007C0AD0 (`__fastcall`),
// the defs' virtual slot 1 on the last release.
// Shape: the owner argument goes through the intelligent pointer's inline conversion (fetched before the stack arguments);
// the melee def pointer releases through the def's inline ReleaseTemporary (ref-count base at +4).
struct CThing;
struct C3DVector;
struct CRightHandedSet;

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                      // slot 1
};
struct CAbilityDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0xa0 - 0x08];
    long ShotDefIndex;                           // +0xa0
};
struct CMeleeCombatAbilityDef : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointerAbility {                      // CDefPointer<const CAbilityDef>
    CAbilityDef* m_p;
    CDefPointerAbility() : m_p(0) { }
    ~CDefPointerAbility() { if (m_p && --m_p->m_count == 0) m_p->Destroy(); }
    CAbilityDef* operator->() const { return m_p; }
};
struct CDefPointerMelee {                        // CDefPointer<const CMeleeCombatAbilityDef>
    CMeleeCombatAbilityDef* m_p;
    CDefPointerMelee() : m_p(0) { }
    ~CDefPointerMelee() { if (m_p) m_p->ReleaseTemporary(); }
};

struct CDefClassBase {
    bool GetSubDef(CDefPointerAbility* out);     // 0x0042B1A6 (GetSubDef<CAbilityDef>)
};

struct CThing {
    unsigned char _pad00[0x70];
    CDefClassBase* PhysicsTC;                    // +0x70 (the def-class base queried for the sub def)
    long GetShotAccuracyPercentage();            // 0x00663B10 (CThingCreatureBase)
};
CThing* __fastcall CreateShot(long shotDef, CThing* owner, CThing* parent, CThing* target, float accuracy, float a, float b,
                              CDefPointerMelee* meleeDef, C3DVector* offset, CRightHandedSet* hands);   // 0x007C0AD0

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                             // +0x04
    CThing* GetPItem() const;                    // 0x00A01B50
    operator CThing*() const { return GetPItem(); }
};

class CCombatActionBase {
public:
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;           // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;             // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
};

class CCreatureAction_DragonFlight : public CCombatActionBase {
public:
    bool  FaceTarget;                            // +0x114 (PDB +0x12c)
    float TurnRate;                              // +0x118 (PDB +0x130)
    virtual void PerformAction();
};

void CCreatureAction_DragonFlight::PerformAction()
{
    if (PTarget.GetPItem()) {
        CDefPointerAbility ability;
        if (PCreature.GetPItem()->PhysicsTC->GetSubDef(&ability)) {
            long shotDef = ability->ShotDefIndex;
            long accuracy = PCreature.GetPItem()->GetShotAccuracyPercentage();
            CDefPointerMelee melee;
            CreateShot(shotDef, PCreature, 0, PTarget.GetPItem(), (float)accuracy, 1.0f, 1.0f, &melee, 0, 0);
        }
    }
}
