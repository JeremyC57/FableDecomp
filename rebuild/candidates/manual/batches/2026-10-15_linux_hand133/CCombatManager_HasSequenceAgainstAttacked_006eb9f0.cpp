// CCombatManager::HasSequenceAgainstAttacked(const CThingCreatureBase& creature) -- true when the creature (as attacker) has a usable combat sequence against the creature it is attacking: GetBestSequenceIndexAttackerOnDefender(attacker info, victim info) >= 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006EB9F0, length 101 bytes (+ int3 padding).
// Callees: the combatant map's find @ 0x006F0250 (twice; manifest "RedblackTree_FindLE"), CCombatManager::GetBestSequenceIndexAttackerOnDefender
// (CCombatantInfo& attacker, CCombatantInfo& defender) @ 0x006EAE20 (thiscall on `this`, long result).
// Label correction: the manifest's "CEngineInternalPrimitive2DStaticMesh::GetBoundingSphereWorldSpace" is a propagated name; the body
// sits among the CCombatManager methods, uses the Combatants map at +0x10 and calls GetBestSequenceIndexAttackerOnDefender on `this`.
// The method name is descriptive. Model: the landed CCombatManager GetCombatantInfo (0x006E9E40) retail layout -- STLport
// map<unsigned __int64, CCombatantInfo> {header, count, comparator} at +0x10, node value at +0x18, AttackerInfo.Attacked at value +4,
// the creature's UniqueID at +0x18.
// Shape: one 8-byte frame slot for the by-value uid of the inline GetCombatantInfo(uid) helper (shared by both lookups), the map address
// kept in ebx, the attacker's value address (node + 0x18) kept in esi across the second find; `setge al` straight into al = `>= 0 ? true : false` (a plain `return x >= 0;` goes through `xor edx,edx ; setge dl ; mov al,dl`).
// Leaf: HasSequenceAgainstAttacked
struct CThingCreatureBase_Retail {
    unsigned char _pad[0x18];
    unsigned __int64 UniqueID;                  // +0x18
    unsigned __int64 GetUID() const { return UniqueID; }
};

struct CAttackingCombatantInfo {                // retail 0x30 bytes
    CThingCreatureBase_Retail* Attacked;        // +0x00
    unsigned char _rest[0x30 - 4];
};

struct CCombatantInfo {                         // retail layout (Ego_r.pdb names; 12-byte vectors/maps)
    CThingCreatureBase_Retail* Parent;          // +0x00
    CAttackingCombatantInfo AttackerInfo;       // +0x04
    unsigned char _defenderInfo[0x70 - 0x34];   // +0x34 DefenderInfo
};

struct CRetailRbMap_UID_CombatantInfo {         // STLport map<unsigned __int64, CCombatantInfo>: {header, count, comparator}
    struct node {
        long  colour; node* parent; node* left; node* right;
        unsigned __int64 key;                   // +0x10
        CCombatantInfo value;                   // +0x18
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key);    // 0x006F0250
};

struct CCombatManager_Retail {
    virtual void v00();
    unsigned char _sequences[0xc];              // +0x04 RegisteredCombatSequences
    CRetailRbMap_UID_CombatantInfo Combatants;  // +0x10
    CCombatantInfo& GetCombatantInfo(unsigned __int64 uid)
    {
        return Combatants.find(uid)->value;
    }
    long GetBestSequenceIndexAttackerOnDefender(CCombatantInfo& attacker, CCombatantInfo& defender);   // 0x006EAE20
    bool HasSequenceAgainstAttacked(const CThingCreatureBase_Retail& creature);
};

bool CCombatManager_Retail::HasSequenceAgainstAttacked(const CThingCreatureBase_Retail& creature)
{
    CCombatantInfo& attacker = GetCombatantInfo(creature.GetUID());
    CCombatantInfo& defender = GetCombatantInfo(attacker.AttackerInfo.Attacked->GetUID());
    return GetBestSequenceIndexAttackerOnDefender(attacker, defender) >= 0 ? true : false;
}
