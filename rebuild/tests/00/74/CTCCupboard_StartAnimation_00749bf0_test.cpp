// Behaviour test for 00749bf0: body copied from src/00749bf0.cpp (CTCCupboard::StartAnimation), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

struct CTCBaseInterface;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
class CTCBase { public: virtual void v00() {} CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; };
class CAnimFrameInfo { public: float Current; float Last; CAnimFrameInfo() : Current(0), Last(0) {} };
static int g_sets = 0;
class CAnimInit { public:
    long  UpdateMode; unsigned char Type; bool Looping; long Anim; float Speed; float Alpha; float LastAlpha; CAnimFrameInfo Frame; unsigned long Flags; unsigned long TypeFlags; long Priority;
    CAnimInit() {}
    void Set(unsigned char type, unsigned long typeFlags, long anim, long updateMode, bool looping, unsigned long flags, long priority)
    { ++g_sets; Type = type; TypeFlags = typeFlags; Anim = anim; UpdateMode = updateMode; Looping = looping; Flags = flags; Priority = priority; }
};
static int g_setMain = 0; static const CAnimInit* g_mainInit = 0; static long g_mainMode = -1; static long g_mainDelay = -1; static void* g_mainThis = 0;
class CAnimateThing { public: void SetMainAnim(const CAnimInit& init, long mode, long delay) { ++g_setMain; g_mainInit = &init; g_mainMode = mode; g_mainDelay = delay; g_mainThis = this; } };
class CTCAnimationComplex { public: CAnimateThing m_anim; CAnimateThing& GetAnimation() { return m_anim; } };
class CTCCupboard : public CTCBase { public:
    bool Animating;
    void StartAnimation(long anim);
};
void CTCCupboard::StartAnimation(long anim)
{
    Animating = true;
    CAnimInit init;
    init.Set(6, 0, anim, 1, false, 0, 100);
    CTCAnimationComplex* animation;
    if (ParentThing->HasTC(0x5a))
        animation = ParentThing->GetTC<CTCAnimationComplex>(0x5a);
    CAnimateThing& animator = animation->GetAnimation();
    animator.SetMainAnim(init, 1, 0);
}
int main()
{
    CThing thing; memset(&thing, 0, sizeof(thing)); CTCAnimationComplex complex;
    CInterfaceMap::SEntry entries[2] = { { 0x30, (CTCBaseInterface*)0x99 }, { 0x5a, (CTCBaseInterface*)&complex } };
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    thing.m_presentBits[0x5a >> 5] = 1UL << (0x5a & 31);     // a cupboard always carries the animation component (the miss arm uses an unset pointer)
    CTCCupboard cupboard; cupboard.ParentThing = &thing; cupboard.Animating = false;
    cupboard.StartAnimation(42);
    CHECK(cupboard.Animating == true);
    CHECK(g_sets == 1 && g_mainInit && g_mainInit->Type == 6 && g_mainInit->TypeFlags == 0 && g_mainInit->Anim == 42 && g_mainInit->UpdateMode == 1 && g_mainInit->Looping == false && g_mainInit->Flags == 0 && g_mainInit->Priority == 100);
    CHECK(g_setMain == 1 && g_mainMode == 1 && g_mainDelay == 0 && g_mainThis == &complex.m_anim);
    if (fails == 0) printf("00749bf0_TEST PASS\n"); else printf("00749bf0_TEST FAIL\n");
    return fails;
}