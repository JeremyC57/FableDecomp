// CThingCreatureBase::GetPossibleCombatAnimations(const CCharString& name, std::vector<const CAnimationEntry*>& animations) -- collect the appearance-def animations named `name` into mode-filter / weapon-filter / no-group buckets, copy the first non-empty bucket into `animations`, else fall back to the "DEFAULT" anim and return false
// Grade: DIFFER(673v673)/3 real lines (was 686v673/81) -- the retail GetSubDef<CAppearanceDef> body (0x0042B0A2) as a same-TU noinline specialization on engine/CDefClassBase.h + the CDefPointer verify text (ENullDef ctor): frame 0x3c, the def slot shared with modeFilter and the def release all retail. Left: the DEFAULT fallback pointer is homed in the dead 'name' arg slot and stored before the string dtor; retail homes it in the 'animations' arg slot (the string temp's slot) and stores it after the dtor. Tried: fallback initialised from the literal-temp call (same), a cast temporary for push_back (same). Needs the header ENullDef ctor (PENDING_HEADER) once matched (retry wave 2 sec2c)
// Was: DIFFER(686v673) -- frame: retail packs the CDefPointer<const CAppearanceDef> slot with the spilled modeFilter (0x3c frame); ours gives the def its own slot (0x40), shifting every frame offset by 4; the def release is `dec [ecx+4]; jne` in retail, ours re-reads Object and compares against the zero register; the fallback is homed in the dead `animations` arg slot in retail, the `name` slot here
// Leaf: ?GetPossibleCombatAnimations@
// 0x006668A0, 688 bytes, __thiscall, ret 8. Callees: GetSubDef<CAppearanceDef> 0x0042B0A2, CInterfaceMap::LowerBound 0x004365B0,
// CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 (twice), CModeBase::GetAnimFilter (vtable +0x10), CAnimationSet::FindFirstAnimWithName
// 0x005DC020, filter operator() (vtable +0), vector<const CAnimationEntry*>::push_back 0x005E1EB0 (no-group bucket, out of line),
// _M_insert_overflow 0x005E1420, operator= 0x00669BB0, CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0,
// GetRandomLoadedAnimFromName 0x00662A00, free 0x00BFEA14.
// Shape: three STLport vectors; the mode/weapon bucket push_backs are inline with _M_finish cached in edi/ebp, which needs
// the same-TU noinline operator= body (0x669BB0 is in this TU) so VC7.1 treats the vectors as uncaptured.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6
inline void* __cdecl operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CDefString { public: long Index; };
class CAnimationEntry {
public:
    long        BankIndex;                             // +0x00
    CDefString  AnimName;                              // +0x04
    CDefString  GroupName;                             // +0x08
    void*       Components;                            // +0x0C CIVCountedPointer<CAnimComponentMap>
};
class CAnimFilterBase {                                // vtable 0x0122FFCC
public:
    virtual bool operator()(const CAnimationEntry& entry) const = 0;   // slot 0
    virtual CAnimFilterBase* Clone() const = 0;        // slot 1
    virtual ~CAnimFilterBase() { }                     // slot 2
};
class CAnimFilterNoGroup : public CAnimFilterBase {   // vtable 0x0122FFDC
public:
    CAnimFilterNoGroup() { }
    virtual bool operator()(const CAnimationEntry& entry) const;   // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;            // 0x00429A2F
};
class CAnimFilterOfGroup : public CAnimFilterBase {
public:
    virtual bool operator()(const CAnimationEntry& entry) const;
    virtual CAnimFilterBase* Clone() const;
    CCharString   Group;                               // +0x04
    unsigned long GroupCRC;                            // +0x08
};

namespace std {
struct __true_type { __true_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
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
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __true_type&)
{
    return __copy_ptrs(__first, __last, __result, __true_type());
}
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_iterator;
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    typedef __true_type _IsPODType;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    typedef const _Tp* const_pointer;
    bool empty() const { return this->_M_start == this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __true_type());
        return __result;
    }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
    vector& operator=(const vector& __x);              // 0x00669BB0
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len, bool __atend = false);
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
};
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&,
                                                                  size_type __fill_len, bool __atend)
{                                    // stand-in for 0x005E1420: reads x, never touches the tag
    if (__atend && __fill_len == 1)
        this->_M_finish = __position + (*(const char*)&__x != 0);
}
template <class _Tp, class _Alloc>
__declspec(noinline) vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        const size_type __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
            _M_clear();
            this->_M_start = __tmp;
            this->_M_end_of_storage._M_data = this->_M_start + __xlen;
        }
        else if (size() >= __xlen) {
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, (pointer)this->_M_start, __true_type());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start, __true_type());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish, __true_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}
#pragma auto_inline(on)
}

