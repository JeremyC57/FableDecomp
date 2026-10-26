// Behaviour test for CCarryableDef::Transfer (0x004DD924): every member is handed to the typed transfer once, in
// the TRANSFER list order (IncludeInThingScansWhenCarried straight after the carry slots).
#include <cstdio>

class C3DVector { public: float X, Y, Z; };

class CPersistContext {
public:
    const void* seen[16]; char kind[16]; int count;
    CPersistContext() : count(0) {}
    void Transfer(long& value)      { seen[count] = &value; kind[count++] = 'l'; }
    void Transfer(bool& value)      { seen[count] = &value; kind[count++] = 'b'; }
    void Transfer(C3DVector& value) { seen[count] = &value; kind[count++] = 'v'; }
    void Transfer(float& value)     { seen[count] = &value; kind[count++] = 'f'; }
};

class CCarryableDef {
public:
    virtual void Transfer(CPersistContext& context);
    virtual ~CCarryableDef() {}
    unsigned char _base[0x24];
    long ActiveCarrySlot;
    long SecondaryActiveCarrySlot;
    long PassiveCarrySlot;
    long OnKillFX;
    C3DVector OffsetCoordRelativeToAttachToDummy;
    float OffsetAmountForThingsOnTopOfMe;
    float PassiveCarrySlotScale;
    bool IncludeInThingScansWhenCarried;
};

void CCarryableDef::Transfer(CPersistContext& context)
{
    context.Transfer(ActiveCarrySlot);
    context.Transfer(SecondaryActiveCarrySlot);
    context.Transfer(PassiveCarrySlot);
    context.Transfer(IncludeInThingScansWhenCarried);
    context.Transfer(OnKillFX);
    context.Transfer(OffsetCoordRelativeToAttachToDummy);
    context.Transfer(OffsetAmountForThingsOnTopOfMe);
    context.Transfer(PassiveCarrySlotScale);
}

int main()
{
    CCarryableDef def;
    CPersistContext ctx;
    def.Transfer(ctx);
    const void* want[8] = { &def.ActiveCarrySlot, &def.SecondaryActiveCarrySlot, &def.PassiveCarrySlot,
                            &def.IncludeInThingScansWhenCarried, &def.OnKillFX, &def.OffsetCoordRelativeToAttachToDummy,
                            &def.OffsetAmountForThingsOnTopOfMe, &def.PassiveCarrySlotScale };
    const char kinds[9] = "lllblvff";
    if (ctx.count != 8) { std::printf("004dd924_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i)
        if (ctx.seen[i] != want[i] || ctx.kind[i] != kinds[i]) { std::printf("004dd924_TEST FAIL\n"); return 1; }
    std::printf("004dd924_TEST PASS\n");
    return 0;
}