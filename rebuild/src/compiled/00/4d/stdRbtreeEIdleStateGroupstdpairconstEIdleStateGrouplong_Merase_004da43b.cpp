// std::_Rb_tree<EIdleStateGroup, pair<const EIdleStateGroup, long>, _Select1st<..>, less<EIdleStateGroup>, allocator<..> >::_M_erase(_Link_type x) -- free a subtree of map<EIdleStateGroup, long> without rebalancing
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DA43B, 45 bytes, `ret 4`: recurse right, keep left, free the node (0x00BFEA14).
// Identity: CTCIdleScheduler::Clear (landed 0x008018B0) clears CTCIdleScheduler::IdleStatesWeight (retail +0x18; Ego_r.pdb map<EIdleStateGroup,
//   long>) through the inline STLport clear() and calls this at 0x008018F6 on that tree; the other callers (0x004DBBFB, 0x008015C2)
//   are the same inline clear in the scheduler's other paths. STLport 4.x `_M_erase` is a private member of
//   _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>; the value is POD, so _Destroy is a no-op (FINDINGS chunk
//   398). Was landed as the AUTO_TINY stub "LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray" (a
//   propagated label).
// Leaf: ?_M_erase@?$_Rb_tree@W4EIdleStateGroup@@U?$pair@$$CBW4EIdleStateGroup@@J@std@@U?$_Select1st@U?$pair@$$CBW4EIdleStateGroup@@J@std@@@3@U?$less@W4EIdleStateGroup@@@3@V?$allocator@U?$pair@$$CBW4EIdleStateGroup@@J@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBW4EIdleStateGroup@@J@std@@@2@@Z
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);             // 0x00BFEA14

enum EIdleStateGroup { IDLE_STATE_GROUP_NONE };

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st { };
template <class _Tp> struct less { };
template <class _Tp> class allocator { };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _Rb_tree_node_base {
    char                _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree {
    typedef _Rb_tree_node<_Value>* _Link_type;
public:
    _Link_type    _M_header;                            // +0x00
    unsigned long _M_node_count;                        // +0x04
    _Compare      _M_key_compare;                       // +0x08
private:
    static _Link_type _S_left(_Rb_tree_node_base* __x) { return (_Link_type)(__x->_M_left); }
    static _Link_type _S_right(_Rb_tree_node_base* __x) { return (_Link_type)(__x->_M_right); }
    void _M_erase(_Link_type __x);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Link_type __x)
{
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _Destroy(&__x->_M_value_field);
        free(__x);
        __x = __y;
    }
}
}

typedef std::pair<const EIdleStateGroup, long> CIdleValue;
template void std::_Rb_tree<EIdleStateGroup, CIdleValue, std::_Select1st<CIdleValue>, std::less<EIdleStateGroup> >::_M_erase(std::_Rb_tree_node<CIdleValue>*);
