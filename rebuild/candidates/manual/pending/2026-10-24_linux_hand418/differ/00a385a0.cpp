// CCRCSymbolMap::AddSymbolDirty(const CCharString& name, long value) -- append (CRC(name), value) to the CRC->value map unsorted and mark it dirty; with duplicate checking on, first record the CRC in the used-names set
// Grade: DIFFER(203v195) -- the second CRC's chars select lands in eax + `mov edx,eax` (retail preloads "" straight into edx), and the pair's value word is stored before the end-of-storage compare and re-read for the placement copy (retail keeps it in edx); 4 grades (forceinline Insert, named pair local, STLport pair copy ctor)
// Leaf: ?AddSymbolDirty@CCRCSymbolMap@@
// Retail: Fable.exe @ 0x00A385A0, 195 bytes (+ int3 padding), thiscall `ret 8`. FableWin.pdb
//   ?AddSymbolDirty@CCRCSymbolMap@@QAEXABVCCharString@@J@Z.
// Callers: CCRCSymbolMap::ParseEnum 0x00A38670 (the landed source calls it "AddEnumValue").
// Callees: Calc 0x004014A0 (CRC-32, `__fastcall(seed, chars, length)`), set<unsigned long>::insert 0x00514C10 (pair
//   result through a hidden pointer, discarded), vector<pair<unsigned long, long> >::_M_insert_overflow 0x009B61F0.
// Label: catalog "EnumValue_InsertIntoMap" is descriptive.
// Shape: the CRC is computed twice (once per use); the set key temporary is homed in the name slot; the inline
//   CVectorMap::Insert builds the pair in the frame and push_backs it (`_Construct` from the registers, `__true_type`
//   tag homed in the name slot on the overflow path), then Dirty = true on both paths.

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

struct CCharStringData {                                    // CBasicString<char> head of the shared block
    const char*   PData;                                    // +0x00
    unsigned long StringLength;                             // +0x04
};
extern const char g_emptyLiteral_0129aaf4[];                // 0x0129AAF4: ""
class CCharString {
public:
    CCharStringData* PStringData;
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetPString() const { return PStringData == 0 ? g_emptyLiteral_0129aaf4 : PStringData->PData; }
};

unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC-32

class CBase {
public:
    virtual ~CBase();
};

namespace _STL {
struct __true_type { __true_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    _Tp* end() const { return _M_finish; }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend);   // 0x009B61F0
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __true_type(), 1UL, true);
    }
};
template <class _Tp> struct less { };
struct _Rb_tree_node_base;
struct _Rb_tree_iterator { _Rb_tree_node_base* _M_node; };
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    void*        _M_header;                                 // +0x00
    unsigned int _M_node_count;                             // +0x04
    _Compare     _M_key_compare;                            // +0x08
    pair<_Rb_tree_iterator, bool> insert(const _Key& __x);  // 0x00514C10
};
}

template <class K, class V> class CKeyPairCompareLess { };

template <class K, class V, class C = CKeyPairCompareLess<K, V> > class CVectorMap {   // retail 16 bytes
public:
    typedef _STL::pair<K, V> value_type;
    _STL::vector<value_type> Data;                          // +0x00
    C                        Compare;                       // +0x0C (empty comparator)
    bool                     Dirty;                         // +0x0D
    const value_type* LowerBound(const K& key) const;       // 0x009B21F0
    const value_type* End() const { return Data.end(); }
    const value_type* Find(const K& key) const
    {
        const value_type* it = LowerBound(key);
        if (it == Data.end() || key < it->first)
            return Data.end();
        return it;
    }
    __forceinline void Insert(const K& key, const V& value)
    {
        Data.push_back(value_type(key, value));
        Dirty = true;
    }
};

// Layout: Ego_r.pdb CCRCSymbolMap with the retail 16-byte CVectorMap and 12-byte STLport set (the landed constructor
// 0x00A38500): SLONGMap +0x04 (Dirty +0x11), NamesUsed +0x14, CheckForDuplicateSymbols +0x20.
class CCRCSymbolMap : public CBase {                        // vtable 0x0129CF84
public:
    CVectorMap<unsigned long, long> SLONGMap;               // +0x04
    _STL::set<unsigned long>        NamesUsed;              // +0x14
    bool                            CheckForDuplicateSymbols;   // +0x20
    void AddSymbolDirty(const CCharString& name, long value);
    bool FindSymbol(const CCharString& name, long& value) const;
};

void CCRCSymbolMap::AddSymbolDirty(const CCharString& name, long value)
{
    if (CheckForDuplicateSymbols)
        NamesUsed.insert(Calc_004014a0(0, name.GetPString(), name.GetLength()));
    SLONGMap.Insert(Calc_004014a0(0, name.GetPString(), name.GetLength()), value);
}
