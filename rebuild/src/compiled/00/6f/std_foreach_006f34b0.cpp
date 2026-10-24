// std::for_each(map<CThingCreatureBase*,CAttackingCombatantInfo*>::iterator first, iterator last, CGetBestAttackerForProng f) -- apply the best-attacker scorer to every (attacker, info) entry, return the functor
// Leaf: ??$for_each@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F34B0, 127 bytes, `ret 0x24` (/Gr; hidden functor result in ecx).
// Label correction: catalog "CDefendingCombatantInfo_ApplyAttackerFunctor" -> std::for_each over the
//   map<CThingCreatureBase*, CAttackingCombatantInfo*> iterator with the anonymous-namespace CGetBestAttackerForProng.
// Callees: CGetBestAttackerForProng::operator()(pair<CThingCreatureBase*, CAttackingCombatantInfo*>) 0x006F2DE0
//   (the entry passed by value as an 8-byte converted pair).
// Shape: the tree iterator's ++ is STLport's static `_Rb_global<bool>::_M_increment(node)` returning the node (an
//   in-place member increment picks other registers); the 0x1c-byte functor returns by `rep movsd`.
class CThingCreatureBase;
class CAttackingCombatantInfo;
class CWorldMap;
class CCombatRing;
class CCombatWheel;
class CProng;

namespace std {
template<class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    template<class U1, class U2> pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}   // STLport's explicit copy constructor
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
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
    _Base_ptr _M_node;
    void _M_increment() { _M_node = _Rb_global_inst::_M_increment(_M_node); }
};

template<class Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    Value& operator*() const { return ((_Rb_tree_node<Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++()
    {
        _M_increment();
        return *this;
    }
};

template<class Value> inline bool operator!=(const _Rb_tree_iterator<Value>& x, const _Rb_tree_iterator<Value>& y)
{
    return x._M_node != y._M_node;
}

template<class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}
}

namespace {
class CGetBestAttackerForProng {                          // Ego_r.pdb: 0x1c bytes
public:
    const CCombatRing* Ring;                              // +0x00
    const CCombatWheel* Wheel;                            // +0x04
    const CProng* Prong;                                  // +0x08
    const CThingCreatureBase* Defender;                   // +0x0c
    const CWorldMap* WorldMap;                            // +0x10
    float BestCompareVal;                                 // +0x14
    CThingCreatureBase* PBestAttacker;                    // +0x18
    CGetBestAttackerForProng(const CWorldMap& worldMap, const CCombatWheel& wheel, const CCombatRing& ring,
                             const CProng& prong, const CThingCreatureBase& defender)
        : Ring(&ring), Wheel(&wheel), Prong(&prong), Defender(&defender), WorldMap(&worldMap), BestCompareVal(-1.0f), PBestAttacker(0) {}
    void operator()(std::pair<CThingCreatureBase*, CAttackingCombatantInfo*> attacker);   // 0x006F2DE0
};
}

typedef std::pair<CThingCreatureBase* const, CAttackingCombatantInfo*> SAttackerEntry;
template CGetBestAttackerForProng __fastcall std::for_each(std::_Rb_tree_iterator<SAttackerEntry> first, std::_Rb_tree_iterator<SAttackerEntry> last,
                                                           CGetBestAttackerForProng f);