// std::__stable_sort_aux(CEngineSubPrimitiveRenderer::CSortElement* first, CEngineSubPrimitiveRenderer::CSortElement* last, CSortElement*, int*, less<CSortElement> comp) -- stable sort through a temporary buffer, in place when none can be allocated
// Leaf: ??$__stable_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B86710, 139 bytes. __fastcall (first ecx, last edx), `ret 0Ch` (the
//   value-type and distance-type tag pointers and the empty comparator on the stack).
// Callees: _Temporary_buffer::_M_allocate_buffer @ 0x00B84E20, std::__inplace_stable_sort<...> @ 0x00B86570,
//   std::__stable_sort_adaptive<...> @ 0x00B865E0, free (CRT import thunk) @ 0x00BFEA14.
// Callers: CEngineSubPrimitiveRenderer::SortList 0x00B84990 (call 0x00B849B3: `stable_sort(list.begin(), list.end())`
//   when the list RequiresStableSort) and 0x00B868B9.
// Identity: the manifest label "IntroSort_Driver" is descriptive and wrong (stable sort, not introsort); STLport
//   `__stable_sort_aux` (the out-of-line body of the inline stable_sort(first, last)).
//   The element is CEngineSubPrimitiveRenderer::CSortElement (8 bytes: unsigned long SortValue, CEngineSubPrimitiveBase*
//   SubPrimitive; a user default constructor makes it non-POD for STLport's __type_traits) and the comparator is
//   less<CSortElement> (CSortElement::operator< compares SortValue unsigned). This is the STLport sort / stable_sort
//   chain of the CEngineSubPrimitiveRenderer TU (SortList 0x00B84990: stable_sort when the list RequiresStableSort, else
//   sort below 256 elements, else RadixSort 0x00B847D0); Ego_r.pdb / FableWin.pdb carry the Dinkumware twins
//   (_Stable_sort, _Buffered_merge_sort, _Make_heap, _Sort_heap, lower_bound, upper_bound ... over CSortElement*).
//   Modelled in namespace std (lane convention; retail RTTI spells std, `pair@...@std@@`); the templates are __fastcall.
// Shape: STLport `__stable_sort_aux` with the _Temporary_buffer constructor (distance, _M_allocate_buffer, then
//   `uninitialized_fill_n(_M_buffer, _M_len, *__first)` -- the __false_type placement loop with its null test, because
//   CSortElement has a user default constructor) and destructor (trivial _Destroy, free) inlined; VC7.1 duplicates the
//   free tail into both branches. The three callees are same-TU stand-in bodies under auto_inline(off) (the STLport text
//   of 0x00B84E20, 0x00B86570, 0x00B865E0) as in the landed 0x00711B80.
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (import thunk)

struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };

class CEngineSubPrimitiveBase;

class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {                                // 8 bytes (Ego_r.pdb / FableWin.pdb)
    public:
        unsigned long SortValue;                        // +0x00
        CEngineSubPrimitiveBase* SubPrimitive;          // +0x04
        CSortElement() {}
        CSortElement(unsigned long sortValue, CEngineSubPrimitiveBase* subPrimitive)
            : SortValue(sortValue), SubPrimitive(subPrimitive) {}
        bool operator<(const CSortElement& other) const { return SortValue < other.SortValue; }
    };
};
typedef CEngineSubPrimitiveRenderer::CSortElement CSortElement;

