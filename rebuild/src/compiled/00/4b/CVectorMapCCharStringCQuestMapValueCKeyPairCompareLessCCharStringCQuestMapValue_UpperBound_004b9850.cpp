// CVectorMap<CCharString,CQuestMapValue,CKeyPairCompareLess<CCharString,CQuestMapValue> >::UpperBound(const CCharString& key) -- binary search of the sorted 12-byte entries for the first entry whose key sorts after the key
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B9850, 121 bytes, `__fastcall`, `ret 4`, the entry pointer (STLport iterator) in eax.
// Instance: the CQuestManager 12-byte-entry map (caller CQuestManager::AddQuestRegion 0x004B3900, next to its LowerBound 0x004B9750). The manifest's value types are propagated across byte-identical bodies (the
//   entry stride here is 12 bytes), so the payload after the CCharString key is modelled opaque (8 bytes).
// Callees: CBasicString<char>::operator< @ 0x00429950 (the string data compare behind the inline CCharString::operator<).
// Body: Dinkumware-style lower/upper bound -- count = (last - first) / sizeof(entry); halve, compare the middle entry,
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
struct CQuestMapValue {
    unsigned char _payload[8];                      // opaque value
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
    value_type* UpperBound(const K& key);
};
template <class K, class V, class C>
typename CVectorMap<K, V, C>::value_type* CVectorMap<K, V, C>::UpperBound(const K& key)
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
            if (!comp(k, *middle))
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
template class CVectorMap<CCharString, CQuestMapValue, CKeyPairCompareLess<CCharString, CQuestMapValue> >;