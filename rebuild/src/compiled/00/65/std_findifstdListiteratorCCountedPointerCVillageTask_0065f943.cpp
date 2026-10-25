// std::find_if<_List_iterator<CCountedPointer<CVillageTaskTorch> >,CIsThingInvolvedWithTask>(first, last, pred) -- STLport
// find_if: dispatch to __find_if with the input_iterator_tag
// Leaf: ??$find_if@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065F943, 42 bytes, `__fastcall` (the /Gr algorithm TU: hidden result pointer in
//   ecx, both iterators and the 4-byte predicate on the stack, `ret 0Ch`); the empty tag is homed in the high byte of the
//   predicate's slot (`lea edx,[ebp+13h]`) and passed by reference in edx (bidirectional -> const input_iterator_tag&); each 4-byte iterator (user copy constructor)
//   is copy-built in its reserved slot (`push ecx ; mov eax,esp ; mov [eax],ecx`).
// Identity (FINDINGS chunk 383; was filed as unattributed Construct_CRangeString, a string range-constructor stand-in):
//   its callee is the landed std::__find_if<_List_iterator<CCountedPointer<CVillageTaskTorch> >,CIsThingInvolvedWithTask>
//   0x0065EA75 (same hidden-result __fastcall convention, `ret 0Ch`), and its caller, the landed
//   CTCVillage::IsThingInTaskList<CVillageTaskTorch> 0x0065FF5E (Ego_r.pdb name set), already declares it as
//   `std::find_if(tasks.begin(), tasks.end(), CIsThingInvolvedWithTask(thing))` on TorchTasks. The type models are those
//   two landed TUs' (one-argument _List_iterator), so all three bind the same symbols.
// Shape: STLport 4.x `find_if(first, last, pred) { return __find_if(first, last, pred, _STLP_ITERATOR_CATEGORY(first,
//   _InputIter)); }`; the tag has a user-declared constructor (repo idiom) so the temporary is not zero-filled.
// Opt: frame-pointer size TU (`optimize("y", off)` + `optimize("s", on)`, as the landed stand-in).
#pragma optimize("y", off)
#pragma optimize("s", on)

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
_InputIter __fastcall find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    return __find_if(__first, __last, __pred, typename _InputIter::iterator_category());
}

template _List_iterator<CCountedPointer<CVillageTaskTorch> > __fastcall find_if(_List_iterator<CCountedPointer<CVillageTaskTorch> >,
    _List_iterator<CCountedPointer<CVillageTaskTorch> >, CIsThingInvolvedWithTask);
}
