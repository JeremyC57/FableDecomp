// Behaviour test for 0x006991F0 CGameCameraManager::AddCameraPoint(thing): append the thing to AvailableCamPoints unless
// it is already there; adding to an empty list sets NewAutoMode.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#endif
class CBaseObject;
class CThing;

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* m_pItem;                                   // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                             // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for T = CThing
public:
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};
template <class T>
inline bool operator==(const CIntelligentPointer<T>& a, const CIntelligentPointer<T>& b) { return a.GetPItem() == b.GetPItem(); }

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };

template <class _Tp> class _Nonconst_traits { };

class _List_node_base {
public:
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> class _List_node : public _List_node_base {
public:
    _Tp _M_data;
};
class _List_iterator_base {
public:
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> class _List_iterator : public _List_iterator_base {
public:
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_incr(); return *this; }
};
template <class _Tp, class _Traits>
inline bidirectional_iterator_tag iterator_category(const _List_iterator<_Tp, _Traits>&)
{
    return bidirectional_iterator_tag();
}

template <class _InputIter, class _Tp>
__declspec(noinline) _InputIter __fastcall __find(_InputIter __first, _InputIter __last, const _Tp& __val,
                                                  const input_iterator_tag&)
{
    while (__first != __last && !(*__first == __val))
        ++__first;
    return __first;
}
template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, iterator_category(__first));
}

template <class _Tp> class allocator { };

template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _List_node<_Tp>* _M_node;                               // header node
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    bool empty() const { return _M_node->_M_next == _M_node; }
    void push_back(const _Tp& __x);                         // 0x004BA4E0
};
}

static int g_live = 0, g_pushes = 0;
void CBaseIntelligentPointer::v00() { }
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { ++g_live; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { --g_live; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = item; }
template <> void CIntelligentPointer<CThing>::v00() { }
namespace std {
template <> void list<CIntelligentPointer<CThing> >::push_back(const CIntelligentPointer<CThing>& x)
{
    ++g_pushes;
    _List_node<CIntelligentPointer<CThing> >* n = (_List_node<CIntelligentPointer<CThing> >*)malloc(sizeof(*n));
    new (&n->_M_data) CIntelligentPointer<CThing>(x);
    n->_M_next = _M_node; n->_M_prev = _M_node->_M_prev;
    _M_node->_M_prev->_M_next = n; _M_node->_M_prev = n;
}
}

class CGameCameraManager {
public:
    unsigned char _pad00[0xA8];
    bool NewAutoMode;                                       // +0xA8
    unsigned char _padA9[3];
    std::list<CIntelligentPointer<CThing> > AvailableCamPoints;   // +0xAC
    void AddCameraPoint(CThing& thing);
};

void CGameCameraManager::AddCameraPoint(CThing& thing)
{
    std::list<CIntelligentPointer<CThing> >::iterator it =
        std::find(AvailableCamPoints.begin(), AvailableCamPoints.end(), CIntelligentPointer<CThing>(&thing));
    if (it == AvailableCamPoints.end()) {
        if (AvailableCamPoints.empty())
            NewAutoMode = true;
        AvailableCamPoints.push_back(CIntelligentPointer<CThing>(&thing));
    }
}

int main()
{
    CGameCameraManager* m = (CGameCameraManager*)malloc(sizeof(CGameCameraManager));
    memset(m, 0, sizeof(*m));
    std::_List_node<CIntelligentPointer<CThing> >* head = (std::_List_node<CIntelligentPointer<CThing> >*)malloc(sizeof(*head));
    head->_M_next = head->_M_prev = head;
    m->AvailableCamPoints._M_node = head;
    int things[3];
    int ok = 1;
    m->AddCameraPoint(*(CThing*)&things[0]);                // empty list: added, auto mode requested
    if (g_pushes != 1 || !m->NewAutoMode) ok = 0;
    m->NewAutoMode = false;
    m->AddCameraPoint(*(CThing*)&things[1]);                // new thing: added, list was not empty
    if (g_pushes != 2 || m->NewAutoMode) ok = 0;
    m->AddCameraPoint(*(CThing*)&things[0]);                // already present: nothing
    m->AddCameraPoint(*(CThing*)&things[1]);
    if (g_pushes != 2 || m->NewAutoMode) ok = 0;
    m->AddCameraPoint(*(CThing*)&things[2]);
    if (g_pushes != 3) ok = 0;
    std::_List_node_base* n = head->_M_next;
    for (int k = 0; k < 3 && ok; ++k, n = n->_M_next)
        if (((std::_List_node<CIntelligentPointer<CThing> >*)n)->_M_data.GetPItem() != (CBaseObject*)&things[k]) ok = 0;
    if (n != head) ok = 0;
    if (g_live != 0) ok = 0;                                // every pointer temporary was destroyed again (stored copies use the implicit copy ctor)
    printf(ok ? "006991f0_TEST PASS\n" : "006991f0_TEST FAIL\n");
    return ok ? 0 : 1;
}