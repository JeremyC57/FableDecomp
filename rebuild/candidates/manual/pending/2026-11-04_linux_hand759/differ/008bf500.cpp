// CCreatureAction_GenericFireProjectileWeapon::FinishAction() -- abort the action when it has not performed; kill every
// Grade: DIFFER(ours v retail) -- 412v412, scratch-register rotation only: retail forms the TCI_CARRYING / TCI_WEAPON lookup-key address in eax / ecx, ours ecx / edx; chunk 759 fixed the dependent-things loop (STLport operator[] as *(begin()+n) stops the i*8 CSE, 15 -> 6 lines) and the PDB const signatures; tried PeekTC over the real CVectorMap, Find, a named attacker
// Leaf: ?FinishAction@CCreatureAction_GenericFireProjectileWeapon@
// Flags: none
// live dependent thing; when the creature is attacking, ready the projectile of every attacker of its target (alive
// primary-slot item with a projectile CTCWeapon); then the base FinishAction
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
class CTCBase { public: virtual ~CTCBase(); };
struct CCombatManager;
struct CThing {
    unsigned char _pad0[0x20];
    std::bitset<274> UsingComponents;                 // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char _pad54[0x91 - 0x54];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    void Kill(bool immediately);                      // 0x004C9B80
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template<class T> __forceinline bool PeekTC(ETCInterfaceType id, T** ptc)
    {
        if (HasTC(id)) {
            ETCInterfaceType type = id;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(type);
            if (it == map->end() || id < it->first)
                it = map->end();
            *ptc = static_cast<T*>(it->second);
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing {
    CCombatManager& GetCombatManager();               // 0x00661ED0
};
struct CTCCarrying : CTCBase {                        // TCI_CARRYING (70)
    CThing* GetThingInPrimarySlot() const;            // 0x006A1440
};
struct CTCWeapon : CTCBase {                          // TCI_WEAPON (72)
    bool IsProjectileWeapon() const;                  // 0x005DAA40
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
    iterator begin() const { return iterator(header->left); }
    iterator end() const { return iterator(header); }
};
struct CCombatantInfo {
    CThingCreatureBase* Parent;                       // +0x00
    CThingCreatureBase* Attacked;                     // +0x04 AttackerInfo.Attacked
    unsigned char _pad08[0x34 - 0x08];
    CAttackerMap Attackers;                           // +0x34 DefenderInfo (PDB +0x38)
};
struct CCombatManager {
    bool IsCreatureAttacking(const CThingCreatureBase& creature);   // 0x006E92C0
    const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) const;   // 0x006E9360
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
    typedef CBaseIntelligentPointer* iterator;
    iterator begin() { return _M_start; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CBaseIntelligentPointer& operator[](unsigned int n) { return *(begin() + n); }
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
    CCombatManager& manager = PCreature.GetPItem()->GetCombatManager();
    if (manager.IsCreatureAttacking(*PCreature.GetPItem())) {
        const CCombatantInfo& info = manager.GetCombatantInfo(*PCreature.GetPItem());
        const CCombatantInfo& target = manager.GetCombatantInfo(*info.Attacked);
        for (CAttackerMap::iterator it = target.Attackers.begin(); it != target.Attackers.end(); ++it) {
            CTCCarrying* carrying;
            if (it.second()->PeekTC(TCI_CARRYING, &carrying)) {
                CThing* item = carrying->GetThingInPrimarySlot();
                CTCWeapon* weapon;
                if (item && !(item->m_flags91 & 1) && item->PeekTC(TCI_WEAPON, &weapon) && weapon->IsProjectileWeapon())
                    weapon->ReadyProjectile();
            }
        }
    }
    CCombatActionBase::FinishAction();
}
