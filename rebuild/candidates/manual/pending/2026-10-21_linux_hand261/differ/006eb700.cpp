// CCombatManager::GetBestEnemyForCombatant(const CThingCreatureBase&, float) -- for a combatant, the attacker of it that is
// Grade: DIFFER(422v411) -- frame: the found combatant node gets its own home (frame 0x20) where retail spills it into the dead find-key slot (frame 0x1C); the range-helper argument scheduling and best-score stores follow; tried named node, value reference, block-scoped key
// targetable, attackable, within `range` (scoring by the range helper) and inside the creature's combat-type arc, with the
// highest score, unless the attempted facing direction overrides that target; else null
// Leaf: GetBestEnemyForCombatant
struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
    const SEntry* LowerBound(const long* type) const; // 0x004365B0
};
struct CCombatTypeDef {
    unsigned char _pad[0x6c];
    long ArcType;                                     // +0x6c
};
struct CTCTargetable {                                // interface 9
    bool IsTargetable() const;                        // 0x007EA280
};
struct CThingCreatureBase {
    unsigned char _pad[0x18];
    unsigned __int64 UniqueID;                        // +0x18
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned __int64 GetUID() const { return UniqueID; }
    bool IsCombatant() const;                         // 0x00662430
    CCombatTypeDef* GetCombatTypeDef() const;         // 0x00666D10
    const C3DVector& GetPosition() const;             // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
bool __fastcall IsAbleToBeAttacked(const CThingCreatureBase& creature);   // 0x0085FB30
bool __fastcall IsWithinRange(const C3DVector& from, float range, float scale, const C3DVector& to, float* score);   // 0x00A4B670
bool __fastcall IsWithinArc(const C3DVector& from, const C3DVector& to, long arcType);   // 0x00A4B7B0

struct CAttackerMap {                                 // STLport map<unsigned __int64, CThingCreatureBase*>
    struct node {
        long  colour; node* parent; node* left; node* right;
        unsigned __int64 key;                         // +0x10
        CThingCreatureBase* value;                    // +0x18
    };
    struct iterator {
        node* _M_node;
        iterator(node* n) : _M_node(n) {}
        CThingCreatureBase* second() const { return _M_node->value; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
        static node* Increment(node* x)
        {
            if (x->right != 0) {
                x = x->right;
                while (x->left != 0)
                    x = x->left;
            } else {
                node* y = x->parent;
                while (x == y->right) {
                    x = y;
                    y = y->parent;
                }
                if (x->right != y)
                    x = y;
            }
            return x;
        }
        iterator& operator++() { _M_node = Increment(_M_node); return *this; }
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    iterator begin() const { return iterator(header->left); }
    iterator end() const { return iterator(header); }
};
struct CCombatantInfo {                               // retail 0x70 bytes
    CThingCreatureBase* Parent;                       // +0x00
    unsigned char _attacker[0x30];                    // +0x04 AttackerInfo
    CAttackerMap Attackers;                           // +0x34 DefenderInfo.Attackers
};
struct CRetailRbMap_UID_CombatantInfo {               // STLport map<unsigned __int64, CCombatantInfo>
    struct node {
        long  colour; node* parent; node* left; node* right;
        unsigned __int64 key;                         // +0x10
        CCombatantInfo value;                         // +0x18
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key);          // 0x006F0250
};
struct CCombatManager {
    virtual void v00();
    unsigned char _sequences[0xc];                    // +0x04
    CRetailRbMap_UID_CombatantInfo Combatants;        // +0x10
    bool IsTargetOverriddenByAttemptedFacingDirection(const CThingCreatureBase& creature, CThingCreatureBase* target, float range);   // 0x006EA5F0
    CThingCreatureBase* GetBestEnemyForCombatant(const CThingCreatureBase& creature, float range);
};

CThingCreatureBase* CCombatManager::GetBestEnemyForCombatant(const CThingCreatureBase& creature, float range)
{
    if (!creature.IsCombatant())
        return 0;
    CRetailRbMap_UID_CombatantInfo::node* combatant = Combatants.find(creature.GetUID());
    CCombatTypeDef* def = creature.GetCombatTypeDef();
    float bestScore = 0.0f;
    float score;
    CThingCreatureBase* best = 0;
    for (CAttackerMap::iterator it = combatant->value.Attackers.begin(); it != combatant->value.Attackers.end(); ++it) {
        CThingCreatureBase* enemy = it.second();
        CTCTargetable* targetable;
        if (enemy->GetInterface(&targetable, 9) && targetable->IsTargetable() && IsAbleToBeAttacked(*enemy)
            && IsWithinRange(creature.GetPosition(), range, 1.0f, enemy->GetPosition(), &score)
            && IsWithinArc(creature.GetPosition(), enemy->GetPosition(), def->ArcType) && score > bestScore) {
            bestScore = score;
            best = enemy;
        }
    }
    if (best && !IsTargetOverriddenByAttemptedFacingDirection(creature, best, range))
        return best;
    return 0;
}
