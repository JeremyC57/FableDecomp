#include <cstdio>
#include <cstring>
static int g_baseAssign;
struct CParentDefClassBase {
    virtual ~CParentDefClassBase() {}
    long m_base;
    CParentDefClassBase& operator=(const CParentDefClassBase& o) { m_base = o.m_base; ++g_baseAssign; return *this; }
};
struct CDefString { long TablePos; };
struct CVillagerInteractionsDef : CParentDefClassBase {
    CDefString Animation1; CDefString Animation2; float Distance;
    void Copy(const CParentDefClassBase& other)
    {
        const CVillagerInteractionsDef& source = static_cast<const CVillagerInteractionsDef&>(other);
        CParentDefClassBase::operator=(source);
        Animation1 = source.Animation1; Animation2 = source.Animation2; Distance = source.Distance;
    }
};
int main()
{
    int ok = 1;
    CVillagerInteractionsDef a, b;
    a.m_base = 1; a.Animation1.TablePos = 10; a.Animation2.TablePos = 20; a.Distance = 2.5f;
    b.m_base = 0; b.Animation1.TablePos = -1; b.Animation2.TablePos = -1; b.Distance = 0.0f;
    b.Copy(a);
    if (g_baseAssign != 1 || b.m_base != 1 || b.Animation1.TablePos != 10 || b.Animation2.TablePos != 20 || b.Distance != 2.5f) ok = 0;
    if (!ok) { std::printf("0045e620_TEST FAIL\n"); return 1; }
    std::printf("0045e620_TEST PASS\n");
    return 0;
}