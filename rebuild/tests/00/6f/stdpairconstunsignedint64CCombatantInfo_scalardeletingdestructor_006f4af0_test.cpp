// Behaviour test for 006f4af0 (the scalar deleting destructor of pair<const unsigned __int64, CCombatantInfo>: the defender info and every counted combat sequence die, the vector block is freed, and `delete` frees the pair).
#include <cstdio>
#include <cstdlib>
#include <new>
#if defined(_MSC_VER)
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

static int gSequenceReleases = 0;
static int gDefenderDtors = 0;

class CCombatSequenceBase;
template<class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
    ~CCountedPointer() { ++gSequenceReleases; }   // stand-in for the out-of-line release
};

class C2DVector { public: float X, Y; };

struct __false_type { __false_type() {} };
template<class T> inline void _Destroy(T* pointer) { pointer->~T(); }
template<class ForwardIterator> void __destroy_aux(ForwardIterator first, ForwardIterator last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template<class ForwardIterator> inline void _Destroy(ForwardIterator first, ForwardIterator last)
{
    __destroy_aux(first, last, __false_type());
}

template<class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            std::free(_M_start);
    }
};

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

class CThingCreatureBase;
class CAttackingCombatantInfo {
public:
    CThingCreatureBase* Attacked;
    long AttackerNumber;
    long CombatWheelRingIndex;
    bool PreferredAttackDollySet;
    long PreferredAttackDollyTimer;
    C2DVector PreferredAttackDollyVector;
    bool UsingCombatWheel;
    long CurrentSequence;
    vector<CCountedPointer<CCombatSequenceBase> > CombatSequences;
};

class CDefendingCombatantInfo {
public:
    unsigned char _body[0x3c];
    ~CDefendingCombatantInfo() { ++gDefenderDtors; }   // stand-in for 0x006F48A0
};

struct CCombatantInfo {
public:
    CThingCreatureBase* Parent;
    CAttackingCombatantInfo AttackerInfo;
    CDefendingCombatantInfo DefenderInfo;
};

typedef pair<const u64, CCombatantInfo> SCombatantByUID;

static void InitValue(SCombatantByUID* value, u64 key, int sequences)
{
    *(u64*)&value->first = key;
    CAttackingCombatantInfo& attacker = value->second.AttackerInfo;
    attacker.CombatSequences._M_start = sequences ? (CCountedPointer<CCombatSequenceBase>*)std::malloc(sequences * sizeof(CCountedPointer<CCombatSequenceBase>)) : 0;
    attacker.CombatSequences._M_finish = attacker.CombatSequences._M_start + sequences;
    attacker.CombatSequences._M_end_of_storage = attacker.CombatSequences._M_finish;
}


int main()
{
    SCombatantByUID* a = (SCombatantByUID*)::operator new(sizeof(SCombatantByUID));
    InitValue(a, 7, 3);
    _Destroy(a);                       // flags 0: destroy only
    int ok = gDefenderDtors == 1 && gSequenceReleases == 3;
    ::operator delete(a);
    SCombatantByUID* b = (SCombatantByUID*)::operator new(sizeof(SCombatantByUID));
    InitValue(b, 8, 0);
    delete b;                          // flags 1: destroy and free
    if (gDefenderDtors != 2 || gSequenceReleases != 3)
        ok = 0;
    if (!ok) {
        std::printf("006f4af0_TEST FAIL\n");
        return 1;
    }
    std::printf("006f4af0_TEST PASS\n");
    return 0;
}