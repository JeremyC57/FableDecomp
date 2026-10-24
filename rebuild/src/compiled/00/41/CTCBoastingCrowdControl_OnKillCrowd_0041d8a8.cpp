// NUISystem::CManager::RemoveThingActionParam(long param) -- erase the first matching entry from ThingActionParams
// Leaf: RemoveThingActionParam
// Retail: Fable.exe @ 0x0041D8A8, 63 bytes, NUISystem::CManager vtable 0x01230134 slot 25 (the only reference),
// __thiscall `ret 4`; the list<long> twin of the landed RemoveUIActionParam 0x0041D1C8 (slot 28, list<CActionParam*>).
// The manifest label CTCBoastingCrowdControl::OnKillCrowd is propagated; the name is the Ego_r.pdb slot neighbour.
// Body (STLport): it = find(ThingActionParams.begin(), ThingActionParams.end(), param); if (it != end()) erase(it);
//   find  @ 0x0042B2EA -- `iterator __fastcall find(iterator first, iterator last, const long& value)` (ecx = hidden
//           result, edx = &value, the by-value iterators built in their stack slots by the user copy ctor);
//   erase @ 0x00429B02 -- `iterator list<long>::erase(iterator)` (result by hidden pointer).
// Shape: frame-pointer size TU (`optimize("y",off)` + `("s",on)`); the discarded erase result is homed in the dead
// `param` slot ([ebp+8]) only when find / __find are same-TU noinline template bodies that only READ the value
// through its reference (with find declared only, `&param` escapes and the result takes its own frame word, 64v63).
// Model: retail offsets (STLport lists are 4 bytes; the PDB layout in engine/CManager.h does not fit), local view.

#pragma optimize("y", off)
#pragma optimize("s", on)

namespace NUISystem { class CActionParam; }

struct _List_node_base {
    _List_node_base* _M_next;                       // +0x00
    _List_node_base* _M_prev;                       // +0x04
};

template <class T>
struct _List_node : public _List_node_base {
    T _M_data;                                      // +0x08
};

template <class T>
struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* node) : _M_node(node) {}
    _List_iterator(const _List_iterator& other) : _M_node(other._M_node) {}
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator==(const _List_iterator& other) const { return _M_node == other._M_node; }
    bool operator!=(const _List_iterator& other) const { return _M_node != other._M_node; }
};

template <class T>
class list {
public:
    typedef _List_iterator<T> iterator;
    _List_node_base* _M_node;                       // +0x00 header node (STLport alloc proxy, 4 bytes)
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    iterator erase(iterator position);              // 0x00429B02 (list<long>)
};

struct input_iterator_tag { input_iterator_tag() {} };

#pragma auto_inline(off)
template <class _InputIter, class _Tp>
__declspec(noinline) _InputIter __fastcall __find(_InputIter __first, _InputIter __last, const _Tp& __val,
                                                  const input_iterator_tag&)      // 0x0042A77F (list<long> instantiation)
{
    while (__first != __last && !(*__first == __val))
        ++__first;
    return __first;
}

template <class _InputIter, class _Tp>
__declspec(noinline) _InputIter __fastcall find(_InputIter __first, _InputIter __last, const _Tp& __val)   // 0x0042B2EA (list<long> instantiation)
{
    return __find(__first, __last, __val, input_iterator_tag());
}
#pragma auto_inline(on)

namespace NUISystem
{
    class CManager
    {
    public:
        virtual void RemoveThingActionParam(long param);
        unsigned char _members[0x5c - 4];           // +0x04..+0x5B (banks, input, inventories, player)
        list<long> ThingActionParams;               // +0x5C
        list<CActionParam*> UIActionParams;         // +0x60
    };

    void CManager::RemoveThingActionParam(long param)
    {
        list<long>::iterator it = find(ThingActionParams.begin(), ThingActionParams.end(), param);
        if (it != ThingActionParams.end())
            ThingActionParams.erase(it);
    }
}
