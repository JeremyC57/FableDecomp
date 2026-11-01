// CCombatManager::Update() -- per-frame combat update: refresh every registered combatant's attacker list (forced), then update the combat dialogue manager
// Leaf: ?Update@CCombatManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006ED200, 101 bytes, thiscall, tail jump to CombatDialogueManager::Update 0x008AF780.
// Identity (proven): the only caller is CWorld::UpdateFrame 0x004A5A40 (`mov ecx,[esi+4Ch]; call`), CWorld+0x4C =
//   Ego_r.pdb CWorld::PThingCombatManager (scoped_ptr<CCombatManager>); the body walks CCombatManager::Combatants (+0x10,
//   map<unsigned __int64, CCombatantInfo>: value at node+0x18) calling ?UpdateAttackingCombatantsList@CCombatManager@@AAEX
//   AAUCCombatantInfo@@_N@Z 0x006ED030 (landed) with true, then DialogueManager (+0x38) Update. Ego_r.pdb / FableWin.pdb
//   ?Update@CCombatManager@@QAEXXZ is the only CCombatManager member of this signature that fits. The manifest label
//   (CombatDialogueManager_UpdateDialogueFrame) is descriptive and wrong.
// Layout: Ego_r.pdb CCombatManager with retail 12-byte STLport maps (RegisteredCombatSequences +0x04, Combatants +0x10,
//   DialogueManager +0x38 -- Ego_r +0x40 less two 4-byte map differences); CombatDialogueManager / CCombatantInfo are opaque
//   (real member types in the comments; retail sizes 0x24 / 0x70). No engine header exists for either class.
// Shape: STLport 4.6 map iteration -- begin() = header->_M_left, end() re-read from the header each test, and the tree
//   iterator's operator++ calls _Rb_global<bool>::_M_increment, whose _tree.c body VC7.1 expands inline (retail has the
//   increment inline too).
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Arg1, class _Arg2, class _Result> struct binary_function { };
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
struct bidirectional_iterator_tag { };
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;
    _Base_ptr   _M_parent;
    _Base_ptr   _M_left;
    _Base_ptr   _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* __x);
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    }
    else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_right) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        if (_M_node->_M_right != __y)
            _M_node = __y;
    }
    return _M_node;
}
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Value value_type;
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    pointer operator->() const { return &(operator*()); }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
protected:
    size_type _M_node_count;
    _Compare _M_key_compare;
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
public:
    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(this->_M_header._M_data); }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Compare key_compare;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, key_compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
};
}

class CThingCreatureBase;
struct CCombatantInfo {                                                  // retail 0x70 (STLport containers)
    CThingCreatureBase* Parent;                                          // +0x00
    unsigned char       AttackerInfo[0x30];                              // +0x04 CAttackingCombatantInfo
    unsigned char       DefenderInfo[0x3c];                              // +0x34 CDefendingCombatantInfo
};
class CWorld;
class CombatDialogueManager {                                            // Ego_r.pdb members, retail 0x24 (STLport map)
public:
    void Update();                                                       // 0x008AF780
    unsigned char SpeechGroupInfoMap[0xc];                               // +0x00 std::map<unsigned long, TSpeechGroupInfo>
    void*         PDialogueDef;                                          // +0x0c CDefPointer<const CCombatDialogueDef>
    unsigned char PCurrentTalker[0x8];                                   // +0x10 CIntelligentPointer<CThingCreatureBase>
    unsigned long CurrentTalkerTextGroupIndex;                           // +0x18
    long          LastSpeechFrame;                                       // +0x1c
    long          LastBattleCryFrame;                                    // +0x20
};
class CCombatManager {
public:
    virtual ~CCombatManager();
    void Update();
private:
    void UpdateAttackingCombatantsList(CCombatantInfo& info, bool force);   // 0x006ED030
    unsigned char RegisteredCombatSequences[0xc];                            // +0x04 std::map<CCharString, CCombatSequenceBase* (__fastcall*)(..)>
    std::map<unsigned __int64, CCombatantInfo> Combatants;                   // +0x10
    bool  CombatStrafeTurningEnabled;                                        // +0x1c
    CWorld* World;                                                           // +0x20
    bool  DrawDebug;                                                         // +0x24
    bool  TraceDebug;                                                        // +0x25
    float ProximityForNavigation;                                            // +0x28
    bool  EnableAINonAttackMode;                                             // +0x2c
    float AttackPatternIdleStandardInterval;                                 // +0x30
    bool  AttacksFromNearestCombatantOnly;                                   // +0x34
    CombatDialogueManager DialogueManager;                                   // +0x38
};
void CCombatManager::Update()
{
    for (std::map<unsigned __int64, CCombatantInfo>::iterator it = Combatants.begin(); it != Combatants.end(); ++it)
        UpdateAttackingCombatantsList(it->second, true);
    DialogueManager.Update();
}