#include <cstdio>

static int g_copies = 0;
struct CCountedPointer { void* m_pObject; void* m_pInfo; CCountedPointer(void* o) : m_pObject(o), m_pInfo(0) { }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pInfo(other.m_pInfo) { ++g_copies; } };
struct CActionParam { virtual void v00() { } long ActionParamType; CActionParam(long type) : ActionParamType(type) { } };
struct CActionParamCCountedPointer : CActionParam { CCountedPointer Value; virtual void v00() { }
    CActionParamCCountedPointer(const CCountedPointer& value) : CActionParam(9), Value(value) { } };
int main()
{
    int o; CCountedPointer p(&o); CActionParamCCountedPointer a(p);
    if (a.ActionParamType != 9 || g_copies != 1 || a.Value.m_pObject != &o) { std::printf("004854df_TEST FAIL\n"); return 1; }
    std::printf("004854df_TEST PASS\n"); return 0;
}
