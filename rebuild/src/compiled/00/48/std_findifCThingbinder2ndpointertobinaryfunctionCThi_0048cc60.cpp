// std::__find_if<CThing**, binder2nd<pointer_to_binary_function<CThing*, CCharString, bool> > >(CThing** first, CThing** last, binder2nd pred, const random_access_iterator_tag&) -- STLport four-way unrolled random-access find_if: the first holy site whose ScriptName the bound name matches, else last
// Leaf: ??$__find_if@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048CC60, 490 bytes, /Gr `__fastcall` (first ecx, last edx), `ret 0Ch` (the 8-byte
//   binder by value + the tag reference). Callers: CPlayer::LocateCharacterStartHolySite 0x00488B20 (landed; its header
//   names this address "SGI __find_if 0x0048CC60" and carries the same body as a same-TU stand-in), 0x0048D037 (the
//   inline find_if wrapper) and 0x004A09C9. The catalog label "Vector_ConstructElements_Unrolled" is wrong.
// Callees: CCharString copy constructor @ 0x0099EC30, CCharString::~CCharString @ 0x0099EAE0; the bound function is
//   reached through the binder's pointer (0x0048BC70 `bool __fastcall IsThingScriptNamed(CThing*, CCharString)`).
// Shape: STLport `__find_if` verbatim in a /Gr TU. The predicate is bind2nd(ptr_fun(fn), name): binder2nd {op (the
//   function pointer) +0, value (CCharString) +4}. Each pred(*first) inlines binder2nd::operator() -> op(x, value), which
//   copies the bound name into the by-value parameter of pointer_to_binary_function::operator() (a frame temporary,
//   one per unrolled call site) and again into the call's argument slot, then destroys the temporary. The by-value
//   binder is destroyed by the callee on both exits (`lea ecx,[esp+28h] ; call ~CCharString`).
// Shape (argument order): retail loads the element (`mov ebx,[esi]`) BEFORE the bound-name copy; the binder's
//   operator() therefore takes its argument by value here (the STLport `const first_argument_type&` form reads the
//   element after the copy, right-to-left argument order of the inlined op(x, value)).
class CThing;

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
};

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _Arg1, class _Arg2, class _Result>
struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};

template <class _Arg1, class _Arg2, class _Result>
class pointer_to_binary_function : public binary_function<_Arg1, _Arg2, _Result> {
protected:
    _Result (*_M_ptr)(_Arg1, _Arg2);
public:
    pointer_to_binary_function() { }
    explicit pointer_to_binary_function(_Result (*__x)(_Arg1, _Arg2)) : _M_ptr(__x) { }
    _Result operator()(_Arg1 __x, _Arg2 __y) const { return _M_ptr(__x, __y); }
};

template <class _Operation>
class binder2nd {
protected:
    _Operation op;
    typename _Operation::second_argument_type value;
public:
    binder2nd(const _Operation& __x, const typename _Operation::second_argument_type& __y) : op(__x), value(__y) { }
    typename _Operation::result_type operator()(typename _Operation::first_argument_type __x) const
    {
        return op(__x, value);
    }
};

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                            const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;

    for ( ; __trip_count > 0 ; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;
    }

    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}

typedef binder2nd<pointer_to_binary_function<CThing*, CCharString, bool> > CScriptNameBinder;
template CThing** __find_if<CThing**, CScriptNameBinder>(CThing**, CThing**, CScriptNameBinder,
                                                          const random_access_iterator_tag&);
}