// CMap::InitThemePalette() -- empty the theme palette, refill it with 256 unused zero entries, mark entry 0 used and add the global def's invalid-theme stand-in
// Leaf: ?InitThemePalette@
// Retail 0x008F71A0, 0xAD bytes (+int3 padding), thiscall, plain `ret`. Manifest label "CMap_InitializePalette"
//   (_global); FableWin PDB name set: `?InitThemePalette@CMap@@IAEXXZ` (the one void() CMap member about the palette
//   besides the landed ZeroAllUnusedThemePaletteEntries). Callers: the two CMap constructors 0x00820CC0 / 0x008224E0
//   (the landed ctor calls it "InitialisePalette").
// Callees: vector<CThemePaletteEntry>::_M_insert_overflow 0x008F7390 (manifest "std_vector_insert_range_fill"),
//   CMap::AddThemeDefIndexToPalette(unsigned long) 0x008F70F0 (`?AddThemeDefIndexToPalette@CMap@@IAEEK@Z`).
// Layout: retail CMap DefinitionManager +0x1c, ThemePaletteEntryList +0x1a6c (the landed CMap ctor model; Ego_r.pdb +0x1a78);
//   CGameDefinitionManager's global-def CDefPointer +0xd0 (landed 0x00450BFF); retail CGlobalDef InvalidThemeStandin +0x50
//   (landed CGlobalDef::operator= 0x00465EB3); CThemePaletteEntry { ulong ThemeDefIndex +0; bool ThemePaletteIndexUsed +4 }.
// Shape: STLport 4.6 clear() = erase(begin(), end()) through __copy_ptrs(__false_type) -> the random-access __copy loop
//   (its `end - end` count is not folded) and the empty trivial _Destroy; the 256 push_backs use one entry local built
//   before the loop. The same-TU noinline _M_insert_overflow stand-in (reads x, never writes it) lets VC7.1 forward the
//   entry's zero ThemeDefIndex from ebx in the inline copy-construct (`mov [eax],ebx`) as retail does.
inline void* operator new(unsigned int, void* __p) { return __p; }

namespace std {
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _Tp>
inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                              // +0x00
    _Tp* _M_finish;                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    _Tp& operator[](unsigned int __n) { return *(begin() + __n); }
#pragma auto_inline(off)
    // 0x008F7390. Same-TU noinline STAND-IN body (only its call matters): it reads `x` and never writes it.
    protected:
    __declspec(noinline) void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                                 unsigned int __fill_len, bool __atend)
    {
        if (__atend && __fill_len == 1 && __position == this->_M_finish)
            _Construct(this->_M_start, __x);
    }
    public:
#pragma auto_inline(on)
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
}

class CGlobalDef {
public:
    unsigned char _pad00[0x50];
    long InvalidThemeStandin;                           // +0x50
};
template <class T> class CDefPointer {
public:
    T* Data;
    T& operator*() const { return *Data; }
};
class CGameDefinitionManager {
public:
    unsigned char _base[0xd0];                          // CDefinitionManager
    CDefPointer<const CGlobalDef> PGlobalDef;           // +0xd0
    const CGlobalDef& PeekGlobalDef() const { return *PGlobalDef; }
};

class CThemePaletteEntry {
public:
    unsigned long ThemeDefIndex;                        // +0x00
    bool ThemePaletteIndexUsed;                         // +0x04
    CThemePaletteEntry(unsigned long themeDefIndex, bool used) : ThemeDefIndex(themeDefIndex), ThemePaletteIndexUsed(used) {}
    void SetThemePaletteIndexUsed(bool used) { ThemePaletteIndexUsed = used; }
};

class CMap {
public:
    unsigned char _pad00[0x1c];
    const CGameDefinitionManager* DefinitionManager;    // +0x1c
    unsigned char _pad20[0x1a6c - 0x20];
    std::vector<CThemePaletteEntry> ThemePaletteEntryList;   // +0x1a6c
    unsigned char AddThemeDefIndexToPalette(unsigned long themeDefIndex);   // 0x008F70F0
    void InitThemePalette();
};

void CMap::InitThemePalette()
{
    ThemePaletteEntryList.clear();
    CThemePaletteEntry entry(0, false);
    for (long i = 0; i < 256; ++i) {
        ThemePaletteEntryList.push_back(entry);
        ThemePaletteEntryList[i].SetThemePaletteIndexUsed(false);
    }
    ThemePaletteEntryList[0].SetThemePaletteIndexUsed(true);
    AddThemeDefIndexToPalette(DefinitionManager->PeekGlobalDef().InvalidThemeStandin);
}