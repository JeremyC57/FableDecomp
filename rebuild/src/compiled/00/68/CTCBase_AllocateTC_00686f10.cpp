// CTCBase::AllocateTC(const CCharString& name, CThing& owner) -- create a thing component by its registered class name: the registered allocator's new component for the owner, or null when the name is not registered
// Leaf: ?AllocateTC@CTCBase@@SIPAV1@ABVCCharString@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686F10, 69 bytes, static __fastcall (name in ecx, owner in edx), plain `ret`.
// Identity (proven): Ego_r.pdb donor order AllocateTC 0x006B2200 -> GetPTCInfo 0x006B2260 matches retail this row -> GetPTCInfo
//   0x00686F60 (landed); the signature ?AllocateTC@CTCBase@@SIPAV1@ABVCCharString@@AAVCThing@@@Z fits (ecx name, edx owner, the
//   allocator called with the owner); its only caller is CThing::AddTC 0x004C9D60 (0x004C9D7F). The catalog label
//   "CMeshVolumeMap::Find" is propagated.
// Callees and globals: exactly the landed GetPTCInfo 0x00686F60 model: LowerBound 0x006870C0 on the registered-TC map 0x013BA89C
//   (end word 0x013BA8A0), CBasicString<char>::operator< 0x00429950, then the info's AllocFunc (__fastcall, the owner in ecx).
//   The landed spellings for 0x013BA89C/0x006870C0 are invented (`g_RegisteredTCs` of type CRegisteredTCMap, LowerBound); Ego_r.pdb
//   has the static member CTCBase::RegisteredTCs, a CVectorMap<CCharString, CRegisteredTCInfo, CKeyPairCompareLess<...> > -> they are
//   reused here unchanged (one address, one name) and flagged for review at landing.
// Shape: `it = LowerBound(name); if (it == end() || name < it->first) return 0; return it->second.AllocFunc(owner);`.
class CThing;
class CTCBase;

class CBasicString {
public:
    bool operator<(const CBasicString* other) const;        // 0x00429950 (CBasicString<char>::operator<)
};
class CCharString {
public:
    CBasicString* PString;                                  // +0x00 (CCharStringData*)
    bool operator<(const CCharString& o) const
    {
        if (o.PString == PString)
            return false;
        if (!PString)
            return true;
        if (!o.PString)
            return false;
        return PString->operator<(o.PString);
    }
};

class CTCBase {
public:
    class CRegisteredTCInfo {                               // 12 bytes
    public:
        CTCBase*      (__fastcall* AllocFunc)(CThing& owner);   // +0x00
        unsigned long DriverType;                           // +0x04
        CCharString   Name;                                 // +0x08
    };
    static CTCBase* __fastcall AllocateTC(const CCharString& name, CThing& owner);
};

namespace std {
template <class _T1, class _T2> class pair {
public:
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
};
}

template <class K, class V> class CRegisteredTCMap {        // CVectorMap<K, V, CKeyPairCompareLess<K, V> > (its vector words)
public:
    std::pair<K, V>* m_first;                               // 0x013BA89C
    std::pair<K, V>* m_last;                                // 0x013BA8A0
    std::pair<K, V>* end() { return m_last; }
    std::pair<K, V>* LowerBound(const K& key);              // 0x006870C0
};
extern CRegisteredTCMap<CCharString, CTCBase::CRegisteredTCInfo> g_RegisteredTCs;   // 0x013BA89C

CTCBase* __fastcall CTCBase::AllocateTC(const CCharString& name, CThing& owner)
{
    std::pair<CCharString, CRegisteredTCInfo>* it = g_RegisteredTCs.LowerBound(name);
    if (it == g_RegisteredTCs.end() || name < it->first)
        return 0;
    return it->second.AllocFunc(owner);
}