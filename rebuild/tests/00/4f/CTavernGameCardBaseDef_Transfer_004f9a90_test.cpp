// behaviour test for 0x004F9A90 CTavernGameCardBaseDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x2C, +0x30, +0x34, +0x38, +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50, +0x54, +0x58, +0x5C, +0x60, +0x64, +0x6C, +0x78),
// so a binary save / load round trip restores the def
#include <cstdio>
#include <cstring>
struct Blob4 { unsigned char b[4]; };
struct Blob8 { unsigned char b[8]; };
struct Blob12 { unsigned char b[12]; };
struct Context {
    unsigned char buf[1024]; unsigned int pos; int load; const void* order[64]; unsigned int count;
    Context(int l) : pos(0), load(l), count(0) { }
    template <class T> void Transfer(T& v)
    {
        order[count++] = &v;
        if (load) std::memcpy(&v, buf + pos, sizeof(T)); else std::memcpy(buf + pos, &v, sizeof(T));
        pos += (unsigned int)sizeof(T);
    }
};
struct Def {
    int ControlType;
    float CardWidth;
    float CardHeight;
    float ImpulseScaleXbox;
    float ImpulseScalePC;
    long PointerModel;
    float MoveSpeed;
    float TurnSpeed;
    float PointerHeightOffset;
    long PointerParticleEffect;
    float HighlightWidth;
    Blob4 HighlightColourGood;
    Blob4 HighlightColourBad;
    float CardSeparation;
    float TableSeparation;
    Blob8 PackOffset;
    Blob12 CardOrders;
    Blob12 CardPack;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.ControlType);
    c.Transfer(d.CardWidth);
    c.Transfer(d.CardHeight);
    c.Transfer(d.ImpulseScaleXbox);
    c.Transfer(d.ImpulseScalePC);
    c.Transfer(d.PointerModel);
    c.Transfer(d.MoveSpeed);
    c.Transfer(d.TurnSpeed);
    c.Transfer(d.PointerHeightOffset);
    c.Transfer(d.PointerParticleEffect);
    c.Transfer(d.HighlightWidth);
    c.Transfer(d.HighlightColourGood);
    c.Transfer(d.HighlightColourBad);
    c.Transfer(d.CardSeparation);
    c.Transfer(d.TableSeparation);
    c.Transfer(d.PackOffset);
    c.Transfer(d.CardOrders);
    c.Transfer(d.CardPack);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[18] = { &d.ControlType, &d.CardWidth, &d.CardHeight, &d.ImpulseScaleXbox, &d.ImpulseScalePC, &d.PointerModel, &d.MoveSpeed, &d.TurnSpeed, &d.PointerHeightOffset, &d.PointerParticleEffect, &d.HighlightWidth, &d.HighlightColourGood, &d.HighlightColourBad, &d.CardSeparation, &d.TableSeparation, &d.PackOffset, &d.CardOrders, &d.CardPack };
    if (save.count != 18u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 18u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.ControlType, 0, sizeof(e.ControlType));
    std::memset(&e.CardWidth, 0, sizeof(e.CardWidth));
    std::memset(&e.CardHeight, 0, sizeof(e.CardHeight));
    std::memset(&e.ImpulseScaleXbox, 0, sizeof(e.ImpulseScaleXbox));
    std::memset(&e.ImpulseScalePC, 0, sizeof(e.ImpulseScalePC));
    std::memset(&e.PointerModel, 0, sizeof(e.PointerModel));
    std::memset(&e.MoveSpeed, 0, sizeof(e.MoveSpeed));
    std::memset(&e.TurnSpeed, 0, sizeof(e.TurnSpeed));
    std::memset(&e.PointerHeightOffset, 0, sizeof(e.PointerHeightOffset));
    std::memset(&e.PointerParticleEffect, 0, sizeof(e.PointerParticleEffect));
    std::memset(&e.HighlightWidth, 0, sizeof(e.HighlightWidth));
    std::memset(&e.HighlightColourGood, 0, sizeof(e.HighlightColourGood));
    std::memset(&e.HighlightColourBad, 0, sizeof(e.HighlightColourBad));
    std::memset(&e.CardSeparation, 0, sizeof(e.CardSeparation));
    std::memset(&e.TableSeparation, 0, sizeof(e.TableSeparation));
    std::memset(&e.PackOffset, 0, sizeof(e.PackOffset));
    std::memset(&e.CardOrders, 0, sizeof(e.CardOrders));
    std::memset(&e.CardPack, 0, sizeof(e.CardPack));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004f9a90_TEST FAIL %d\n", fail); return 1; }
    std::printf("004f9a90_TEST PASS\n");
    return 0;
}