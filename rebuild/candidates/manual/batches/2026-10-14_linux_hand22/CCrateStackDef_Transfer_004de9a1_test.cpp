#include <cstdio>
#include <cstring>
struct CPersistContext {
    long* enumTarget; const char* name; long* valueTarget; long defaultSeen;
    void TransferEnum_00431102(long* value) { enumTarget = value; }
    void Transfer(const char* n, long& value, const long& defaultValue) { name = n; valueTarget = &value; defaultSeen = defaultValue; }
};
struct CCrateStackDef {
    void* __vftable;
    unsigned char _pad[0x24];
    long StackType;
    long Priority;
};
struct CCrateStackDef_Methods : CCrateStackDef {
    void Transfer(CPersistContext& context);
};
void CCrateStackDef_Methods::Transfer(CPersistContext& context)
{
    context.TransferEnum_00431102(&StackType);
    long defaultPriority = 0;
    context.Transfer("Priority", Priority, defaultPriority);
}
int main()
{
    CCrateStackDef_Methods def; CPersistContext ctx = { 0, 0, 0, -1 };
    def.Transfer(ctx);
    if (ctx.enumTarget != &def.StackType || ctx.valueTarget != &def.Priority || std::strcmp(ctx.name, "Priority") != 0 || ctx.defaultSeen != 0) { std::printf("004de9a1_TEST FAIL\n"); return 1; }
    std::printf("004de9a1_TEST PASS\n");
    return 0;
}
