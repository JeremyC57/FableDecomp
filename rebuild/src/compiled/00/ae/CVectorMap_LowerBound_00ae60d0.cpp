// CVectorMap::LowerBound(const CCharString& key) const -- (CVectorMap<CCharString,unsigned long,CKeyPairCompareLess<CCharString,unsigned long> >) binary search of the sorted 8-byte (name, value) entries for the first one whose name does not sort before the key
// Leaf: ?LowerBound@?$CVectorMap@VCCharString@@KV?$CKeyPairCompareLess@VCCharString@@K@@@@QBEPBU?$pair@VCCharString@@K@std@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE60D0, 86 bytes, thiscall `ret 4`, the entry pointer (STLport const_iterator) in eax.
// Identity (probable): the only CVectorMap with a CCharString key, a 4-byte value and an operator<-based comparator in the
//   Ego_r.pdb name set is ?LowerBound@?$CVectorMap@VCCharString@@KV?$CKeyPairCompareLess@VCCharString@@K@@@@QBE... (Dinkumware
//   const_iterator there; STLport's is a pointer). Its callers fit: 0x00AE16C0 looks the name up in the map at owner+0x88 and
//   returns entry.second or -1, and 0x00AE6380 is the map's Find (LowerBound, then the same CCharString < test). The
//   CCharString/CCharString map (also 8-byte entries) is ruled out by 0x00AE16C0 returning the value as a dword.
// Callees: CBasicString<char>::operator< 0x00429950 through the inline CCharString::operator< (equal data pointers -> false,
//   null entry name -> true, null key -> false); the key's data pointer is hoisted (ebp), which VC7.1 does only when the
//   callee's body is visible in the TU (noinline, as the landed 0x007564F0 of the same family).
// Model: the landed 0x00A6F2F0 LowerBound text with the landed 0x007564F0 CCharString / CBasicString model.
template <class T> class CBasicString {
public:
    __forceinline long Compare(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    bool operator<(const CBasicString& other) const;
    T* m_pData;
};
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CBasicString<T>::operator<(const CBasicString& other) const    // 0x00429950
{
    return Compare(other) < 0;
}
#pragma auto_inline(on)
class CCharStringData {
public:
    CBasicString<char> Data;
};
class CCharString {
public:
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return PStringData->Data < other.PStringData->Data;
    }
    CCharStringData* PStringData;
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class T> class vector {
public:
    typedef const T* const_iterator;
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
};
}
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
};
template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    typedef std::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    C    Compare;                                        // +0x0C
    bool Dirty;                                          // +0x0D
    const_iterator LowerBound(const K& key) const;
};
template <class K, class V, class C>
typename CVectorMap<K, V, C>::const_iterator CVectorMap<K, V, C>::LowerBound(const K& key) const
{
    int count = this->end() - this->begin();
    const_iterator first = this->begin();
    while (count > 0)
    {
        int half = count / 2;
        const_iterator middle = first + half;
        if (Compare(*middle, key))
        {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}
typedef CVectorMap<CCharString, unsigned long, CKeyPairCompareLess<CCharString, unsigned long> > CNameIndexMap;
template CNameIndexMap::const_iterator CNameIndexMap::LowerBound(const CCharString& key) const;   // emit just this member