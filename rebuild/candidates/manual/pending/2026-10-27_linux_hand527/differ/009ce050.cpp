// CBankFile::InitEntry(unsigned long index, const CCharString& symbol, const CCharString& filename) -- index a new entry: by symbol (OpenFlags 0x40), by symbol CRC in the vector map (0x100), and by its lower-cased file name in the filename multimap (0x80, non-empty names only)
// Grade: DIFFER(294v302) -- the CRC insert now matches (it needs the vector's _M_insert_overflow body in the TU); the one residue is the lower-cased name's intermediate: retail destroys the ConvertPathToFilename temporary right after GetAsLowerCase (a named `CCharString name = ConvertPathToFilename(filename).GetAsLowerCase();` statement, probe/ce050_b shows that shape), ours keeps it to the end of the insert expression. 4 of 4 grades (probe/ce050_a..d); next step: probe b's named local on top of this file.
// Leaf: ?InitEntry@CBankFile@@AAEXKABVCCharString@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CE050, 302 bytes, thiscall, `ret 0Ch`. Callers 0x009CE6FD, 0x009CE892 (in CBankFile
//   code) and 0x009D026A.
// Identity: Ego_r.pdb `?InitEntry@CBankFile@@AAEXKABVCCharString@@0@Z` (the CBankFile member taking (unsigned long, const
//   CCharString&, const CCharString&)); the body fills exactly the PDB's SymbolIndices (+0xac retail), FilenameIndices
//   (+0xc4) and SymbolCRCIndices (+0xd0) under the OpenFlags (+0x78) bits. The manifest label "BankEntry_ProcessPath" is
//   descriptive.
// Callees: map<CCharString,unsigned long>::operator[] 0x0042D131 (bound as the invented CRetailMap), CCRC::Calc 0x004014A0,
//   vector<pair<unsigned long,unsigned long> >::_M_insert_overflow 0x009B6300, CBankFile::ConvertPathToFilename 0x009CBE10,
//   CCharString::GetAsLowerCase 0x0099ED10 / copy ctor 0x0099EC30 / dtor 0x0099EAE0, multimap insert_equal 0x009D34E0.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
extern const char g_emptyText_0129aaf4[];                  // 0x0129AAF4 ("", CCharString's empty text)
class CCharStringData { public:
    const char*   PData;                                   // +0
    unsigned long StringLength;                            // +4
};
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString GetAsLowerCase() const;                    // 0x0099ED10
    const char* GetData() const { return PStringData == 0 ? g_emptyText_0129aaf4 : PStringData->PData; }
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    bool IsEmpty() const { return PStringData == 0 || PStringData->StringLength == 0; }
};
class CCRC { public:
    static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
};

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6 _pair.h
};
template <class _T1, class _T2> inline pair<_T1, _T2> make_pair(const _T1& __x, const _T2& __y) { return pair<_T1, _T2>(__x, __y); }
template <class _Pair> struct _Select1st { };
struct _Rb_tree_node_base { char _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right; };
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree { public:
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    _Compare            _M_key_compare;
    _Rb_tree_iterator<_Value> insert_equal(const _Value& __v);   // 0x009D34E0 (multimap)
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
    _Tp& operator[](const _Key& __k);                      // 0x0042D131
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class multimap { public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
    _Rb_tree_iterator<value_type> insert(const value_type& __x) { return _M_t.insert_equal(__x); }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(__cur, __x);
    return __cur;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    __forceinline void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x009B6300
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                                            bool __atend)   // STLport 4.6 _vector.c (the TU's own COMDAT copy)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (__old_size < __fill_len ? __fill_len : __old_size);
    _Tp* __new_start = __len != 0 ? (_Tp*)malloc(__len * sizeof(_Tp)) : 0;
    _Tp* __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
    if (_M_start)
        free(_M_start);
    _M_start = __new_start;
    _M_finish = __new_finish;
    _M_end_of_storage = __new_start + __len;
}
}

template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class Compare> class CVectorMap {   // 13+ bytes: the pair vector, the comparator, Dirty
public:
    typedef std::pair<K, V> value_type;
    std::vector<value_type> Data;                          // +0x00
    Compare                 Comparator;                    // +0x0c
    bool                    Dirty;                         // +0x0d
    __forceinline void Insert(const K& key, const V& value)
    {
        Data.push_back(value_type(key, value));
        Dirty = true;
    }
};

class CBankFile {                                          // the retail layout (Ego_r.pdb names, STLport sizes)
public:
    unsigned char _pad000[0x78];
    unsigned long OpenFlags;                               // +0x78
    unsigned char _pad07c[0xac - 0x7c];
    std::map<CCharString, unsigned long>      SymbolIndices;      // +0xac
    std::map<CCharString, unsigned long>      OldSymbolIndices;   // +0xb8
    std::multimap<CCharString, unsigned long> FilenameIndices;    // +0xc4
    CVectorMap<unsigned long, unsigned long, CKeyPairCompareLess<unsigned long, unsigned long> > SymbolCRCIndices;   // +0xd0
private:
    CCharString ConvertPathToFilename(const CCharString& path) const;   // 0x009CBE10
    void InitEntry(unsigned long index, const CCharString& symbol, const CCharString& filename);
};

void CBankFile::InitEntry(unsigned long index, const CCharString& symbol, const CCharString& filename)
{
    if (OpenFlags & 0x40)
        SymbolIndices[symbol] = index;
    if (OpenFlags & 0x100)
        SymbolCRCIndices.Insert(CCRC::Calc(0, symbol.GetData(), symbol.GetLength()), index);
    if ((OpenFlags & 0x80) && !filename.IsEmpty())
        FilenameIndices.insert(std::make_pair(ConvertPathToFilename(filename).GetAsLowerCase(), index));
}
