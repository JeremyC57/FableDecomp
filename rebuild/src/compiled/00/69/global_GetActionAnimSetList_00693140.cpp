// GetActionAnimSetList(vector<CAnimSet*>& sets, CThingCreatureBase& creature, long action) -- collect the creature's animation sets playing `action`: its combination anims of type 1 whose first anim is the action, or else its main anim set when that plays the action as a type-1 set
// Leaf: ?GetActionAnimSetList@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00693140, 269 bytes (+ padding), `__fastcall` `ret 4`: sets in ecx (-> ebx), creature in
//   edx (-> edi), action on the stack. Callers 0x0069326B and 0x00694BBF.
// Identity: Ego_r.pdb ?GetActionAnimSetList@?A0x...@@YIXAAV?$vector@PAVCAnimSet@@V?$allocator@PAVCAnimSet@@@std@@@std@@
//   AAVCThingCreatureBase@@J@Z -- an anonymous-namespace helper (the ?A0x hash is per TU; anonymous leaves match by prefix).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> >::LowerBound 0x0040F020 (the inline GetTC of
//   interface 0x5A CTCAnimationComplex, unchecked: the miss path reads the uninitialised component, as the landed 0x00835E30),
//   CTCAnimationComplex::GetAnimation 0x0070B460, CThingCreatureBase::IsAnimCombinationAnim(long) const 0x00662820 (Ego_r QBE),
//   CAnimateThing::GetAllCombinationAnimsFromType(long, vector<CAnimSet*>&) 0x0070D0A0, vector<CAnimSet*>::_M_insert_overflow
//   0x0066C3F0 (the inline push_back's growth path) and the out-of-line vector<CAnimSet*>::push_back 0x0066C4D0 (the main-anim
//   path), free 0x00BFEA14 (the local list's buffer).
// Layout: CThing interface bitset +0x20 (0x5A = bit 26 of +0x28), interface map +0x44; CAnimateThing CurrentAnim +0 (whose
//   main CAnimSet starts the object); CAnimSet (Ego_r.pdb with 12-byte STLport vectors) Anims +0x04, Type +0x50.
// Shape: the loop's push_back is inlined (placement-new null test, `add [ebx+4],4`), the main-anim one is the out-of-line
//   push_back. The loop push_back's x is homed in the dead `action` argument slot only when the STLport
//   vector<CAnimSet*>::_M_insert_overflow(__true_type) COMDAT body is in the TU (noinline, defined before the caller, the landed
//   0x0066C3F0 body): VC7.1 then knows the callee keeps no reference to x (chunk 599 out-param rule); with an external
//   declaration x takes the dead GetTC key slot [esp+10h] instead.
// Device: GetActionAnimSetList_Use is not a retail function; it only keeps the anonymous-namespace body emitted (GOTCHAS,
//   chunk 390 `_Use` caller).
#include "fable_compiler.h"
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* __p) throw() { return __p; }

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
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef __true_type _IsPODType;
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    ~vector() { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
protected:
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend = false);
public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
};
}

namespace std {
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&,
                                                                  unsigned int __fill_len, bool __atend)
{                                                                                 // 0x0066C3F0
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);
    _Tp* __new_start = this->_M_end_of_storage.allocate(__len);
    _Tp* __new_finish = (_Tp*)__copy_trivial(this->_M_start, __position, __new_start);
    __new_finish = fill_n(__new_finish, __fill_len, __x);
    if (!__atend)
        __new_finish = (_Tp*)__copy_trivial(__position, this->_M_finish, __new_finish);
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
#pragma auto_inline(on)
}

class CTCBase;
class CAnimSet {                                       // Ego_r.pdb, retail 12-byte vectors
public:
    long                    UpdateMode;                // +0x00
    std::vector<short>      Anims;                     // +0x04
    unsigned char           _pad10[0x50 - 0x10];
    unsigned char           Type;                      // +0x50
};
class CCompleteAnimation {
public:
    CAnimSet MainAnim;                                 // +0x00
};
class CAnimateThing {
public:
    CCompleteAnimation CurrentAnim;                    // +0x00
    void GetAllCombinationAnimsFromType(long type, std::vector<CAnimSet*>& sets);   // 0x0070D0A0
};
class CTCAnimationComplex {
public:
    CAnimateThing& GetAnimation();                     // 0x0070B460
};

enum ETCInterfaceType { };                                   // CTCBase interface type
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > {
public:
    // agentNF16: value_type is std::pair<ETCInterfaceType, CTCBase*> (not a nested SEntry), so 0x0040F020 binds the NF14 / established
    //   ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z.
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;
    value_type* _M_finish;
    value_type* _M_end_of_storage;
    value_type* LowerBound(const ETCInterfaceType& key);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                   // +0x20
    CInterfaceMap  m_interfaces;                       // +0x44
    FABLE_FORCEINLINE bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> FABLE_FORCEINLINE T* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(key);
        return (T*)((it == map->_M_finish || type < it->first) ? map->_M_finish : it)->second;
    }
    FABLE_FORCEINLINE CTCAnimationComplex* GetAnimationComplex()
    {
        CTCAnimationComplex* complex;
        if (HasTC(0x5a))
            complex = GetTC<CTCAnimationComplex>(0x5a);
        return complex;
    }
};

class CThingCreatureBase : public CThing {
public:
    bool IsAnimCombinationAnim(long action) const;     // 0x00662820
};

namespace {
void __fastcall GetActionAnimSetList(std::vector<CAnimSet*>& sets, CThingCreatureBase& creature, long action)
{
    CAnimateThing& animator = creature.GetAnimationComplex()->GetAnimation();
    if (creature.IsAnimCombinationAnim(action)) {
        std::vector<CAnimSet*> combinationAnims;
        animator.GetAllCombinationAnimsFromType(1, combinationAnims);
        for (std::vector<CAnimSet*>::iterator it = combinationAnims.begin(); it != combinationAnims.end(); ++it) {
            CAnimSet* set = *it;
            if (set->Anims._M_start[0] == action)
                sets.push_back(set);
        }
    }
    else {
        CAnimSet& main = animator.CurrentAnim.MainAnim;
        if (main.Anims._M_start[0] == action && main.Type == 1)
            sets.push_back(&main);
    }
}
}

void __fastcall GetActionAnimSetList_Use(std::vector<CAnimSet*>& sets, CThingCreatureBase& creature, long action)
{
    GetActionAnimSetList(sets, creature, action);
}