// Link test for 0x00A36F80: the source object's CClass::PostConstructionInit must pass *this to the factory's
// OnCreateClass (vtable slot 2) and nothing else.
#include <cstdio>
class CClass;
class CClassTypeInit;
static int g_calls[4];
static CClass* g_created = 0;
class CClassFactory {
    friend class CClass;
public:
    virtual ~CClassFactory() { ++g_calls[0]; }
protected:
    virtual void AddNewClassType(const CClassTypeInit&) { ++g_calls[1]; }
    virtual void OnCreateClass(CClass& c) { ++g_calls[2]; g_created = &c; }
    virtual void OnDeleteClass(CClass&) { ++g_calls[3]; }
};
class CClass {
public:
    int payload;
    void PostConstructionInit(CClassFactory& factory);   // from the source object
};
static int g_derived = 0;
class CDerivedFactory : public CClassFactory {
protected:
    virtual void OnCreateClass(CClass& c) { ++g_derived; CClassFactory::OnCreateClass(c); }
};
int main()
{
    CDerivedFactory f;
    CClass a, b;
    a.PostConstructionInit(f);
    int ok = g_derived == 1 && g_calls[2] == 1 && g_created == &a && g_calls[1] == 0 && g_calls[3] == 0;
    b.PostConstructionInit(f);
    ok = ok && g_derived == 2 && g_created == &b && g_calls[0] == 0;
    if (!ok) { std::printf("00a36f80_TEST FAIL\n"); return 1; }
    std::printf("00a36f80_TEST PASS\n");
    return 0;
}
