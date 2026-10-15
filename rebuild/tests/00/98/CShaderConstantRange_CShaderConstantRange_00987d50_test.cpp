#include <cstdio>
static int g_base = 0;
struct CBaseClass { CBaseClass() { ++g_base; } virtual void v00() { } virtual ~CBaseClass() { } };
struct CShaderConstantRange : CBaseClass {
    long Offset; unsigned long Count;
    CShaderConstantRange() : Offset(0), Count(0) { }
    virtual void v00() { }
};
int main()
{
    CShaderConstantRange* r = new CShaderConstantRange;
    if (g_base != 1 || r->Offset != 0 || r->Count != 0) { std::printf("00987d50_TEST FAIL\n"); return 1; }
    delete r;
    std::printf("00987d50_TEST PASS\n");
    return 0;
}