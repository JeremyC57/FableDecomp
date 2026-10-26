// Behaviour test for CLensFlareElementDef::Transfer (0x0046F727): Radius, Texture, Position, Colour, each once.
#include <cstdio>

class CRGBColour { public: unsigned char B, G, R, A; };

class CPersistContext {
public:
    const void* seen[8]; char kind[8]; int count;
    CPersistContext() : count(0) {}
    void Transfer(float& value)      { seen[count] = &value; kind[count++] = 'f'; }
    void Transfer(long& value)       { seen[count] = &value; kind[count++] = 'l'; }
    void Transfer(CRGBColour& value) { seen[count] = &value; kind[count++] = 'c'; }
};

class CLensFlareElementDef {
public:
    virtual void Transfer(CPersistContext& context);
    virtual ~CLensFlareElementDef() {}
    unsigned char _base[0x38];
    float Radius;
    CRGBColour Colour;
    float Position;
    long Texture;
};

void CLensFlareElementDef::Transfer(CPersistContext& context)
{
    context.Transfer(Radius);
    context.Transfer(Texture);
    context.Transfer(Position);
    context.Transfer(Colour);
}

int main()
{
    CLensFlareElementDef def;
    CPersistContext ctx;
    def.Transfer(ctx);
    const void* want[4] = { &def.Radius, &def.Texture, &def.Position, &def.Colour };
    const char kinds[5] = "flfc";
    if (ctx.count != 4) { std::printf("0046f727_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (ctx.seen[i] != want[i] || ctx.kind[i] != kinds[i]) { std::printf("0046f727_TEST FAIL\n"); return 1; }
    std::printf("0046f727_TEST PASS\n");
    return 0;
}