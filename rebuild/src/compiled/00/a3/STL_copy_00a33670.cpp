// std::__copy(pair<unsigned long, C3DMeshVolume>* first, pair<unsigned long, C3DMeshVolume>* last, pair<unsigned long, C3DMeshVolume>* result, const random_access_iterator_tag&, int*) -- assign [first, last) element by element onto result (counted loop), return the end of the destination
// Leaf: __copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A33670, 162 bytes (+ int3 padding), `ret 0Ch` (__fastcall: first in ecx,
//   last in edx, result, the tag reference and the distance pointer on the stack).
// Identity: the forward copy of the 16-byte C3DMeshStats table (vector erase / insert); the catalog's
//   "_Move<CScriptThing*,CScriptThing*>" is propagated. Callees: CCharString::operator= @ 0x0099EFB0, the count block's
//   DeleteFunc, operator delete @ 0x00BFE9BC.
// Element: the 16-byte C3DMeshStats volume-table entry pair<unsigned long, C3DMeshVolume> (Ego_r.pdb C3DMeshVolume, 12 bytes:
//   CCharString Name +0, CCountedPointer<CContainmentVolume> ConvexVolume +4; the landed swap 0x00A331B0 names it) -- the
//   model of the landed 0x00A333A0 / 0x00A34420: the volume's assignment is an inline member whose out-of-line COMDAT is
//   0x00A32960, the counted pointer's assignment is ShareData's `Info != info` test, release, two stores and add-ref;
//   ordered by the unsigned key (`jae`/`jb`).
// Shape: SGI `for (Distance n = last - first; n > 0; --n) { *result = *first; ++first; ++result; } return result;` -- the
//   pair assignment inlined (key, string operator= call, the counted-pointer ShareData), result kept in its argument slot,
//   strength-reduced source/destination pointers, the count spilled to the frame.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CCPPointerInfo { public:
    long  RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4, Data in ecx
    void* Data;                                         // +8
};

template <class T> class CCountedPointer { public:
    T*              Data;                               // +0
    CCPPointerInfo* Info;                               // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            operator delete(Info);
        }
        Data = 0;
        Info = 0;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    operator delete(Info);
                }
            }
            Data = data;
            Info = info;
            if (Info)
                ++Info->RefCount;
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other) { ShareData(other.Data, other.Info); return *this; }
};

class CContainmentVolume;

class C3DMeshVolume { public:                                        // 12 bytes (Ego_r.pdb)
    CCharString                         Name;                   // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;           // +0x04
    C3DMeshVolume& operator=(const C3DMeshVolume& other)        // out of line @ 0x00A32960 where not inlined
    {
        Name = other.Name;
        ConvexVolume = other.ConvexVolume;
        return *this;
    }
};

namespace std {

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};

typedef pair<unsigned long, C3DMeshVolume> TEntry;      // 0x10 bytes

struct CKeyPairCompareLess {                            // CKeyPairCompareLess<unsigned long, ...>
    bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

}

namespace std {

template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter __fastcall __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                             const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

template TEntry* __fastcall __copy<TEntry*, TEntry*, int>(TEntry*, TEntry*, TEntry*, const random_access_iterator_tag&, int*);

}