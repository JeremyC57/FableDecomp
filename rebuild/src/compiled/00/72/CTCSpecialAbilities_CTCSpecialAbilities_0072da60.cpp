// CTCSpecialAbilities::~CTCSpecialAbilities() -- the compiler-generated destructor: free the ability-availability vector's block, clear and free the undead-follower list, then run ~CTCBase
// Leaf: ??1CTCSpecialAbilities@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072DA60, 54 bytes, thiscall, tail `jmp ~CTCBase`.
// Identity (proven): sole caller is the slot-0 `??_G` 0x0072DA40 of vtable 0x0126683C (RTTI .?AVCTCSpecialAbilities@@,
//   rtti-vtables.tsv); Ego_r.pdb (0x007448F0) and FableWin.pdb (0x0254AD70) spell ??1CTCSpecialAbilities@@UAE@XZ. No
//   0x0126683C store: the destructor is the implicit one. The landed clear() 0x0072D4D0 names this row as its caller.
// Callees: free 0x00BFEA14, std::_List_base<CTCSpecialAbilities::CUndeadFollower>::clear 0x0072D4D0 (landed leaf),
//   CTCBase::~CTCBase 0x00686830.
// Layout (retail, 0x28; landed ctor 0x0072C6E0): CTCBase 0xC, AbilityMutexLock +0x0c, UsingBed +0x0d, CurrentAbility +0x10,
//   UndeadFollowers +0x14 (STLport list: the sentinel pointer), Creature +0x18, AbilityAvailability +0x1c (STLport vector of
//   one-byte bools: start / finish / end_of_storage).
// Shape: members destroyed in reverse order: ~vector (trivial _Destroy) + ~_Vector_base's null-tested block free, then
//   ~_List_base = out-of-line clear() + the sentinel's null-tested free. DestroyOne (a destruction by name, ungraded) makes
//   VC7.1 emit this out-of-line implicit `??1` (chunks 656 / 657 / 696 / 697).
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"

class CThing;
class CThingCreatureBase;

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    ~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }
    void clear();                                        // 0x0072D4D0 (out of line)
protected:
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, allocator_type> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

}   // namespace std

class CTCBase : public CBaseClassNonCopyable {
public:
    virtual ~CTCBase();                                  // 0x00686830
    CThing* ParentThing;                                 // +0x04
    bool ToBeRemoved;                                    // +0x08
    bool IsInGlobalUpdateTCs;                            // +0x09
};

enum EHeroAbility { HERO_ABILITY_NULL = 0 };

class CTCSpecialAbilities : public CTCBase {             // vtable 0x0126683C, 0x28 bytes
public:
    class CUndeadFollower {                              // 0x14 bytes (Ego_r.pdb)
    public:
        long Health;
        void* PThing[2];                                 // CIntelligentPointer<CThing> (destroyed by clear())
        bool Active;
        unsigned long AppearanceSeed;
    };
    bool AbilityMutexLock;                               // +0x0c
    bool UsingBed;                                       // +0x0d
    EHeroAbility CurrentAbility;                         // +0x10
    std::list<CUndeadFollower> UndeadFollowers;          // +0x14
    CThingCreatureBase* Creature;                        // +0x18
    std::vector<bool> AbilityAvailability;               // +0x1c
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(CTCSpecialAbilities* abilities)
{
    abilities->CTCSpecialAbilities::~CTCSpecialAbilities();
}