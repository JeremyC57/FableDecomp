// std::__destroy_aux<CRecordWithWideStringAt0*>(first, last, const __false_type&) -- runs ~CWideString over a range of 0x14-byte records
// Leaf: __destroy_aux
// Retail: Fable.exe @ 0x00440BA0, 35 bytes, __fastcall (first in ecx, last in edx, the __false_type tag by reference on
// the stack, `ret 4`). Catalog label std::_Destroy_range<allocator<CSpeechFrame>> is wrong: the stride is 0x14 and each
// element's only destructor call is ~CWideString (0x0099B510) on +0x00. Callers (0x004940D2 vector::erase,
// 0x004940F1 vector dtor) pass `lea eax,[ebp+0Bh]`, the address of a __false_type temporary, i.e. STLport's
// _Destroy -> __destroy(first, last, value_type*) -> __destroy_aux(first, last, __false_type()) with the last step out of
// line.
// Layout: element is 0x14 bytes, CWideString at +0x00 and 16 trivially destructible bytes (several Ego_r.pdb records
// share this shape, e.g. CEmbeddedHighlight/CClanInfo/CBankInfo; identical-code folding hides which); named behaviourally.
// Shape: /O2 loop with the `lea ebx,[ebx]` alignment pad before the loop head.
class CWideString {
public:
    void* PData;
    ~CWideString();                                      // 0x0099B510
};
struct CRecordWithWideStringAt0 {
    CWideString Text;                                    // +0x00
    long        Data[4];                                 // +0x04
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
template void __fastcall __destroy_aux<CRecordWithWideStringAt0*>(CRecordWithWideStringAt0*, CRecordWithWideStringAt0*, const __false_type&);
}