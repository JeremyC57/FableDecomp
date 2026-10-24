// _global::UninitCopy_PairCharStringCountedPointer_00cba4a0(const PairCharStringCountedPointer* first, const PairCharStringCountedPointer* last, PairCharStringCountedPointer* result, const __false_type&) -- STLport __uninitialized_copy over 12-byte pair<CCharString, CCountedPointer<T>> entries: placement-copy each entry (string copy out of line, counted pointer add-ref'd), return the advanced result
// Leaf: UninitCopy_PairCharStringCountedPointer_00cba4a0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBA4A0, length 64 bytes, `ret 8`.
// Callers: the landed vector::_M_insert_overflow @ 0x00CBC940 (calls 0x00CBC9A6 / 0x00CBCA03; its element model is the
//   same 12-byte {CCharString, word, add-ref'd count pointer}).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Layout: pair<CCharString first +0x0, CCountedPointer<T> second +0x4 = { Data +0x4, Info +0x8 }>; the count block keeps
//   its count as word 0.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` under /Gr (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack). With an out-of-line call in the loop, first/last/cur live
//   in esi/ebx/edi (`mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[esp+10h]`); placement new (null test)
//   of the pair's copy: the key string through its copy constructor, then Data and Info with the add-ref (`inc [eax]`).
// Label correction: the catalog's `_Uninit_copy<..CScriptThing..>` is propagated (CScriptThing starts with a vptr).
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    CCharString(const CCharString& other);     // 0x0099EC30
    void* PData;                               // +0x0
};

struct CCountBlock { unsigned long RefCount; };

template <class T> struct CCountedPointer {
    T* Data;                                   // +0x0
    CCountBlock* Info;                         // +0x4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

struct CCountedObject;

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
}
typedef std::pair<CCharString, CCountedPointer<CCountedObject> > PairCharStringCountedPointer;

struct __false_type { __false_type() { } };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

PairCharStringCountedPointer* __fastcall UninitCopy_PairCharStringCountedPointer_00cba4a0(const PairCharStringCountedPointer* first, const PairCharStringCountedPointer* last, PairCharStringCountedPointer* result, const __false_type&)
{
    PairCharStringCountedPointer* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}