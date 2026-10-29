#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
static int g_base = 0;
class CBaseClass {
public:  virtual void v00() { } CBaseClass() { ++g_base; } };
struct __declspec(novtable) CShaderConstantRangeBase : CBaseClass { long m_first; CShaderConstantRangeBase(long f) : CBaseClass(), m_first(f) { } };
class CShaderConstantRange : public CShaderConstantRangeBase { public:
    long m_count; virtual void v00() { }
    CShaderConstantRange(long f, long c) : CShaderConstantRangeBase(f), m_count(c) { }
};
int main()
{
    CShaderConstantRange v(12, 4);
    if (g_base != 1 || v.m_first != 12 || v.m_count != 4) { std::printf("00bdaf80_TEST FAIL\n"); return 1; }
    std::printf("00bdaf80_TEST PASS\n"); return 0;
}