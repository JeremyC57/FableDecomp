// CVectorMap<CCharString,CScriptInfo,CKeyPairCompareLess<CCharString,CScriptInfo> >::LowerBound(const CCharString& key) -- binary search of the sorted 24-byte entries for the first entry whose key does not sort before the key
// Leaf: ?LowerBound@?$CVectorMap@VCCharString@@VCScriptInfo@@V?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@@QAEPAU?$pair@VCCharString@@VCScriptInfo@@@std@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB65D0, 128 bytes, `__fastcall`, `ret 4`, the entry pointer (STLport iterator) in eax.
// Instance: the CCharString -> CScriptInfo map (callers 0x00CB5AD0 quest-persist lookup, the pair<CCharString,CScriptInfo> _Adjust_heap 0x00CB68C0). The manifest's value types are propagated across byte-identical bodies (the
//   entry stride here is 24 bytes), so the payload after the CCharString key is modelled opaque (20 bytes).
// Library (fix, sweep1 2026-10-25, FINDINGS chunk 450): this TU links STLport 4.6, not Dinkumware; the loop is not STLport's
//   __lower_bound either (that halves with `>> 1`, a bare `sar`), so it is CVectorMap's own search over the STLport vector.
// Callees: CBasicString<char>::operator< @ 0x00429950 (the string data compare behind the inline CCharString::operator<).
// Body: a signed `count / 2` binary search (`cdq ; sub ; sar 1`) -- count = (last - first) / sizeof(entry); halve, compare the middle entry,
//   step past it (`count -= half + 1`, emitted `or -1 ; sub ; add`) or keep the lower half.
// Shape: the key's string-data pointer is read ONCE after the empty-map test (the call could alias a re-read), so the
//   key is copied into a local inside `if (count > 0) { ...; do { } while (count > 0); }` -- a copy before the test
//   is scheduled above the `jle`. The inline CCharString::operator< tests `other.PData == PData`, then a null left,
//   then a null right, then calls the data compare (the same order as std::less<CCharString> 0x0042AAB3).
// Order: the count is computed before `first` is copied from First (the other order loads First straight into ebp
//   and drops retail's `mov ebp,esi`).
// Layout: STLport vector {first, last, end_of_storage}; entry pair<CCharString, value>, key at +0 (one data pointer).
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
    value_type* LowerBound(const K& key);
};
template <class K, class V, class C>
typename CVectorMap<K, V, C>::value_type* CVectorMap<K, V, C>::LowerBound(const K& key)
{
    int count = Last - First;
    value_type* first = First;
    C comp;
    if (count > 0)
    {
        K k = key;
        do
        {
            int half = count / 2;
            value_type* middle = first + half;
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
template class CVectorMap<CCharString, CScriptInfo, CKeyPairCompareLess<CCharString, CScriptInfo> >;