// std::__destroy_aux<CRecordWithCharStringAtC*>(first, last, const __false_type&) -- runs ~CCharString over a range of 0x10-byte records
// Leaf: __destroy_aux
// Retail: Fable.exe @ 0x00440BD0, 36 bytes, __fastcall (first in ecx, last in edx, the __false_type tag by reference on
// the stack, `ret 4`). Catalog label std::_Destroy_range<allocator<CSpeechFrame>> is wrong: the stride is 0x10 and each
// element's only destructor call is ~CCharString (0x0099EAE0) on +0x0C. Callers 0x0065685D/0x006568DD (inside
// CTCInventoryMap screen construction) are the STLport vector paths; out-of-line last step of
// _Destroy -> __destroy -> __destroy_aux(first, last, __false_type()).
// Layout: element is 0x10 bytes, 12 trivially destructible bytes then a CCharString at +0x0C; named behaviourally.
// Shape: /O2 loop with the `lea ebx,[ebx]` alignment pad before the loop head.
class CCharString {
public:
    void* PData;
    ~CCharString();                                      // 0x0099EAE0
};
struct CRecordWithCharStringAtC {
    long        Data[3];                                 // +0x00
    CCharString Name;                                    // +0x0c
};
namespace std {
struct __false_type {};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class ForwardIter>
void __fastcall __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template void __fastcall __destroy_aux<CRecordWithCharStringAtC*>(CRecordWithCharStringAtC*, CRecordWithCharStringAtC*, const __false_type&);
}