// CAnimationSet::GetRandomLoadedAnim(const CCharString& name, unsigned long& seed, bool returnUnloaded, const CAnimFilterBase& filter) const -- picks a random filter-accepted animation stored under the name: the pick if its bank animation is loaded, else starts loading it and picks among the loaded ones (or returns the unloaded pick, or 0)
// Grade: DIFFER(659v608) -- register allocation: retail keeps this in esi, the iterator in ebx, &it->second in ebp and only the accepted vector's finish in edi (the loaded vector's finish is re-read from its slot); VC7.1 here also caches the loaded vector's finish (ebp) and spills this to an extra frame slot (sub esp,20h), so every later register role and the three free/return tails differ. Tried: opaque vs same-TU reserve/_M_insert_overflow (stand-in and full STLport bodies), continue/while/break loop forms, it-before-first, vector declaration order (B first: 271 normalised lines, best g7-form 321).
// Leaf: ?GetRandomLoadedAnim@CAnimationSet@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DCD10, 606 bytes (0x25E, no padding), thiscall `ret 10h`. Ego_r.pdb
//   `?GetRandomLoadedAnim@CAnimationSet@@QBEPBVCAnimationEntry@@ABVCCharString@@AAK_NABVCAnimFilterBase@@@Z`.
//   Callers: CThingCreatureBase helpers 0x00663001 / 0x00663062.
// Callees: CAnimationSet::FindFirstAnimWithName(const CCharString&) const 0x005DC020 (landed),
//   vector<const pair<unsigned long, CAnimationEntry>*>::reserve 0x005E15E0 / _M_insert_overflow 0x005E1500, the
//   filter's virtual operator()(const CAnimationEntry&) const (slot 0), CMeshDataBank::PeekAnimation(unsigned long) const
//   0x00A242C0, C3DAnimationInfo::IsLoaded() const 0x00A23F50 / BeginLoading(bool) const 0x00A26730, GFROR13 0x00497890
//   (__fastcall), free 0x00BFEA14; g_pMainGameComponent 0x013B86A0.
// Layout: CAnimationSet::Anims (CVectorMap, its STLport vector begin/end at +0/+4) first; entries are
//   pair<unsigned long, CAnimationEntry> (20 bytes, CAnimationEntry::BankIndex at entry +4). CMainGameComponent::PWorld
//   +0x24, CWorld::PMainMeshBank +0x3c.
// Shape: speed TU. Two iterator vectors (accepted, loaded), each reserved to 5; the equal-key run from the first match
//   as in the landed GetNoAnims 0x005DC290 (end() tested on entry, the key test against the first match heads the
//   body); push_back inlines to the placement store or the growth call, whose `__true_type` tag shares the live
//   `returnUnloaded` slot; reserve and the growth template are same-TU STLport bodies (the accepted vector's finish is
//   then cached in edi across the opaque calls, as retail does; the growth body never reads the tag); the private IsAnimLoaded /
//   BeginLoading helpers inline to the main mesh bank's PeekAnimation; the inline GFRandom picks the indices.
extern "C" void* __cdecl malloc(unsigned int size);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);    // 0x00BFEAE6

inline void* operator new(unsigned int, void* p) { return p; }

unsigned long __fastcall GFROR13(unsigned long value);                                // 0x00497890 (rotate right by 13)

__forceinline unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

namespace std {

struct __true_type { __true_type() { } };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
          ((const char*)__last - (const char*)__first);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;                                                                     // +0x00
    _Tp* _M_finish;                                                                    // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;                            // +0x08
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp& reference;
    typedef typename _Vector_base<_Tp, _Alloc>::allocator_type allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    bool empty() const { return this->_M_start == this->_M_finish; }
    reference operator[](size_type __n) { return *(this->_M_start + __n); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    void reserve(size_type __n);                                                       // 0x005E15E0
    pointer _M_allocate_and_copy(size_type __n, pointer __first, pointer __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __copy_trivial(__first, __last, __result);
        return __result;
    }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false);
};

