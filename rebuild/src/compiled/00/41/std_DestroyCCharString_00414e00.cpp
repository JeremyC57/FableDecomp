// std::_Destroy<CCharString*>(CCharString* first, CCharString* last) -- runs ~CCharString over a range
// Leaf: _Destroy
// Retail: Fable.exe @ 0x00414E00, 33 bytes, __fastcall (first in ecx, last in edx, plain `ret`). Catalog label
// std::_Destroy_range<allocator<CSpeechFrame>> is wrong: the element stride is 4 and each element's destructor is
// ~CCharString (0x0099EAE0), i.e. STLport's range _Destroy -> __destroy_aux(first, last, __false_type) fully inlined.
// Shape: /O2 loop with the 2-byte `lea ebx,[ebx]` alignment pad before the loop head.
class CCharString {
public:
    void* PData;
    ~CCharString();                                      // 0x0099EAE0
};
namespace std {
struct __false_type {};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class ForwardIter>
inline void __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template <class ForwardIter>
void __fastcall _Destroy(ForwardIter first, ForwardIter last)
{
    __destroy_aux(first, last, __false_type());
}
template void __fastcall _Destroy<CCharString*>(CCharString*, CCharString*);
}