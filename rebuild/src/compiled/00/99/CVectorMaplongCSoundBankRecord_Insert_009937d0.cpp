// CVectorMap<long, CSoundBankRecord>::Insert(const long& key, const CSoundBankRecord& value) -- append a {key, record} entry to the map's vector (unsorted) and mark the map dirty
// Leaf: ?Insert@?$CVectorMap@JVCSoundBankRecord@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009937D0, length 132 bytes (+ int3 padding), `ret 8`.
// PDB: CVectorMap<K,V,CKeyPairCompareLess<K,V> >::Insert(const K&, const V&) (FableWin, e.g.
// ?Insert@?$CVectorMap@KVCSoundPair@@...@@QAEXABKABVCSoundPair@@@Z); layout vector +0x00 (STLport, 12 bytes), empty
// Compare +0x0C, Dirty +0x0D (as the landed CVectorMap::Sort 0x00993A10).
// Callees: CSoundBankRecord copy constructor @ 0x009922F0 (the pair temporary, then the placed copy), ~CSoundBankRecord @
// 0x00992510 (the temporary), std::vector<CSoundBankEntry>::_M_insert_overflow @ 0x00993310.
// Label: the manifest's "NScript_CIsThingAlive_Vector_Insert" is propagated; the landed WoundMorphs twin 0x00759570
// calls this method "Add".
// Element: std::pair<long, CSoundBankRecord> (0x24 bytes) -- the CASoundManager's bank map entries (see the landed
// ~CSoundBankRecord 0x00992510 and the sort family 0x00992750..0x00993420); record = two CCountedPointers + four
// CCharStrings with an implicit copy constructor (here called out of line, retail 0x009922F0) and out-of-line
// assignment / destructor. Record / comparator names are descriptive (the sound module is absent from the Ego PDB).
// Shape: `Data.push_back(value_type(key, value)); Dirty = true;` -- the pair temporary in the frame (key dword, record by
// its out-of-line copy), push_back inlined (placement copy under its null test; overflow with the tag homed on the dead
// key slot), the temporary destroyed and Dirty set on both paths.
#include <new>
#include <stddef.h>

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};

class CCPPointerInfo {
public:
    long RefCount;                              // +0x00
};

template <class T> class CCountedPointer {
public:
    T*              Data;                       // +0x00
    CCPPointerInfo* Info;                       // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};

class CSoundBank;
class CSoundBankData;

// The 0x20-byte sound-bank record (descriptive; see the landed ~CSoundBankRecord @ 0x00992510): implicit copy
// constructor, out-of-line assignment and destructor.
class CSoundBankRecord {
public:
    CCountedPointer<CSoundBank>     Bank;       // +0x00
    CCountedPointer<CSoundBankData> BankData;   // +0x08
    CCharString Name0;                          // +0x10
    CCharString Name1;                          // +0x14
    CCharString Name2;                          // +0x18
    CCharString Name3;                          // +0x1c
    CSoundBankRecord& operator=(const CSoundBankRecord& other);   // 0x00992630
    ~CSoundBankRecord();                        // 0x00992510
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;                                   // +0x00
    T2 second;                                  // +0x04
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

}   // namespace std

typedef std::pair<long, CSoundBankRecord> CSoundBankEntry;     // 0x24 bytes

// CVectorMap<long, CSoundBankRecord>'s key order (an empty functor passed by value)
struct CSoundBankEntryLess {
    bool operator()(const CSoundBankEntry& a, const CSoundBankEntry& b) const { return a.first < b.first; }
};

namespace std {

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> class allocator { public: allocator() { } };

template <class Value, class T, class Alloc> class _STLP_alloc_proxy : public Alloc {
public:
    Value _M_data;
};

template <class T, class Alloc> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;
};

template <class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    void _M_insert_overflow(T* position, const T& x, const __false_type&, size_t fillLen, bool atEnd = false);   // 0x00993310
    void push_back(const T& x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, x, __false_type(), 1UL, true);
    }
};

}   // namespace std

template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

template <class K, class V, class C = CKeyPairCompareLess<K, V> > class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    std::vector<value_type> Data;              // +0x00
    C Compare;                                  // +0x0c (empty)
    bool Dirty;                                 // +0x0d
    void Insert(const K& key, const V& value)
    {
        Data.push_back(value_type(key, value));
        Dirty = true;
    }
};

template class CVectorMap<long, CSoundBankRecord>;