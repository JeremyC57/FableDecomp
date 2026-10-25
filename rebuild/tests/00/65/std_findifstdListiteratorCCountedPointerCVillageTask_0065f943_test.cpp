// Test for 0x0065F943 std::find_if<_List_iterator<CCountedPointer<CVillageTaskTorch> >,CIsThingInvolvedWithTask>: the
// source object's find_if must hand (first, last, pred) to __find_if (stubbed here) and return its result.
#include <stdio.h>
class CThing;

class CVillageTaskBase;
class CVillageTaskTorch;

template <class T> class CCountedPointer {
public:
    T*    m_pObject;                                  // +0x00
    void* m_pInfo;                                    // +0x04 the shared count block
};

class CIsThingInvolvedWithTask {                      // Ego_r.pdb, 4 bytes
public:
    const CThing* Thing;                              // +0x00
};

namespace std {
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };

struct _List_node_base {
    _List_node_base* _M_next;                         // +0x00
    _List_node_base* _M_prev;                         // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                      // +0x08
};
template <class _Tp> struct _List_iterator {
    typedef bidirectional_iterator_tag iterator_category;
    _List_node_base* _M_node;
    _List_iterator(_List_node<_Tp>* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};

template <class _InputIter, class _Predicate>
_InputIter __fastcall __find_if(_InputIter __first, _InputIter __last, _Predicate __pred, const input_iterator_tag&);   // 0x0065EA75

template <class _InputIter, class _Predicate>
_InputIter __fastcall find_if(_InputIter __first, _InputIter __last, _Predicate __pred);
static int g_calls;
template <>
_List_iterator<CCountedPointer<CVillageTaskTorch> > __fastcall __find_if(_List_iterator<CCountedPointer<CVillageTaskTorch> > __first,
    _List_iterator<CCountedPointer<CVillageTaskTorch> > __last, CIsThingInvolvedWithTask __pred, const input_iterator_tag&)
{
    ++g_calls;
    while (__first._M_node != __last._M_node &&
           ((_List_node<CCountedPointer<CVillageTaskTorch> >*)__first._M_node)->_M_data.m_pInfo != (void*)__pred.Thing)
        __first._M_node = __first._M_node->_M_next;
    return __first;
}
}

int main()
{
    typedef std::_List_node<CCountedPointer<CVillageTaskTorch> > N;
    N s, a, b;
    s._M_next = &a; a._M_next = &b; b._M_next = &s;
    a._M_data.m_pInfo = (void*)0x10; b._M_data.m_pInfo = (void*)0x20;
    std::_List_iterator<CCountedPointer<CVillageTaskTorch> > first((N*)s._M_next), last(&s);
    CIsThingInvolvedWithTask p; p.Thing = (const CThing*)0x20;
    std::_List_iterator<CCountedPointer<CVillageTaskTorch> > r = std::find_if(first, last, p);
    if (r._M_node != &b || std::g_calls != 1) return 1;
    p.Thing = (const CThing*)0x30;
    r = std::find_if(first, last, p);
    if (r._M_node != &s || std::g_calls != 2) return 2;
    puts("0065f943_TEST PASS");
    return 0;
}
