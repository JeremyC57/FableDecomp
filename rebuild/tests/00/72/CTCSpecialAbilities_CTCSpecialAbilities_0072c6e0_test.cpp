// Behaviour test for 0x0072C6E0 CTCSpecialAbilities::CTCSpecialAbilities(CThing&).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class CThing { public: int dummy; };
class CThingCreatureBase;

inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result :
        ((char*)memmove(result, first, ((const char*)last - (const char*)first))) + ((const char*)last - (const char*)first);
}
struct CUndeadFollower { long Health; void* PThingItem; long PThingHandle; bool Active; unsigned int AppearanceSeed; };
struct CUndeadFollowerList {
    struct _Node { _Node* _M_next; _Node* _M_prev; CUndeadFollower _M_data; };
    _Node* _M_node;
    CUndeadFollowerList() : _M_node(0)
    {
        _Node* n = (_Node*)malloc(sizeof(_Node));
        n->_M_next = n;
        n->_M_prev = n;
        _M_node = n;
    }
    ~CUndeadFollowerList() { free(_M_node); }
};
template <class T> struct allocator { allocator() {} allocator(const allocator&) {} };
template <class Value, class Tp, class MaybeReboundAlloc>
struct _STLP_alloc_proxy : public MaybeReboundAlloc {
    Value _M_data;
    _STLP_alloc_proxy(const MaybeReboundAlloc& a, Value p) : MaybeReboundAlloc(a), _M_data(p) {}
};
struct CBoolVector {
    typedef allocator<bool> allocator_type;
    bool* _M_start;
    bool* _M_finish;
    _STLP_alloc_proxy<bool*, bool, allocator_type> _M_end_of_storage;
    explicit CBoolVector(const allocator_type& a = allocator_type()) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (bool*)0) {}
    ~CBoolVector() { free(_M_start); }
    bool* begin() { return _M_start; }
    bool* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    bool* erase(bool* first, bool* last)
    {
        bool* i = (bool*)__copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void _M_fill_insert(bool* pos, unsigned int n, const bool& x)   // model: append only (pos == end here)
    {
        unsigned int old = size();
        bool* p = (bool*)malloc(old + n);
        if (old) memcpy(p, _M_start, old);
        for (unsigned int k = 0; k < n; ++k) p[old + k] = x;
        free(_M_start);
        _M_start = p; _M_finish = p + old + n; _M_end_of_storage._M_data = _M_finish;
        (void)pos;
    }
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
    CTCBase(CThing& thing) : ParentThing(&thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) {}
    virtual ~CTCBase() {}
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
};
enum EHeroAbility { HERO_ABILITY_NULL = 0, NUMBER_OF_HERO_ABILITIES = 20 };
class CTCSpecialAbilities : public CTCBase {
public:
    CTCSpecialAbilities(CThing& thing);
    virtual ~CTCSpecialAbilities() {}
    bool                AbilityMutexLock;
    bool                UsingBed;
    EHeroAbility        CurrentAbility;
    CUndeadFollowerList UndeadFollowers;
    CThingCreatureBase* Creature;
    CBoolVector         AbilityAvailability;
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
int main()
{
    int fail = 0;
    CThing thing;
    CTCSpecialAbilities* sa = new CTCSpecialAbilities(thing);
    if (sa->ParentThing != &thing || (void*)sa->Creature != (void*)&thing) fail |= 1;
    if (sa->AbilityMutexLock || sa->UsingBed || sa->CurrentAbility != HERO_ABILITY_NULL) fail |= 2;
    if (!sa->UndeadFollowers._M_node || sa->UndeadFollowers._M_node->_M_next != sa->UndeadFollowers._M_node
        || sa->UndeadFollowers._M_node->_M_prev != sa->UndeadFollowers._M_node) fail |= 4;
    if (sa->AbilityAvailability.size() != 20) fail |= 8;
    for (unsigned int i = 0; i < sa->AbilityAvailability.size(); ++i)
        if (!sa->AbilityAvailability._M_start[i]) fail |= 16;
    // the shrinking branch of the same inline resize
    sa->AbilityAvailability.resize(25, false);
    sa->AbilityAvailability.resize(20, true);
    if (sa->AbilityAvailability.size() != 20 || !sa->AbilityAvailability._M_start[19]) fail |= 32;
    delete sa;
    if (fail) { printf("0072c6e0_TEST FAIL %d\n", fail); return 1; }
    printf("0072c6e0_TEST PASS\n");
    return 0;
}