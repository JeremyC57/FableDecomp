// CCountedPointer<CExpressionSet>::DeleteData(void* data) -- the counted pointer's per-type deleter: destroy the expression set (free its entry vector's block) and free the set
// Leaf: ?DeleteData@
//
// Retail 0x00674E80, 33 bytes (+ int3 padding to 48), plain `ret` (static __fastcall: the object in ecx).
// Callees: free @ 0x00BFEA14 (STLport __malloc_alloc deallocate of the vector block), operator delete @ 0x00BFE9BC.
// Identity: its one reference is the `mov dword ptr [eax+4], 0x674E80` of CCountedPointer<..>::Reset(T*) 0x006750D0
//   (landed), which the CCreatureDef constructor calls (0x006766CE) on `this + 0x38` with a fresh 16-byte object
//   (vector zeroed, the sorted byte +0xd cleared) -- CCreatureDef::PExpressions (landed CCreatureDef::operator=
//   0x00675C00 puts CCountedPointer<CExpressionSet> at +0x38).  Ego_r.pdb names the per-type deleters
//   `?DeleteData@?$CCountedPointer@V...@@@@SIXPAX@Z` (static, __fastcall, void*).  The catalog's
//   "Object_FreeInnerAndDelete_00674e80" is descriptive.
// Layout: Ego_r.pdb CExpressionSet = { CVectorMap<EExpressionType, CExpressionSet::CEntry, ..> Expressions; }; retail
//   CVectorMap = STLport vector (12 bytes) + empty compare +0xc + Dirty +0xd (16 bytes).  No engine header carries the
//   vector's destructor, so the model is local.
// Shape: `delete static_cast<T*>(data)`: null test, the inline implicit ~CExpressionSet -> ~CVectorMap -> STLport
//   ~vector (trivial element _Destroy, then `if (_M_start) deallocate` = free), then operator delete.
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
void __cdecl operator delete(void* p);                      // 0x00BFE9BC

enum EExpressionType { EXPRESSION_TYPE_NONE = -1, EXPRESSION_TYPE_0 = 0 };

class CExpressionSet;

namespace std {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
};

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    struct : _Alloc { _Tp* _M_data; } _M_end_of_storage;
    ~vector()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
}

template <class K, class V> class CKeyPairCompareLess { };

template <class K, class V, class C> class CVectorMap {
public:
    std::vector<std::pair<K, V> > Entries;                   // +0x00
    C    Compare;                                            // +0x0c (empty)
    bool Dirty;                                              // +0x0d
};

class CExpressionSet {
public:
    class CEntry {
    public:
        EExpressionType Type;                                // +0x00
        long AnimIndex;                                      // +0x04
    };
    CVectorMap<EExpressionType, CEntry, CKeyPairCompareLess<EExpressionType, CEntry> > Expressions;   // +0x00
};

class CCPPointerInfo;

template <class T> class CCountedPointer {
public:
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
    static void __fastcall DeleteData(void* data)
    {
        delete static_cast<T*>(data);
    }
};

template class CCountedPointer<CExpressionSet>;