#include <cstdio>
// Behaviour test for 007ad7d0: TransferableComponent<EScriptingStateGroups>::TransferOut widens the target into a long for the writer.
enum EScriptingStateGroups { GROUP_A, GROUP_B, GROUP_C };
struct CPersistContext { int calls; };
struct CCharString { long written; };
static void TransferOutLong(CPersistContext& context, CCharString& out, long* value) { out.written = *value; ++context.calls; }
template <class T> struct Traits {
    T DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, T* value) { long v = *value; TransferOutLong(context, out, &v); }
};
struct Component {
    EScriptingStateGroups* TransferableValue;
    void TransferOut(CPersistContext& context, CCharString& out) { Traits<EScriptingStateGroups> traits; traits.TransferOut(context, out, TransferableValue); }
};
int main()
{
    EScriptingStateGroups g = GROUP_B;
    Component c; c.TransferableValue = &g;
    CPersistContext ctx; ctx.calls = 0;
    CCharString out; out.written = -5;
    c.TransferOut(ctx, out);
    if (out.written != 1 || ctx.calls != 1 || g != GROUP_B) { std::printf("007ad7d0_TEST FAIL\n"); return 1; }
    std::printf("007ad7d0_TEST PASS\n");
    return 0;
}
