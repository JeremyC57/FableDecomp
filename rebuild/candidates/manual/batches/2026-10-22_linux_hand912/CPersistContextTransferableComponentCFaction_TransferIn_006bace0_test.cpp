#include <cstdio>
// Behaviour test for 006bace0: TransferableComponent<CFaction>::TransferIn builds and destroys a default faction around the reader call.
static int g_live = 0;
static int g_built = 0;
struct CFaction {
    int id;
    CFaction() : id(0) { ++g_live; ++g_built; }
    ~CFaction() { --g_live; }
};
struct CPersistContext { int empty; };
struct CStringParser { int id; };
struct Traits {
    CFaction DefaultVal;
    void TransferIn(CPersistContext&, CStringParser& parser, CFaction* value) { value->id = parser.id + DefaultVal.id; }
};
struct Component {
    CFaction* TransferableValue;
    void TransferIn(CPersistContext& context, CStringParser& parser) { Traits traits; traits.TransferIn(context, parser, TransferableValue); }
};
int main()
{
    int ok = 1;
    {
        CFaction f;
        Component c; c.TransferableValue = &f;
        CPersistContext ctx; ctx.empty = 0;
        CStringParser p; p.id = 7;
        c.TransferIn(ctx, p);
        if (f.id != 7 || g_live != 1 || g_built != 2) ok = 0;
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("006bace0_TEST FAIL\n"); return 1; }
    std::printf("006bace0_TEST PASS\n");
    return 0;
}
