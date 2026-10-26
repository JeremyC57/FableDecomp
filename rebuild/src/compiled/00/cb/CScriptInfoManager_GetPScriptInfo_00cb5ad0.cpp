// CScriptInfoManager::GetPScriptInfo(const CCharString& name) -- look a script up by name in the sorted Scripts map; its CScriptInfo, or NULL when no script has that name
// Leaf: ?GetPScriptInfo@CScriptInfoManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB5AD0, 80 bytes. __fastcall, `ret 4`. Ego_r.pdb
// ?GetPScriptInfo@CScriptInfoManager@@QAEPAVCScriptInfo@@ABVCCharString@@@Z (PeekPScriptInfo is its const twin with
// the same body). The manifest label "QuestPersist_FindInfoByName" is descriptive.
// Callees: CVectorMap<CCharString,CScriptInfo,CKeyPairCompareLess<..>>::LowerBound 0x00CB65D0 (landed; __fastcall on
// the map at +4, the entry pointer in eax), CBasicString<char>::operator< 0x00429950.
// Layout: Ego_r.pdb CScriptInfoManager with retail's 12-byte STLport vector: CurrentScriptID +0, Scripts +4 (vector
// +4..+0xf, Compare +0x10, Dirty +0x11), CodeSectionNames +0x14 (as the landed ctor 0x00CB5C70 / Init 0x00CB5D80).
// Shape: the map's inline Find `it = LowerBound(key); last = end(); return (it == last || key < it->first) ? last :
// it;` (end() read once into esi), then a fresh `it != Scripts.end()` (`cmp edi,[ebp+8]`) and `&it->second`.
template <class C> class CBasicString {
public:
    bool operator<(const CBasicString& other) const;    // 0x00429950
};

class CCharString {
public:
    CBasicString<char>* PData;                          // +0x00
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
};

class CScriptBase;
class CScriptDataBase;
class CGameScriptInterfaceBase;

class CScriptInfo {                                     // Ego_r.pdb, 0x14 bytes
public:
    CScriptBase*     (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                                  // +0x04
    CCharString      Name;                              // +0x08
    long             ID;                                // +0x0c
    bool             MasterScript;                      // +0x10
};

namespace std {
template <class _T1, class _T2>
struct pair {
    _T1 first;
    _T2 second;
};
}

template <class K, class V>
class CKeyPairCompareLess { public: };

template <class K, class V, class C>
class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* _M_start;                               // +0x00
    value_type* _M_finish;                              // +0x04
    value_type* _M_end_of_storage;                      // +0x08
    C           Compare;                                // +0x0c
    bool        Dirty;                                  // +0x0d

    value_type* end() const { return _M_finish; }
    value_type* LowerBound(const K& key);               // 0x00CB65D0
    value_type* Find(const K& key)
    {
        value_type* it = LowerBound(key);
        value_type* last = end();
        return (it == last || key < it->first) ? last : it;
    }
};

class CScriptInfoManager {
public:
    long CurrentScriptID;                               // +0x00
    CVectorMap<CCharString, CScriptInfo, CKeyPairCompareLess<CCharString, CScriptInfo> > Scripts;   // +0x04
    CScriptInfo* GetPScriptInfo(const CCharString& name);
};

CScriptInfo* CScriptInfoManager::GetPScriptInfo(const CCharString& name)
{
    CVectorMap<CCharString, CScriptInfo, CKeyPairCompareLess<CCharString, CScriptInfo> >::value_type* it = Scripts.Find(name);
    if (it != Scripts.end())
        return &it->second;
    return 0;
}