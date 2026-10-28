// NScript::CRuntimeScriptedEntityPersist::SetEntityPersistScript(unsigned __int64 entityID, const CCharString& script) -- record the persisted script text of a scripted entity: overwrite the entry for the UID when there is one, else append a new (UID, info) pair and re-sort the map
// Leaf: ?SetEntityPersistScript@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB82F0, 213 bytes (`ret 0Ch`, thiscall: this + u64 + const CCharString&).
// Identity: Ego_r.pdb `?SetEntityPersistScript@CRuntimeScriptedEntityPersist@NScript@@...`; called from
//   CScriptBase::SaveEntityScript (0x00CB8422). The manifest label "ScriptEntity_InsertOrUpdatePersist" is descriptive.
// Callees: CVectorMap<unsigned __int64, CPersistInfo>::LowerBound 0x00CB9740, CCharString copy/dtor/assign
//   0x0099EC30 / 0x0099EAE0 / 0x0099EFB0, vector<CPersistPair>::push_back 0x00CBC7B0, std::sort 0x00CBD370.
// Shape: Find + `it->second = info` or Add (push_back of a pair temporary, Dirty = true, sort, Dirty = false). Add and
//   Sort are __forceinline; LowerBound gets a same-TU __declspec(noinline) body (it only reads), which lets VC7.1 keep
//   the key and `this` in registers across it as retail does instead of reloading them from the argument slots.
class CCharString {
public:
    void* m_pStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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
    void push_back(const _Tp& __x);                                   // 0x00CBC7B0 (out of line)
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
    iterator LowerBound(const K& key);                     // 0x00CB9740 (same-TU body below)
    iterator end() { return Vector.end(); }
    iterator Find(const K& key)
    {
        iterator it = LowerBound(key);
        if (it == end() || key < it->first)
            return end();
        return it;
    }
    __forceinline void Sort()
    {
        std::sort(Vector.begin(), Vector.end(), Compare);
        Dirty = false;
    }
    __forceinline void Add(const K& key, const V& value)
    {
        Vector.push_back(value_type(key, value));
        Dirty = true;
        Sort();
    }
};
#pragma auto_inline(off)
// Same-TU body of the out-of-line LowerBound (landed 0x00CB9740): with it VC7.1 sees that the key reference is only
// read, so the caller keeps the entity UID in ebp/ebx across the later calls instead of re-reading its argument slot.
template <class K, class V, class C>
__declspec(noinline) typename CVectorMap<K, V, C>::iterator CVectorMap<K, V, C>::LowerBound(const K& key)
{
    iterator first = Vector.begin();
    int count = Vector.end() - Vector.begin();
    while (count > 0) {
        int half = count / 2;
        iterator middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}
#pragma auto_inline(on)
namespace NScript {
class CRuntimeScriptedEntityPersist {
public:
    class CPersistInfo {                                   // Ego_r.pdb: 16 bytes (8-aligned)
    public:
        unsigned __int64 UID;                              // +0x00
        CCharString      PersistScript;                    // +0x08
        CPersistInfo(unsigned __int64 uid, const CCharString& script) : UID(uid), PersistScript(script) { }
    };
    CVectorMap<unsigned __int64, CPersistInfo, CKeyPairCompareLess<unsigned __int64, CPersistInfo> > PersistScripts;   // +0x00
    void SetEntityPersistScript(unsigned __int64 entityID, const CCharString& script);
};

void CRuntimeScriptedEntityPersist::SetEntityPersistScript(unsigned __int64 entityID, const CCharString& script)
{
    CVectorMap<unsigned __int64, CPersistInfo, CKeyPairCompareLess<unsigned __int64, CPersistInfo> >::iterator it = PersistScripts.Find(entityID);
    CPersistInfo info(entityID, script);
    if (it != PersistScripts.end())
        it->second = info;
    else
        PersistScripts.Add(entityID, info);
}
}