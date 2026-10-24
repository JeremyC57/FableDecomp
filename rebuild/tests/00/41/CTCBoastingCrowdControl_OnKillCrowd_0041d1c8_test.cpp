// Behaviour test for NUISystem::CManager::RemoveUIActionParam (0x0041D1C8): the first node holding the param is
// unlinked from the +0x60 UI action-param list and freed; an absent param leaves the list untouched.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace NUISystem { class CActionParam { public: int id; }; }

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
    iterator erase(iterator position)               // 0x00429AB9 (list<CActionParam*>): STLport unlink + free
    {
        _List_node_base* next = position._M_node->_M_next;
        _List_node_base* prev = position._M_node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(position._M_node);
        return iterator(next);
    }
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
        CManager() {}
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

typedef _List_node<NUISystem::CActionParam*> SNode;

static SNode* PushBack(list<NUISystem::CActionParam*>& l, NUISystem::CActionParam* p)
{
    SNode* n = (SNode*)malloc(sizeof(SNode));
    _List_node_base* head = l._M_node;
    n->_M_data = p;
    n->_M_next = head;
    n->_M_prev = head->_M_prev;
    head->_M_prev->_M_next = n;
    head->_M_prev = n;
    return n;
}

static int Collect(list<NUISystem::CActionParam*>& l, NUISystem::CActionParam** out)
{
    int n = 0;
    _List_node_base* head = l._M_node;
    for (_List_node_base* p = head->_M_next; p != head; p = p->_M_next) {
        if (p->_M_next->_M_prev != p) return -1;
        out[n++] = ((SNode*)p)->_M_data;
    }
    return n;
}

int main()
{
    NUISystem::CActionParam a, b, c, d;
    NUISystem::CActionParam* got[8];
    _List_node_base header;
    _List_node_base thingHeader;
    NUISystem::CManager manager;
    header._M_next = header._M_prev = &header;
    thingHeader._M_next = thingHeader._M_prev = &thingHeader;
    memset(manager._members, 0, sizeof(manager._members));
    manager.UIActionParams._M_node = &header;
    manager.ThingActionParams._M_node = &thingHeader;
    PushBack(manager.UIActionParams, &a);
    PushBack(manager.UIActionParams, &b);
    PushBack(manager.UIActionParams, &c);

    manager.RemoveUIActionParam(&b);
    if (Collect(manager.UIActionParams, got) != 2 || got[0] != &a || got[1] != &c) { printf("FAIL middle\n"); return 1; }
    manager.RemoveUIActionParam(&d);
    if (Collect(manager.UIActionParams, got) != 2 || got[0] != &a || got[1] != &c) { printf("FAIL absent\n"); return 1; }
    manager.RemoveUIActionParam(&a);
    manager.RemoveUIActionParam(&c);
    if (Collect(manager.UIActionParams, got) != 0 || header._M_next != &header || header._M_prev != &header) { printf("FAIL empty\n"); return 1; }
    manager.RemoveUIActionParam(&a);
    if (thingHeader._M_next != &thingHeader) { printf("FAIL other list\n"); return 1; }
    printf("PASS_0041d1c8\n");
    return 0;
}
