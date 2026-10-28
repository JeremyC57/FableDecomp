// CombatStressTestDebug(CThingAICreature& creature, const CThingSearchTools& searchTools, unsigned long* seed) -- (anonymous namespace) combat stress test: suspend the creature's AI, drop it from the combat manager when it is in melee as an attacker, then register it to fight a random half-or-fewer sample of all AI creatures in the world
// Grade: DIFFER(558v558) -- the empty CThingFilter_None temporary gets its own frame slot (retail overlaps it with the seed parameter home: frame 0x24 vs 0x28, every local 4 bytes lower), and the vector size is computed before the empty test (retail loads _M_finish into ebp before the test and subtracts after it); g2 (count inside the test) shrink-wraps ebp
// Leaf: ?CombatStressTestDebug@?A0x
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00834530, length 558 bytes (__fastcall, `ret 4`: creature in ecx,
//   search tools in edx, the random seed pointer on the stack).
// Name: Ego_r.pdb / FableWin.pdb `?CombatStressTestDebug@?A0x4ed42fe4@@YIXAAVCThingAICreature@@ABVCThingSearchTools@@PAK@Z`
//   (anonymous namespace: the hash is per translation unit) and its only caller, the landed CThingAICreature::WorldUpdate
//   0x00834760 (`if (g_CombatStressTestEnabled) CombatStressTestDebug(*this, PeekThingSearchTools(), GetRandomSeed())`).
// Callees: _Rb_tree<void*,...>::insert_unique 0x00835560 (the creature's Suspenders set: inline CThingAICreature::SuspendAI
//   (true, 0), its out-of-line copy is the landed 0x00833A10), CThingSearchTools::GetAllThings<CThingAICreature,
//   vector<CThingAICreature*>, CThingFilter_None> const 0x0052B270 (Ego_r donor 0x00532B40 spelling),
//   CThingCreatureBase::IsInMeleeAsAttacker const 0x00662410 (landed), CThingCreatureBase::GetCombatManager 0x00661ED0
//   (Ego_r `QAEAAVCCombatManager@@XZ`), CCombatManager::DeregisterCombatant 0x006ED9C0 / RegisterCombatant 0x006EDC50
//   (Ego_r spellings, CThingCreatureBase&), GFROR13 0x00497890 (inline GFRandom's rotate), _Rb_tree<long,...>::insert_unique
//   0x00512DA0 and _M_erase 0x004A9CB0 (signed `setl` key compare: set<long>), malloc 0x00BFEA0E / free 0x00BFEA14
//   (STLport node and vector storage).
// Layout: retail CThingAICreature +0x1c8 bitfield byte (AISuspended = bit 1), Suspenders set<void*> +0x1cc (size +0x1d0),
//   as the landed SuspendAI 0x00833A10; engine/CThingAICreature.h is a flat pad class, so modelled locally.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

long __fastcall GFROR13(unsigned long value);          // 0x00497890

__forceinline unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Tp __p) : _Alloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                               // +0x00
    _Base_ptr   _M_parent;                              // +0x04
    _Base_ptr   _M_left;                                // +0x08
    _Base_ptr   _M_right;                               // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    void _M_increment()
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
    }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    typename _Traits::reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    _Self& operator++() { _M_increment(); return *this; }
    bool operator!=(const _Self& __y) const { return _M_node != __y._M_node; }
};

template <class _Value, class _Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Value> _Node;
    _Rb_tree_base(const _Alloc& __a) : _M_header(allocator<_Node>(), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_header;   // +0x00
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;

    unsigned int _M_node_count;                         // +0x04
    _Compare     _M_key_compare;                        // +0x08

    _Rb_tree(const _Compare& __comp, const _Alloc& __a)
        : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp)
    {
        _M_empty_initialize();
    }
    ~_Rb_tree() { clear(); }

    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }

    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;      // red: tells the header from the root
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
    const_iterator begin() const { return const_iterator(_M_leftmost()); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
    unsigned int size() const { return _M_node_count; }

    pair<iterator, bool> insert_unique(const value_type& __x);   // 0x00512DA0 / 0x00835560 (per instantiation)
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
private:
    void _M_erase(_Link_type __x)                       // 0x004A9CB0 for set<long>
    {
        while (__x != 0) {
            _M_erase(_S_right(__x));
            _Link_type __y = _S_left(__x);
            this->_M_header.deallocate(__x, 1);
            __x = __y;
        }
    }
};

template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator iterator;
    _Rep_type _M_t;
    set() : _M_t(_Compare(), _Alloc()) { }
    iterator begin() const { return _M_t.begin(); }
    iterator end() const { return _M_t.end(); }
    unsigned int size() const { return _M_t.size(); }
    bool empty() const { return _M_t.size() == 0; }
    void insert(const _Key& __x) { _M_t.insert_unique(__x); }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;  // +0x08
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(_Alloc(), (_Tp*)0) { }
    ~vector() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    bool empty() const { return _M_start == _M_finish; }
    _Tp& operator[](unsigned int __n) { return *(_M_start + __n); }
};

} // namespace std

class CThingFilter_None { public:
    CThingFilter_None() { }
};

class CThingAICreature;

class CThingSearchTools { public:
    template <class T, class TContainer, class TFilter>
    void GetAllThings(const TFilter& filter, TContainer& things) const;   // 0x0052B270 for <CThingAICreature, vector, None>
};

class CThingCreatureBase;

class CCombatManager { public:
    void DeregisterCombatant(CThingCreatureBase& creature);                                       // 0x006ED9C0
    void RegisterCombatant(CThingCreatureBase& creature, CThingCreatureBase& target, bool flag);  // 0x006EDC50
};

class CThingCreatureBase { public:
    bool IsInMeleeAsAttacker() const;                   // 0x00662410
    CCombatManager& GetCombatManager();                 // 0x00661ED0
};

class CThingAICreature : public CThingCreatureBase { public:
    unsigned char _pad000[0x1c8];
    bool          PerformingVillageTaskFlag : 1;        // +0x1c8 bit 0
    bool          AISuspended : 1;                      // +0x1c8 bit 1
    std::set<void*> Suspenders;                         // +0x1cc
    void SuspendAI(bool suspend, void* suspender)       // out of line at 0x00833A10, inlined here
    {
        if (suspend)
            Suspenders.insert(suspender);
        AISuspended = !Suspenders.empty();
    }
};

namespace {

void __fastcall CombatStressTestDebug(CThingAICreature& creature, const CThingSearchTools& searchTools, unsigned long* seed)
{
    creature.SuspendAI(true, 0);
    std::vector<CThingAICreature*> creatures;
    searchTools.GetAllThings<CThingAICreature>(CThingFilter_None(), creatures);
    if (creature.IsInMeleeAsAttacker())
        creature.GetCombatManager().DeregisterCombatant(creature);
    unsigned long count = creatures.size();
    if (!creatures.empty()) {
        unsigned long numberToPick = GFRandom(count / 2, seed);
        std::set<long> picked;
        for (; numberToPick != 0; --numberToPick)
            picked.insert(GFRandom(count, seed));
        for (std::set<long>::iterator it = picked.begin(); it != picked.end(); ++it) {
            CThingAICreature* target = creatures[*it];
            if (target != &creature)
                creature.GetCombatManager().RegisterCombatant(creature, *target, true);
        }
    }
}

}

void (__fastcall* g_keepCombatStressTestDebug)(CThingAICreature&, const CThingSearchTools&, unsigned long*) = CombatStressTestDebug;
