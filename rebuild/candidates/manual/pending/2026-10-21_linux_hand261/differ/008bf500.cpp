// CCreatureAction_GenericFireProjectileWeapon::FinishAction() -- abort the action when it has not performed; kill every
// Grade: DIFFER(412v412) -- dependent-things loop: ours CSEs i*8 into a register (lea esi,[edi*8]; add) and swaps esi/edi, retail re-forms [base+i*8] per access; the attacker-map walk and weapon checks match; tried index types, operator->, helper, post-increment
// live dependent thing; when the creature is attacking, ready the projectile of every attacker of its target (alive
// primary-slot item with a projectile CTCWeapon); then the base FinishAction
// Leaf: ?FinishAction@CCreatureAction_GenericFireProjectileWeapon
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
struct CCombatManager;
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    void Kill(bool immediately);                      // 0x004C9B80
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing {
    CCombatManager* GetCombatManager();               // 0x00661ED0
};
struct CTCCarrying {                                  // interface 0x46
    CThing* GetThingInPrimarySlot();                  // 0x006A1440
};
struct CTCWeapon {                                    // interface 0x48
    bool IsProjectileWeapon();                        // 0x005DAA40
    void ReadyProjectile();                           // 0x005DA4E0
};
struct CAttackerMap {                                 // STLport map<unsigned __int64, CThingCreatureBase*>
    struct node {
        long  colour; node* parent; node* left; node* right;   // +0 / +4 / +8 / +0xc
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
    iterator begin() { return iterator(header->left); }
    iterator end() { return iterator(header); }
};
struct CCombatantInfo {
    CThingCreatureBase* Parent;                       // +0x00
    CThingCreatureBase* Attacked;                     // +0x04 AttackerInfo.Attacked
    unsigned char _pad08[0x34 - 0x08];
    CAttackerMap Attackers;                           // +0x34 DefenderInfo (PDB +0x38)
};
struct CCombatManager {
    bool IsCreatureAttacking(const CThingCreatureBase& creature);   // 0x006E92C0
    CCombatantInfo* GetCombatantInfo(const CThingCreatureBase* creature);   // 0x006E9360
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                         // 0x00A01B50
};
struct CCreaturePointer {
    void* __vftable;
    CThingCreatureBase* m_pItem;
    CThingCreatureBase* GetPItem() const;             // 0x00A01B50
};
struct CThingPointerVector {                          // STLport vector<CIntelligentPointer<CThing>> (12 bytes)
    CBaseIntelligentPointer* _M_start;
    CBaseIntelligentPointer* _M_finish;
    CBaseIntelligentPointer* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CBaseIntelligentPointer& operator[](unsigned int i) { return _M_start[i]; }
};
struct CCreatureActionBase {                          // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void Abort();                             // slot 3 (+0xc)
    unsigned char _pad04[4];
    CCreaturePointer PCreature;                       // +0x08
    unsigned char _pad10[0x9c - 0x10];
    CThingPointerVector DependentThings;              // +0x9c
    bool HasPerformedAction();                        // 0x006925A0
};
struct CCombatActionBase : CCreatureActionBase {      // retail sizeof 0x114
    unsigned char _pad0a8[0x114 - 0xa8];
    void FinishAction();                              // 0x00859A20
};
struct CCreatureAction_GenericFireProjectileWeapon : CCombatActionBase {
    void FinishAction();
};

void CCreatureAction_GenericFireProjectileWeapon::FinishAction()
{
    if (!HasPerformedAction())
        Abort();
    for (unsigned int i = 0; i < DependentThings.size(); ++i) {
        if (DependentThings[i].GetPItem())
            DependentThings[i].GetPItem()->Kill(true);
    }
    CCombatManager* manager = PCreature.GetPItem()->GetCombatManager();
    if (manager->IsCreatureAttacking(*PCreature.GetPItem())) {
        CCombatantInfo* info = manager->GetCombatantInfo(PCreature.GetPItem());
        CCombatantInfo* target = manager->GetCombatantInfo(info->Attacked);
        for (CAttackerMap::iterator it = target->Attackers.begin(); it != target->Attackers.end(); ++it) {
            CTCCarrying* carrying;
            if (it.second()->GetInterface(&carrying, 0x46)) {
                CThing* item = carrying->GetThingInPrimarySlot();
                CTCWeapon* weapon;
                if (item && !(item->m_flags91 & 1) && item->GetInterface(&weapon, 0x48) && weapon->IsProjectileWeapon())
                    weapon->ReadyProjectile();
            }
        }
    }
    CCombatActionBase::FinishAction();
}
