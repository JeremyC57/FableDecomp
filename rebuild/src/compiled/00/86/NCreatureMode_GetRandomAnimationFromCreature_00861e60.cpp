// NCreatureMode::GetRandomAnimationFromCreature(const CCharString& name, const CThingCreatureBase& creature, const CAnimFilterBase& filter, unsigned long* seed) -- pick a random animation entry filed under the name in the creature's appearance def: one passing the filter when any does, else one of any group (CAnimFilterNoGroup); a null counted pointer when the def is missing or no entry qualifies
// Leaf: ?GetRandomAnimationFromCreature@NCreatureMode@@
// Flags: /Oa
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00861E60, 832 bytes incl. int3 padding, /Gr `__fastcall` (hidden result in
//   ecx, the name in edx, creature/filter/seed on the stack), `ret 0Ch`.
// Identity: catalog "Animation_SelectRandomFromAbilityAnimations" (_global) is Ego_r.pdb / FableWin.pdb
//   `?GetRandomAnimationFromCreature@NCreatureMode@@YI?AV?$CCountedPointer@VCAnimEntryLite@NAnimationEntryBodge@@@@
//   ABVCCharString@@ABVCThingCreatureBase@@ABVCAnimFilterBase@@PAK@Z`; six callers in the mode code right after it
//   (CModeStandBase::OnActivate 0x008621A0 first).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const CAppearanceDef>*) const 0x0042B0A2 (manifest
//   "GetSubDef<CAbilityDef>" is propagated; a same-TU stand-in below, the GetSubDef<T> body of the landed AddWound
//   0x00757B30), CCRC::Calc 0x004014A0, CVectorMap<ulong, CAnimationEntry>::UpperBound 0x00863030 (landed) and
//   ::LowerBound 0x005DF960, CAnimFilterBase slot 0 `operator()(const CAnimationEntry&)`,
//   vector<const CAnimationEntry*>::_M_insert_overflow 0x005E1420 (landed; same-TU body here), GFROR13 0x00497890,
//   operator new 0x00BFEA1A, NAnimationEntryBodge::CAnimEntryLite(const CAnimationEntry&) 0x0073A9B0,
//   CCountedPointer<CAnimEntryLite>(CAnimEntryLite*) 0x0073D180, free 0x00BFEA14, the def's slot 1 on the last release.
// Layout: CThing PBaseDef +0x70; CAppearanceDef Animation (CAnimationSet, its CVectorMap first) +0x34; map entries
//   pair<ulong CRC, CAnimationEntry> (20 bytes); CAnimFilterNoGroup vtable 0x0122FFDC over CAnimFilterBase 0x0122FFCC.
// Shape: an `/Oa` TU (the name's string data pointer is loaded once and kept in esi across both CRC calls; the two
//   vectors' finish pointers live in edi/ebp across the virtual filter calls with write-through stores); the vector
//   regrowth body and the GetSubDef stand-in in the same TU tell VC7.1 that neither captures the locals' addresses;
//   `end = UpperBound(CRC)` is taken before `LowerBound(CRC)` (two CRC computations, two key temporaries); the no-group
//   filter is a named local (it is homed in the dead creature argument slot, its virtual call kept), destroyed through the
//   CAnimFilterBase vtable store; the def release is the DecRefToZero form (`dec [ecx+4] ; jne` with no reload); the
//   pick is the inline GFRandom (0x24A1/0x24DF LCG + GFROR13, `range ? seed % range : 0`), the entry copied into a new
//   CAnimEntryLite before the counted pointer is built in the hidden result; both returns and the null fall-through keep
//   their own release + epilogue.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6
inline void* operator new(unsigned int, void* where) { return where; }

class CCharStringData {
public:
    const char*   Data;                                     // +0x00
    unsigned long Length;                                   // +0x04
};
extern const char g_EmptyCharString[];                      // 0x0129AAF4 ""
class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long crc, const void* data, unsigned long length);   // 0x004014A0
};
class CCharString {
public:
    CCharStringData* PStringData;                           // +0x00
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
    const char* GetAsCharArray() const { return PStringData == 0 ? g_EmptyCharString : PStringData->Data; }
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
    unsigned long GetCRC() const { return CCRC::Calc(0, GetAsCharArray(), GetLength()); }
};

unsigned long __fastcall GFROR13(unsigned long value);       // 0x00497890
__forceinline unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

namespace std {

struct __true_type { __true_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
                                     + ((const char*)__last - (const char*)__first);
}

template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __value;
    return __first;
}

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    ~vector() { }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    bool empty() const { return this->_M_start == this->_M_finish; }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
protected:
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false)   // 0x005E1420
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
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

} // namespace std

