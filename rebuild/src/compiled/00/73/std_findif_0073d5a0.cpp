// std::__find_if(_List_iterator<CCountedPointer<CModeBase> >, _List_iterator<...>, CModeBase::CIsType, const input_iterator_tag&) -- first creature mode in the list whose GetType() equals the predicate's type
// Leaf: ??$__find_if@U?$_List_iterator@V?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@U?$_Nonconst_traits@V?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@std@@@std@@VCIsType@CModeBase@CTCCreatureModeManager@@@std@@YI?AU?$_List_iterator@V?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@U?$_Nonconst_traits@V?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@std@@@0@U10@0VCIsType@CModeBase@CTCCreatureModeManager@@ABUinput_iterator_tag@0@@Z
// Retail: Fable.exe @ 0x0073D5A0, 51 bytes, `ret 0Ch` (link-time __fastcall: ecx = the hidden iterator result,
//   edx = the iterator-tag reference; first, last and the 4-byte predicate on the stack).
// Identity (probable): the STLport 4.6 `__find_if(first, last, pred, const input_iterator_tag&)` loop
//   (`while (first != last && !pred(*first)) ++first; return first;`); its only caller is the out-of-line
//   std::find_if 0x0073DCE0 (it passes the address of an argument slot as the tag). The element is
//   CCountedPointer<CTCCreatureModeManager::CModeBase> (node data +8, the counted pointer's object word first) and the
//   predicate calls virtual slot 1, CModeBase::GetType() const (pure in CModeBase's vtable 0x012673CC; e.g.
//   NCreatureMode::CModeSad::GetType) against its own dword: Ego_r's CModeBase::CIsType (`??0CIsType@CModeBase@
//   CTCCreatureModeManager@@QAE@J@Z`, FableWin find_if<..., CIsType> over the same list). The iterator traits
//   (const vs non-const) are not provable from the bytes; _Nonconst_traits is used.
class CThing;
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;                       // +0x00
    CCPPointerInfo* Info;                       // +0x04
    T* operator->() const { return Data; }
};
class CTCCreatureModeManager { public:
    class CModeBase { public:
        virtual ~CModeBase();
        virtual unsigned long GetType() const = 0;     // slot 1
        class CIsType { public:
            unsigned long Type;                         // descriptive member name
            CIsType(long type) : Type(type) {}
            bool operator()(const CCountedPointer<CModeBase>& mode) const { return mode->GetType() == Type; }
        };
    };
};
typedef CCountedPointer<CTCCreatureModeManager::CModeBase> TModePtr;

namespace std {
struct input_iterator_tag { input_iterator_tag() {} };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp  value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef typename _Traits::reference reference;
    _List_iterator(_List_node<_Tp>* __x) : _List_iterator_base(__x) {}
    _List_iterator(const _Self& __x) : _List_iterator_base(__x._M_node) {}
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
};

template <class _InputIter, class _Predicate>
_InputIter __fastcall __find_if(_InputIter __first, _InputIter __last, _Predicate __pred, const input_iterator_tag&)
{
    while (__first != __last && !__pred(*__first))
        ++__first;
    return __first;
}
template _List_iterator<TModePtr, _Nonconst_traits<TModePtr> > __fastcall
__find_if(_List_iterator<TModePtr, _Nonconst_traits<TModePtr> >, _List_iterator<TModePtr, _Nonconst_traits<TModePtr> >,
          CTCCreatureModeManager::CModeBase::CIsType, const input_iterator_tag&);
}