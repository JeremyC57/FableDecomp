// Behaviour test for NUISystem::CManager::RemoveThingActionParam (0x0041D8A8): the first node holding the value is
// unlinked from the +0x5C list<long> and freed; an absent value leaves the list untouched; duplicates lose only one.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE __declspec(noinline)
#endif

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> struct _List_node : public _List_node_base { T _M_data; };
template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* node) : _M_node(node) {}
    _List_iterator(const _List_iterator& other) : _M_node(other._M_node) {}
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator==(const _List_iterator& other) const { return _M_node == other._M_node; }
    bool operator!=(const _List_iterator& other) const { return _M_node != other._M_node; }
};
template <class T> class list {
public:
    typedef _List_iterator<T> iterator;
    _List_node_base* _M_node;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    iterator erase(iterator position)               // 0x00429B02: STLport unlink + free
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
template <class _InputIter, class _Tp>
NOINLINE _InputIter __fastcall __find(_InputIter __first, _InputIter __last, const _Tp& __val, const input_iterator_tag&)
{
    while (__first != __last && !(*__first == __val))
        ++__first;
    return __first;
}
template <class _InputIter, class _Tp>
NOINLINE _InputIter __fastcall find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, input_iterator_tag());
}
namespace NUISystem {
    class CActionParam;
    class CManager {
    public:
        virtual void RemoveThingActionParam(long param);
        CManager() {}
        unsigned char _members[0x5c - 4];
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
typedef _List_node<long> SNode;
static void PushBack(list<long>& l, long v)
{
    SNode* n = (SNode*)malloc(sizeof(SNode));
    _List_node_base* head = l._M_node;
    n->_M_data = v; n->_M_next = head; n->_M_prev = head->_M_prev;
    head->_M_prev->_M_next = n; head->_M_prev = n;
}
static int Collect(list<long>& l, long* out)
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
    long got[8];
    _List_node_base header, uiHeader;
    NUISystem::CManager m;
    header._M_next = header._M_prev = &header;
    uiHeader._M_next = uiHeader._M_prev = &uiHeader;
    memset(m._members, 0, sizeof(m._members));
    m.ThingActionParams._M_node = &header;
    m.UIActionParams._M_node = &uiHeader;
    PushBack(m.ThingActionParams, 5); PushBack(m.ThingActionParams, 7); PushBack(m.ThingActionParams, 9); PushBack(m.ThingActionParams, 7);
    m.RemoveThingActionParam(7);
    if (Collect(m.ThingActionParams, got) != 3 || got[0] != 5 || got[1] != 9 || got[2] != 7) { printf("0041d8a8_TEST FAIL dup\n"); return 1; }
    m.RemoveThingActionParam(42);
    if (Collect(m.ThingActionParams, got) != 3) { printf("0041d8a8_TEST FAIL absent\n"); return 1; }
    m.RemoveThingActionParam(5); m.RemoveThingActionParam(9); m.RemoveThingActionParam(7);
    if (Collect(m.ThingActionParams, got) != 0 || header._M_next != &header || header._M_prev != &header) { printf("0041d8a8_TEST FAIL empty\n"); return 1; }
    m.RemoveThingActionParam(5);
    if (uiHeader._M_next != &uiHeader) { printf("0041d8a8_TEST FAIL other list\n"); return 1; }
    printf("0041d8a8_TEST PASS\n");
    return 0;
}
