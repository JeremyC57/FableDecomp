// Behaviour test for CTCBed::StartGetInBedAnimation (0x007469C0).
#include <cstdio>
struct CTCBaseInterface;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool HasTC(long interfaceType) const
    { if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) return true; return false; }
    template<class T> T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CBlendPair { float m_first, m_second; CBlendPair() : m_first(0.0f), m_second(0.0f) {} };
struct CAnimInit {
    long Mode; unsigned char AnimType; bool Flag05; long AnimIndex; float Scales[3]; CBlendPair Blend;
    unsigned long Flags; long Arg24; long Percent;
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    { AnimType = (unsigned char)animType; Arg24 = arg24; AnimIndex = animIndex; Mode = mode; Flag05 = flag05; Flags = flags; Percent = percent; }
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    { Set(animType, arg24, animIndex, mode, flag05, flags, percent); }
};
static CAnimInit g_played(0, 0, 0, 0, true, 0, 0);
static int g_plays = 0; static long g_mode = -1, g_delay = -1;
struct CAnimateThing { void SetMainAnim(const CAnimInit& init, long mode, long delay) { g_played = init; ++g_plays; g_mode = mode; g_delay = delay; } };
struct CTCAnimationComplex { CAnimateThing Animator; CAnimateThing& GetAnimation() { return Animator; } };
struct CBedDef { bool Animating; };
class CTCBed {
public:
    CThing* ParentThing; bool Empty; bool GettingIn; bool GettingOut; bool Animating; long BedSleepAnim; const CBedDef* PDef;
    CTCAnimationComplex* GetAnimationComplex() const
    {
        CTCAnimationComplex* complex = 0;
        if (ParentThing->HasTC(0x5a)) complex = ParentThing->GetTC<CTCAnimationComplex>(0x5a);
        return complex;
    }
    void Start(long animation, long sleepAnimation)
    {
        if (PDef->Animating) {
            BedSleepAnim = sleepAnimation;
            GettingIn = true;
            CAnimInit init(6, 0, animation, 1, false, 0, 100);
            CAnimateThing& animate = GetAnimationComplex()->GetAnimation();
            animate.SetMainAnim(init, 1, 0);
            Animating = true;
        }
        Empty = false;
    }
};

int main()
{
    CTCAnimationComplex complex;
    CInterfaceMap::SEntry entries[1] = { { 0x5a, (CTCBaseInterface*)&complex } };
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_presentBits[2] = 1UL << 26; thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 1;
    CBedDef def; def.Animating = true;
    CTCBed bed; bed.ParentThing = &thing; bed.Empty = true; bed.GettingIn = !true; bed.GettingOut = false; bed.Animating = false;
    bed.BedSleepAnim = 0; bed.PDef = &def;
    bed.Start(12, 34);
    bool ok = g_plays == 1 && g_played.AnimIndex == 12 && g_played.AnimType == 6 && g_played.Mode == 1 && !g_played.Flag05
        && g_played.Percent == 100 && g_mode == 1 && g_delay == 0 && bed.Animating && !bed.Empty
        && bed.GettingIn == true && bed.GettingOut == !true && bed.BedSleepAnim == (true ? 34 : 0);
    CTCBed still = bed; def.Animating = false; still.Empty = true; still.Animating = false;
    still.Start(5, 6);
    ok = ok && g_plays == 1 && !still.Empty && !still.Animating;
    if (!ok) { std::printf("007469c0_TEST FAIL\n"); return 1; }
    std::printf("007469c0_TEST PASS\n");
    return 0;
}