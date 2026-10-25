// GetActionAnimSetList(vector<CAnimSet*>& anim_set_list, CThingCreatureBase& creature, long action) -- collect the creature's anim sets playing the given action: for a combination anim every combination set (type 1) whose first anim is the action, otherwise the main anim set when it plays the action as a type-1 set
// Leaf: ?GetActionAnimSetList@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F4990, 272 bytes incl. int3 padding, /Gr `__fastcall` (list in ecx,
//   creature in edx, action on the stack), `ret 4`.
// Identity: FableWin/Ego_r PDB, anonymous namespace (two TUs carry a copy):
//   `?GetActionAnimSetList@?A0x...@@YIXAAV?$vector@PAVCAnimSet@@V?$allocator@PAVCAnimSet@@@std@@@std@@AAVCThingCreatureBase@@J@Z`;
//   the FableWin image places CAnimSet::GetAnimIndex() const and CAnimSet::GetType() const right after it.
// Callees: CTCAnimationComplex::GetAnimation() 0x0070B460 (returns the CAnimateThing& at +0xC; catalog
//   "DrawGetEnvironment"), CThingCreatureBase::IsAnimCombinationAnim(long) 0x00662820, CAnimateThing::
//   GetAllCombinationAnimsFromType(long, vector<CAnimSet*>&) 0x0070D0A0, vector<CAnimSet*>::_M_insert_overflow 0x0066C3F0
//   (inlined push_back's grow path), vector<CAnimSet*>::push_back 0x0066C4D0 (the second push_back stays out of line),
//   free 0x00BFEA14 (the local vector's storage).
// Layout: CThing interface bitset +0x20 (type 0x5A = CTCAnimationComplex: word 2 bit 26), interface map +0x44; retail
//   CAnimSet (Ego_r with 12-byte STLport vectors): Anims vector<short> +4, Type +0x50; CAnimateThing starts with
//   CurrentAnim.MainAnimSet.
// Shape: `if (creature.HasTC(0x5A)) complex = GetTC(0x5A);` into an otherwise unset pointer (the absent path reads the
//   dead `action` slot; the out-pointer lookup form adds `mov ecx,eax`), GetAnimation, then
//   `if (creature.IsAnimCombinationAnim(action))` a local vector filled by
//   GetAllCombinationAnimsFromType(1, ...) and walked with a named `CAnimSet*` (address taken by push_back, homed in the
//   dead `action` slot, which the __true_type temporary shares); else the main anim set by address.
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14

inline void* operator new(unsigned int, void* where) { return where; }

namespace std {

struct __true_type { __true_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef _Tp* iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { _Alloc().deallocate(_M_start, _M_end_of_storage - _M_start); }
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend);   // 0x0066C3F0
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __true_type(), 1UL, true);
    }
};

// Same-TU stand-in body (the real one, 0x0066C3F0, is a COMDAT of another TU): VC7.1 uses what it learns from it.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&,
                                                                  unsigned int __fill_len, bool __atend)
{
    unsigned int __old_size = (unsigned int)(_M_finish - _M_start);
    unsigned int __len = __old_size + (__old_size < __fill_len ? __fill_len : __old_size);
    _Tp* __new_start = (_Tp*)malloc(__len * sizeof(_Tp));
    _Tp* __new_finish = __new_start;
    for (_Tp* __p = _M_start; __p != __position; ++__p, ++__new_finish)
        *__new_finish = *__p;
    for (; __fill_len > 0; --__fill_len, ++__new_finish)
        *__new_finish = __x;
    if (!__atend)
        for (_Tp* __q = __position; __q != _M_finish; ++__q, ++__new_finish)
            *__new_finish = *__q;
    _M_start = __new_start;
    _M_finish = __new_finish;
    _M_end_of_storage = __new_start + __len;
}
#pragma auto_inline(on)

} // namespace std

class CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                          // +0x00
    SEntry* last;                                           // +0x04
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CAnimSet {                                            // retail 0x54 bytes
public:
    long                 UpdateMode;                        // +0x00
    std::vector<short>   Anims;                             // +0x04
    unsigned char        _pad10[0x50 - 0x10];
    unsigned char        Type;                              // +0x50
    long GetAnimIndex() const { return Anims._M_start[0]; }
    long GetType() const { return Type; }
};

class CAnimateThing {
public:
    CAnimSet MainAnimSet;                                   // +0x00 CurrentAnim.MainAnimSet
    void GetAllCombinationAnimsFromType(long type, std::vector<CAnimSet*>& out);   // 0x0070D0A0
};

class CTCAnimationComplex {                                 // interface type 0x5A
public:
    CAnimateThing& GetAnimation();                          // 0x0070B460
};

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 UsingComponents bitset
    CInterfaceMap m_interfaces;                             // +0x44 Components
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

class CThingCreatureBase : public CThing {
public:
    bool IsAnimCombinationAnim(long action);                // 0x00662820
};

namespace {
void __fastcall GetActionAnimSetList(std::vector<CAnimSet*>& anim_set_list, CThingCreatureBase& creature, long action);
}

void GetActionAnimSetList_Use(std::vector<CAnimSet*>& anim_set_list, CThingCreatureBase& creature, long action)
{
    GetActionAnimSetList(anim_set_list, creature, action);
}

namespace {
void __fastcall GetActionAnimSetList(std::vector<CAnimSet*>& anim_set_list, CThingCreatureBase& creature, long action)
{
    CTCAnimationComplex* complex;
    if (creature.HasTC(0x5a))
        complex = (CTCAnimationComplex*)creature.GetTC(0x5a);
    CAnimateThing& animator = complex->GetAnimation();
    if (creature.IsAnimCombinationAnim(action)) {
        std::vector<CAnimSet*> combination_anims;
        animator.GetAllCombinationAnimsFromType(1, combination_anims);
        for (std::vector<CAnimSet*>::iterator it = combination_anims.begin(); it != combination_anims.end(); ++it) {
            CAnimSet* anim_set = *it;
            if (anim_set->GetAnimIndex() == action)
                anim_set_list.push_back(anim_set);
        }
    }
    else {
        CAnimSet& main_set = animator.MainAnimSet;
        if (main_set.GetAnimIndex() == action && main_set.GetType() == 1)
            anim_set_list.push_back(&main_set);
    }
}
}