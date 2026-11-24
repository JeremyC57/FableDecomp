// CCombatManager::PerformSequenceReset(CThingCreatureBase&) -- end the creature's running combat sequence: a running melee sequence advances its victim's attack pattern and, unless the creature already circles on a rotating ring, may (by its combat type's ring-rotation chance) set its ring turning either way at the def's speed; the sequence is stopped and the current index cleared
// Leaf: ?PerformSequenceReset@CCombatManager@@AAEXAAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EA2C0, 355 bytes (368 with padding). `ret 4`.
// Callees: the combatant map's find 0x006F0250 (twice), CCombatManager::IncrementDefenderAttackPattern 0x006E8600,
//          IsCreatureOnRotatingCombatRing 0x006E9700 (catalogued "CCombatWheel_ValidateSequencePosition"; name from
//          the Ego_r CCombatManager predicates), CThingCreatureBase::GetCombatTypeDef 0x00666D10, CDefClassBase::
//          GetSubDef<CCombatAbilityStrafeDef> 0x006F0F60 (shared body, noinline, same TU), CWorld::GetWorldSeed 0x0049D810, `__fastcall` GFROR13
//          0x00497890, SetAttackerRingAsRotating 0x006E9D90, CCombatSequenceBase::Stop 0x008B0640, the sequence's
//          virtual slot 7 (+0x1c, IsMeleeSequence), the sub-def's virtual slot 1 on release. Floats 1.0f 0x0122DED8,
//          -1.0f 0x0122DEE0.
//
// Shape: GetCombatantInfo by reference (each UniqueID a GetUID() temporary in one 8-byte slot, as the landed
// 0x006E9D90, with the same-TU find body); the victim is kept as the found NODE and `->value` is formed at its one
// use (a CCombatantInfo& local adds the +0x18 early and swaps esi/edi); the sub-def pointer is zeroed before the rotating-ring test and loaded once (ebp) after GetSubDef:
// the visible same-TU noinline GetSubDef body (writes only through the out pointer) tells VC7.1 its address does not escape; it is released
// with the def's inline Release (`dec [ebp+4] ; jne`) and a null test. The world random step is inline
// (seed * 0x24A1 + 0x24DF, GFROR13), the roll a signed long.
// Layout: CCombatManager / CCombatantInfo as the landed 0x006E9D90 and 0x006E97F0 (World +0x20, Combatants +0x10,
// Attacked +4, CurrentSequence +0x24, CombatSequences +0x28); the ring-rotation sub-def's chance +0x2c and speed
// +0x40 (descriptive names).
// Name fix (agentNF5 2026-11-10): CWorld 0x0049D810 is ?GetWorldSeed@CWorld@@QBEPAKXZ (Ego_r.pdb / FableWin.pdb; engine/CWorld.h), was GetPRandomSeed.
// Name fix (agentNF6 2026-11-10): CCombatManager::IsCreatureOnOuterRing 0x006E9700 is ?IsCreatureOnOuterRing@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z (Ego_r.pdb / FableWin.pdb), was IsCreatureOnRotatingCombatRing.
// NF170 (q4267): the sub-def is the shared CDefClassBase::GetSubDef<CCombatAbilityStrafeDef> template (Ego_r.pdb + FableWin.pdb
//   ??$GetSubDef@VCCombatAbilityStrafeDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCCombatAbilityStrafeDef@@@@@Z, retail 0x006F0F60,
//   whose body pushes the literal 'CCombatAbilityStrafeDef'), with its noinline body visible (engine/CDefClassBase_GetSubDef.h
//   FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE: the callee only writes through the out pointer). The def's chance +0x2c and speed
//   +0x40 are its CCombatAbilityStrafingData CombatAbility (+0x28) PercentageChance / RotationSpeed (Ego_r layout). Was a local
//   CCombatTypeDef / CDefPointer / CRingRotationDef stand-in with a stand-in GetSubDef body.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CCOMBATABILITYSTRAFINGDATA
#define FABLE_RETAIL_CCOMBATABILITYSTRAFEDEF
#define FABLE_RETAIL_CCOMBATABILITYSTRAFEDEF_STATGETTYPENAME
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
// Name fix (NF185, 2026-11-20, q4389): the invented CRetailRbMap_UID_CombatantInfo stand-in is replaced in place by the STLport
//   map<unsigned __int64, CCombatantInfo> (stlport/fable_map.h); its _M_find<unsigned __int64> 0x006F0250 is visible but out of line
//   (FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE), as the stand-in same-TU noinline body was; FABLE_RETAIL_STLP_TREE_CONSTFIND gives the const find body.
#define FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#include "stlport/fable_map.h"
#include "fable_compiler.h"
long __fastcall GFROR13(unsigned long);   // 0x00497890

