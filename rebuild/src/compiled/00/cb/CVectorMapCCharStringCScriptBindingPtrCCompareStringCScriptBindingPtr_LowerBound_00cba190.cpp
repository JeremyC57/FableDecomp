// CVectorMap<CCharString,CScriptBindingPtr,CCompareString<CScriptBindingPtr> >::LowerBound(const CCharString& key) -- binary search of the sorted 12-byte entries for the first entry whose key does not sort before the key
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBA190, 96 bytes, `__fastcall`, `ret 4`, the entry pointer (STLport iterator) in eax.
// Instance: a CCharString -> CCountedPointer<CEntityScriptBindingBase> map compared with CCharString::FastCompare (callers CScriptBase::AttachNavigator 0x00CB8960, the pair _Copy_backward 0x00CBAE30). The manifest's value types are propagated across byte-identical bodies (the
//   entry stride here is 12 bytes), so the payload after the CCharString key is modelled opaque (8 bytes).
// Callees: CCharString::FastCompare(const CCharString&) const @ 0x009E6530 (the CCompareString "less", called on the
//   middle entry's key with the caller's key reference re-pushed every iteration).
// Body: Dinkumware-style lower bound -- count = (last - first) / sizeof(entry); halve, compare the middle entry,
//   step past it (`count -= half + 1`, emitted `or -1 ; sub ; add`) or keep the lower half.
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
    bool FastCompare(const CCharString& other) const;   // 0x009E6530
};
struct CScriptBindingPtr {
    unsigned char _payload[8];                      // opaque value
};
namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
};
}
template <class V> class CCompareString { public:
    bool operator()(const std::pair<CCharString, V>& entry, const CCharString& key) const { return entry.first.FastCompare(key); }
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
    while (count > 0)
    {
        int half = count / 2;
        value_type* middle = first + half;
        if (comp(*middle, key))
        {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}
template class CVectorMap<CCharString, CScriptBindingPtr, CCompareString<CScriptBindingPtr> >;