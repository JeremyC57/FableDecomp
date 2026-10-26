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
// Model of list<CCountedPointer<CModeBase> >::remove_if<CModeBase::CIsType> @ 0x0073E790: erase every mode of the
// predicate type, releasing the list's counted reference (the mode is deleted when it was the last one).

class CCPPointerInfo { public:                                     // Ego_r.pdb
    unsigned long RefCount;                                 // +0x0
    void (__fastcall* DeleteFunc)(void* data);              // +0x4
    void* Data;                                             // +0x8
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x0
    CCPPointerInfo* Info;                                   // +0x4
    T* operator->() const { return Data; }
    void ReleaseInfo()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
};

class CTCCreatureModeManager {
public:
    class CModeBase {
    public:
        virtual void v00() { }
        virtual unsigned long GetModeType() { return ModeType; }
        unsigned int ModeType;                // slot 1
        class CIsType {
        public:
            unsigned long Type;
            bool operator()(const CCountedPointer<CModeBase>& mode) const { return mode->GetModeType() == Type; }
        };
    };
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

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

template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator(__next_node);
    }
    template <class _Predicate> void remove_if(_Predicate __pred)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__pred(*__first))
                erase(__first);
            __first = __next;
        }
    }
};

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
    std::_List_node_base* s = l._M_node._M_data;
    n->_M_next = s;
    n->_M_prev = s->_M_prev;
    s->_M_prev->_M_next = n;
    s->_M_prev = n;
    return n;
}
template <class L> int CountList(L& l)
{
    int c = 0;
    for (std::_List_node_base* p = l._M_node._M_data->_M_next; p != l._M_node._M_data; p = p->_M_next)
        ++c;
    return c;
}
template <class L> bool LinksOk(L& l)
{
    std::_List_node_base* s = l._M_node._M_data;
    for (std::_List_node_base* p = s->_M_next; p != s; p = p->_M_next)
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
    typedef CTCCreatureModeManager::CModeBase Mode;
    typedef std::list<CCountedPointer<Mode> > L;
    L l;
    InitList(l);
    Mode modes[6];
    unsigned int types[6] = { 1, 2, 1, 3, 1, 2 };
    unsigned int refs[6] = { 1, 1, 2, 1, 1, 3 };
    CCPPointerInfo* infos[6];
    for (int i = 0; i < 6; ++i) {
        modes[i].ModeType = types[i];
        L::_Node* n = NewBack(l);
        n->_M_data.Data = &modes[i];
        infos[i] = NewInfo(&modes[i], refs[i]);
        n->_M_data.Info = infos[i];
    }
    Mode::CIsType pred;
    pred.Type = 1;
    l.remove_if(pred);
    unsigned int expect[3] = { 2, 3, 2 };
    int k = 0;
    for (L::iterator it = l.begin(); it != l.end(); ++it, ++k)
        if (k >= 3 || (*it)->ModeType != expect[k])
            ok = false;
    if (k != 3 || CountList(l) != 3 || !LinksOk(l) || gDeleted != 2 || infos[2]->RefCount != 1)
        ok = false;
    pred.Type = 9;
    l.remove_if(pred);
    if (CountList(l) != 3 || gDeleted != 2)
        ok = false;
    if (!ok) { printf("0073e790_TEST FAIL\n"); return 1; }
    printf("0073e790_TEST PASS\n");
    return 0;
}