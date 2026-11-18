// CTCSpecialAbilities::CTCSpecialAbilities(CThing& thing) -- construct the hero special-abilities component: no mutex/bed flags, no current ability, an empty undead-follower list, the owning creature, and an ability-availability vector resized to the 20 hero abilities (all available)
// Leaf: ??0CTCSpecialAbilities
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072C6E0, 159 bytes (176 with padding), `ret 4`.
// Label correction: catalog "CEngineSubPrimitive2DClockSprite_ctor" (the base constructor 0x00686800 carries that
//   propagated label; it is CTCBase::CTCBase(CThing&): ParentThing +4, two flag bytes, vtable 0x0125BE0C).  The only
//   caller is CTCSpecialAbilities::Alloc (`new` 0x28 bytes, 0x004D2DAA); Ego_r.pdb names
//   ??0CTCSpecialAbilities@@QAE@AAVCThing@@@Z.
// Callees: CTCBase::CTCBase(CThing&) 0x00686800, malloc 0x00BFEA0E (list sentinel, 0x1C-byte node = 8 + the 20-byte
//   CUndeadFollower), memmove 0x00BFEAE6 (inline __copy_trivial of the shrinking resize),
//   vector<bool>::_M_fill_insert(iterator, size_type, const bool&) 0x004676EF (thiscall).
// Layout (retail, 0x28 bytes): CTCBase 0x0C; AbilityMutexLock +0x0c, UsingBed +0x0d, CurrentAbility +0x10,
//   UndeadFollowers +0x14 (STLport list: one sentinel pointer), Creature +0x18, AbilityAvailability +0x1c (STLport
//   vector of one-byte bools: start/finish/end_of_storage).  vtable ??_7CTCSpecialAbilities 0x0126683C.
// Shape: STLport member constructors in declaration order (`_List_base`: proxy zeroed, node allocated, self-linked,
//   then stored), then the body `AbilityAvailability.resize(20, true)` fully inlined: `if (n < size()) erase(begin() +
//   n, end()); else insert(end(), n - size(), x);` -- erase's `__copy_trivial(last, finish, first)` keeps its
//   `last == first` test (`cmp eax,eax`), and the `true` temporary lives in the dead argument slot.  The vector
//   constructor must be STLport's `explicit vector(const allocator_type& a = allocator_type())` with the
//   `_STLP_alloc_proxy(a, 0)` end-of-storage member: the address-taken allocator temporary makes VC7.1 reload
//   start/finish from memory in resize (`mov eax,[edi+4] ; mov ecx,[edi]`); with plain zero member initialisers the
//   zeros are forwarded into registers (`mov eax,ebx ; mov ecx,ebx`, 162 v 161 bytes).
// NF75 (round 7, land1117fu q10917): the local CBoolVector / allocator / _STLP_alloc_proxy / __copy_trivial stand-ins are replaced by
//   the shared STLport std::vector<bool> of stlport/fable_containers.h (Ego_r.pdb CTCSpecialAbilities::AbilityAvailability is
//   std::vector<bool>); the fill-insert call binds ?_M_fill_insert@?$vector@_NV?$allocator@_N@std@@@std@@QAEXPA_NIAB_N@Z (0x004676EF).
extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E

#include "stlport/fable_containers.h"

class CThing;
class CThingCreatureBase;

struct CUndeadFollower {                        // CTCSpecialAbilities::CUndeadFollower, 20 bytes
    long          Health;
    void*         PThingItem;
    long          PThingHandle;
    bool          Active;
    unsigned long AppearanceSeed;
};

struct CUndeadFollowerList {                    // STLport list<CUndeadFollower>: 4 bytes
    struct _Node {
        _Node*          _M_next;
        _Node*          _M_prev;
        CUndeadFollower _M_data;
    };
    _Node* _M_node;
    CUndeadFollowerList() : _M_node(0)
    {
        _Node* n = (_Node*)malloc(sizeof(_Node));
        n->_M_next = n;
        n->_M_prev = n;
        _M_node = n;
    }
    ~CUndeadFollowerList();
};

class CTCBase {
public:
    CTCBase(CThing& thing);                     // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
};

enum EHeroAbility { HERO_ABILITY_NULL = 0, NUMBER_OF_HERO_ABILITIES = 20 };

class CTCSpecialAbilities : public CTCBase {
public:
    CTCSpecialAbilities(CThing& thing);
    virtual ~CTCSpecialAbilities();
    bool                AbilityMutexLock;       // +0x0c
    bool                UsingBed;               // +0x0d
    EHeroAbility        CurrentAbility;         // +0x10
    CUndeadFollowerList UndeadFollowers;        // +0x14
    CThingCreatureBase* Creature;               // +0x18
    std::vector<bool>   AbilityAvailability;    // +0x1c (STLport 4.6, stlport/fable_containers.h)
};

CTCSpecialAbilities::CTCSpecialAbilities(CThing& thing)
    : CTCBase(thing),
      AbilityMutexLock(false),
      UsingBed(false),
      CurrentAbility(HERO_ABILITY_NULL),
      Creature((CThingCreatureBase*)&thing)
{
    AbilityAvailability.resize(NUMBER_OF_HERO_ABILITIES, true);
}