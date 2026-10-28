// CTCCombat::GetAppropriateCombatAnimationFromList(const vector<const CAnimationEntry*>& entries) const -- pick a random entry index, preferring flourish or fully matching (combo + handedness) animations, then handedness-only, then combo-only matches; 0 when none fits
// Leaf: ?GetAppropriateCombatAnimationFromList@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007345D0, 685 bytes, __fastcall, `ret 4`, returns long.
// PDB: ?GetAppropriateCombatAnimationFromList@CTCCombat@@QBEJABV?$vector@PBVCAnimationEntry@@V?$allocator@PBVCAnimationEntry@@@std@@@std@@@Z
//   (name set); catalog "GetAppropriateCombatAnimation_FilterAndSelect" was descriptive.
// Callees: CAnimationEntry::GetComponent<CAnimComponentCombatComboChain> 0x00738980 (component 4),
//   GetComponent<CAnimComponentCombatHandedness> 0x00669CB0 (component 3), GetComponent<CAnimComponentCombatMisc>
//   0x00738A00 (component 6), vector<long>::_M_insert_overflow 0x00440990 (the push_back growth path),
//   CTCBase::GetWorld 0x00686A80, CWorld::GetPRandomSeed 0x0049D810, GFROR13 0x00497890, free 0x00BFEA14.
// Shape: three STLport vector<long> index lists (finish pointers in ebp/edi/ebx, re-stored after every push_back);
//   the creature's previous action handedness is read once before the loop; per entry two bools (handedness match,
//   combo match); the random pick is `range ? r % range : 0` on the world seed (GFRandom); the second and third
//   lists share the pick tail. Operand order matters: `previous == start` gives retail's `cmp [esp+20h],eax`, and the
//   combo stage is read once ahead of the ComboChainPos branch (retail loads [combo+8] before the `jne`).
// Layout: CTCCombat (engine/CTCCombat.h): ComboChainPos +0x0C (-777 = none), ComboChainID +0x10, ParentCreature
//   +0x24; retail CThingCreatureBase PreviousActionHandedness +0x138; components: ComboStage +8 / ComboID +0x0C,
//   StartHandedness +8, MeleeFlourish +8 (bool).
#include "engine/CTCCombat.h"

extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (import thunk)
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6 (import thunk)
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

enum { COMBO_STAGE_NONE = -777, COMBO_STAGE_FINISHER = -888 };
enum ECombatAnimationHandedness { COMBAT_ANIMATION_HANDEDNESS_EITHER = 2 };   // value 2 accepted for any previous action

class CAnimComponentCombatComboChain { public:
    void* m_pVtable;                                    // +0x00
    unsigned long IVRefCount;                           // +0x04
    long ComboStage;                                    // +0x08
    long ComboID;                                       // +0x0C
};
class CAnimComponentCombatHandedness { public:
    void* m_pVtable;                                    // +0x00
    unsigned long IVRefCount;                           // +0x04
    ECombatAnimationHandedness StartHandedness;         // +0x08
    ECombatAnimationHandedness EndHandedness;           // +0x0C
};
class CAnimComponentCombatMisc { public:
    void* m_pVtable;                                    // +0x00
    unsigned long IVRefCount;                           // +0x04
    bool MeleeFlourish;                                 // +0x08
    bool MeleeKnockdown;                                // +0x09
};

class CAnimationEntry {
public:
    long BankIndex;                                     // +0x00
    long AnimName;                                      // +0x04 CDefString
    long GroupName;                                     // +0x08 CDefString
    void* Components;                                   // +0x0C CIVCountedPointer<CAnimComponentMap>
    template <class T> void GetComponent(const T** out) const;   // 0x00738980 / 0x00669CB0 / 0x00738A00
};

class CThingCreatureBase { public:
    unsigned char _pad00[0x138];
    ECombatAnimationHandedness PreviousActionHandedness;   // +0x138 (retail)
};

long __fastcall GFROR13(unsigned long);   // 0x00497890
class CWorld {
public:
    unsigned long* GetPRandomSeed();                    // 0x0049D810
};
inline unsigned long GFRandom(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed;
}

namespace std {
struct __true_type { __true_type() {} };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    unsigned int __n = (const char*)__last - (const char*)__first;
    return __n ? (void*)((char*)memmove(__result, __first, __n) + __n) : __result;
}
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}
template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) {}
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) {}
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) {}
    ~vector() {}
    _Tp* begin() const { return this->_M_start; }
    _Tp* end() const { return this->_M_finish; }
    unsigned int size() const { return (unsigned int)(end() - begin()); }
    bool empty() const { return begin() == end(); }
    _Tp& operator[](unsigned int __n) const { return *(begin() + __n); }
    protected:
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    protected:
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
    protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend);   // 0x00440990 for long
    public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1, true);
    }
};

#pragma auto_inline(off)
// Same-TU body of vector<long>::_M_insert_overflow (the template is compiled in this TU; the linker kept 0x00440990):
// with it visible VC7.1 sees the vectors' addresses do not escape, so the finish pointers live in ebp/edi/ebx.
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend)
{
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

struct CTCCombat_Methods : CTCCombat {
    CWorld* GetWorld() const;                           // 0x00686A80 (CTCBase)
    __forceinline unsigned long GetRandomIndex(unsigned long range) const
    {
        unsigned long r = GFRandom(GetWorld()->GetPRandomSeed());
        if (range == 0)
            return 0;
        return r % range;
    }
    long GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*>& entries) const;
};

long CTCCombat_Methods::GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*>& entries) const
{
    ECombatAnimationHandedness previousHandedness = ParentCreature->PreviousActionHandedness;
    std::vector<long> preferred;
    std::vector<long> handednessMatches;
    std::vector<long> comboMatches;
    for (long i = 0; i != (long)entries.size(); ++i) {
        const CAnimationEntry* entry = entries[i];
        const CAnimComponentCombatComboChain* combo;
        entry->GetComponent(&combo);
        const CAnimComponentCombatHandedness* handedness;
        entry->GetComponent(&handedness);
        const CAnimComponentCombatMisc* misc;
        entry->GetComponent(&misc);
        bool handednessOK = false;
        bool comboOK = false;
        if (previousHandedness == handedness->StartHandedness || handedness->StartHandedness == COMBAT_ANIMATION_HANDEDNESS_EITHER)
            handednessOK = true;
        long stage = combo->ComboStage;
        if (ComboChainPos == COMBO_STAGE_NONE) {
            if (stage == 0 || stage == ComboChainPos || stage == COMBO_STAGE_FINISHER)
                comboOK = true;
        }
        else if (ComboChainID == combo->ComboID) {
            if (stage == ComboChainPos || stage == COMBO_STAGE_FINISHER)
                comboOK = true;
        }
        if (misc->MeleeFlourish || (comboOK && handednessOK))
            preferred.push_back(i);
        if (handednessOK)
            handednessMatches.push_back(i);
        if (comboOK)
            comboMatches.push_back(i);
    }
    long result;
    if (!preferred.empty())
        result = preferred[GetRandomIndex(preferred.size())];
    else if (!handednessMatches.empty())
        result = handednessMatches[GetRandomIndex(handednessMatches.size())];
    else if (!comboMatches.empty())
        result = comboMatches[GetRandomIndex(comboMatches.size())];
    else
        result = 0;
    return result;
}