#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Model of list<CCountedPointer<CQuestManager::CActiveQuest> >::_M_insert_dispatch(pos, first, last, __false_type) @
// 0x004B92D0: copy every counted quest pointer of [first, last) into a new node linked before pos (adding a reference).

struct CCPPointerInfo {                                     // Ego_r.pdb
    unsigned long RefCount;                                 // +0x0
    void (__fastcall* DeleteFunc)(void* data);              // +0x4
    void* Data;                                             // +0x8
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x0
    CCPPointerInfo* Info;                                   // +0x4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

class CQuestManager {
public:
    class CActiveQuest { public: int Id; };
};

namespace _STL {
struct __false_type { };

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
};
template <class _Tp> struct _Const_traits {
    typedef _Tp value_type;
    typedef const _Tp& reference;
    typedef const _Tp* pointer;
};

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    explicit _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;  // +0x0 sentinel
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    template <class _InputIter>
    void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, const __false_type&)
    {
        for (; __first != __last; ++__first)
            insert(__pos, *__first);
    }
};

typedef CCountedPointer<CQuestManager::CActiveQuest> _QuestPtr;
}

template <class L> void InitList(L& l)
{
    typedef typename L::_Node Node;
    Node* s = (Node*)malloc(sizeof(Node));
    s->_M_next = s;
    s->_M_prev = s;
    l._M_node._M_data = s;
}
template <class L> typename L::_Node* NewBack(L& l)
{
    typedef typename L::_Node Node;
    Node* n = (Node*)malloc(sizeof(Node));
    _STL::_List_node_base* s = l._M_node._M_data;
    n->_M_next = s;
    n->_M_prev = s->_M_prev;
    s->_M_prev->_M_next = n;
    s->_M_prev = n;
    return n;
}
template <class L> int CountList(L& l)
{
    int c = 0;
    for (_STL::_List_node_base* p = l._M_node._M_data->_M_next; p != l._M_node._M_data; p = p->_M_next)
        ++c;
    return c;
}
template <class L> bool LinksOk(L& l)
{
    _STL::_List_node_base* s = l._M_node._M_data;
    for (_STL::_List_node_base* p = s->_M_next; p != s; p = p->_M_next)
        if (p->_M_next->_M_prev != p || p->_M_prev->_M_next != p)
            return false;
    return s->_M_next->_M_prev == s && s->_M_prev->_M_next == s;
}

static int gDeleted = 0;
static void __fastcall CountDelete(void*) { ++gDeleted; }
static CCPPointerInfo* NewInfo(void* data, unsigned int refs)
{
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = refs;
    info->DeleteFunc = CountDelete;
    info->Data = data;
    return info;
}
int main()
{
    bool ok = true;
    typedef CCountedPointer<CQuestManager::CActiveQuest> P;
    typedef _STL::list<P> L;
    L from;
    InitList(from);
    L to;
    InitList(to);
    static CQuestManager::CActiveQuest quests[4];
    CCPPointerInfo* infos[3];
    for (int i = 0; i < 3; ++i) {
        L::_Node* n = NewBack(from);
        n->_M_data.Data = &quests[i];
        infos[i] = NewInfo(&quests[i], 1);
        n->_M_data.Info = infos[i];
    }
    L::_Node* tail = NewBack(to);
    tail->_M_data.Data = &quests[3];
    tail->_M_data.Info = 0;
    L::const_iterator first(from._M_node._M_data->_M_next);
    L::const_iterator last(from._M_node._M_data);
    to._M_insert_dispatch(L::iterator(tail), first, last, _STL::__false_type());
    int k = 0;
    for (_STL::_List_node_base* p = to._M_node._M_data->_M_next; p != to._M_node._M_data; p = p->_M_next, ++k)
        if (k >= 4 || ((L::_Node*)p)->_M_data.Data != &quests[k])
            ok = false;
    if (k != 4 || !LinksOk(to) || CountList(from) != 3)
        ok = false;
    for (int i = 0; i < 3; ++i)
        if (infos[i]->RefCount != 2)
            ok = false;
    to._M_insert_dispatch(L::iterator(to._M_node._M_data), last, last, _STL::__false_type());
    if (CountList(to) != 4)
        ok = false;
    if (!ok) { printf("004b92d0_TEST FAIL\n"); return 1; }
    printf("004b92d0_TEST PASS\n");
    return 0;
}