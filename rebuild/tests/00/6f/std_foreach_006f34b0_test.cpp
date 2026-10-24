// Behaviour test for 006f34b0 (std::for_each over map<CThingCreatureBase*, CAttackingCombatantInfo*> with
// CGetBestAttackerForProng): the in-order walk (STLport _M_increment) visits every entry once, in key order, and the
// functor comes back by value with its accumulated state.
#include <cstdio>

class CThingCreatureBase { public: int id; };
class CAttackingCombatantInfo { public: float score; };

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    template<class U1, class U2> pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}
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
template<class Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    Value& operator*() const { return ((_Rb_tree_node<Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++()
    {
        _M_node = _Rb_global::_M_increment(_M_node);
        return *this;
    }
};
template<class Value> inline bool operator!=(const _Rb_tree_iterator<Value>& x, const _Rb_tree_iterator<Value>& y)
{
    return x._M_node != y._M_node;
}

template<class InputIter, class Function> Function for_each(InputIter first, InputIter last, Function f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}

class CGetBestAttackerForProng {                 // the scorer (0x006F2DE0) is replaced by a highest-score pick
public:
    const void* Ring;
    const void* Wheel;
    const void* Prong;
    const CThingCreatureBase* Defender;
    const void* WorldMap;
    float BestCompareVal;
    CThingCreatureBase* PBestAttacker;
    int Visits;
    int Order;
    CGetBestAttackerForProng() : Ring(0), Wheel(0), Prong(0), Defender(0), WorldMap(0), BestCompareVal(-1.0f), PBestAttacker(0), Visits(0), Order(0) {}
    void operator()(pair<CThingCreatureBase*, CAttackingCombatantInfo*> attacker)
    {
        ++Visits;
        Order = Order * 10 + attacker.first->id;
        if (attacker.second->score > BestCompareVal) {
            BestCompareVal = attacker.second->score;
            PBestAttacker = attacker.first;
        }
    }
};

typedef pair<CThingCreatureBase* const, CAttackingCombatantInfo*> SEntry;
typedef _Rb_tree_node<SEntry> SNode;

int main()
{
    CThingCreatureBase c[4];
    CAttackingCombatantInfo info[4];
    for (int i = 0; i < 4; ++i) {
        c[i].id = i + 1;
        info[i].score = (float)((i * 7) % 4);   // 0, 3, 2, 1: creature 2 is best
    }
    SNode* nodes[5];
    for (int i = 0; i < 5; ++i) {
        nodes[i] = (SNode*)new char[sizeof(SNode)];
        nodes[i]->_M_color = false;
        nodes[i]->_M_parent = nodes[i]->_M_left = nodes[i]->_M_right = 0;
    }
    SNode& header = *nodes[4];
    SNode& n1 = *nodes[0];
    SNode& n2 = *nodes[1];
    SNode& n3 = *nodes[2];
    SNode& n4 = *nodes[3];
    for (int i = 0; i < 4; ++i) {
        *(CThingCreatureBase**)&nodes[i]->_M_value_field.first = &c[i];
        nodes[i]->_M_value_field.second = &info[i];
    }
    //      n2
    //    n1  n4
    //       n3
    header._M_parent = &n2; header._M_left = &n1; header._M_right = &n4;
    n2._M_parent = &header; n2._M_left = &n1; n2._M_right = &n4;
    n1._M_parent = &n2;
    n4._M_parent = &n2; n4._M_left = &n3;
    n3._M_parent = &n4;
    _Rb_tree_iterator<SEntry> first, last;
    first._M_node = &n1;
    last._M_node = &header;
    CGetBestAttackerForProng result = for_each(first, last, CGetBestAttackerForProng());
    int ok = result.Visits == 4 && result.Order == 1234 && result.PBestAttacker == &c[1] && result.BestCompareVal == 3.0f;
    if (!ok) {
        std::printf("006f34b0_TEST FAIL\n");
        return 1;
    }
    std::printf("006f34b0_TEST PASS\n");
    return 0;
}