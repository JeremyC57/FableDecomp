// CCombatManager::SetAttackerProngAsInvalid(CThingCreatureBase& attacker) -- for a combat-eligible attacker that holds a ring on its victim's combat wheel, block its prong there and take it off the wheel
// Leaf: ?SetAttackerProngAsInvalid@CCombatManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E9D10, 115 bytes (128 with int3 padding), thiscall, `ret 4`.
// Identity: Ego_r.pdb name set `?SetAttackerProngAsInvalid@CCombatManager@@QAEXAAVCThingCreatureBase@@@Z`; the body is a
//   guarded call of CCombatWheel::SetAttackerProngAsBlocked 0x006E9200 (this chunk) on the victim's wheel. The catalog's
//   "CombatBlend_SetupDefenderBlock" is descriptive.
// Callees: the creature combat-eligibility test @ 0x00662410, the combatant map find(const unsigned __int64&) @ 0x006F0250
//   (twice), CDefendingCombatantInfo::CCombatWheel::SetAttackerProngAsBlocked(const CThingCreatureBase&,
//   CAttackingCombatantInfo&) @ 0x006E9200.
// Layout: as the landed 0x006E9D90: CCombatManager Combatants +0x10 (node value +0x18); CCombatantInfo AttackerInfo +4
//   (Attacked +0, CombatWheelRingIndex +8), CombatWheel +0x40.
// Shape: as 0x006E93C0: the victim's UID key is taken before the ring index is tested (one shared key slot through the
//   same-TU noinline find); the call's two arguments are pushed before the victim's find runs.
class CThingCreatureBase {
public:
    unsigned char    _pad[0x18];
    unsigned __int64 UniqueID;                              // +0x18
    unsigned __int64 GetUID() const { return UniqueID; }
public:
    bool IsInMeleeAsAttacker() const;   // 0x00662410
};

template <class T> struct CRetailVector {                   // retail std::vector: {first, last, end_of_storage}
    T* first;
    T* last;
    T* end_of_storage;
};

class CAttackingCombatantInfo {
public:
    const CThingCreatureBase* Attacked;                     // +0x00
    long AttackerNumber;                                    // +0x04
    long CombatWheelRingIndex;                              // +0x08
    unsigned char _rest[0x30 - 0x0c];
};

struct CCombatRing;
struct CCombatWheel {                                       // CDefendingCombatantInfo::CCombatWheel
    CRetailVector<CCombatRing> CombatRings;                 // +0x00
    void SetAttackerProngAsBlocked(const CThingCreatureBase& attacker, CAttackingCombatantInfo& attackerInfo);   // 0x006E9200
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
    node* find(const unsigned __int64& key);                // 0x006F0250
};
#pragma auto_inline(off)
// Same-TU body of the STLport map::find instantiation 0x006F0250 (as the landed 0x006E9D90 / 0x006EA770).
__declspec(noinline) CRetailRbMap_UID_CombatantInfo::node* CRetailRbMap_UID_CombatantInfo::find(const unsigned __int64& key)
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
    CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature)
    {
        return Combatants.find(creature.GetUID())->value;
    }
    void SetAttackerProngAsInvalid(CThingCreatureBase& attacker);
};

void CCombatManager::SetAttackerProngAsInvalid(CThingCreatureBase& attacker)
{
    if (attacker.IsInMeleeAsAttacker()) {
        CCombatantInfo& info = GetCombatantInfo(attacker);
        unsigned __int64 attackedUID = info.AttackerInfo.Attacked->GetUID();
        if (info.AttackerInfo.CombatWheelRingIndex >= 0)
            Combatants.find(attackedUID)->value.CombatWheel.SetAttackerProngAsBlocked(attacker, info.AttackerInfo);
    }
}