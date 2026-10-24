// Behaviour test for 00702810 (CTCQuestCard::IsActivatable): false only when a renown-level prerequisite is above the
// living hero's renown level; a missing def, a missing or dead hero, or a hero without stats never blocks.
#include <cstdio>
#include <new>

template<class T1, class T2> class pair {
public:
    T1 first;
    T2 second;
};

struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template<class Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    Value _M_value_field;
};
struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* node)
    {
        if (node->_M_right != 0) {
            node = node->_M_right;
            while (node->_M_left != 0)
                node = node->_M_left;
        }
        else {
            _Rb_tree_node_base* y = node->_M_parent;
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
        _M_node = _Rb_global::_M_increment(_M_node);
        return *this;
    }
    bool operator!=(const _Rb_tree_const_iterator& other) const { return _M_node != other._M_node; }
};
template<class Key, class T> class map {
public:
    typedef pair<const Key, T> value_type;
    typedef _Rb_tree_const_iterator<value_type> const_iterator;
    _Rb_tree_node<value_type>* _M_header;
    unsigned int _M_node_count;
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
};

enum EQuestCardPrerequisite { QUEST_CARD_PREREQUISITE_RENOWN_LEVEL = 0, QUEST_CARD_PREREQUISITE_OTHER = 1 };
enum ETCInterfaceType { TC_HERO_STATS = 4 };

class CQuestCardDef {
public:
    map<EQuestCardPrerequisite, long> Prerequisites;
};
template<class T> class CDefPointer {
public:
    T* Object;
    T* operator->() const { return Object; }
};
class CTCHeroStats {
public:
    long RenownLevel;
};
class CTCBase;
class CInterfaceMap {
public:
    struct SEntry {
        long type;
        CTCBase* component;
    };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long& type) const   // stand-in for 0x004365B0
    {
        const SEntry* it = first;
        while (it != last && it->type < type)
            ++it;
        return it;
    }
};
class CThing {
public:
    unsigned long PresentInterfaces[9];
    CInterfaceMap Interfaces;
    unsigned char Dead : 1;
    template<class T> bool GetInterface(long type, const T** out) const
    {
        if (PresentInterfaces[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* m = &Interfaces;
            const CInterfaceMap::SEntry* it = m->LowerBound(key);
            if (it == m->last || type < it->type)
                it = m->last;
            *out = (const T*)it->component;
            return true;
        }
        return false;
    }
};
static CThing* gHero = 0;
class CPlayer {
public:
    CThing* GetControlledCreature() { return gHero; }   // 0x00487DC0 stand-in
};
static CPlayer gPlayer;
class CPlayerManager {
public:
    CPlayer* GetMainPlayer() const { return &gPlayer; }  // 0x00449960 stand-in
};
static CPlayerManager gPlayerManager;

class CTCQuestCard {
public:
    CDefPointer<const CQuestCardDef> PDef;
    CPlayerManager* GetPlayerManager() const { return &gPlayerManager; }   // 0x00686D50 stand-in
    bool IsActivatable() const
    {
        if (PDef.Object) {
            for (map<EQuestCardPrerequisite, long>::const_iterator it = PDef->Prerequisites.begin(); it != PDef->Prerequisites.end(); ++it) {
                if (it->first == QUEST_CARD_PREREQUISITE_RENOWN_LEVEL) {
                    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetControlledCreature();
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
};

typedef pair<const EQuestCardPrerequisite, long> SPrereq;
typedef _Rb_tree_node<SPrereq> SNode;

static SNode* MakeNode(EQuestCardPrerequisite key, long value)
{
    SNode* n = (SNode*)new char[sizeof(SNode)];
    *(EQuestCardPrerequisite*)&n->_M_value_field.first = key;
    n->_M_value_field.second = value;
    n->_M_color = false;
    n->_M_parent = n->_M_left = n->_M_right = 0;
    return n;
}

int main()
{
    // def with {renown: 3, other: 99}
    SNode* header = MakeNode(QUEST_CARD_PREREQUISITE_RENOWN_LEVEL, 0);
    SNode* renown = MakeNode(QUEST_CARD_PREREQUISITE_RENOWN_LEVEL, 3);
    SNode* other = MakeNode(QUEST_CARD_PREREQUISITE_OTHER, 99);
    header->_M_parent = renown; header->_M_left = renown; header->_M_right = other;
    renown->_M_parent = header; renown->_M_right = other; other->_M_parent = renown;
    CQuestCardDef def;
    def.Prerequisites._M_header = header;
    def.Prerequisites._M_node_count = 2;
    // def with {other: 99} only
    SNode* header2 = MakeNode(QUEST_CARD_PREREQUISITE_RENOWN_LEVEL, 0);
    SNode* other2 = MakeNode(QUEST_CARD_PREREQUISITE_OTHER, 99);
    header2->_M_parent = other2; header2->_M_left = other2; header2->_M_right = other2;
    other2->_M_parent = header2;
    CQuestCardDef def2;
    def2.Prerequisites._M_header = header2;
    def2.Prerequisites._M_node_count = 1;

    CTCHeroStats stats;
    stats.RenownLevel = 2;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 1;
    entries[0].component = 0;
    entries[1].type = TC_HERO_STATS;
    entries[1].component = (CTCBase*)&stats;
    CThing hero;
    for (int i = 0; i < 9; ++i)
        hero.PresentInterfaces[i] = 0;
    hero.PresentInterfaces[0] = (1UL << TC_HERO_STATS) | 2;
    hero.Interfaces.first = entries;
    hero.Interfaces.last = entries + 2;
    hero.Dead = 0;

    CTCQuestCard card;
    int ok = 1;
    card.PDef.Object = 0;
    gHero = &hero;
    if (!card.IsActivatable())
        ok = 0;                                  // no def
    card.PDef.Object = &def;
    if (card.IsActivatable())
        ok = 0;                                  // renown 2 < 3
    stats.RenownLevel = 3;
    if (!card.IsActivatable())
        ok = 0;                                  // renown reached
    stats.RenownLevel = 1;
    hero.Dead = 1;
    if (!card.IsActivatable())
        ok = 0;                                  // dead hero is not checked
    hero.Dead = 0;
    hero.PresentInterfaces[0] = 2;
    if (!card.IsActivatable())
        ok = 0;                                  // no stats interface
    hero.PresentInterfaces[0] = (1UL << TC_HERO_STATS) | 2;
    gHero = 0;
    if (!card.IsActivatable())
        ok = 0;                                  // no hero
    gHero = &hero;
    card.PDef.Object = &def2;
    if (!card.IsActivatable())
        ok = 0;                                  // no renown prerequisite
    if (!ok) {
        std::printf("00702810_TEST FAIL\n");
        return 1;
    }
    std::printf("00702810_TEST PASS\n");
    return 0;
}