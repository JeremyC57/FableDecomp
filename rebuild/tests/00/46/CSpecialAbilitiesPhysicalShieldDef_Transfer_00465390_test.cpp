#include <cstdio>
// behaviour test: CSpecialAbilitiesPhysicalShieldDef::Transfer visits every member once, with retail's callee-type sequence


struct CDefString { long m_index; };
template <class T> struct CStlVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };

struct CPersistContext {
    const void* order[128]; int kind[128]; int count;
    void Transfer(CDefString& v) { kind[count] = 1; order[count++] = &v; }
    void Transfer(CStlVector<float>& v) { kind[count] = 2; order[count++] = &v; }
    void Transfer(bool& v) { kind[count] = 3; order[count++] = &v; }
    void Transfer(float& v) { kind[count] = 4; order[count++] = &v; }
};

struct CSpecialAbilitiesPhysicalShieldDef {
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x38];                       // CSpecialAbilitiesDef base fields (+0x04)
    CDefString InitSound;                            // +0x3C
    CDefString PowerUpSound[3];                      // +0x40
    CDefString CastSound;                            // +0x4C
    CDefString LoopingSound[4];                      // +0x50
    CDefString EndSound;                             // +0x60
    float Health[4];                                 // +0x64
    float LevelUpTimings[3];                         // +0x74
    float CamDist[4];                                // +0x80
    float CamHeight[4];                              // +0x90
    float CamFOV[4];                                 // +0xA0
    float CamSwitchTime[4];                          // +0xB0
    float CastCamDist;                               // +0xC0
    float CastCamHeight;                             // +0xC4
    float CastCamFOV;                                // +0xC8
    float CastCamSwitchTime;                         // +0xCC
    float MinWidth;                                  // +0xD0
    CStlVector<float> MaxWidth;                      // +0xD4
    float BeenHitExtraWidth;                         // +0xE0
    float BeenHitFadeOffTimeSecs;                    // +0xE4
    float WobbleRate;                                // +0xE8
    CStlVector<float> ColorStartLevel1;              // +0xEC
    CStlVector<float> ColorStartLevel2;              // +0xF8
    CStlVector<float> ColorStartLevel3;              // +0x104
    CStlVector<float> ColorStartLevel4;              // +0x110
    CStlVector<float> ColorEnd;                      // +0x11C
    CStlVector<float> ColorHit;                      // +0x128
    float FadeOutPhysicalShieldTimeSecs;             // +0x134
    CStlVector<float> ManaDamage;                    // +0x138
    bool ShieldCoversCarriedItems;                   // +0x144
};

void CSpecialAbilitiesPhysicalShieldDef::Transfer(CPersistContext& context)
{
    context.Transfer(InitSound);
    context.Transfer(PowerUpSound[0]);
    context.Transfer(PowerUpSound[1]);
    context.Transfer(PowerUpSound[2]);
    context.Transfer(CastSound);
    context.Transfer(LoopingSound[0]);
    context.Transfer(LoopingSound[1]);
    context.Transfer(LoopingSound[2]);
    context.Transfer(LoopingSound[3]);
    context.Transfer(EndSound);
    context.Transfer(CamDist[0]);       context.Transfer(CamDist[1]);
    context.Transfer(CamDist[2]);       context.Transfer(CamDist[3]);
    context.Transfer(CamHeight[0]);     context.Transfer(CamHeight[1]);
    context.Transfer(CamHeight[2]);     context.Transfer(CamHeight[3]);
    context.Transfer(CamFOV[0]);        context.Transfer(CamFOV[1]);
    context.Transfer(CamFOV[2]);        context.Transfer(CamFOV[3]);
    context.Transfer(CamSwitchTime[0]); context.Transfer(CamSwitchTime[1]);
    context.Transfer(CamSwitchTime[2]); context.Transfer(CamSwitchTime[3]);
    context.Transfer(Health[0]);        context.Transfer(Health[1]);
    context.Transfer(Health[2]);        context.Transfer(Health[3]);
    context.Transfer(LevelUpTimings[0]); context.Transfer(LevelUpTimings[1]); context.Transfer(LevelUpTimings[2]);
    context.Transfer(CastCamDist);
    context.Transfer(CastCamHeight);
    context.Transfer(CastCamFOV);
    context.Transfer(CastCamSwitchTime);
    context.Transfer(MinWidth);
    context.Transfer(MaxWidth);
    context.Transfer(ColorStartLevel1);
    context.Transfer(ColorStartLevel2);
    context.Transfer(ColorStartLevel3);
    context.Transfer(ColorStartLevel4);
    context.Transfer(ColorEnd);
    context.Transfer(ColorHit);
    context.Transfer(WobbleRate);
    context.Transfer(FadeOutPhysicalShieldTimeSecs);
    context.Transfer(BeenHitExtraWidth);
    context.Transfer(BeenHitFadeOffTimeSecs);
    context.Transfer(ManaDamage);
    context.Transfer(ShieldCoversCarriedItems);
}

int main()
{
    static CSpecialAbilitiesPhysicalShieldDef def;
    static CPersistContext ctx; ctx.count = 0;
    def.Transfer(ctx);
    static const int kinds[] = { 1,1,1,1,1,1,1,1,1,1,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,2,2,2,2,2,2,2,4,4,4,4,2,3 };
    const int n = sizeof(kinds) / sizeof(kinds[0]);
    bool ok = ctx.count == n;
    for (int i = 0; ok && i < n; ++i) {
        ok = ctx.kind[i] == kinds[i];
        const char* p = (const char*)ctx.order[i];
        if (p < (const char*)&def || p >= (const char*)&def + sizeof(def)) ok = false;
        for (int j = 0; ok && j < i; ++j) if (ctx.order[j] == ctx.order[i]) ok = false;
    }
    if (!ok) { std::printf("00465390_TEST FAIL\n"); return 1; }
    std::printf("00465390_TEST PASS\n");
    return 0;
}