class CDefString { public: long TablePos; };
class CAnimationEntry {                                     // 16 bytes (Ego_r)
public:
    long       BankIndex;                                   // +0x00
    CDefString AnimName;                                    // +0x04
    CDefString GroupName;                                   // +0x08
    void*      Components;                                  // +0x0c
};

class CAnimFilterBase {                                     // vtable 0x0122FFCC
public:
    virtual bool operator()(const CAnimationEntry& entry) const = 0;   // slot 0
    virtual CAnimFilterBase* Clone() const = 0;             // slot 1
    virtual ~CAnimFilterBase() { }                          // slot 2
};
class CAnimFilterNoGroup : public CAnimFilterBase {         // vtable 0x0122FFDC
public:
    virtual bool operator()(const CAnimationEntry& entry) const;       // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                 // 0x00429A2F
};

template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class Cmp> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    const value_type* _M_start;                             // +0x00
    const value_type* _M_finish;                            // +0x04
    const value_type* _M_end_of_storage;                    // +0x08
    const_iterator LowerBound(const K& key) const;          // 0x005DF960
    const_iterator UpperBound(const K& key) const;          // 0x00863030
};
class CAnimationSet {
public:
    typedef CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess<unsigned long, CAnimationEntry> > TAnimMap;
    TAnimMap Anims;                                         // +0x00
};

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }

template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};

class CDefinitionManager {
public:
    template <class T> CDefPointer<const T> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                           // slot 1
    virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual const long* GetSubDefIndex(const CCharString& name) const;   // slot 14 (+0x38)
    mutable long RefCount;                                  // +0x04
    unsigned char _pad08[0x1c - 0x08];
    CDefinitionManager* m_pManager;                         // +0x1c
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;
};
class CAppearanceDef : public CDefClassBase {
public:
    unsigned char _pad20[0x34 - 0x20];
    CAnimationSet Animation;                                // +0x34
};
class CThingBaseDef : public CDefClassBase { };

// Same-TU stand-in for GetSubDef<CAppearanceDef> 0x0042B0A2 (the GetSubDef<T> body of the landed AddWound 0x00757B30).
#pragma auto_inline(off)
template <> bool CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const CAppearanceDef>* out) const
{
    const long* index;
    {
        CCharString name("CAppearanceDef");
        index = GetSubDefIndex(name);
    }
    if (index) {
        CDefPointer<const CAppearanceDef> result = m_pManager->GetPDefFromGlobalIndex<CAppearanceDef>(*index);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)

class CThingCreatureBase {
public:
    unsigned char _pad00[0x70];
    CDefPointer<const CThingBaseDef> PBaseDef;              // +0x70
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
};

namespace NAnimationEntryBodge {
class CAnimEntryLite {
public:
    unsigned char _data[0x10];
    CAnimEntryLite(const CAnimationEntry& entry);           // 0x0073A9B0
};
}

struct SCountBlock {
    long  m_references;                                     // +0
    void (__fastcall* m_pfnDestroy)(void*);                 // +4
    void* m_pObject;                                        // +8
};
template <class T> class CCountedPointer {
public:
    T*           m_pObject;                                 // +0
    SCountBlock* m_pCount;                                  // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer(T* object);                             // 0x0073D180
    ~CCountedPointer();
};

namespace NCreatureMode {

CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> __fastcall GetRandomAnimationFromCreature(const CCharString& name,
    const CThingCreatureBase& creature, const CAnimFilterBase& filter, unsigned long* seed)
{
    CDefPointer<const CAppearanceDef> appearance_def;
    if (creature.GetPBaseDef()->GetSubDef(&appearance_def)) {
        const CAnimationSet::TAnimMap& anims = appearance_def->Animation.Anims;
        std::vector<const CAnimationEntry*> filtered_anims;
        std::vector<const CAnimationEntry*> no_group_anims;
        CAnimFilterNoGroup no_group_filter;
        CAnimationSet::TAnimMap::const_iterator end = anims.UpperBound(name.GetCRC());
        for (CAnimationSet::TAnimMap::const_iterator it = anims.LowerBound(name.GetCRC()); it != end; ++it) {
            if (filter(it->second))
                filtered_anims.push_back(&it->second);
            if (no_group_filter(it->second))
                no_group_anims.push_back(&it->second);
        }
        if (!filtered_anims.empty())
            return CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>(
                new NAnimationEntryBodge::CAnimEntryLite(*filtered_anims[GFRandom(filtered_anims.size(), seed)]));
        if (!no_group_anims.empty())
            return CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>(
                new NAnimationEntryBodge::CAnimEntryLite(*no_group_anims[GFRandom(no_group_anims.size(), seed)]));
    }
    return CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>();
}

}