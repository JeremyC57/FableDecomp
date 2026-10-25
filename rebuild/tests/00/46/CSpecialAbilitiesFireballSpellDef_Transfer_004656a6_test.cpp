// Behaviour test for CSpecialAbilitiesFireballSpellDef::Transfer (0x004656A6): Inclination, ReleaseDelay,
// StaminaCost, TargettingArcAngle, each once.
#include <cstdio>

template <class T> struct CStlVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };

class CPersistContext {
public:
    const void* seen[8]; char kind[8]; int count;
    CPersistContext() : count(0) {}
    void Transfer(float& value)             { seen[count] = &value; kind[count++] = 'f'; }
    void Transfer(CStlVector<float>& value) { seen[count] = &value; kind[count++] = 'v'; }
};

class CSpecialAbilitiesFireballSpellDef {
public:
    virtual void Transfer(CPersistContext& context);
    virtual ~CSpecialAbilitiesFireballSpellDef() {}
    unsigned char _base[0x38];
    float ReleaseDelay;
    float Inclination;
    CStlVector<float> StaminaCost;
    float TargettingArcAngle;
};

void CSpecialAbilitiesFireballSpellDef::Transfer(CPersistContext& context)
{
    context.Transfer(Inclination);
    context.Transfer(ReleaseDelay);
    context.Transfer(StaminaCost);
    context.Transfer(TargettingArcAngle);
}

int main()
{
    CSpecialAbilitiesFireballSpellDef def;
    CPersistContext ctx;
    def.Transfer(ctx);
    const void* want[4] = { &def.Inclination, &def.ReleaseDelay, &def.StaminaCost, &def.TargettingArcAngle };
    const char kinds[5] = "ffvf";
    if (ctx.count != 4) { std::printf("004656a6_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (ctx.seen[i] != want[i] || ctx.kind[i] != kinds[i]) { std::printf("004656a6_TEST FAIL\n"); return 1; }
    std::printf("004656a6_TEST PASS\n");
    return 0;
}