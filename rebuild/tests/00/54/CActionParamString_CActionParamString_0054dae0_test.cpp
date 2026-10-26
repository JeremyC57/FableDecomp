#include <cstdio>

static int g_copies = 0;
class CWideString { public: void* m_p; CWideString() : m_p((void*)0x31) { } CWideString(const CWideString& o) : m_p(o.m_p) { ++g_copies; } };
struct CActionParam { virtual void v00() { } long ActionParamType; CActionParam(long type) : ActionParamType(type) { } };
struct CActionParamString : CActionParam { CWideString Value; virtual void v00() { }
    CActionParamString(const CWideString& value) : CActionParam(4), Value(value) { } };
int main()
{
    CWideString s; CActionParamString a(s);
    if (a.ActionParamType != 4 || g_copies != 1 || a.Value.m_p != (void*)0x31) { std::printf("0054dae0_TEST FAIL\n"); return 1; }
    std::printf("0054dae0_TEST PASS\n"); return 0;
}