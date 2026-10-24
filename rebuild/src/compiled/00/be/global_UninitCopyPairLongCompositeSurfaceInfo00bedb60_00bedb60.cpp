// _global::UninitCopy_PairLongCompositeSurfaceInfo_00bedb60(const PairLongCompositeSurfaceInfo* first, const PairLongCompositeSurfaceInfo* last, PairLongCompositeSurfaceInfo* result, const __false_type&) -- STLport __uninitialized_copy over pair<long, CCompositeSurfaceInfo>: placement-copy each 16-byte entry (key, vtable, counted cache with its add-ref), return the advanced result
// Leaf: UninitCopy_PairLongCompositeSurfaceInfo_00bedb60
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEDB60, length 66 bytes (+ int3 padding), `ret 8`.
// Callers: the vector helpers of the composite-surface map @ 0x00BEDBF0 (call 0x00BEDCF9), 0x00BEEB80 (call 0x00BEEBDF) and
//   0x00BEEF90 (calls 0x00BEEFDB / 0x00BEF03D; catalog "CCompositeSurfaceMap_Reallocate").
// Callees: none (the copy constructors are implicit and inline). Vtable 0x012A8154 = CCompositeSurfaceInfo.
// Layout: the element of the landed __adjust_heap @ 0x00BEDD70 (pair<long, CCompositeSurfaceInfo>, 16 bytes): key +0x0,
//   CCompositeSurfaceInfo +0x4 = { vptr (CBase-derived, landed ctor @ 0x00BED440), CCountedPointer<CCompositeSurfaceCache>
//   Cache { Data +0x8, Info +0xC } }; the count block keeps its count as word 0.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` under /Gr (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack). The pair copy writes the key, then the info's implicit
//   copy: one vtable store (the member-less CBase's own store is dead) and the Cache copy through a second destination
//   pointer at dest+8 (`lea esi,[eax+8]`).
// Label correction: the catalog's `_Uninit_copy<..pair<unsigned __int64,CCountedPointer<CActiveEntityScriptBase>>..>` is
//   propagated.
inline void* operator new(unsigned int, void* where) { return where; }

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

class CBase {
public:
    virtual ~CBase();
};

struct CCompositeSurfaceCache;

class CCompositeSurfaceInfo : public CBase {
public:
    virtual ~CCompositeSurfaceInfo();          // vtable 0x012A8154
    CCountedPointer<CCompositeSurfaceCache> Cache;   // +0x4
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
}
typedef std::pair<long, CCompositeSurfaceInfo> PairLongCompositeSurfaceInfo;

struct __false_type { __false_type() { } };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

PairLongCompositeSurfaceInfo* __fastcall UninitCopy_PairLongCompositeSurfaceInfo_00bedb60(const PairLongCompositeSurfaceInfo* first, const PairLongCompositeSurfaceInfo* last, PairLongCompositeSurfaceInfo* result, const __false_type&)
{
    PairLongCompositeSurfaceInfo* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}