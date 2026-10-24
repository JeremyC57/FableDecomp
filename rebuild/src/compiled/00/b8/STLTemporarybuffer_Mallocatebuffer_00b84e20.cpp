// _STL::_Temporary_buffer::_M_allocate_buffer() -- (the buffer of CEngineSubPrimitiveRenderer::CSortElement for stable_sort) remember the requested length, clamp it to INT_MAX / 8 and try malloc, halving the length until a buffer is found or it reaches 0
// Leaf: ?_M_allocate_buffer@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B84E20, 75 bytes. __fastcall (this = {_M_original_len, _M_len,
//   _M_buffer}), plain `ret`.
// Callees: malloc (CRT import thunk) @ 0x00BFEA0E.
// Callers: the inlined _Temporary_buffer constructor in __stable_sort_aux 0x00B86710 (call 0x00B86729).
// Identity: the manifest label "Vector_AllocateWithFallback" is descriptive; STLport
//   `_Temporary_buffer<CSortElement*, CSortElement>::_M_allocate_buffer`.
//   The element is CEngineSubPrimitiveRenderer::CSortElement (8 bytes: unsigned long SortValue, CEngineSubPrimitiveBase*
//   SubPrimitive; a user default constructor makes it non-POD for STLport's __type_traits) and the comparator is
//   less<CSortElement> (CSortElement::operator< compares SortValue unsigned). This is the STLport sort / stable_sort
//   chain of the CEngineSubPrimitiveRenderer TU (SortList 0x00B84990: stable_sort when the list RequiresStableSort, else
//   sort below 256 elements, else RadixSort 0x00B847D0); Ego_r.pdb / FableWin.pdb carry the Dinkumware twins
//   (_Stable_sort, _Buffered_merge_sort, _Make_heap, _Sort_heap, lower_bound, upper_bound ... over CSortElement*).
//   Modelled in namespace _STL (lane convention; retail RTTI spells std, `pair@...@std@@`); the templates are __fastcall.
// Shape: STLport `_M_allocate_buffer` verbatim (landed 0x0070F410 form): `_M_len > (ptrdiff_t)(INT_MAX / sizeof(_Tp))`
//   -> 0x0FFFFFFF, `_M_len * sizeof(_Tp)` as `shl eax,3`, `_M_len /= 2` as cdq ; sub ; sar.
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

namespace _STL {
template <class _Tp> struct less {                      // empty functor (a 4-byte argument slot)
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
}

namespace _STL {
template <class _ForwardIterator, class _Tp>
class _Temporary_buffer {
private:
    int _M_original_len;                                // +0x00
    int _M_len;                                         // +0x04
    _Tp* _M_buffer;                                     // +0x08
    void _M_allocate_buffer()
    {
        _M_original_len = _M_len;
        _M_buffer = 0;
        if (_M_len > (int)(0x7FFFFFFF / sizeof(_Tp)))   // INT_MAX / sizeof(_Tp)
            _M_len = 0x7FFFFFFF / sizeof(_Tp);
        while (_M_len > 0) {
            _M_buffer = (_Tp*)malloc(_M_len * sizeof(_Tp));
            if (_M_buffer)
                break;
            _M_len /= 2;
        }
    }
};

template class _Temporary_buffer<CSortElement*, CSortElement>;
}