template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::reserve(size_type __n)
{                                                   // 0x005E15E0 (same-TU STLport body)
    if (capacity() < __n) {
        const size_type __old_size = size();
        pointer __tmp;
        if (this->_M_start) {
            __tmp = _M_allocate_and_copy(__n, this->_M_start, this->_M_finish);
            _M_clear();
        }
        else
            __tmp = this->_M_end_of_storage.allocate(__n);
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}

template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x,
                                                                  const __true_type&, size_type __fill_len,
                                                                  bool __atend)
{                                                   // 0x005E1500 (same-TU STLport body: never reads the tag)
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
    for (size_type __i = 0; __i < __fill_len; ++__i, ++__new_finish)
        _Construct(__new_finish, __x);
    if (!__atend)
        __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

}

class CCharString;

class C3DAnimationInfo {
public:
    bool IsLoaded() const;                                                             // 0x00A23F50
    void BeginLoading(bool immediately) const;                                         // 0x00A26730
};
class CMeshDataBank {
public:
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;                  // 0x00A242C0
};
class CWorld {
public:
    unsigned char  _pad_0x00[0x3c];
    CMeshDataBank* PMainMeshBank;                                                      // +0x3c
};
class CMainGameComponent {
public:
    unsigned char _pad_0x00[0x24];
    CWorld*       PWorld;                                                              // +0x24 (scoped_ptr<CWorld>)
};
extern CMainGameComponent* g_pMainGameComponent;                                      // 0x013B86A0

class CAnimationEntry {                                 // 16 bytes (Ego_r): BankIndex, AnimName, GroupName, Components
public:
    long  BankIndex;                                    // +0x00
    void* AnimName;                                     // +0x04 CDefString
    void* GroupName;                                    // +0x08 CDefString
    void* Components;                                   // +0x0c CIVCountedPointer<CAnimComponentMap>
};
class CAnimFilterBase {
public:
    virtual bool operator()(const CAnimationEntry& entry) const = 0;   // slot 0
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class Cmp> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    const value_type* _M_start;                         // +0x00 (the STLport vector)
    const value_type* _M_finish;                        // +0x04
    const value_type* _M_end_of_storage;                // +0x08
    const_iterator end() const { return _M_finish; }
};

class CAnimationSet {
public:
    typedef CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess<unsigned long, CAnimationEntry> > TAnimMap;
    TAnimMap Anims;                                     // +0x00
    TAnimMap::const_iterator FindFirstAnimWithName(const CCharString& name) const;   // 0x005DC020
    const CAnimationEntry* GetRandomLoadedAnim(const CCharString& name, unsigned long& seed, bool returnUnloaded,
                                               const CAnimFilterBase& filter) const;
private:
    bool IsAnimLoaded(const CAnimationEntry& entry) const
    {
        return g_pMainGameComponent->PWorld->PMainMeshBank->PeekAnimation(entry.BankIndex)->IsLoaded();
    }
    void BeginLoading(const CAnimationEntry& entry) const
    {
        g_pMainGameComponent->PWorld->PMainMeshBank->PeekAnimation(entry.BankIndex)->BeginLoading(false);
    }
};

const CAnimationEntry* CAnimationSet::GetRandomLoadedAnim(const CCharString& name, unsigned long& seed,
                                                          bool returnUnloaded, const CAnimFilterBase& filter) const
{
    std::vector<TAnimMap::const_iterator> anims;
    anims.reserve(5);
    std::vector<TAnimMap::const_iterator> loadedAnims;
    loadedAnims.reserve(5);
    TAnimMap::const_iterator first = FindFirstAnimWithName(name);
    for (TAnimMap::const_iterator it = first; it != Anims.end() && it->first == first->first; ++it) {
        if (filter(it->second)) {
            anims.push_back(it);
            if (IsAnimLoaded(it->second))
                loadedAnims.push_back(it);
        }
    }
    unsigned long index = GFRandom(anims.size(), &seed);
    if (IsAnimLoaded(anims[index]->second))
        return &anims[index]->second;
    BeginLoading(anims[index]->second);
    if (!loadedAnims.empty())
        return &loadedAnims[GFRandom(loadedAnims.size(), &seed)]->second;
    if (returnUnloaded)
        return &anims[index]->second;
    return 0;
}
