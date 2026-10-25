// GetActionAnimSetList(vector<CAnimSet*>& sets, CThingCreatureBase& creature, long action) -- collect the creature's animation sets playing `action`: its combination anims of type 1 whose first anim is the action, or else its main anim set when that plays the action as a type-1 set
// Grade: DIFFER(269v269) -- frame slot only: the loop's anim-set local (the inline push_back's x) is homed in the dead GetTC key slot [esp+10h] where retail homes it in the dead `action` argument slot [esp+24h] (the main-anim temp already lands there); tried function-scope / if-scope / for-body declarations, a cast rvalue, a local copy of action, a function-scope key and the GetInterface(T**) form -- all keep the key slot
// Leaf: ?GetActionAnimSetList@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00693140, 272 bytes (264 without padding), `__fastcall` `ret 4`: sets in ecx (-> ebx),
//   creature in edx (-> edi), action on the stack. Ego_r.pdb: `?GetActionAnimSetList@?A0x...@@YIXAAV?$vector@PAVCAnimSet@@
//   ...@std@@AAVCThingCreatureBase@@J@Z` (an anonymous-namespace helper). Callers 0x0069326B and 0x00694BBF.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (the inline GetInterface 0x5A
//   CTCAnimationComplex, unchecked: the miss path reads the uninitialised component from the dead `action` slot, as the
//   landed 0x00835E30 / 0x00753930), CTCAnimationComplex::GetAnimation 0x0070B460, CThingCreatureBase::
//   IsAnimCombinationAnim(long) 0x00662820, CAnimateThing::GetAllCombinationAnimsFromType(long, vector<CAnimSet*>&)
//   0x0070D0A0, vector<CAnimSet*>::_M_insert_overflow 0x0066C3F0 (the inline push_back's growth path), the out-of-line
//   vector<CAnimSet*>::push_back 0x0066C4D0 (the main-anim path), free 0x00BFEA14 (the local list's buffer).
// Label: catalog "GetActionAnimSetList" is the PDB name.
// Layout: CThing interface bitset +0x20 (0x5A = bit 26 of +0x28), interface map +0x44; CAnimateThing CurrentAnim +0
//   (whose main CAnimSet starts the object); CAnimSet (Ego_r.pdb with 12-byte vectors) Anims +0x04, Type +0x50.
// Shape: the anim set pointer handed to push_back is homed in the dead `action` slot (ebp keeps action); the loop's push_back
//   is inlined (placement-new null test, `add [ebx+4],4`), the main-anim one is the out-of-line push_back.
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* __p) throw() { return __p; }

namespace std {
struct __true_type { __true_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
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
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend = false);
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

class CInterfaceMap {
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
};

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                   // +0x20
    CInterfaceMap  m_interfaces;                       // +0x44
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    __forceinline CTCAnimationComplex* GetAnimationComplex()
    {
        CTCAnimationComplex* complex;
        if (HasTC(0x5a))
            complex = GetTC<CTCAnimationComplex>(0x5a);
        return complex;
    }
};

class CThingCreatureBase : public CThing {
public:
    bool IsAnimCombinationAnim(long action);           // 0x00662820
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

void* g_GetActionAnimSetList = (void*)&GetActionAnimSetList;
