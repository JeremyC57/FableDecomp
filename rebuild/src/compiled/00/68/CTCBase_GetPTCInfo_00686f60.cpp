// CTCBase::GetPTCInfo(const CCharString& name) -- look a TC class name up in the registered-TC map: the registration info, or null when the name is not registered
// Leaf: ?GetPTCInfo@CTCBase@@
//
// Retail 0x00686F60, 62 bytes (+ int3 padding to 64), plain `ret` (static __fastcall: the name in ecx).
// Callees: CVectorMap<CCharString, CTCBase::CRegisteredTCInfo>::LowerBound @ 0x006870C0 on the global map @ 0x013BA89C
//   (end word @ 0x013BA8A0; the landed CTCBase::Clone 0x00686FA0 uses the same pair), CBasicString<char>::operator< @
//   0x00429950 (the inline CCharString compare's last step).
// Identity: Ego_r.pdb ?GetPTCInfo@CTCBase@@SIPBVCRegisteredTCInfo@1@ABVCCharString@@@Z (static, __fastcall); nine
//   callers in CThingComponentSet (0x0087033B..0x0087075A; landed CThingComponentSet::Add 0x00870750 names it).  The
//   catalog's "CVectorMap_FindMeshVolume" is propagated.
// Layout: Ego_r.pdb CTCBase::CRegisteredTCInfo (12 bytes) = { AllocFunc; ulong DriverType; CCharString Name; }, so the
//   map's pair<CCharString, CRegisteredTCInfo> is 16 bytes and the info sits at +4.
// Shape: `it = LowerBound(name); if (it == end() || name < it->first) return 0; return &it->second;` with the inline CCharString operator< (same pointer: not less; null name: less; null key: not less;
//   else the string compare).
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
    static const CRegisteredTCInfo* __fastcall GetPTCInfo(const CCharString& name);
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

const CTCBase::CRegisteredTCInfo* __fastcall CTCBase::GetPTCInfo(const CCharString& name)
{
    std::pair<CCharString, CRegisteredTCInfo>* it = g_RegisteredTCs.LowerBound(name);
    if (it == g_RegisteredTCs.end() || name < it->first)
        return 0;
    return &it->second;
}