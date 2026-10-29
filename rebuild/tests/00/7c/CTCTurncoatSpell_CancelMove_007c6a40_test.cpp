// Behaviour test for CTCTurncoatSpell::CancelMove (0x007C6A40): links the reconstructed object; LowerBound is a stub.
#include <cstdio>
#include <cstring>
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const ETCInterfaceType& type)
{
    SEntry* it = first;
    while (it != last && it->first < type) ++it;
    return it;
}
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long UsingComponents[9];
    CInterfaceMap Components;
};
class CTCSpecialAbilities { public: unsigned char _pad00[0x0c]; bool AbilityMutexLock; long CurrentAbility; };
struct CWillButtonHelperTimer { long ButtonHeldDownTimer; long WillAccumulatedLevel; unsigned char PCreature[0x8]; long MyAbility; long CurrentState; };
class CTCTurncoatSpell {
public:
    void* __vftable; CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; CWillButtonHelperTimer Timer;
    void CancelMove();
};
static int fail(int n) { std::printf("007c6a40_TEST FAIL %d\n", n); return 1; }
int main()
{
    CTCSpecialAbilities ab; std::memset(&ab, 0, sizeof ab);
    CInterfaceMap::SEntry e[2] = { { (ETCInterfaceType)0x9E, 0 }, { (ETCInterfaceType)0xA7, (CTCBase*)&ab } };
    CThing t; std::memset(&t, 0, sizeof t);
    t.Components.first = e; t.Components.last = e + 2;
    CTCTurncoatSpell s; std::memset(&s, 0, sizeof s); s.ParentThing = &t;
    // no 0xA7 component: only the timer state changes
    s.Timer.CurrentState = 1; s.CancelMove();
    if (s.Timer.CurrentState != 3) return fail(1);
    t.UsingComponents[0xA7 >> 5] |= 1UL << (0xA7 & 31);
    // turncoat running: lock and ability cleared, timer state 3
    ab.AbilityMutexLock = true; ab.CurrentAbility = 5; s.Timer.CurrentState = 1; s.CancelMove();
    if (ab.AbilityMutexLock || ab.CurrentAbility != 0 || s.Timer.CurrentState != 3) return fail(2);
    // another ability running (enflame, 3): early return, nothing touched
    ab.AbilityMutexLock = true; ab.CurrentAbility = 3; s.Timer.CurrentState = 1; s.CancelMove();
    if (!ab.AbilityMutexLock || ab.CurrentAbility != 3 || s.Timer.CurrentState != 1) return fail(3);
    std::printf("007c6a40_TEST PASS\n");
    return 0;
}