FABLE_FORCEINLINE unsigned long WorldRandom(unsigned long& seed)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    return seed;
}

#include "engine/CCombatTypeDef.h"
#include "engine/CCombatAbilityStrafeDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
class CThingCreatureBase { public:
    unsigned char _pad[0x18];
    unsigned __int64 UniqueID;              // +0x18
    unsigned __int64 GetUID() const { return UniqueID; }
    const CCombatTypeDef& GetCombatTypeDef() const;   // 0x00666D10
};
class CCombatSequenceBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual bool IsMeleeSequence();         // slot 7 (+0x1c)
    bool Running;                           // +0x04
    void Stop();                            // 0x008B0640
};
struct CSequencePointer {                   // CCountedPointer<CCombatSequenceBase>
    CCombatSequenceBase* Object;
    void* Count;
};
template<class T> struct CRetailVector {    // retail std::vector: {first, last, end_of_storage}, 12 bytes
    T* first;
    T* last;
    T* end_of_storage;
    T& operator[](unsigned long n) const { return first[n]; }
};
class CAttackingCombatantInfo { public:            // retail 0x30 bytes
    CThingCreatureBase* Attacked;           // +0x00
    long AttackerNumber;                    // +0x04
    long CombatWheelRingIndex;              // +0x08
    bool PreferredAttackDollySet;           // +0x0c
    long PreferredAttackDollyTimer;         // +0x10
    float PreferredAttackDollyVector[2];    // +0x14
    bool UsingCombatWheel;                  // +0x1c
    long CurrentSequence;                   // +0x20
    CRetailVector<CSequencePointer> CombatSequences;   // +0x24
};
struct CCombatantInfo {                     // retail layout (Ego_r.pdb names; 12-byte vectors/maps)
    CThingCreatureBase* Parent;             // +0x00
    CAttackingCombatantInfo AttackerInfo;   // +0x04
    unsigned char DefenderInfo[0x3c];       // +0x34
};
class CWorld { public:
    unsigned long* GetWorldSeed() const;        // 0x0049D810
};
class CCombatManager { public:
    virtual void v00();
    unsigned char _sequences[0xc];          // +0x04 RegisteredCombatSequences
    std::map<unsigned __int64, CCombatantInfo> Combatants;  // +0x10
    bool    CombatStrafeTurningEnabled;     // +0x1c
    CWorld* World;                          // +0x20
    CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature)
    {
        return Combatants.find(creature.GetUID())->second;
    }
    private:
    void IncrementDefenderAttackPattern(CCombatantInfo& info);
    public:
             // 0x006E8600
    bool IsCreatureOnOuterRing(const CThingCreatureBase& creature) const;   // 0x006E9700
    void SetAttackerRingAsRotating(const CThingCreatureBase& attacker, float degreesPerSecond);   // 0x006E9D90
    private: void PerformSequenceReset(CThingCreatureBase& creature);   // NF308: AAE (private), Ego_r + FableWin
};

void CCombatManager::PerformSequenceReset(CThingCreatureBase& creature)
{
    CCombatantInfo& info = GetCombatantInfo(creature);
    std::map<unsigned __int64, CCombatantInfo>::iterator  victim = Combatants.find(info.AttackerInfo.Attacked->GetUID());
    if (info.AttackerInfo.CurrentSequence >= 0) {
        CCombatSequenceBase* sequence = info.AttackerInfo.CombatSequences[info.AttackerInfo.CurrentSequence].Object;
        if (sequence->Running) {
            if (sequence->IsMeleeSequence()) {
                IncrementDefenderAttackPattern(victim->second);
                CDefPointer<const CCombatAbilityStrafeDef> rotation;
                if (!IsCreatureOnOuterRing(creature)) {
                    if (creature.GetCombatTypeDef().GetSubDef(&rotation)) {
                        long roll = WorldRandom(*World->GetWorldSeed()) % 100;
                        if (roll <= rotation->CombatAbility.PercentageChance) {
                            float speed = rotation->CombatAbility.RotationSpeed;
                            float direction = (WorldRandom(*World->GetWorldSeed()) & 1) == 0 ? 1.0f : -1.0f;
                            SetAttackerRingAsRotating(creature, direction * speed);
                        }
                    }
                }
            }
            sequence->Stop();
        }
        info.AttackerInfo.CurrentSequence = -1;
    }
}