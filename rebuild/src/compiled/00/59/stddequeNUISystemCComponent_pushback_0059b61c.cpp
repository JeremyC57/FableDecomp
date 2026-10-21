// std::deque<NUISystem::CComponent*>::push_back(const value_type&) -- append in place, or grow through _M_push_back_aux_v at the block end
// Leaf: push_back
// Retail: Fable.exe @ 0x0059B61C, 37 bytes, `ret 4`. Catalog label CCreatureAction_FireMissileWeapon::FrameUpdate is
// wrong: the body is STLport deque::push_back -- when _M_finish._M_cur (+0x10) != _M_finish._M_last (+0x18) - 1, construct
// the 4-byte element in place (placement-new null test) and advance _M_cur; else tail-jump to _M_push_back_aux_v
// (0x0059B576). Callers 0x00596758/0x00596801/0x005968FC/0x00597BC8 are CFrontEndManager screen pushes, the deque of
// CFrontEndManager::m_stackScreens (Ego_r.pdb stack<NUISystem::CComponent*>) whose _M_create_nodes is 0x0059AA43.
// Layout: STLport deque: _M_start / _M_finish iterators {cur, first, last, node} at +0x00 / +0x10, map +0x20.
// Opt: size TU (unaligned entry).
#pragma optimize("s", on)
#include <new>
namespace NUISystem { class CComponent; }
namespace std {
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> struct _Deque_iterator {
    T*  _M_cur;
    T*  _M_first;
    T*  _M_last;
    T** _M_node;
};
template <class T> class deque {
public:
    _Deque_iterator<T> _M_start;                         // +0x00
    _Deque_iterator<T> _M_finish;                        // +0x10
    T**                _M_map;                           // +0x20
    size_t             _M_map_size;                      // +0x24
    void push_back(const T& t)
    {
        if (_M_finish._M_cur != _M_finish._M_last - 1) {
            _Construct(_M_finish._M_cur, t);
            ++_M_finish._M_cur;
        }
        else
            _M_push_back_aux_v(t);
    }
    void _M_push_back_aux_v(const T& t);                 // 0x0059B576
};
template void deque<NUISystem::CComponent*>::push_back(NUISystem::CComponent* const&);
}