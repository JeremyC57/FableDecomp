// std::vector::_M_fill_insert(pointer position, size_type n, const value_type& x) -- (vector<CClassTypeInfo>) STLport 4.6 insert of n copies of a non-POD 20-byte class: shift the tail inside the spare capacity, else grow through the __false_type _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCClassTypeInfo@@V?$allocator@VCClassTypeInfo@@@std@@@std@@QAEXPAVCClassTypeInfo@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A37F40, 311 bytes, thiscall `ret 0Ch`.
// Identity (proven): settled symbol (work/SETTLED_NAMES_2026-10-28.tsv, public QAE as STLport 4.6 declares it); its only
//   caller is the landed vector<CClassTypeInfo>::resize(n, x) 0x00A380E0 (CClassFactory::ClassTypeInfo), its overflow
//   callee the landed vector<CClassTypeInfo>::_M_insert_overflow 0x00A37E20, the 20-byte stride (66666667h, sar 3) is
//   retail's CClassTypeInfo and x_copy's implicit destructor calls ~CCharString (Name +0x10) then ~_List_base<CClass*>
//   (TypeList +0). The catalog label "Vector_Insert_CCharStringPair" is propagated.
// Callees: CClassTypeInfo(const CClassTypeInfo&) 0x00A37B50; __uninitialized_copy<CClassTypeInfo*, CClassTypeInfo*>
//   0x00A37D50; __copy_backward<CClassTypeInfo*, CClassTypeInfo*, int> 0x00A37A80 (random-access tag, (int*)0);
//   uninitialized_fill_n<CClassTypeInfo*, unsigned int, CClassTypeInfo> 0x00A37D80; fill<CClassTypeInfo*,
//   CClassTypeInfo> 0x00A37BE0; ~CCharString 0x0099EAE0; ~_List_base<CClass*> 0x00A376A0; _M_insert_overflow 0x00A37E20.
// Shape: STLport 4.6 vector.c verbatim, /Gr TU (the algorithm templates are __fastcall). The four out-of-line helpers
//   sit next to this function in retail (0x00A37A80..0x00A37D80, same TU) and are given same-TU noinline bodies under
//   auto_inline(off): declared only, VC7.1 homes `old_finish - n` in the dead &x slot instead of retail's dead n slot.
//   At landing the retail COMDATs of those helpers win (their bodies here are STLport 4.6 verbatim).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CClass;

class CCharString {
public:
    void* PStringData;                                      // +0x00
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};

namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
    ~_List_base();                                          // 0x00A376A0
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    list<_Tp, _Alloc>& operator=(const list<_Tp, _Alloc>& __x);   // 0x00A37940
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                   // 0x00A37D50
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{                                                                                   // 0x00A37D80
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                   // 0x00A37A80
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                   // 0x00A37BE0
    for (; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_backward_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x00A37E20
public:
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        } else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}
}

typedef CClass* (__cdecl* ClassAllocFunc)();

class CClassTypeInfo {
public:
    CClassTypeInfo(const CClassTypeInfo& other);            // 0x00A37B50
    std::list<CClass*>  TypeList;                           // +0x00
    unsigned long       NoAllocated;                        // +0x04
    bool                Used;                               // +0x08
    bool                Local;                              // +0x09
    ClassAllocFunc      AllocFunc;                          // +0x0C
    CCharString         Name;                               // +0x10
};

template void std::vector<CClassTypeInfo>::_M_fill_insert(CClassTypeInfo*, unsigned int, const CClassTypeInfo&);