namespace std {
template <class _Tp> struct less {                      // empty functor (a 4-byte argument slot)
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
}

namespace std {
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) {}   // trivial destructor: nothing to run

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        new (&*__cur) _Tp(__x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _ForwardIterator, class _Tp>
class _Temporary_buffer {
private:
    int _M_original_len;
    int _M_len;
    _Tp* _M_buffer;
    void _M_allocate_buffer();                          // 0x00B84E20
    void _M_initialize_buffer(const _Tp&, const __true_type&) {}
    void _M_initialize_buffer(const _Tp& __val, const __false_type&) { uninitialized_fill_n(_M_buffer, _M_len, __val); }
public:
    int size() const { return _M_len; }
    _Tp* begin() { return _M_buffer; }
    _Tp* end() { return _M_buffer + _M_len; }
    _Temporary_buffer(_ForwardIterator __first, _ForwardIterator __last)
    {
        _M_len = __last - __first;
        _M_allocate_buffer();
        if (_M_len > 0)
            _M_initialize_buffer(*__first, __false_type());
    }
    ~_Temporary_buffer()
    {
        _Destroy(_M_buffer, _M_buffer + _M_len);
        free(_M_buffer);
    }
};

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00B85B30
template <class _BidirectionalIter, class _Distance, class _Compare>
void __fastcall __merge_without_buffer(_BidirectionalIter __first, _BidirectionalIter __middle, _BidirectionalIter __last,
                                       _Distance __len1, _Distance __len2, _Compare __comp);         // 0x00B86140
template <class _RandomAccessIter, class _Pointer, class _Distance, class _Compare>
void __fastcall __merge_sort_with_buffer(_RandomAccessIter __first, _RandomAccessIter __last, _Pointer __buffer,
                                         _Distance*, _Compare __comp);                             // 0x00B86260
template <class _BidirectionalIter, class _Distance, class _Pointer, class _Compare>
void __fastcall __merge_adaptive(_BidirectionalIter __first, _BidirectionalIter __middle, _BidirectionalIter __last,
                                 _Distance __len1, _Distance __len2, _Pointer __buffer, _Distance __buffer_size,
                                 _Compare __comp);                                                 // 0x00B862C0

// Same-TU stand-in bodies (the STLport text of 0x00B84E20 / 0x00B86570 / 0x00B865E0, all in this TU): VC7.1 uses what it
// learns from them.
#pragma auto_inline(off)
template <class _ForwardIterator, class _Tp>
__declspec(noinline) void _Temporary_buffer<_ForwardIterator, _Tp>::_M_allocate_buffer()
{
    _M_original_len = _M_len;
    _M_buffer = 0;
    if (_M_len > (int)(0x7FFFFFFF / sizeof(_Tp)))
        _M_len = 0x7FFFFFFF / sizeof(_Tp);
    while (_M_len > 0) {
        _M_buffer = (_Tp*)malloc(_M_len * sizeof(_Tp));
        if (_M_buffer)
            break;
        _M_len /= 2;
    }
}
template <class _RandomAccessIter, class _Compare>
__declspec(noinline) void __fastcall __inplace_stable_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first < 15) {
        __insertion_sort(__first, __last, __comp);
        return;
    }
    _RandomAccessIter __middle = __first + (__last - __first) / 2;
    __inplace_stable_sort(__first, __middle, __comp);
    __inplace_stable_sort(__middle, __last, __comp);
    __merge_without_buffer(__first, __middle, __last, __middle - __first, __last - __middle, __comp);
}
template <class _RandomAccessIter, class _Pointer, class _Distance, class _Compare>
__declspec(noinline) void __fastcall __stable_sort_adaptive(_RandomAccessIter __first, _RandomAccessIter __last, _Pointer __buffer,
                                                            _Distance __buffer_size, _Compare __comp)
{
    _Distance __len = (__last - __first + 1) / 2;
    _RandomAccessIter __middle = __first + __len;
    if (__len > __buffer_size) {
        __stable_sort_adaptive(__first, __middle, __buffer, __buffer_size, __comp);
        __stable_sort_adaptive(__middle, __last, __buffer, __buffer_size, __comp);
    }
    else {
        __merge_sort_with_buffer(__first, __middle, __buffer, (_Distance*)0, __comp);
        __merge_sort_with_buffer(__middle, __last, __buffer, (_Distance*)0, __comp);
    }
    __merge_adaptive(__first, __middle, __last, _Distance(__middle - __first), _Distance(__last - __middle), __buffer,
                     __buffer_size, __comp);
}
#pragma auto_inline(on)

template <class _RandomAccessIter, class _Tp, class _Distance, class _Compare>
void __fastcall __stable_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Distance*, _Compare __comp)
{
    _Temporary_buffer<_RandomAccessIter, _Tp> buf(__first, __last);
    if (buf.begin() == 0)
        __inplace_stable_sort(__first, __last, __comp);
    else
        __stable_sort_adaptive(__first, __last, buf.begin(), _Distance(buf.size()), __comp);
}

template void __fastcall __stable_sort_aux<CSortElement*, CSortElement, int, less<CSortElement> >(CSortElement*,
    CSortElement*, CSortElement*, int*, less<CSortElement>);
}