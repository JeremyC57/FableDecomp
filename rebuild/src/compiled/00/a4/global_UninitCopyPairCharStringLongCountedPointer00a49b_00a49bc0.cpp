// _global::UninitCopy_PairCharStringLongCountedPointer_00a49bc0(const SNamedSurfaceEntry* first, const SNamedSurfaceEntry* last, SNamedSurfaceEntry* result, const __false_type&) -- STLport __uninitialized_copy over 16-byte {CCharString, long, CCountedPointer<T>} entries: placement-copy each entry (string copy out of line, counted pointer add-ref'd), return the advanced result
// Leaf: UninitCopy_PairCharStringLongCountedPointer_00a49bc0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A49BC0, length 70 bytes (+ int3 padding), `ret 8`.
// Callers: the vector regrowth @ 0x00A4AC60 (catalog "Vector_Reallocate_CCharString_CountedPointer", calls 0x00A4ACB5 /
//   0x00A4AD19).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Layout (from the copy): CCharString +0x0, a plain word +0x4, CCountedPointer<T> { Data +0x8, Info +0xC }; the count
//   block keeps its count as word 0.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` under /Gr (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack); first/last/cur in esi/ebx/edi around the out-of-line
//   string copy; the three trailing words alternate eax/ecx and the count block is add-ref'd (`inc [eax]`).
// Label correction: the catalog's `_Uninit_copy<..pair<unsigned __int64,CCountedPointer<CActiveEntityScriptBase>>..>` is
//   propagated (the element starts with a CCharString).
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

struct SNamedSurfaceEntry {                    // 16 bytes
    CCharString Name;                          // +0x0
    long Value;                                // +0x4
    CCountedPointer<CCountedObject> Object;    // +0x8
};

struct __false_type { __false_type() { } };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

SNamedSurfaceEntry* __fastcall UninitCopy_PairCharStringLongCountedPointer_00a49bc0(const SNamedSurfaceEntry* first, const SNamedSurfaceEntry* last, SNamedSurfaceEntry* result, const __false_type&)
{
    SNamedSurfaceEntry* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}