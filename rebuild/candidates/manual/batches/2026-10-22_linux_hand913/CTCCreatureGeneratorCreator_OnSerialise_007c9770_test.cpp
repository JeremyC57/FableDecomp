#include <cstdio>

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
struct CThing { bool dead; u64 uid; };
struct CPersistContext {
    bool saving; long frameStore; u64 uidStore;
    void Transfer(const char*, long& v) { if (saving) frameStore = v; else v = frameStore; }
    void Transfer(const char*, u64& v) { if (saving) uidStore = v; else v = uidStore; }
};
static u64 Remap(bool saving, u64 uid) { return saving ? uid + 1000 : uid - 1000; }
struct CTCCreatureGeneratorCreator {
    long FrameToRegenerate; u64 GeneratorUID; CThing* PGenerator;
    void OnSerialise(CPersistContext& context, bool fullState)
    {
        if (fullState) {
            context.Transfer("FrameToRegenerate", FrameToRegenerate);
            if (context.saving) {
                u64 uid = 0;
                if (PGenerator && !PGenerator->dead)
                    uid = Remap(true, PGenerator->uid);
                context.Transfer("GeneratorUID", uid);
            } else {
                context.Transfer("GeneratorUID", GeneratorUID);
                if (GeneratorUID != 0)
                    GeneratorUID = Remap(false, GeneratorUID);
            }
        }
    }
};
static bool Run()
{
    CThing g; g.dead = false; g.uid = 5;
    CTCCreatureGeneratorCreator c; c.FrameToRegenerate = 33; c.GeneratorUID = 0; c.PGenerator = &g;
    CPersistContext ctx; ctx.saving = true; ctx.frameStore = 0; ctx.uidStore = 9;
    c.OnSerialise(ctx, false);
    bool ok = ctx.frameStore == 0 && ctx.uidStore == 9;
    c.OnSerialise(ctx, true);
    ok = ok && ctx.frameStore == 33 && ctx.uidStore == 1005;
    g.dead = true; c.OnSerialise(ctx, true); ok = ok && ctx.uidStore == 0;
    CTCCreatureGeneratorCreator d; d.FrameToRegenerate = 0; d.GeneratorUID = 7; d.PGenerator = 0;
    ctx.saving = false; ctx.uidStore = 1005; d.OnSerialise(ctx, true);
    ok = ok && d.FrameToRegenerate == 33 && d.GeneratorUID == 5;
    ctx.uidStore = 0; d.OnSerialise(ctx, true);
    return ok && d.GeneratorUID == 0;
}

int main()
{
    if (!Run()) { std::printf("007c9770_TEST FAIL\n"); return 1; }
    std::printf("007c9770_TEST PASS\n");
    return 0;
}
