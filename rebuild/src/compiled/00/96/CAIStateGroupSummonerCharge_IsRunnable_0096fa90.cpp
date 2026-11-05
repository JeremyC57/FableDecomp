// CAIStateGroup_SummonerCharge::IsRunnable() -- once the next charge frame is reached and the summoner holds a live thing in its primary slot, charge a live enemy that lies (2D) between the def's near and far charge ranges and is either running away from combat (stealth timer >= 0) or a creature the summoner is fighting that the combat manager says is chargeable; latch it as the target
// Leaf: IsRunnable
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096FA90, length 451 bytes (plain `ret`, virtual slot 11).
// Callees: CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (evaluated for the static call), CWorld::GetWorldFrame @ 0x0049D870
//          (static), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: 0x46 CTCCarrying = bit 6 of +0x28, 8 the
//          enemy source = bit 8 of +0x20, 0x36 CTCStealth = bit 22 of +0x24), CTCCarrying::GetThingInPrimarySlot
//          @ 0x006A1440, the enemy source's virtual slot 29 (+0x74), CThing::GetPos @ 0x004C73D0 (x2),
//          CThingCreatureBase::GetCombatManager @ 0x00661ED0, CCombatManager::IsCreatureFightingThing @ 0x006E9310 /
//          CheckSummonerChargeable @ 0x006EA050, CIntelligentPointer<CThing>(CThing*) @ 0x004A96F0, CIntelligentPointer::
//          operator= @ 0x004A9760, ~CBaseIntelligentPointer @ 0x00A01C10.
// Layout: Ego_r.pdb CAIStateGroup_SummonerCharge (PTarget +0x14, PDef +0x1c, NextFrameToCharge +0x20) over the landed
//          CAIStateGroupBase model (the generated header flattens the vptr); CSummonerDef ChargeRangeNear +0x3c /
//          ChargeRangeFar +0x40 (PDB); CTCStealth RunningAwayFromCombatTimer +0x30 (PDB); CThing ThingTypeFlags +0x6c
//          (retail, 0x10 below the PDB).
// Shape: the first three tests are early returns (the 0x46 one before esi is pushed, the weapon one through an inline
//          IS_THING_ALIVE, so both get their own epilogues); the enemy-source lookup is the ternary GetInterfaceX (the
//          component lands in ecx for the virtual call), the carrying/stealth lookups the plain out-parameter form; the
//          two charge reasons are SEPARATE `if`s each ending in `PTarget = enemy; return true;` (VC merges the tails) --
//          one `||` condition or a bool flag rotates eax/edx in the combat-manager block; the target assignment is the
//          implicit CThing* -> CIntelligentPointer conversion (the temporary re-addressed with `lea` for operator=).
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
#include "engine/C3DVector.h"
class C2DVector { public:
    float x, y;
    C2DVector(float ax, float ay) : x(ax), y(ay) {}
    float GetLengthSquared() const { return x * x + y * y; }
};

inline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing != 0 && !thing->IsDead())
        return true;
    return false;
}
class CCombatManager;
class CThingCreatureBase : public CThing { public:
    CCombatManager& GetCombatManager();         // 0x00661ED0
};
class CCombatManager { public:
    bool IsCreatureFightingThing(const CThingCreatureBase& creature, const CThing* thing) const;   // 0x006E9310
    bool CheckSummonerChargeable(CThing* summoner);   // 0x006EA050
};
class CTCCarrying { public:                            // interface 0x46
    CThing* GetThingInPrimarySlot() const;            // 0x006A1440
};
class CTCStealth { public:                             // interface 0x36
    unsigned char _pad[0x30];
    long RunningAwayFromCombatTimer;            // +0x30
};
struct CTCEnemySource {                         // interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetEnemy();                 // slot 29 (+0x74)
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};
template <class T> class CIntelligentPointer;
template <> class CIntelligentPointer<CThing> : public CBaseIntelligentPointer { public:
    CIntelligentPointer(CThing* thing);    // 0x004A96F0
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760
};
class CSummonerDef { public:
    unsigned char _pad[0x3c];
    float ChargeRangeNear;                      // +0x3c
    float ChargeRangeFar;                       // +0x40
};
class CWorld { public:
    static long __fastcall GetFrame();                // 0x0049D870
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0
};
struct CAIStateGroup_SummonerCharge : CAIStateGroupBase {
    CIntelligentPointer<CThing> PTarget;           // +0x14
    const CSummonerDef* PDef;                   // +0x1c CDefPointer<const CSummonerDef>
    long NextFrameToCharge;                     // +0x20
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    virtual bool IsRunnable();
};

bool CAIStateGroup_SummonerCharge::IsRunnable()
{
    if (GetPWorld()->GetFrame() < NextFrameToCharge)
        return false;
    CTCCarrying* carrying;
    if (!GetCreature()->GetTC(TCI_CARRYING, &carrying))
        return false;
    CThing* weapon = carrying->GetThingInPrimarySlot();
    if (!IS_THING_ALIVE(weapon))
        return false;
    CTCEnemySource* source;
    if (GetCreature()->GetTC(TCI_TARGETING, &source)) {
        CThing* enemy = source->GetEnemy();
        if (enemy && !enemy->IsDead()) {
            const C3DVector& from = GetCreature()->GetPos();
            const C3DVector& to = enemy->GetPos();
            float distanceSquared = C2DVector(to.X - from.X, to.Y - from.Y).GetLengthSquared();
            float nearRange = PDef->ChargeRangeNear;
            if (distanceSquared > nearRange * nearRange) {
                float farRange = PDef->ChargeRangeFar;
                if (distanceSquared < farRange * farRange) {
                    CTCStealth* stealth;
                    if (enemy->GetTC(TCI_STEALTH, &stealth) && stealth->RunningAwayFromCombatTimer >= 0) {
                        PTarget = enemy;
                        return true;
                    }
                    if (enemy->GetTypeFlags() & 8) {
                        CCombatManager* combat = &GetCreature()->GetCombatManager();
                        if (combat->IsCreatureFightingThing(*GetCreature(), enemy) && combat->CheckSummonerChargeable(enemy)) {
                            PTarget = enemy;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}