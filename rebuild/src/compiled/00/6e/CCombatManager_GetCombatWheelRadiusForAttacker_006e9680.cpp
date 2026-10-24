// CCombatManager::GetCombatWheelRadiusForAttacker(const CThingCreatureBase& attacker) const -- the full radius (all rings) of the combat wheel around the creature this attacker is attacking
// Leaf: ?GetCombatWheelRadiusForAttacker@CCombatManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E9680, 113 bytes (128 with int3 padding), thiscall, `ret 4`, float in st0.
// Identity: Ego_r.pdb name set `?GetCombatWheelRadiusForAttacker@CCombatManager@@QBEMABVCThingCreatureBase@@@Z`; the callee
//   0x006E8850 is CCombatWheel::GetRingRadius(long) const (landed as "SumPrefixWeights": the sum of RingThickness +0xC over
//   the rings up to the index), called on the victim's wheel with size() - 1 = the inline GetWheelRadius(). Single
//   caller 0x0093DBA1. The catalog's "Combat_ProximityCalculation" is descriptive.
// Callees: the combatant map find(const unsigned __int64&) @ 0x006F0250 (twice), CCombatWheel::GetRingRadius(long) const
//   @ 0x006E8850.
// Layout: as the landed 0x006E9D90: CCombatManager Combatants +0x10 (STLport map, node value +0x18); CCombatantInfo
//   AttackerInfo +4 (Attacked +0), CombatWheel +0x40 (vector<CCombatRing>, 28-byte rings).
// Shape: two inline GetCombatantInfo lookups whose GetUID() key temporaries share one frame slot (the find has a same-TU
//   noinline body, as 0x006E93C0); GetWheelRadius() = GetRingRadius(size() - 1) with the signed /28 size (`lea edx,
//   [edx+eax-1]`).
class CThingCreatureBase {
public:
    unsigned char    _pad[0x18];
    unsigned __int64 UniqueID;                              // +0x18
    unsigned __int64 GetUID() const { return UniqueID; }
};

template <class T> struct CRetailVector {                   // retail std::vector: {first, last, end_of_storage}
    T* first;
    T* last;
    T* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
};

struct CCombatRing {                                        // CDefendingCombatantInfo::CCombatRing, 28 bytes in retail
    float RotationAngle;                                    // +0x00
    float DeltaRotation;                                    // +0x04
    long  Index;                                            // +0x08
    float RingThickness;                                    // +0x0C
    unsigned char Prongs[0x0c];                             // +0x10 vector<CProng>
};

struct CCombatWheel {                                       // CDefendingCombatantInfo::CCombatWheel
    CRetailVector<CCombatRing> CombatRings;                 // +0x00
    float GetRingRadius(long index) const;                  // 0x006E8850
    float GetWheelRadius() const { return GetRingRadius(CombatRings.size() - 1); }
};

struct CAttackingCombatantInfo {
    const CThingCreatureBase* Attacked;                     // +0x00
    long AttackerNumber;                                    // +0x04
    long CombatWheelRingIndex;                              // +0x08
    unsigned char _rest[0x30 - 0x0c];
};

struct CCombatantInfo {                                     // retail layout (Ego_r.pdb names; 12-byte vectors/maps)
    const CThingCreatureBase* Parent;                       // +0x00
    CAttackingCombatantInfo AttackerInfo;                   // +0x04
    unsigned char Attackers[0x0c];                          // +0x34
    CCombatWheel CombatWheel;                               // +0x40
};

struct CRetailRbMap_UID_CombatantInfo {                     // STLport map<unsigned __int64, CCombatantInfo>
    struct node {
        long  colour; node* parent; node* left; node* right;
        unsigned __int64 key;                               // +0x10
        CCombatantInfo value;                               // +0x18
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key) const;          // 0x006F0250
};
#pragma auto_inline(off)
// Same-TU body of the STLport map::find instantiation 0x006F0250 (as the landed 0x006E9D90 / 0x006EA770).
__declspec(noinline) CRetailRbMap_UID_CombatantInfo::node* CRetailRbMap_UID_CombatantInfo::find(const unsigned __int64& key) const
{
    node* y = header;
    node* x = header->parent;
    while (x != 0) {
        if (!(x->key < key)) {
            y = x;
            x = x->left;
        } else
            x = x->right;
    }
    if (y == header || key < y->key)
        return header;
    return y;
}
#pragma auto_inline(on)

class CCombatManager {
public:
    virtual void v00();
    unsigned char _sequences[0xc];                          // +0x04
    CRetailRbMap_UID_CombatantInfo Combatants;              // +0x10
    const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) const
    {
        return Combatants.find(creature.GetUID())->value;
    }
    float GetCombatWheelRadiusForAttacker(const CThingCreatureBase& attacker) const;
};

float CCombatManager::GetCombatWheelRadiusForAttacker(const CThingCreatureBase& attacker) const
{
    const CCombatantInfo& info = GetCombatantInfo(attacker);
    return GetCombatantInfo(*info.AttackerInfo.Attacked).CombatWheel.GetWheelRadius();
}