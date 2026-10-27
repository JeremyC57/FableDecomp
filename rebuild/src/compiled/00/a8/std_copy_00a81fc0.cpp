// std::__copy(first, last, result, const random_access_iterator_tag&, int*) -- (std::list<C2DLineF> elements) STLport random-access forward copy: `for (n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }`, returns the advanced result
// Leaf: ??$__copy@PAV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A81FC0, 53 bytes, /Gr __fastcall (first ecx, last edx; result, the tag reference and the
//   int* distance tag on the stack), `ret 0Ch`.
// Identity (probable): __copy<std::list<C2DLineF>*, std::list<C2DLineF>*, int>; the per-element call is the landed std::list<C2DLineF>::operator= 0x00517720, whose callers include the CANavQuadTree code at 0x00A81D48.. next to this body; 4-byte (STLport list) elements; no direct callers.
// Callee: std::list<C2DLineF>::operator= 0x00517720.
// Shape (family K073, representative 0x0040E290): the landed __copy<CQuestCardItem*> 0x00623150 text; the element's own
//   operator= is out of line, so each pass is one call with the source pushed and the destination in ecx.
class C2DLineF;
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc = allocator<_Tp> > class list {           // STLport, 4 bytes (node pointer)
public:
    void* _M_node;
    list& operator=(const list& __x);                    // 0x00517720 for list<C2DLineF>
};
}

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                              _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template std::list<C2DLineF>* __fastcall __copy<std::list<C2DLineF>*, std::list<C2DLineF>*, int>(std::list<C2DLineF>*, std::list<C2DLineF>*, std::list<C2DLineF>*, const random_access_iterator_tag&, int*);
}   // namespace std