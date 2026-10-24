// std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CCombatantInfo>, ...>::erase(iterator position) -- unlink and rebalance, destroy the combatant info, free the node
// Leaf: ?erase@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F4BC0, 94 bytes, `ret 4`.
// Label correction: catalog "CMemoryAllocatorVariableSize_EraseAndDestroy" -> STLport _Rb_tree::erase(iterator) of the
//   UID -> CCombatantInfo map.
// Callees: _Rb_global<bool>::_Rebalance_for_erase 0x0042A34E (__cdecl, root/leftmost/rightmost by reference),
//   ~CDefendingCombatantInfo 0x006F48A0, __destroy_aux 0x006F2300 (same-TU noinline body), free 0x00BFEA14.
// Shape: STLport erase verbatim; the value destructor inlines (defender info, then the attacker's sequence vector).
extern "C" void* __cdecl malloc(unsigned int size);      // MSVCR71 import thunk 0x00BFEA0E
extern "C" void __cdecl free(void* block);                // MSVCR71 import thunk 0x00BFEA14
void __cdecl operator delete(void* block);                // MSVCR71 import thunk 0x00BFE9BC

class CThingCreatureBase;
class CCombatSequenceBase;
class CCPPointerInfo;

template<class T> class CCountedPointer {                 // Ego_r.pdb: 8 bytes
public:
    T* Data;                                              // +0x00
    CCPPointerInfo* Info;                                 // +0x04
    ~CCountedPointer();
};

class C2DVector {
public:
    float X, Y;
};

namespace std {
struct __false_type { __false_type() {} };

template<class T> inline void _Destroy(T* pointer) { pointer->~T(); }

#pragma auto_inline(off)
// 0x006F2300 (landed separately; same-TU body so the empty tag can share a dead or live argument slot)
template<class ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(ForwardIterator first, ForwardIterator last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
#pragma auto_inline(on)

template<class ForwardIterator> inline void _Destroy(ForwardIterator first, ForwardIterator last)
{
    __destroy_aux(first, last, __false_type());
}

template<class T> class allocator { public: allocator() {} };

template<class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                          // +0x00
    T* _M_finish;                                         // +0x04
    T* _M_end_of_storage;                                 // +0x08
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};

template<class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
};

template<class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

template<class Pair> struct _Select1st {
    const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
};
}

class CAttackingCombatantInfo {                           // Ego_r.pdb (retail 0x30: 12-byte STLport vector)
public:
    CThingCreatureBase* Attacked;                         // +0x00
    long AttackerNumber;                                  // +0x04
    long CombatWheelRingIndex;                            // +0x08
    bool PreferredAttackDollySet;                         // +0x0c
    long PreferredAttackDollyTimer;                       // +0x10
    C2DVector PreferredAttackDollyVector;                 // +0x14
    bool UsingCombatWheel;                                // +0x1c
    long CurrentSequence;                                 // +0x20
    std::vector<CCountedPointer<CCombatSequenceBase> > CombatSequences;   // +0x24
};

class CDefendingCombatantInfo {                           // retail 0x3c bytes (12-byte STLport containers)
public:
    unsigned char _body[0x3c];
    ~CDefendingCombatantInfo();                           // 0x006F48A0
};

class CCombatantInfo {                                    // Ego_r.pdb (retail 0x70)
public:
    CThingCreatureBase* Parent;                           // +0x00
    CAttackingCombatantInfo AttackerInfo;                 // +0x04
    CDefendingCombatantInfo DefenderInfo;                 // +0x34
};

typedef std::pair<const unsigned __int64, CCombatantInfo> SCombatantByUID;   // 0x78 bytes; tree node 0x88

namespace std {
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
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root,
                                                            _Rb_tree_node_base*& leftmost, _Rb_tree_node_base*& rightmost);   // 0x0042A34E
};
typedef _Rb_global<bool> _Rb_global_inst;

template<class Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
};

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc = allocator<Value> >
class _Rb_tree {
public:
    typedef _Rb_tree_node<Value>* _Link_type;
    typedef _Rb_tree_iterator<Value> iterator;
    _Link_type _M_header;                                 // +0x00
    unsigned int _M_node_count;                           // +0x04
    Compare _M_key_compare;                               // +0x08

    static _Link_type& _S_left(_Link_type x) { return (_Link_type&)(x->_M_left); }
    static _Link_type& _S_right(_Link_type x) { return (_Link_type&)(x->_M_right); }
    static void _M_deallocate(_Link_type x)
    {
        if (x)
            free(x);
    }
    _Rb_tree();
    void erase(iterator position);
    void _M_erase(_Link_type x);
};
}

namespace std {
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator position)
{
    _Link_type y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(position._M_node, _M_header->_M_parent,
                                                                     _M_header->_M_left, _M_header->_M_right);
    _Destroy(&y->_M_value_field);
    _M_deallocate(y);
    --_M_node_count;
}
}

template class std::_Rb_tree<unsigned __int64, SCombatantByUID, std::_Select1st<SCombatantByUID>, std::less<unsigned __int64> >;