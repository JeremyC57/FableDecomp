// CTCQuestCard::IsActivatable() const -- a quest card can be activated unless one of its def's prerequisites is a renown level the living hero has not reached
// Leaf: ?IsActivatable@CTCQuestCard@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00702810, 213 bytes, plain `ret`.
// Label correction: catalog "CViewBase_FindParticleMorph" is propagated; callers are CQuestManager::ActivateFeatCard
//   0x004B1590 / ActivateQuestCard 0x004B4AA0 on the interface-0x6C component (CTCQuestCard::GetInterfaceType 0x007029F0
//   returns 0x6C), and Ego_r.pdb's `?IsActivatable@CTCQuestCard@@QBE_NXZ` is the only bool() const member that fits.
// Callees: CTCBase::GetPlayerManager 0x00686D50, CPlayerManager::GetMainPlayer 0x00449960, CPlayer::GetPControlledCreature
//   0x00487DC0, CVectorMap LowerBound (const) 0x004365B0.
// Layout: CTCQuestCard PDef +0x0C; CQuestCardDef Prerequisites (map<EQuestCardPrerequisite,long>) at retail +0x5C
//   (PDB +0x60, its 16-byte vector is 12 here); interface 4 = CTCHeroStats (GetInterfaceType returns 4), RenownLevel at
//   retail +0x70 (PDB +0x78: the 16-byte map and vector are 12 each); CThing Dead +0x91 bit 0.
// Shape: a const_iterator for loop re-reading `PDef->Prerequisites.end()`, the STLport increment as a static
//   `_Rb_global<bool>::_M_increment(node)`, the hero fetched per renown prerequisite, the stats pointer `= 0` then
//   the inline GetInterface (jump-threaded when the bit is clear).
// Layout (supersedes any local-model note above): engine/CTCHeroStats.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCHeroStats CTCHeroStatsDonor
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
namespace std {
template<class T1, class T2> class pair {
public:
    T1 first;
    T2 second;
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;                                        // +0x00
    _Base_ptr _M_parent;                                  // +0x04
    _Base_ptr _M_left;                                    // +0x08
    _Base_ptr _M_right;                                   // +0x0c
};

template<class Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    Value _M_value_field;                                 // +0x10
};

template<class Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr _M_increment(_Base_ptr node)
    {
        if (node->_M_right != 0) {
            node = node->_M_right;
            while (node->_M_left != 0)
                node = node->_M_left;
        }
        else {
            _Base_ptr y = node->_M_parent;
            while (node == y->_M_right) {
                node = y;
                y = y->_M_parent;
            }
            if (node->_M_right != y)
                node = y;
        }
        return node;
    }
};

template<class Value> class _Rb_tree_const_iterator {
public:
    _Rb_tree_node_base* _M_node;
    _Rb_tree_const_iterator(_Rb_tree_node_base* node) : _M_node(node) {}
    const Value* operator->() const { return &((_Rb_tree_node<Value>*)_M_node)->_M_value_field; }
    _Rb_tree_const_iterator& operator++()
    {
        _M_node = _Rb_global<bool>::_M_increment(_M_node);
        return *this;
    }
    bool operator!=(const _Rb_tree_const_iterator& other) const { return _M_node != other._M_node; }
};

template<class Key, class T> class map {                  // STLport map: header pointer, node count, comparator (12 bytes)
public:
    typedef pair<const Key, T> value_type;
    typedef _Rb_tree_const_iterator<value_type> const_iterator;
    _Rb_tree_node<value_type>* _M_header;                 // +0x00
    unsigned int _M_node_count;                           // +0x04
    bool _M_key_compare;                                  // +0x08 (empty less<Key>)
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
};
}

enum EQuestCardPrerequisite { QUEST_CARD_PREREQUISITE_RENOWN_LEVEL = 0 };
enum ETCInterfaceType { TC_HERO_STATS = 4 };

class CTCBase;
class CPlayerManager;

class CQuestCardDef {                                     // Ego_r.pdb (124 bytes); retail vector 12 bytes, so the map is at +0x5c
public:
    unsigned char _pad00[0x5c];
    std::map<EQuestCardPrerequisite, long> Prerequisites; // +0x5c
};

template<class T> class CDefPointer {
public:
    T* Object;
    T* operator->() const { return Object; }
};

class CTCHeroStats : public CTCHeroStatsDonor {
                                      // Ego_r.pdb CTCHeroStats; retail RenownLevel +0x70 (PDB +0x78)
public:
};

class CInterfaceMap {                                     // CVectorMap<ETCInterfaceType, CTCBase*>: sorted {type, component} vector
public:
    struct SEntry {
        long type;
        CTCBase* component;
    };
    SEntry* first;
    SEntry* last;
    SEntry* end_of_storage;
    const SEntry* LowerBound(const long& type) const;     // 0x004365B0 (const)
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long PresentInterfaces[9];                   // +0x20 bitset of interface types
    CInterfaceMap Interfaces;                             // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char Dead : 1;                               // +0x91 bit 0
    template<class T> __forceinline bool GetInterface(long type, const T** out) const
    {
        if (PresentInterfaces[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &Interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (const T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };

class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;                      // 0x00487DC0
};

class CPlayerManager {
public:
    CPlayer* GetMainPlayer() const;                       // 0x00449960
};

class CTCBase {                                      // CTCBase: vtable +0x00, ParentThing +0x04, +0x08
public:
    void* _vtable;                                        // +0x00
    CThing* ParentThing;                                  // +0x04
    long _base08;                                         // +0x08
protected:
    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50
};

class CTCQuestCard : public CTCBase {                // Ego_r.pdb CTCQuestCard (52 bytes)
public:
    CDefPointer<const CQuestCardDef> PDef;                // +0x0c
    bool IsActivatable() const;
};

bool CTCQuestCard::IsActivatable() const
{
    if (PDef.Object) {
        for (std::map<EQuestCardPrerequisite, long>::const_iterator it = PDef->Prerequisites.begin(); it != PDef->Prerequisites.end(); ++it) {
            if (it->first == QUEST_CARD_PREREQUISITE_RENOWN_LEVEL) {
                CThing* hero = PeekPlayerManager().GetMainPlayer()->GetPControlledCreature();
                if (hero && !hero->Dead) {
                    const CTCHeroStats* stats = 0;
                    hero->GetInterface(TC_HERO_STATS, &stats);
                    if (stats && stats->RenownLevel < it->second)
                        return false;
                }
            }
        }
    }
    return true;
}