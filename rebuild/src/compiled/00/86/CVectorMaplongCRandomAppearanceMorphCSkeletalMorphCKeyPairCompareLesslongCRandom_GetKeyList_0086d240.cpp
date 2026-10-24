// CVectorMap<long,CRandomAppearanceMorph::CSkeletalMorph,CKeyPairCompareLess<long,CRandomAppearanceMorph::CSkeletalMorph> >::GetKeyList(std::vector<long>& keys) const -- append each distinct key of the sorted map to a vector, in order
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086D240, 90 bytes, `__fastcall`, `ret 4`.
// Callees: the STLport vector<long> grow-and-insert `_M_insert_overflow(pos, const long&, __true_type, 1, true)` @ 0x00440990
//   (the landed push_back model of CEnginePrimitiveManager*::GetSupportedTypes 0x00BBAC20 calls it as InsertGrow).
// Instance: the long-keyed 16-byte-entry map whose LowerBound/UpperBound are 0x0086A930/0x0086A980 (the value type in the
//   label is a propagated hint; the 16-byte stride is what the body proves).
// Body: walk the entries; the first key, and every key different from the last one appended, is push_back'ed (sorted keys
//   -> each distinct key once). The map's end is re-read every iteration (the push_back may alias it).
// Shape: `bool first = true` + `long lastKey = 0` give retail's `mov cl,1` / `xor eax,eax`; `first || key != lastKey`
//   tests the flag first; the inline push_back stores through a null-checked placement new, else calls the grow insert
//   with the empty scalar tag homed in the dead `keys` argument slot.
#include <new>

struct SScalarTag { SScalarTag() { } };

namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
};
template <class T> class vector {
public:
    T* _M_start;                                          // +0x00
    T* _M_finish;                                         // +0x04
    T* _M_end_of_storage;                                 // +0x08
    void _M_insert_overflow(T* pos, const T& x, const SScalarTag& tag, unsigned long n, bool atend);   // 0x00440990
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, SScalarTag(), 1, true);
    }
};
}
struct SValue {
    unsigned char _payload[12];                           // opaque CRandomAppearanceMorph::CSkeletalMorph
};
template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
};
template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                    // +0x00
    value_type* Last;                                     // +0x04
    value_type* EndOfStorage;                             // +0x08
    void GetKeyList(std::vector<K>& keys) const;
};
template <class K, class V, class C>
void CVectorMap<K, V, C>::GetKeyList(std::vector<K>& keys) const
{
    K lastKey = 0;
    bool first = true;
    for (const value_type* it = First; it != Last; ++it)
    {
        if (first || it->first != lastKey)
        {
            keys.push_back(it->first);
            lastKey = it->first;
            first = false;
        }
    }
}
template class CVectorMap<long, SValue, CKeyPairCompareLess<long, SValue> >;