// _STL::__adjust_heap(pair<unsigned long,C3DMeshVolume>* first, int holeIndex, int len, pair<unsigned long,C3DMeshVolume> val, CKeyPairCompareLess<unsigned long,C3DMeshVolume> comp) -- STLport heap sift-down of the (CRC key, named convex mesh volume) table, then __push_heap val back up
// Leaf: ??$__adjust_heap@PAU?$pair@KVC3DMeshVolume@@@_STL@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A33A70, 335 bytes (+ int3 padding), `ret 18h` (__fastcall: first in ecx,
//   holeIndex in edx; len, the 16-byte value and the empty comparator on the stack: 4 + 0x10 + 4 = 0x18).
// Element (16 bytes, `shl eax,4`): unsigned long key +0 (UNSIGNED compare, `jae` over the `dec ebx`), C3DMeshVolume +4
//   (Ego_r.pdb, 12 bytes: CCharString Name +0, CCountedPointer<CContainmentVolume> ConvexVolume +4 = {Data, Info}).
// Label correction: the catalog's `pair<unsigned long,CCountedPointer<CFaction>>` is propagated; the stride, the
//   CCharString + counted-pointer payload and the callee 0x00A32960 (C3DMeshVolume::operator=, shared with the signed
//   twin 0x00A71220) name the mesh-volume pair.
// Callees: CCharString::operator= @ 0x0099EFB0, operator delete @ 0x00BFE9BC (the inline CCountedPointer release:
//   drop the count, call the block's __fastcall destroy function on the object, free the block),
//   C3DMeshVolume::operator= @ 0x00A32960 (the second, tail element move: VC7.1 inlines the implicit operator= in the
//   loop and calls the COMDAT copy at the tail), CCharString(const CCharString&) @ 0x0099EC30 (the memberwise pair
//   copy built in __push_heap's argument slot, the counted pointer copy bumping the count), __push_heap @ 0x00A333A0 (same instantiation),
//   ~CCharString @ 0x0099EAE0 (val: ConvexVolume released and nulled first, then Name).
// Shape: STLport 4.6 `__adjust_heap`; first and topIndex homed in the `sub esp,8` slots.
extern void __cdecl operator delete(void* block);                                    // 0x00BFE9BC

class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
private:
    void* PStringData;
};

struct CCPPointerInfo {                                 // Ego_r.pdb, 12 bytes
    unsigned long RefCount;                             // +0x0
    void (__fastcall* DeleteFunc)(void* data);          // +0x4
    void* Data;                                         // +0x8
};

template <class T> class CCountedPointer {
public:
    CCountedPointer(const CCountedPointer<T>& other) : Data(other.Data), Info(other.Info)
    {
        if (other.Info)
            ++other.Info->RefCount;
    }
    ~CCountedPointer()
    {
        Release();
        Data = 0;
        Info = 0;
    }
    CCountedPointer<T>& operator=(const CCountedPointer<T>& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    T* Data;                                            // +0x0
    CCPPointerInfo* Info;                               // +0x4
};

class CContainmentVolume;
class C3DMeshVolume {                                   // Ego_r.pdb, 12 bytes
public:
    CCharString Name;                                   // +0x0
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x4
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
}

template <class K, class V> class CKeyPairCompareLess {     // empty (Ego_r.pdb: 1 byte)
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};
typedef _STL::pair<unsigned long, C3DMeshVolume> CMeshVolumePair;

namespace _STL {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);   // 0x00A333A0

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}

template void __fastcall __adjust_heap(CMeshVolumePair*, int, int, CMeshVolumePair, CKeyPairCompareLess<unsigned long, C3DMeshVolume>);
}