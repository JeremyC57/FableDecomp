// _global::UninitCopy_PairULongMeshVolume_00a6fe00(const PairULongMeshVolume* first, const PairULongMeshVolume* last, PairULongMeshVolume* result, const __false_type&) -- STLport __uninitialized_copy over 16-byte pair<unsigned long, C3DMeshVolume> entries: placement-copy each entry (key word, volume name out of line, the counted convex volume add-ref'd), return the advanced result
// Leaf: UninitCopy_PairULongMeshVolume_00a6fe00
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6FE00, length 99 bytes (+ int3 padding), `ret 8`.
// Callers: the vector regrowth of the same TU @ 0x00A72CC0 (calls 0x00A72D19 / 0x00A72D84).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (the volume name).
// Layout: pair<unsigned long key +0x0, Ego_r.pdb C3DMeshVolume +0x4 = { CCharString Name +0x4,
//   CCountedPointer<CContainmentVolume> ConvexVolume { Data +0x8, Info +0xC } }> (the TU's other entry type is
//   pair<CCharString, C3DMeshVolume>, copied by the neighbour 0x00A6FED0).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` under /Gr (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack). `last` lives in a `push ecx` frame word; first/cur in
//   ebx/ebp and the nested C3DMeshVolume through a second strength-reduced src+8/dst+8 pair in esi/edi. The key word is
//   loaded before the name's call arguments and stored right before the call (`mov eax,[ebx] ; ... ; mov [ebp],eax ;
//   call`), then the counted pointer is copied with its add-ref (`inc [eax]`).
// Label correction: the catalog's `_Uninit_copy<..pair<unsigned __int64,CCountedPointer<CActiveEntityScriptBase>>..>` is
//   propagated (the element is the mesh-volume pair).
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    CCharString(const CCharString& other);     // 0x0099EC30
    void* PData;                               // +0x0
};

struct CCPPointerInfo { unsigned long RefCount; };

template <class T> struct CCountedPointer {
    T* Data;                                   // +0x0
    CCPPointerInfo* Info;                      // +0x4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

struct CContainmentVolume;

struct C3DMeshVolume {                         // Ego_r.pdb, 12 bytes
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
typedef std::pair<unsigned long, C3DMeshVolume> PairULongMeshVolume;   // 16 bytes

struct __false_type { __false_type() { } };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

PairULongMeshVolume* __fastcall UninitCopy_PairULongMeshVolume_00a6fe00(const PairULongMeshVolume* first, const PairULongMeshVolume* last, PairULongMeshVolume* result, const __false_type&)
{
    PairULongMeshVolume* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}