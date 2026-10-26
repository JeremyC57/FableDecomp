// std::__push_heap(pair<CCharString,C3DMeshVolume>* first, int holeIndex, int topIndex, pair<CCharString,C3DMeshVolume> val, CKeyPairCompareLess<CCharString,C3DMeshVolume> comp) -- STLport heap sift-up of the (name, named convex mesh volume) table: move parents that sort before val down, then store val in the hole
// Leaf: ??$__push_heap@PAU?$pair@VCCharString@@VC3DMeshVolume@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A708E0, 292 bytes (+ int3 padding), `ret 18h` (__fastcall: first in ecx,
//   holeIndex in edx; topIndex, the 16-byte value and the empty comparator on the stack: 4 + 0x10 + 4 = 0x18).
// Element (16 bytes, `shl eax,4`): CCharString key +0, C3DMeshVolume +4 (Ego_r.pdb, 12 bytes: CCharString Name +0,
//   CCountedPointer<CContainmentVolume> ConvexVolume +4 = {Data, Info}).
// Callees: CBasicString<char>::operator< @ 0x00429950 (payload compare of the inline CCharString::operator<:
//   comp(first[parent], val)), CCharString::operator= @ 0x0099EFB0, operator delete @ 0x00BFE9BC (the inline
//   CCountedPointer release), C3DMeshVolume::operator= @ 0x00A32960 (the final `first[hole] = val`: VC7.1 inlines
//   the implicit operator= in the loop and calls its COMDAT copy after it), ~CCharString @ 0x0099EAE0 x2 (val:
//   ConvexVolume released and nulled first, then Name, then the key).
// Callers: __adjust_heap 0x00A71370 (same instantiation) and the make_heap / push_heap users of the table.
// Shape: STLport 4.6 `__push_heap` (`(hole - 1) / 2` as `cdq ; sub ; sar`); the parent index lives in the `push ecx` slot.
extern void __cdecl operator delete(void* block);                                    // 0x00BFE9BC

struct CBasicStringChar {
    char* PData;
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950
};

class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (!PStringData)
            return true;
        if (!other.PStringData)
            return false;
        return *PStringData < *other.PStringData;
    }
private:
    CBasicStringChar* PStringData;
};

class CCPPointerInfo { public:                                 // Ego_r.pdb, 12 bytes
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
typedef std::pair<CCharString, C3DMeshVolume> CMeshVolumePair;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}

template void __fastcall __push_heap(CMeshVolumePair*, int, int, CMeshVolumePair, CKeyPairCompareLess<CCharString, C3DMeshVolume>);
}