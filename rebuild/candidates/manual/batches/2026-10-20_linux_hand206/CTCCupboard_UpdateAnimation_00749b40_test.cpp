// Behaviour test for CTCCupboard::UpdateAnimation (0x00749B40): only while Animating, the animation advances by {1/fps, 0, 0}
// and Animating clears once the play fraction is above 1 - 1e-4.
#include <cstdio>
long g_framesPerSecond = 25;
float g_epsilon_0129ba3c = 1e-4f;
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    CTCBase* GetTCIfPresent(long interfaceType)
    {
        CTCBase* tc = 0;                                // unset in retail when absent; the test keeps the component present
        if (HasTC(interfaceType))
            tc = GetTC(interfaceType);
        return tc;
    }
};
struct CAnimFrameTime {
    float Seconds;
    long  Reserved1;
    long  Reserved2;
    CAnimFrameTime() : Seconds(0), Reserved1(0), Reserved2(0) { }
};
struct CAnimateThing {
    float PlayFraction; int Updates; float LastSeconds; long LastR1, LastR2;
    void UpdateFrameLogic(const CAnimFrameTime& time) { ++Updates; LastSeconds = time.Seconds; LastR1 = time.Reserved1; LastR2 = time.Reserved2; PlayFraction += 0.5f; }
};
struct CTCAnimationComplex {
    CAnimateThing Anim;
    CAnimateThing& GetAnimation() { return Anim; }
};
struct CTCCupboard {
    virtual void v00() { }
    CThing* ParentThing;
    bool Animating;
    void UpdateAnimation();
};

void CTCCupboard::UpdateAnimation()
{
    CAnimateThing& anim = ((CTCAnimationComplex*)ParentThing->GetTCIfPresent(0x5a))->GetAnimation();
    CAnimFrameTime time;
    if (Animating) {
        time.Seconds = 1.0f / g_framesPerSecond;
        anim.UpdateFrameLogic(time);
        if (1.0f - g_epsilon_0129ba3c < anim.PlayFraction)
            Animating = false;
    }
}

int main()
{
    int bad = 0;
    CTCAnimationComplex ac; ac.Anim.PlayFraction = 0.0f; ac.Anim.Updates = 0; ac.Anim.LastR1 = ac.Anim.LastR2 = 7;
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    CInterfaceMap::SEntry e[1] = { { 0x5a, (CTCBase*)&ac } };
    thing.m_interfaces.first = e; thing.m_interfaces.last = e + 1; thing.m_presentBits[2] = 1UL << 26;
    CTCCupboard c; c.ParentThing = &thing; c.Animating = true;
    float frame = 1.0f / 25;
    c.UpdateAnimation();                             // 0.5: keep animating
    if (ac.Anim.Updates != 1 || ac.Anim.LastSeconds != frame || ac.Anim.LastR1 || ac.Anim.LastR2 || !c.Animating) bad |= 1;
    c.UpdateAnimation();                             // 1.0 > 1 - eps: done
    if (ac.Anim.Updates != 2 || c.Animating) bad |= 2;
    c.UpdateAnimation();                             // not animating: no update
    if (ac.Anim.Updates != 2) bad |= 4;
    if (bad) { std::printf("00749b40_TEST FAIL\n"); return 1; }
    std::printf("00749b40_TEST PASS\n");
    return 0;
}
