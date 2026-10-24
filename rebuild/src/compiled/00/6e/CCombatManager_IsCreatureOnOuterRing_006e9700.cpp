// CCombatManager::IsCreatureOnOuterRing(const CThingCreatureBase& creature) const -- false for a creature that is not combat-eligible; true when it is not using its victim's combat wheel or holds no ring, else whether its ring is the wheel's outer ring for the victim's combat type
// Leaf: ?IsCreatureOnOuterRing@CCombatManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E9700, 145 bytes (160 with int3 padding), thiscall, `ret 4`.
// Identity: Ego_r.pdb name set `?IsCreatureOnOuterRing@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z` (the one
//   CCombatManager bool query that ends in CCombatWheel::IsCombatRingOuterRing). Shape and layout as the landed
//   CCombatManager::SetAttackerRingAsRotating 0x006E9D90. The catalog's "CCombatWheel_ValidateSequencePosition" is
//   descriptive.
// Callees: the creature combat-eligibility test @ 0x00662410, the combatant map find(const unsigned __int64&) @ 0x006F0250
//   (twice), CThingCreatureBase::GetCombatTypeDef() @ 0x00666D10, CCombatWheel::IsCombatRingOuterRing(const
//   CCombatTypeDef&, long) const @ 0x006E84A0 (landed).
// Layout: CCombatManager Combatants +0x10 (STLport map, node value +0x18); CCombatantInfo AttackerInfo +4 (Attacked +0,
//   CombatWheelRingIndex +8, UsingCombatWheel +0x1C), CombatWheel +0x40.
// Shape: one `if (eligible) { ...; return true; } return false;` block (all three callee-saved registers pushed up
//   front; an early `return false` shrink-wraps ebx and duplicates epilogues); the attacker's info is a reference
//   (`add esi,18h` at once) but the victim's entry is kept as the find's node (`lea ecx,[ebx+58h]` at the use), looked
//   up before the ring index is tested; `index < 0 || outer` returns true through the shared block. The outer-ring
//   call pushes the index, then the GetCombatTypeDef() result. The find has a same-TU noinline body (one shared key
//   slot, as 0x006E93C0).
class CCombatTypeDef;

class CThingCreatureBase {
public:
    unsigned char    _pad[0x18];
    unsigned __int64 UniqueID;                              // +0x18
    unsigned __int64 GetUID() const { return UniqueID; }
    bool IsCombatEligible() const;                          // 0x00662410
    const CCombatTypeDef* GetCombatTypeDef() const;         // 0x00666D10
};

template <class T> struct CRetailVector {                   // retail std::vector: {first, last, end_of_storage}
    T* first;
    T* last;
    T* end_of_storage;
};

struct CCombatRing;
struct CCombatWheel {                                       // CDefendingCombatantInfo::CCombatWheel
    CRetailVector<CCombatRing> CombatRings;                 // +0x00
    bool IsCombatRingOuterRing(const CCombatTypeDef& def, long index) const;   // 0x006E84A0
};

struct CAttackingCombatantInfo {
    const CThingCreatureBase* Attacked;                     // +0x00
    long AttackerNumber;                                    // +0x04
    long CombatWheelRingIndex;                              // +0x08
    bool PreferredAttackDollySet;                           // +0x0C
    long PreferredAttackDollyTimer;                         // +0x10
    float PreferredAttackDollyVector[2];                    // +0x14 C2DVector
    bool UsingCombatWheel;                                  // +0x1C
    unsigned char _rest[0x30 - 0x1d];
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
    bool IsCreatureOnOuterRing(const CThingCreatureBase& creature) const;
};

bool CCombatManager::IsCreatureOnOuterRing(const CThingCreatureBase& creature) const
{
    if (creature.IsCombatEligible()) {
        const CCombatantInfo& info = GetCombatantInfo(creature);
        if (!info.AttackerInfo.UsingCombatWheel)
            return true;
        const CThingCreatureBase* attacked = info.AttackerInfo.Attacked;
        const CRetailRbMap_UID_CombatantInfo::node* attackedEntry = Combatants.find(attacked->GetUID());
        long ringIndex = info.AttackerInfo.CombatWheelRingIndex;
        if (ringIndex < 0
            || attackedEntry->value.CombatWheel.IsCombatRingOuterRing(*attacked->GetCombatTypeDef(), ringIndex))
            return true;
    }
    return false;
}