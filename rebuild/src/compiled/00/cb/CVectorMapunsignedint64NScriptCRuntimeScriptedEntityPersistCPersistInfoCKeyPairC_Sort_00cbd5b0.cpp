// CVectorMap<unsigned __int64,CPersistInfo,CKeyPairCompareLess<unsigned __int64,CPersistInfo> >::Sort() -- sort the persisted-entity-script map (STLport sort over its vector with the empty comparator) and clear Dirty
// Leaf: ?Sort@?$CVectorMap@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@V?$CKeyPairCompareLess@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBD5B0, 24 bytes (+ int3 padding), thiscall, plain `ret`: `std::sort(begin (+0) in ecx,
//   end (+4) in edx, Compare (+0x0c, one byte pushed))`, then Dirty (+0x0d) = false.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 451 "0xCBD5B0 = CVectorMap<u64,CPersistInfo>::Sort with callee 0xCBD370
//   std::sort (landed as GuiVarBool_Apply)"): 0x00CBD370 is the std::sort over pair<unsigned __int64, CPersistInfo> that the
//   landed NScript::CRuntimeScriptedEntityPersist::SetEntityPersistScript 0x00CB82F0 reaches through its inlined
//   CVectorMap::Add -> Sort (same `sort(begin, end, Compare) ; Dirty = false` body there); this is the out-of-line copy of that
//   Sort (no references: the scripting library objects are linked whole). Model: that row's CVectorMap / std / Ego_r.pdb
//   CPersistInfo (UID +0, PersistScript +8; 16 bytes) with Sort out of line.
// Label correction: the landed CVectorMap_T_00cbd5b0::Sort modelled the callee as `GuiVarBool_Apply(void*, int, bool)` over
//   pad fields (the fistp2 rename's element type was "not recoverable").
class CCharString {
public:
    void* m_pStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                                    // +0
    _Tp* _M_finish;                                                   // +4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;           // +8
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
};
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00CBD370
}
template <class K, class V> class CKeyPairCompareLess {    // empty
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef value_type* iterator;
    std::vector<value_type> Vector;                       // +0x00
    C    Compare;                                          // +0x0c
    bool Dirty;                                            // +0x0d
    void Sort();
};
template <class K, class V, class C>
void CVectorMap<K, V, C>::Sort()
{
    std::sort(Vector.begin(), Vector.end(), Compare);
    Dirty = false;
}
namespace NScript {
class CRuntimeScriptedEntityPersist {
public:
    class CPersistInfo {                                   // Ego_r.pdb: 16 bytes (8-aligned)
    public:
        unsigned __int64 UID;                              // +0x00
        CCharString      PersistScript;                    // +0x08
    };
};
}
template class CVectorMap<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo,
                          CKeyPairCompareLess<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> >;
