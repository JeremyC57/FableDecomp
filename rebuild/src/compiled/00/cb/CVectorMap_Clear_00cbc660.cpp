// CVectorMap::Clear() -- CVectorMap<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo>: erase every entry, release the storage by swapping with an empty vector, clear Dirty
// Leaf: ?Clear@?$CVectorMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBC660, 86 bytes. Callers: 0x00CB80F0 (tail jump, after its own
// erase(begin, end)) and NScript::CRuntimeScriptedEntityPersist::LoadFromGameStateScript 0x00CB8440.
// Callees: vector::erase(first, last) @ 0x00CBBDA0 (`ret 8`, returns `first`), CCharString::~CCharString @ 0x0099EAE0
//          (per element, at +0x10), free @ 0x00BFEA14.
//
// LABEL CORRECTION: the manifest's "CTCInventoryMap::ResetPrimitiveHandles" is propagated. The element stride 0x18 with
// a CCharString at +0x10, the callers in the NScript persist module and the Dirty byte at +0x0D identify the
// CRuntimeScriptedEntityPersist::PersistScripts map: PDB CVectorMap<unsigned __int64, CPersistInfo, CKeyPairCompareLess>
// = vector (PDB 16 bytes, retail 12) + Compare + Dirty; pair<unsigned __int64, CPersistInfo{UID, PersistScript}> = 0x18.
//
// Shape: `Data.clear()` (erase(begin, end) out of line), then the swap-with-temporary idiom: the old first/last are
// read into ebp/edi while the three words are zeroed with IMMEDIATE stores, Dirty is cleared, and the temporary's
// inlined _Tidy destroys [first, last) and frees `first` behind a null test. Retail's interleaving (load first, store
// 0, load last, store 0, store 0) and its ebp=first/edi=last colouring come only from a std::swap-style reference
// helper (`Swap(T&, T&)`); a hand-written three-temporary swap loads both before the stores and swaps the registers.
// The destructor and _Tidy are __forceinline (VC7.1 otherwise calls the temporary's destructor out of line).
// The retail container is reconstructed by hand (the DDK STL is not the retail revision; GOTCHAS wave 22).
extern "C" void __cdecl free(void* p);              // 0x00BFEA14

class CCharString { public:
    void* m_pData;
    ~CCharString();                                 // 0x0099EAE0
};

namespace NScript {
struct CRuntimeScriptedEntityPersist {
    class CPersistInfo { public:
        unsigned __int64 UID;                       // +0x00
        CCharString PersistScript;                  // +0x08
    };
};
}

template <class K, class V> struct SPair {
    K first;
    V second;
};

template <class T> inline void Swap(T& left, T& right)
{
    T tmp = left;
    left = right;
    right = tmp;
}

template <class T> struct CRetailVector {           // VC6-era Dinkumware vector, 12 bytes
    T* _First;
    T* _Last;
    T* _End;
    CRetailVector() : _First(0), _Last(0), _End(0) {}
    __forceinline ~CRetailVector() { _Tidy(); }
    T* begin() { return _First; }
    T* end() { return _Last; }
    T* erase(T* first, T* last);                    // 0x00CBBDA0
    void clear() { erase(begin(), end()); }
    void swap(CRetailVector& x)
    {
        Swap(_First, x._First);
        Swap(_Last, x._Last);
        Swap(_End, x._End);
    }
    __forceinline void _Tidy()
    {
        for (T* p = _First; p != _Last; ++p)
            p->~T();
        if (_First != 0)
            free(_First);
        _First = 0; _Last = 0; _End = 0;
    }
};

template <class K, class V> class CKeyPairCompareLess { public:
};

template <class K, class V, class C> class CVectorMap { public:
    CRetailVector< SPair<K, V> > Data;              // +0x00
    C Compare;                                      // +0x0C
    bool Dirty;                                     // +0x0D
    void Clear();
};

template <class K, class V, class C> void CVectorMap<K, V, C>::Clear()
{
    Data.clear();
    {
        CRetailVector< SPair<K, V> > empty;
        Data.swap(empty);
        Dirty = false;
    }
}

typedef NScript::CRuntimeScriptedEntityPersist::CPersistInfo CPersistInfo;
template void CVectorMap<unsigned __int64, CPersistInfo, CKeyPairCompareLess<unsigned __int64, CPersistInfo> >::Clear();