// NUISystem::CManager::RemoveUIActionParam(CActionParam* param) -- unlink one UI action param from the manager's list
// Retail 0x0041D1C8, 63 bytes: NUISystem::CManager vtable 0x01230134 slot 28. Ego_r.pdb declares
// ?RemoveUIActionParam@CManager@NUISystem@@UAEXPAVCActionParam@2@@Z next to RemoveThingActionParam(long); the
// retail slots are 24 AddThingActionParam (+0x5C list), 25 RemoveThingActionParam (+0x5C, 0x0041D8A8, this
// body's twin over list<long>), 27 AddUIActionParam (+0x60), 28 this, 29 ClearActionParams (clears +0x60 then
// +0x5C). The catalog label "CTCBoastingCrowdControl::OnKillCrowd" is propagated (that class is 0x24 bytes).
// Body (STLport): it = find(UIActionParams.begin(), UIActionParams.end(), param); if (it != end()) erase(it);
//   find   @ 0x0042A2EC -- `iterator __fastcall find(iterator first, iterator last, const T& value)` (ecx = hidden
//            result, edx = &value, the by-value iterators built in their stack slots by the user copy ctor);
//   erase  @ 0x00429AB9 -- `iterator list::erase(iterator)` (unlinks, free()s the node, result by hidden pointer).
// Shape: frame-pointer size TU (`optimize("y",off)` + `("s",on)`, odd address); the found iterator lives in the
// `push ecx` frame word, the discarded erase result is homed in the dead `param` slot ([ebp+8]). That homing
// needs the STLport find / __find instantiations (COMDATs of this TU in retail: find 0x0042A2EC forwards to
// __find 0x004294AC with an input_iterator_tag temporary) as same-TU noinline template bodies that only READ
// the value through its reference; with find declared only, `&param` escapes and the erase result takes its
// own frame word (frame 8 v 4).
// Model: retail offsets (the STLport map Keys is 12 bytes and the lists 4, so the PDB's +0x70/+0x7C lists sit at
// +0x5C/+0x60); engine/CManager.h carries the PDB layout and does not fit, hence the local view.
// Leaf: ?RemoveUIActionParam@CManager@NUISystem

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
    iterator erase(iterator position);              // 0x00429AB9 (list<CActionParam*>)
};

struct input_iterator_tag { input_iterator_tag() {} };

#pragma auto_inline(off)
template <class _InputIter, class _Tp>
__declspec(noinline) _InputIter __fastcall __find(_InputIter __first, _InputIter __last, const _Tp& __val,
                                                  const input_iterator_tag&)      // 0x004294AC
{
    while (__first != __last && !(*__first == __val))
        ++__first;
    return __first;
}

template <class _InputIter, class _Tp>
__declspec(noinline) _InputIter __fastcall find(_InputIter __first, _InputIter __last, const _Tp& __val)   // 0x0042A2EC
{
    return __find(__first, __last, __val, input_iterator_tag());
}
#pragma auto_inline(on)

namespace NUISystem
{
    class CManager
    {
    public:
        virtual void RemoveUIActionParam(CActionParam* param);
        unsigned char _members[0x5c - 4];           // +0x04..+0x5B (banks, input, inventories, player)
        list<long> ThingActionParams;               // +0x5C
        list<CActionParam*> UIActionParams;         // +0x60
    };

    void CManager::RemoveUIActionParam(CActionParam* param)
    {
        list<CActionParam*>::iterator it = find(UIActionParams.begin(), UIActionParams.end(), param);
        if (it != UIActionParams.end())
            UIActionParams.erase(it);
    }
}
