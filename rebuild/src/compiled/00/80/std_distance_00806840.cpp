// std::__distance(CUnprovenValue_00806840* const& first, CUnprovenValue_00806840* const& last, const random_access_iterator_tag&) -- STLport random-access distance: last - first over a 4-byte element
// Leaf: ??$__distance@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00806840, 16 bytes, `__cdecl` (STLport's _STLP_CALL; both iterators by reference).
// Types: unproven -- the element is 4 bytes (`sar eax, 2`) and nothing calls this unreferenced COMDAT of the player targeting
//   TU, so the element is the per-row placeholder CUnprovenValue_00806840.
// Model: STLport 4.6 stl/_iterator_base.h `__distance(const _RandomAccessIterator&, const _RandomAccessIterator&,
//   const random_access_iterator_tag&) { return __last - __first; }`.
struct CUnprovenValue_00806840 { long _M_unproven; };

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _RandomAccessIterator>
int __cdecl __distance(const _RandomAccessIterator& __first, const _RandomAccessIterator& __last,
                       const random_access_iterator_tag&)
{
    return __last - __first;
}

template int __cdecl __distance(CUnprovenValue_00806840* const&, CUnprovenValue_00806840* const&,
                                const random_access_iterator_tag&);
}