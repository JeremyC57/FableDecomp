// std::__uninitialized_fill_n<pair<long, CCountedPointer<T> >*, unsigned int, pair<long, CCountedPointer<T> > >(...) -- copy-construct n key/counted-pointer pairs, return the end
// Leaf: ??$__uninitialized_fill_n
// Retail: Fable.exe @ 0x004482C0, 52 bytes, __fastcall (first in ecx, n in edx, &x and the __false_type tag on the stack,
// `ret 8`), returns first + n; caller 0x004491C8 inside the vector's insert-overflow path 0x00449130 (12-byte elements:
// `imul 2AAAAAABh`, `lea [ebx+ebx*2] ; shl 2`). Catalog label std::_Uninit_fill_n<pair<unsigned __int64,
// CCountedPointer<CActiveEntityScriptBase>>*, ...> is wrong about the element: it is 12 bytes -- a dword key (+0x00) and
// an 8-byte CCountedPointer (Data +0x04, Info +0x08) whose copy bumps Info->RefCount (CCPPointerInfo +0x00) when
// non-null. The pointee type does not reach the bytes; it is named behaviourally.
// Layout: Ego_r.pdb CCountedPointer<T> {T* Data; CCPPointerInfo* Info}, CCPPointerInfo {RefCount, DeleteFunc, Data}.
// Shape: /O2 (`lea ecx,[ecx]` loop-head pad; &x held in edx); n is unsigned (`test edx,edx ; jbe`).
#include <new>
class CCountedValue;
struct CCPPointerInfo {
    unsigned long RefCount;                              // +0x00
    void (__cdecl* DeleteFunc)(void*);                   // +0x04
    void*         Data;                                  // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    CCountedPointer(const CCountedPointer& x) : Data(x.Data), Info(x.Info) { if (Info) ++Info->RefCount; }
};
namespace std {
struct __false_type {};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) {}
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
typedef pair<long, CCountedPointer<CCountedValue> > CKeyedCountedPointer;
template CKeyedCountedPointer* __fastcall __uninitialized_fill_n<CKeyedCountedPointer*, unsigned int, CKeyedCountedPointer>(CKeyedCountedPointer*, unsigned int, const CKeyedCountedPointer&, const __false_type&);
}