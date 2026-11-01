// std::__uninitialized_fill_n(CLevelFiles* first, unsigned int n, const CLevelFiles& x, const __false_type&) -- (CLevelLoader::CObject::CLevelFiles) STLport 4.6 fill of n copies of a 28-byte level-file record (each copy bumps the three counted file pointers' blocks)
// Leaf: ??$__uninitialized_fill_n@PAVCLevelFiles@CObject@CLevelLoader@@IV123@@std@@YIPAVCLevelFiles@CObject@CLevelLoader@@PAV123@IABV123@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00513570, length 92 bytes (+4 int3 pad), `ret 8`.
// Callers: 0x0050121F (0x00501150), 0x00514D5A (0x00514C10), 0x00515911 (0x005158B0, the landed vector<CLevelFiles>(n) -- it binds this
//   spelling) and 0x0051A4FC (0x0051A430, the landed _M_insert_overflow of the same vector).
// Identity (naming wave 20, chunk 688): the STLport 4.6 `__uninitialized_fill_n(first, n, x, const __false_type&)` instantiation over
//   CLevelLoader::CObject::CLevelFiles (Ego_r.pdb, 28 bytes: LevelID, then CCountedPointer<CAFile> PTopologyFile / PNavigationFile / PThingFile),
//   the element of the vector the landed 0x005158B0 / 0x0051A430 / 0x0051AE60 rows model; was the descriptive UninitFillN_ThreeCountedPointers
//   (the catalog's `_Uninit_fill_n<pair<unsigned __int64, ...>>` label is propagated: the stride is 28).
// Shape: `__fastcall` (first in ecx -> eax as `cur`, n in edx -> ecx counted down, &x on the stack -> edx loaded once); the record is copied by
//   its implicit memberwise copy constructor (each counted pointer through its user copy constructor, placement-new null test); VC7.1 keeps a
//   second induction register at the middle pointer's Info (`lea esi,[eax+10h]`).
inline void* operator new(unsigned int, void* p) { return p; }

class CAFile;

class CCPPointerInfo { public:                               // 12 bytes
    unsigned long RefCount;                           // +0x00
    void (__cdecl *DeleteFunc)(void*);                // +0x04
    void* Data;                                       // +0x08
};

template <class T> class CCountedPointer {            // 8 bytes
public:
    T* Data;                                          // +0x00
    CCPPointerInfo* Info;                             // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};

class CLevelLoader {
public:
    class CObject {
    public:
        class CLevelFiles {                           // 28 bytes (Ego_r.pdb)
        public:
            long LevelID;                             // +0x00
            CCountedPointer<CAFile> PTopologyFile;    // +0x04
            CCountedPointer<CAFile> PNavigationFile;  // +0x0C
            CCountedPointer<CAFile> PThingFile;       // +0x14
        };
    };
};

namespace std {
struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template CLevelLoader::CObject::CLevelFiles* __fastcall __uninitialized_fill_n<CLevelLoader::CObject::CLevelFiles*, unsigned int, CLevelLoader::CObject::CLevelFiles>(
    CLevelLoader::CObject::CLevelFiles*, unsigned int, const CLevelLoader::CObject::CLevelFiles&, const __false_type&);
}
