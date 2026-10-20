#include <cstdio>
#include <cstring>
struct CPersistContext;
template <class T>
struct CRetailVector {                          // STLport vector: _M_start / _M_finish / _M_end_of_storage
    T* first;
    T* last;
    T* end;
};

struct CPersistContext {
    const void* log[32]; int n;
    void Transfer(float& value) { log[n++] = &value; }
    void Transfer(CRetailVector<float>& value) { log[n++] = &value; }
};

struct CParentDefClassBase {
    virtual void Transfer(CPersistContext&) {}
    unsigned char _base[0x38];                  // +0x04..+0x3b: the def root's own members
};

struct CSpecialAbilitiesEnflameDef : CParentDefClassBase {
    float CamDist[4];                           // +0x3c
    float CamHeight[4];                         // +0x4c
    float CamFOV[4];                            // +0x5c
    float CamSwitchTime[4];                     // +0x6c
    float LevelUpTimings[3];                    // +0x7c
    float Damage[4];                            // +0x88
    CRetailVector<float> RadiusOfEffect;        // +0x98
    float EnflameCombatMultiplierFactor;        // +0xa4
    virtual void Transfer(CPersistContext& context);
};

void CSpecialAbilitiesEnflameDef::Transfer(CPersistContext& context)
{
    context.Transfer(LevelUpTimings[0]);
    context.Transfer(LevelUpTimings[1]);
    context.Transfer(LevelUpTimings[2]);
    context.Transfer(Damage[0]);
    context.Transfer(Damage[1]);
    context.Transfer(Damage[2]);
    context.Transfer(Damage[3]);
    context.Transfer(RadiusOfEffect);
    context.Transfer(CamDist[0]);
    context.Transfer(CamDist[1]);
    context.Transfer(CamDist[2]);
    context.Transfer(CamDist[3]);
    context.Transfer(CamHeight[0]);
    context.Transfer(CamHeight[1]);
    context.Transfer(CamHeight[2]);
    context.Transfer(CamHeight[3]);
    context.Transfer(CamFOV[0]);
    context.Transfer(CamFOV[1]);
    context.Transfer(CamFOV[2]);
    context.Transfer(CamFOV[3]);
    context.Transfer(CamSwitchTime[0]);
    context.Transfer(CamSwitchTime[1]);
    context.Transfer(CamSwitchTime[2]);
    context.Transfer(CamSwitchTime[3]);
    context.Transfer(EnflameCombatMultiplierFactor);
}

int main()
{
    static CSpecialAbilitiesEnflameDef d;
    static CPersistContext c; c.n = 0;
    CParentDefClassBase* b = &d;
    b->Transfer(c);                                 // virtual dispatch reaches the derived hook
    const void* want[25];
    int k = 0;
    for (int i = 0; i < 3; ++i) want[k++] = &d.LevelUpTimings[i];
    for (int i = 0; i < 4; ++i) want[k++] = &d.Damage[i];
    want[k++] = &d.RadiusOfEffect;
    for (int i = 0; i < 4; ++i) want[k++] = &d.CamDist[i];
    for (int i = 0; i < 4; ++i) want[k++] = &d.CamHeight[i];
    for (int i = 0; i < 4; ++i) want[k++] = &d.CamFOV[i];
    for (int i = 0; i < 4; ++i) want[k++] = &d.CamSwitchTime[i];
    want[k++] = &d.EnflameCombatMultiplierFactor;
    int ok = c.n == 25;
    for (int i = 0; ok && i < 25; ++i) if (c.log[i] != want[i]) ok = 0;
    if (!ok) { std::printf("00465258_TEST FAIL\n"); return 1; }
    std::printf("00465258_TEST PASS\n");
    return 0;
}