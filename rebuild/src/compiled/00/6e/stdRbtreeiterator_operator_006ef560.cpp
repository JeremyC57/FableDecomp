// std::_Rb_tree_iterator::operator++(int) -- Return the old iterator and advance to its successor.
// Leaf: ??E?$_Rb_tree_iterator@U?$pair@QAVCThingCreatureBase@@PAVCAttackingCombatantInfo@@@std@@U?$_Nonconst_traits@U?$pair@QAVCThingCreatureBase@@PAVCAttackingCombatantInfo@@@std@@@2@@std@@QAE?AU01@H@Z
// Types (proven, agentFAM11, 2026-11-12): map<CThingCreatureBase*, CAttackingCombatantInfo*> (node 0x18 = 0x10 + 4 + 4). Sources: (1) retail call chain: the landed
//   map<CThingCreatureBase*, CAttackingCombatantInfo*>::erase(const key&) 0x006F3340 (proven types; called on the attackers map by
//   AssignAttackersToRing 0x006EBF70) calls this tree's erase(first, last) 0x006F1CF0, which clears through _M_erase 0x006EFCE0; the
//   ~_Rb_tree 0x006F1BA0, ~map 0x006F24D0 and clear 0x006F0BC0 call 0x006EFCE0, and CCombatWheel::PopulateCombatRings 0x006EC850 destroys
//   its two local attacker maps (filled by insert_unique 0x006F2A40, passed to AssignAttackersToRing) through it; (2) Ego_r.pdb
//   CDefendingCombatantInfo::CCombatRing::AssignAttackersToRing(..., map<CThingCreatureBase*, CAttackingCombatantInfo*>&) and
//   CCombatWheel::Init(..., const map<CThingCreatureBase*, CAttackingCombatantInfo*>&); FableWin.pdb has ??0 / ??1 / clear / _Erase / erase
//   of this map / tree.
//   Was the placeholder std::_Rb_tree_iterator<CUnprovenValue_006ef560, std::_Nonconst_traits<CUnprovenValue_006ef560>>::operator++ (leaf ??E?$_Rb_tree_iterator@VCUnprovenValue_006ef560@@U?$_Nonconst_traits@VCUnprovenValue_006ef560@@@std@@@std@@QAE?AU01@H@Z); name-only change: the
//   same body, the placeholder types replaced by the proven ones.
class CThingCreatureBase;
class CAttackingCombatantInfo;
namespace std {
template<class _T1,class _T2> struct pair;
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template<class Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* x) {
        if(x->_M_right) {
            x=x->_M_right;
            while(x->_M_left) x=x->_M_left;
        } else {
            _Rb_tree_node_base* y=x->_M_parent;
            while(x==y->_M_right) { x=y; y=y->_M_parent; }
            if(x->_M_right!=y) x=y;
        }
        return x;
    }
};
template<class Value> struct _Nonconst_traits {};
struct _Rb_tree_base_iterator { _Rb_tree_node_base* _M_node; };
template<class Value,class Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<Value,Traits> _Self;
    _Rb_tree_iterator() { _M_node=0; }
    _Rb_tree_iterator(const _Self& x) { _M_node=x._M_node; }
    _Self operator++(int) {
        _Self old=*this;
        _M_node=_Rb_global<bool>::_M_increment(_M_node);
        return old;
    }
};
template _Rb_tree_iterator<std::pair<CThingCreatureBase* const, CAttackingCombatantInfo*> ,_Nonconst_traits<std::pair<CThingCreatureBase* const, CAttackingCombatantInfo*> > > _Rb_tree_iterator<std::pair<CThingCreatureBase* const, CAttackingCombatantInfo*> ,_Nonconst_traits<std::pair<CThingCreatureBase* const, CAttackingCombatantInfo*> > >::operator++(int);
}