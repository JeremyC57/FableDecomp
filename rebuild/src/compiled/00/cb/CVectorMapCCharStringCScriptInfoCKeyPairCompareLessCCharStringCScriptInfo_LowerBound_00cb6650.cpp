// CVectorMap<CCharString,CScriptInfo,CKeyPairCompareLess<CCharString,CScriptInfo> >::LowerBound(const CCharString& key) const -- the const overload of the 24-byte-entry binary search: first entry whose key does not sort before the key
// Leaf: ?LowerBound@?$CVectorMap@VCCharString@@VCScriptInfo@@V?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@@QBEPBU?$pair@VCCharString@@VCScriptInfo@@@std@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB6650, 121 bytes (+ padding), thiscall, `ret 4`, the entry pointer in eax.
// Identity (proven): byte-identical twin of the landed non-const LowerBound 0x00CB65D0. Ego_r.pdb has both overloads
//   (?LowerBound@?$CVectorMap@VCCharString@@VCScriptInfo@@...@@QAE... and ...@@QBE...). The non-const one is called by
//   CScriptInfoManager::GetPScriptInfo 0x00CB5AD0 (QAE); this one by the byte-identical const twin 0x00CB5B30 (site 0x00CB5B40),
//   i.e. CScriptInfoManager::PeekPScriptInfo(const CCharString&) const (Ego_r.pdb QBE), and by 0x00CB6A7A beside the non-const
//   call 0x00CB6A2A. The manifest label _gapscan::sub_00cb6650 was a placeholder.
// Model: the landed 0x00CB65D0 source with the member made const (STLport vector storage: a const entry pointer).
template <class C> class CBasicString {
public:
    bool operator<(const CBasicString& other) const;     // 0x00429950
};
class CCharString {
public:
    CBasicString<char>* PData;
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
class CScriptInfo { public:
    unsigned char _payload[20];                      // opaque value
};
namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
};
}
template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
    bool operator()(const K& key, const std::pair<K, V>& entry) const { return key < entry.first; }
};
template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                   // +0x00
    value_type* Last;                                    // +0x04
    value_type* EndOfStorage;                            // +0x08
    value_type* LowerBound(const K& key);              // 0x00CB65D0 (landed)
    const value_type* LowerBound(const K& key) const;
};
template <class K, class V, class C>
const typename CVectorMap<K, V, C>::value_type* CVectorMap<K, V, C>::LowerBound(const K& key) const
{
    int count = Last - First;
    const value_type* first = First;
    C comp;
    if (count > 0)
    {
        K k = key;
        do
        {
            int half = count / 2;
            const value_type* middle = first + half;
            if (comp(*middle, k))
            {
                first = middle + 1;
                count -= half + 1;
            }
            else
                count = half;
        } while (count > 0);
    }
    return first;
}
template const CVectorMap<CCharString, CScriptInfo, CKeyPairCompareLess<CCharString, CScriptInfo> >::value_type*
    CVectorMap<CCharString, CScriptInfo, CKeyPairCompareLess<CCharString, CScriptInfo> >::LowerBound(const CCharString& key) const;