// _global::UninitCopy_PairCharStringMeshVolume_00a6fed0(const PairCharStringMeshVolume* first, const PairCharStringMeshVolume* last, PairCharStringMeshVolume* result, const __false_type&) -- STLport __uninitialized_copy over 16-byte pair<CCharString, C3DMeshVolume> entries: placement-copy each entry (key and volume name out of line, the counted convex volume add-ref'd), return the advanced result
// Leaf: UninitCopy_PairCharStringMeshVolume_00a6fed0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6FED0, length 102 bytes (+ int3 padding), `ret 8`.
// Callers: the single-element insert of the same vector @ 0x00A72DD0 (calls 0x00A72E29 / 0x00A72E98).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (twice per element).
// Layout: the CVectorMap<CCharString, C3DMeshVolume> entry of this TU (landed _Construct @ 0x00A6FDC0, __copy_backward
//   @ 0x00A70AB0, LowerBound/UpperBound @ 0x00A6FC40/0x00A6FCB0): key +0x0, Ego_r.pdb C3DMeshVolume +0x4 = { CCharString
//   Name +0x4, CCountedPointer<CContainmentVolume> ConvexVolume { Data +0x8, Info +0xC } }.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` under /Gr (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack). `last` is spilled to a `push ecx` frame word (five live
//   pointers): first/cur in ebx/ebp, and the nested C3DMeshVolume copy addressed through a second strength-reduced
//   src+8/dst+8 pair in esi/edi. STLport's explicit pair copy (key string), then the implicit C3DMeshVolume copy (name
//   string, Data, Info with `inc [eax]`).
// Label correction: the catalog's `_Uninit_copy<..CScriptThing..>` is propagated (CScriptThing starts with a vptr).
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    CCharString(const CCharString& other);     // 0x0099EC30
    void* PData;                               // +0x0
};

class CCPPointerInfo { public: unsigned long RefCount; };

template <class T> class CCountedPointer { public:
    T* Data;                                   // +0x0
    CCPPointerInfo* Info;                      // +0x4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

class CContainmentVolume;

class C3DMeshVolume { public:                         // Ego_r.pdb, 12 bytes
    CCharString Name;                          // +0x0
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x4
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
}
typedef std::pair<CCharString, C3DMeshVolume> PairCharStringMeshVolume;   // 16 bytes

struct __false_type { __false_type() { } };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

PairCharStringMeshVolume* __fastcall UninitCopy_PairCharStringMeshVolume_00a6fed0(const PairCharStringMeshVolume* first, const PairCharStringMeshVolume* last, PairCharStringMeshVolume* result, const __false_type&)
{
    PairCharStringMeshVolume* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}