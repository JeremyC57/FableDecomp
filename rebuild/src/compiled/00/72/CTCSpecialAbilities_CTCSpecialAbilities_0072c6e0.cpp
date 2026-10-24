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
extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

class CThing;
class CThingCreatureBase;

inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result :
        ((char*)memmove(result, first, ((const char*)last - (const char*)first))) + ((const char*)last - (const char*)first);
}

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

template <class T> struct allocator {
    allocator() {}
    allocator(const allocator&) {}
};
template <class Value, class Tp, class MaybeReboundAlloc>
struct _STLP_alloc_proxy : public MaybeReboundAlloc {
    Value _M_data;
    _STLP_alloc_proxy(const MaybeReboundAlloc& a, Value p) : MaybeReboundAlloc(a), _M_data(p) {}
};
struct CBoolVector {                            // STLport vector<bool> as retail builds it: one byte per element
    typedef allocator<bool> allocator_type;
    bool* _M_start;
    bool* _M_finish;
    _STLP_alloc_proxy<bool*, bool, allocator_type> _M_end_of_storage;
    explicit CBoolVector(const allocator_type& a = allocator_type()) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (bool*)0) {}
    ~CBoolVector();
    bool* begin() { return _M_start; }
    bool* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    bool* erase(bool* first, bool* last)
    {
        bool* i = (bool*)__copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void _M_fill_insert(bool* pos, unsigned int n, const bool& x);   // 0x004676EF
    void insert(bool* pos, unsigned int n, const bool& x) { _M_fill_insert(pos, n, x); }
    void resize(unsigned int newSize, const bool& x)
    {
        if (newSize < size())
            erase(begin() + newSize, end());
        else
            insert(end(), newSize - size(), x);
    }
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
    CBoolVector         AbilityAvailability;    // +0x1c
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