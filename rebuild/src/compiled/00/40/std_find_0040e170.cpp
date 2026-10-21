// std::__find(long* first, long* last, const long& val, const random_access_iterator_tag&) -- STLport random-access linear find: four compares per trip, then the 3/2/1 remainder switch
// Leaf: ??$__find@PAJJ@std@@YIPAJPAJ0ABJABUrandom_access_iterator_tag@0@@Z
// Retail: Fable.exe @ 0x0040E170, 112 bytes, `ret 8` (__fastcall: first ecx, last edx; val and the tag on the stack). A
// folded body shared by every 4-byte element find (callers CHitParameters::IsDivineWrathStrike 0x00407330,
// GiveTutorialForCategory 0x0040B790 over vector<ETutorialCategory>, OnKill 0x0040F560), so the element type is modelled as long.
// The catalog label Array_LinearSearchInt is descriptive only.
// Body: STLport 4.6 _algobase.c __find: trip_count = (last - first) >> 2 loop (val hoisted into esi), then
// switch (last - first) { case 3/2/1 fall through; default: return last; }.
// Shape: /O2 (16-aligned entry); out of line as in 0x004129F0's STLport __copy.
namespace std {
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
template <class RandomAccessIter, class T>
RandomAccessIter __fastcall __find(RandomAccessIter first, RandomAccessIter last, const T& val, const random_access_iterator_tag&)
{
    int trip_count = (last - first) >> 2;
    for ( ; trip_count > 0 ; --trip_count) {
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (*first == val) return first;
        ++first;
    case 2:
        if (*first == val) return first;
        ++first;
    case 1:
        if (*first == val) return first;
    case 0:
    default:
        return last;
    }
}
template long* __fastcall __find<long*, long>(long*, long*, const long&, const random_access_iterator_tag&);
}