class CAnimationSet {                                  // CVectorMap<unsigned long, CAnimationEntry>
public:
    typedef std::pair<unsigned long, CAnimationEntry> value_type;
    typedef const value_type* const_iterator;
    std::vector<value_type> Anims;                     // +0x00
    const_iterator end() const { return Anims.end(); }
    const_iterator FindFirstAnimWithName(const CCharString& name) const;   // 0x005DC020
};

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#define FABLE_DEFPTR_INLINE __forceinline
enum ENullDef { NULL_DEF };                                  // enumerator name unproven (in neither PDB name list)
template <class T> class CDefPointer {
public:
    CDefPointer(ENullDef) : Object(0) { }                               // PROPOSED header addition (FableWin ??0..QAE@W4ENullDef@@@Z)
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      // unproven body (FableWin ?Reset@..QAEXPBV..@Z)
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             // +0x00
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 // the inline body of operator= / SetDynamicCast
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};

class CAppearanceDef : public CDefClassBase {           // RTTI CAppearanceDef : CDefClassBase
public:
    unsigned char _pad25[0x34 - 0x25];
    CAnimationSet Animation;                           // +0x34
};
class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
// The retail GetSubDef<CAppearanceDef> body, same-TU noinline (only the caller is graded).
template <>
__declspec(noinline) bool CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const CAppearanceDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CCharString("CAppearanceDef", -1));
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;        // 0x004365B0
};
class CModeBase {                                      // CTCCreatureModeManager::CModeBase
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual const CAnimFilterBase& GetAnimFilter() const;   // slot 4 (+0x10)
    void*              ParentThing;                    // +0x04
    CAnimFilterOfGroup WeaponBasedFilter;              // +0x08
};
class CTCCreatureModeManager {
public:
    CModeBase& GetCurrentMode() const;                 // 0x0073A6E0
};

class CThingCreatureBase {
public:
    void* __vftable;                                   // +0x00
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<const CDefClassBase> PBaseDef;         // +0x70 CDefPointer<const CThingBaseDef>
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline const T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (const T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    __forceinline const CAnimationSet* GetAnimationSet() const
    {
        CDefPointer<const CAppearanceDef> def(NULL_DEF);
        PBaseDef->GetSubDef(&def);
        return &def->Animation;
    }
    const CAnimationEntry* GetRandomLoadedAnimFromName(const CCharString& name, bool loadedOnly) const;   // 0x00662A00
    bool GetPossibleCombatAnimations(const CCharString& name, std::vector<const CAnimationEntry*>& animations) const;
};

bool CThingCreatureBase::GetPossibleCombatAnimations(const CCharString& name, std::vector<const CAnimationEntry*>& animations) const
{
    bool found = true;
    const CAnimationSet* animSet = GetAnimationSet();
    CAnimFilterNoGroup noGroupFilter;
    const CAnimFilterBase* modeFilter = 0;
    const CAnimFilterBase* weaponFilter = 0;
    if (HasTC(0x31)) {
        const CTCCreatureModeManager* modeManager = GetTC<CTCCreatureModeManager>(0x31);
        modeFilter = &modeManager->GetCurrentMode().GetAnimFilter();
        weaponFilter = &modeManager->GetCurrentMode().WeaponBasedFilter;
    }
    std::vector<const CAnimationEntry*> modeAnims;
    std::vector<const CAnimationEntry*> weaponAnims;
    std::vector<const CAnimationEntry*> noGroupAnims;
    CAnimationSet::const_iterator first = animSet->FindFirstAnimWithName(name);
    for (CAnimationSet::const_iterator it = first; it != animSet->end() && it->first == first->first; ++it) {
        const CAnimationEntry& entry = it->second;
        if (modeFilter && !(*modeFilter)(entry)) {
            if (weaponFilter && !(*weaponFilter)(entry)) {
                const CAnimFilterBase* defaultFilter = &noGroupFilter;
                if ((*defaultFilter)(entry))
                    noGroupAnims.push_back(&entry);
            } else
                weaponAnims.push_back(&entry);
        } else
            modeAnims.push_back(&entry);
    }
    if (!modeAnims.empty())
        animations = modeAnims;
    else if (!weaponAnims.empty())
        animations = weaponAnims;
    else if (!noGroupAnims.empty())
        animations = noGroupAnims;
    if (animations.empty()) {
        found = false;
        const CAnimationEntry* fallback;
        {
            CCharString defaultName("DEFAULT");
            fallback = GetRandomLoadedAnimFromName(defaultName, false);
        }
        animations.push_back(fallback);
    }
    return found;
}
