// CTCObjectAugmentations::GetAugmentations(std::vector<long>& augmentations) const -- the global def index of the augmentation in each slot (0 for an empty slot), in slot order, into a cleared and reserved vector
// Leaf: ?GetAugmentations@CTCObjectAugmentations@@QBEXAAV?$vector@JV?$allocator@J@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00766920, 320 bytes, thiscall `ret 4`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?GetAugmentations@CTCObjectAugmentations@@QBEXAAV?$vector@JV?$allocator@J@std@@@std@@@Z
//   (Ego_r donor 0x007756A0); `this` holds the PDB member Augmentations (std::vector<CDefPointer<const CThingBaseDef> >, Ego_r and
//   retail +0x14); both retail callers (0x005BEE81, 0x00868D78) fetch the component through the interface map first, 0x00868D78
//   with interface 0xBF (CTCObjectAugmentations). The catalog label "CParticleMorphs_InitFromDef" is propagated.
// Callees: memmove 0x00BFEAE6 (STLport __copy_trivial inside clear()), std::vector<long>::reserve(size_type) 0x00660684,
//   CDefClassBase::GetInstantiationName() const (virtual slot 10, +0x28, CDefString by value), CDefStringTable::GetString(long)
//   const 0x009D49B0 on g_DefStringTable 0x013CA828 (CDefString's inline operator CCharString), CCharString(const CCharString&)
//   0x0099EC30, ~CCharString 0x0099EAE0, CTCBase::GetDefinitionManager() const 0x006869D0 (protected; ParentThing->
//   PeekDefinitionManager()), CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) const 0x009AD410,
//   std::vector<long>::_M_insert_overflow(long*, const long&, const __true_type&, size_type, bool) 0x00440990 (malloc 0x00BFEA0E,
//   free 0x00BFEA14 inside it).
// Layout: CTCObjectAugmentations (Ego_r): CTCBase 0x0C, PDef +0x0C, LoadedFromSaveGame +0x10, Augmentations +0x14.
// Shape: STLport 4.6 clear() (erase(begin(), end()) through __copy_trivial: the unfolded `cmp eax,eax`), reserve(size()), and
//   inline push_back (placement construct with its null test, else _M_insert_overflow(finish, x, __true_type(), 1, true));
//   the instantiation name is copy-initialised from the conversion temporary; __true_type has a user default constructor (no
//   tag store); _M_insert_overflow is a same-TU noinline STLport body so the temporaries share frame words.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int count);   // 0x00BFEAE6
extern "C" void* __cdecl malloc(unsigned int size);                                 // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                          // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

namespace std {
struct __true_type { __true_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return (_OutputIter)__copy_trivial(__first, __last, __result);
}
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef value_type* pointer;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    typedef const value_type& const_reference;
    typedef unsigned int size_type;
    typedef __true_type _TrivialAss;
    typedef __true_type _IsPODType;
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(end() - begin()); }
    const_reference operator[](size_type __n) const { return *(begin() + __n); }
    void reserve(size_type __n);                                                                 // 0x00660684
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
protected:
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
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false);                                              // 0x00440990
};
// The STLport 4.6 POD _M_insert_overflow body, same TU and not inlined (it is header code, visible to the retail TU too): it lets
// the push_back argument and tag temporaries pool their frame words as retail does.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&,
                                                                  size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
    __new_finish = fill_n(__new_finish, __fill_len, __x);
    if (!__atend)
        __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
#pragma auto_inline(on)
}   // namespace std

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
class CDefStringTable {
public:
    CCharString GetString(long table_pos) const;        // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
class CDefString {
public:
    long TablePos;
    operator CCharString() const { return g_DefStringTable.GetString(TablePos); }
};

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();                   // slot 0
    virtual void Delete() const;                        // slot 1
    unsigned long IVRefCount;                           // +0x04
};
class CResource : public CIVCountedPointeeBase {        // vtable slots from CDefClassBase 0x01230BCC
public:
    virtual bool UnloadResource();                      // slot 2
    virtual unsigned long GetUnloadableMemory() const;  // slot 3
    virtual bool IsResourceLoaded() const;              // slot 4
    virtual void OnResourceLoaded();                    // slot 5
    virtual bool CanAbortResourceLoad(long) const;      // slot 6
    virtual void OnResourceLoadAborted();               // slot 7
    virtual unsigned long GetDebugMemoryInUse() const;  // slot 8
    virtual void ResetDebugMemoryInUse();               // slot 9
    unsigned char _body[0x1c - 0x08];                   // ResourceList .. LastUsedFrame
};
class CDefPointeeBase : public CResource { };
class CDefClassBase : public CDefPointeeBase {
public:
    virtual CDefString GetInstantiationName() const;    // slot 10 (+0x28)
};
class CThingBaseDef : public CDefClassBase { };
enum ENullDef { NULL_DEF = 0 };
template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;                            // Ego_r.pdb
    bool IsNull() const { return Object == 0; }
    const T* operator->() const { return static_cast<const T*>(Object); }
};

class CDefinitionManager {
public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { };
class CThing;
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};
class CObjectAugmentationsDef;
class CTCObjectAugmentations : public CTCBase {
public:
    CDefPointer<const CObjectAugmentationsDef>                  PDef;                 // +0x0C
    bool                                                        LoadedFromSaveGame;   // +0x10
    std::vector<CDefPointer<const CThingBaseDef> >              Augmentations;        // +0x14
    void GetAugmentations(std::vector<long>& augmentations) const;
};

void CTCObjectAugmentations::GetAugmentations(std::vector<long>& augmentations) const
{
    augmentations.clear();
    augmentations.reserve(Augmentations.size());
    for (unsigned long i = 0; i < Augmentations.size(); ++i) {
        if (!Augmentations[i].IsNull()) {
            CCharString name = Augmentations[i]->GetInstantiationName();
            augmentations.push_back(GetDefinitionManager().GetDefGlobalIndexFromName(name));
        } else
            augmentations.push_back(0);
    }
}