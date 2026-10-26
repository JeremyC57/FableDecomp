#include <cstdio>
#include <cstring>

struct CComponent { int tag; };
struct CInterfaceMap {
    struct SEntry { long type; CComponent* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    CInterfaceMap::SEntry m_storage[4];
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    void Init() { std::memset(m_presentBits, 0, sizeof(m_presentBits)); m_interfaces.first = m_storage; m_interfaces.last = m_storage; }
    void Add(long type, CComponent* c)   /* keys added in ascending order */
    {
        m_interfaces.last->type = type; m_interfaces.last->component = c; ++m_interfaces.last;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
    }
};

class CAnimInit { public: int id; };
class CAnimSet { public: long UpdateMode; float CurrentPercentageComplete; float LastPercentageComplete; };
class CAnimateThing { public: CAnimSet CurrentAnim; const CAnimInit* m_init; long m_mode, m_delay; int m_plays;
    void SetMainAnim(const CAnimInit& init, long mode, long delay) { m_init = &init; m_mode = mode; m_delay = delay; ++m_plays; } };
class CTCAnimationComplex : public CComponent { public: CAnimateThing* PAnimation; CAnimateThing& GetAnimation() { return *PAnimation; } };
class CTCAnimatingObject { public:
    CThing* ParentThing; bool Animating; long CurrentAnimationPriority;
    void SetCurrentAnimAsManualUpdate()
    {
        CTCAnimationComplex* animation;
        if (ParentThing->GetInterface(&animation, 0x5a))
            animation->GetAnimation().CurrentAnim.UpdateMode = 0;
    }
    void SetFractionComplete(float fraction)
    {
        CTCAnimationComplex* animation;
        if (ParentThing->GetInterface(&animation, 0x5a)) {
            CAnimSet& set = animation->GetAnimation().CurrentAnim;
            float previous = set.CurrentPercentageComplete;
            set.CurrentPercentageComplete = fraction;
            set.LastPercentageComplete = previous;
        }
    }
    void PlayAnimation(const CAnimInit& init, long priority)
    {
        CTCAnimationComplex* animation;
        if (ParentThing->GetInterface(&animation, 0x5a)) {
            CAnimateThing& animator = animation->GetAnimation();
            animator.SetMainAnim(init, 1, 0);
            Animating = true;
            CurrentAnimationPriority = priority;
        }
    }
};
int main()
{
    CThing thing; thing.Init();
    CAnimateThing animator; animator.CurrentAnim.UpdateMode = 2; animator.CurrentAnim.CurrentPercentageComplete = 0.25f;
    animator.CurrentAnim.LastPercentageComplete = 0.0f; animator.m_init = 0; animator.m_mode = animator.m_delay = -1; animator.m_plays = 0;
    CTCAnimationComplex complex; complex.tag = 5; complex.PAnimation = &animator;
    thing.Add(0x5a, &complex);
    CTCAnimatingObject o; o.ParentThing = &thing; o.Animating = false; o.CurrentAnimationPriority = 0;
    if ((thing.m_presentBits[2] & 0x4000000UL) == 0) { std::printf("007f8dc0_TEST FAIL\n"); return 1; }
    CAnimInit init; init.id = 17;
    o.PlayAnimation(init, 4);
    if (animator.m_init != &init || animator.m_mode != 1 || animator.m_delay != 0 || animator.m_plays != 1 || !o.Animating || o.CurrentAnimationPriority != 4) { std::printf("007f8dc0_TEST FAIL\n"); return 1; }
    CThing empty; empty.Init(); o.ParentThing = &empty; o.Animating = false;
    o.PlayAnimation(init, 9);
    if (animator.m_plays != 1 || o.Animating || o.CurrentAnimationPriority != 4) { std::printf("007f8dc0_TEST FAIL\n"); return 1; }
    std::printf("007f8dc0_TEST PASS\n");
    return 0;
}