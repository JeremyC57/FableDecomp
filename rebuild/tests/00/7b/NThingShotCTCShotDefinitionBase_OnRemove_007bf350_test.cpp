// Behaviour test for 0x007BF350 NThingShot::CTCShotDefinitionBase::OnRemove
#include <cstdio>
struct CAug { int stops; void StopTrailEffect() { ++stops; } };
struct CThing {
    unsigned long UsingComponents[9]; CAug* aug; unsigned char Flags91; int kills; bool lastImmediate;
    void Kill(bool immediately) { ++kills; lastImmediate = immediately; }
    bool GetInterface(CAug** out, long type)
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) { *out = aug; return true; }
        return false;
    }
};
struct CPtr { CThing* item; CThing* GetPItem() const { return item; } };
inline bool IS_THING_ALIVE(const CPtr& p) { if (p.GetPItem() != 0) { if (!(p.GetPItem()->Flags91 & 1)) return true; } return false; }
struct CShot {
    CThing* PThing; CPtr PEffect;
    void OnRemove()
    {
        CAug* a;
        if (PThing->GetInterface(&a, 0xbf))
            a->StopTrailEffect();
        if (IS_THING_ALIVE(PEffect))
            PEffect.GetPItem()->Kill(true);
    }
};
int main()
{
    CAug aug; aug.stops = 0;
    CThing owner; int i; for (i = 0; i < 9; ++i) owner.UsingComponents[i] = 0;
    owner.UsingComponents[5] = 0x80000000UL; owner.aug = &aug; owner.Flags91 = 0; owner.kills = 0;
    CThing effect; for (i = 0; i < 9; ++i) effect.UsingComponents[i] = 0; effect.Flags91 = 0; effect.kills = 0; effect.lastImmediate = false;
    CShot s; s.PThing = &owner; s.PEffect.item = &effect;
    s.OnRemove();
    int ok = aug.stops == 1 && effect.kills == 1 && effect.lastImmediate;
    effect.Flags91 = 1;
    s.OnRemove();
    ok = ok && aug.stops == 2 && effect.kills == 1;
    owner.UsingComponents[5] = 0; s.PEffect.item = 0;
    s.OnRemove();
    ok = ok && aug.stops == 2 && effect.kills == 1;
    if (!ok) { std::printf("007bf350_TEST FAIL\n"); return 1; }
    std::printf("007bf350_TEST PASS\n");
    return 0;
}