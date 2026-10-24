// NUISystem::CFrontEndManager::IsScreenOnStack(NUISystem::CComponent* screen) const -- walk a copy of the screen stack from the top and report whether the screen is on it
// Leaf: IsScreenOnStack@CFrontEndManager
// Retail: Fable.exe (TLC, VC7.1) @ 0x005959AB, 88 bytes, `ret 4` (thiscall; bool in al).
// Name: FableWin.pdb `?IsScreenOnStack@CFrontEndManager@NUISystem@@QBE_NPAVCComponent@2@@Z`. Only caller 0x00597CA2.
// Callees: deque<CComponent*>::deque(const deque&) @ 0x0059B4FE (the local stack's implicit copy ctor),
// deque::back() @ 0x0059B039 (stack::top; it calls the iterator's _M_decrement 0x0059AC41), deque::_M_pop_back_aux() @ 0x0059AD35 (the slow arm of the inline
// pop_back), deque::~deque() @ 0x0059AC77 (the local's implicit destructor).
// Label: the catalog "CShaderRenderManager_IterateElements" is propagated and wrong.
// Body: `stack<CComponent*> screens = m_stackScreens; while (!screens.empty()) { if (screens.top() == screen)
// return true; screens.pop(); } return false;` -- the result sits in bl across the copy's destructor.
// Shape: the copy at [ebp-28h]; `empty()` re-reads `_M_finish._M_cur` ([ebp-18h], kept in esi) against
// `_M_start._M_cur` (cached across the out-of-line back(), whose body and _M_decrement's must be visible); here pop_back is inline (`cmp esi,[ebp-14h] ; sub esi,4`), unlike ClearComponents 0x0059525D
// whose member stack calls the out-of-line copy 0x0059AEE5. Frame-pointer size TU: `optimize("y", off)` +
// `optimize("s", on)`.
// Layout: retail CFrontEndManager m_stackScreens +0x20 (STLport deque of CComponent*, 32 per 128-byte node); local
// model as in ClearComponents 0x0059525D (engine/CFrontEndManager.h is the Ego_r.pdb layout).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>

namespace NUISystem {
class CComponent;
}

namespace std {
template <class T> struct _Deque_iterator {
    T*  _M_cur;
    T*  _M_first;
    T*  _M_last;
    T** _M_node;
    void _M_set_node(T** new_node) { _M_last = (_M_first = *(_M_node = new_node)) + 32; }   // 32 per 128-byte node
    void _M_decrement();                             // 0x0059AC41 (body below)
    _Deque_iterator& operator--() { _M_decrement(); return *this; }
    T& operator*() const { return *_M_cur; }
};
template <class T> class deque {
public:
    _Deque_iterator<T> _M_start;                     // +0x00
    _Deque_iterator<T> _M_finish;                    // +0x10
    T**                _M_map;                       // +0x20
    size_t             _M_map_size;                  // +0x24
    deque(const deque& x);                           // 0x0059B4FE
    ~deque();                                        // 0x0059AC77
    bool empty() const { return _M_finish._M_cur == _M_start._M_cur; }
    T& back();                                       // 0x0059B039 (body below)
    void _M_pop_back_aux();                          // 0x0059AD35
    void pop_back()
    {
        if (_M_finish._M_cur != _M_finish._M_first)
            --_M_finish._M_cur;
        else
            _M_pop_back_aux();
    }
};
// Same-TU stand-ins for deque::back() and the iterator's _M_decrement() (their STLport bodies; 0x0059B039 and
// 0x0059AC41 sit in this TU): with both visible VC7.1 knows the call leaves the copy's `_M_finish._M_cur` alone and
// keeps it in esi across the call, as retail.
#pragma auto_inline(off)
template <class T> void _Deque_iterator<T>::_M_decrement()
{
    if (_M_cur == _M_first) {
        _M_set_node(_M_node - 1);
        _M_cur = _M_last;
    }
    --_M_cur;
}
template <class T> T& deque<T>::back()
{
    _Deque_iterator<T> __tmp = _M_finish;
    --__tmp;
    return *__tmp;
}
#pragma auto_inline(on)

template <class T, class C = deque<T> > class stack {
public:
    C c;
    bool empty() const { return c.empty(); }
    T& top() { return c.back(); }
    void pop() { c.pop_back(); }
};
}

namespace NUISystem {
class CFrontEndManager {
public:
    unsigned char           _pad00[0x20];            // vtable, MediaPlayerErrorString, ...
    std::stack<CComponent*> m_stackScreens;          // +0x20
    bool IsScreenOnStack(CComponent* screen) const;
};

bool CFrontEndManager::IsScreenOnStack(CComponent* screen) const
{
    std::stack<CComponent*> screens = m_stackScreens;
    while (!screens.empty()) {
        if (screens.top() == screen)
            return true;
        screens.pop();
    }
    return false;
}
}