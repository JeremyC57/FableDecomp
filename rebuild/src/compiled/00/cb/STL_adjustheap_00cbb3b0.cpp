// std::__adjust_heap(pair<unsigned __int64,CCountedPointer<CActiveEntityScriptBase> >* first, int holeIndex, int len, pair<unsigned __int64,CCountedPointer<CActiveEntityScriptBase> > val, CKeyPairCompareLess comp) -- STLport heap sift-down of the active-entity script table keyed by a 64-bit id, then __push_heap val back up
// Leaf: ??$__adjust_heap@PAU?$pair@_KV?$CCountedPointer@VCActiveEntityScriptBase@@@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBB3B0, 288 bytes (incl. int3 padding), `ret 18h` (__fastcall: first in ecx,
//   holeIndex in edx; len, the 16-byte value and the empty comparator on the stack: 4 + 0x10 + 4 = 0x18).
// Element (16 bytes, `shl eax,4`): unsigned __int64 key +0 (high dword compared first: `ja` / `jb`, then the low
//   dword `jae`), CCountedPointer<CActiveEntityScriptBase> +8 = {Data +8, CCPPointerInfo* Info +0xC}.
// Callees: operator delete @ 0x00BFE9BC (the inline CCountedPointer release: drop the count, call the block's
//   __fastcall destroy function on the object, free the block), pair::operator= @ 0x00CBA520 (the second, tail
//   element move: VC7.1 inlines the implicit pair operator= in the loop and calls its COMDAT copy at the tail),
//   __push_heap @ 0x00CBABA0 (same instantiation).
// Shape: STLport 4.6 `__adjust_heap`; topIndex homed in a `sub esp,8` slot, the copied Data in the other; val's
//   count block stays in esi across __push_heap (no out-of-line call sees val, so its destructor's null stores die).
extern void __cdecl operator delete(void* block);                                    // 0x00BFE9BC

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

class CActiveEntityScriptBase;

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
}

template <class K, class V> class CKeyPairCompareLess {     // empty (Ego_r.pdb: 1 byte)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef std::pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > CScriptPair;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);   // 0x00CBABA0

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

template void __fastcall __adjust_heap(CScriptPair*, int, int, CScriptPair,
                                       CKeyPairCompareLess<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> >);
}