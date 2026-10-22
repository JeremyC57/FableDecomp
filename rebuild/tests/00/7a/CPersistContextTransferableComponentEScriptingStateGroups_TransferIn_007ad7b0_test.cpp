#include <cstdio>
// Behaviour test for 007ad7b0: TransferableComponent<EScriptingStateGroups>::TransferIn narrows the parsed long into the target.
enum EScriptingStateGroups { GROUP_A, GROUP_B, GROUP_C };
struct CPersistContext { long bias; };
struct CStringParser { long token; };
static void TransferInLong(CPersistContext& context, CStringParser& parser, long* value) { *value = parser.token + context.bias; }
template <class T> struct Traits {
    T DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, T* value) { long v; TransferInLong(context, parser, &v); *value = (T)v; }
};
struct Component {
    EScriptingStateGroups* TransferableValue;
    void TransferIn(CPersistContext& context, CStringParser& parser) { Traits<EScriptingStateGroups> traits; traits.TransferIn(context, parser, TransferableValue); }
};
int main()
{
    EScriptingStateGroups g = GROUP_A;
    Component c; c.TransferableValue = &g;
    CPersistContext ctx; ctx.bias = 1;
    CStringParser p; p.token = 1;
    c.TransferIn(ctx, p);
    if (g != GROUP_C) { std::printf("007ad7b0_TEST FAIL\n"); return 1; }
    std::printf("007ad7b0_TEST PASS\n");
    return 0;
}