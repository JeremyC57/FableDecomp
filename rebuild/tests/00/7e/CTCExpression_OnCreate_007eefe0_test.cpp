// Behaviour test for 007eefe0: body copied from src/007eefe0.cpp (CTCExpression::OnCreate), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CDefClassBase;
class CExpressionDef;
static int g_destroyed = 0;
struct CRefCountedDef {
    virtual void v00() {}
    virtual void DestroySelf() { ++g_destroyed; }
    long RefCount;
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) DestroySelf(); }
};
template<class T> struct CDefPointerOut {
    T* Object;
    CDefPointerOut() : Object(0) { }
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object != 0) Object->AddRef(); }
    ~CDefPointer() { if (Object != 0) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CExpressionUserDef : CRefCountedDef {
    unsigned char _pad08[0x28 - 0x08];
    long          ExpressionDefIndex;
};
static CExpressionUserDef* g_userDef = 0; static int g_getSubDef = 0;
class CDefClassBase : public CRefCountedDef { public:
    bool GetSubDef(CDefPointerOut<CExpressionUserDef>* out);
};
bool CDefClassBase::GetSubDef(CDefPointerOut<CExpressionUserDef>* out)
{
    ++g_getSubDef;
    if (!g_userDef) return false;
    ++g_userDef->RefCount; out->Object = g_userDef; return true;
}
class CExpressionDef : public CRefCountedDef { public: };
static CExpressionDef* g_exprDef = 0; static long g_askedIndex = -1; static int g_getDef = 0;
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CExpressionDef>* out);
};
bool CDefinitionManager::GetDef(long index, CDefPointer<CExpressionDef>* out)
{
    ++g_getDef; g_askedIndex = index;
    if (!g_exprDef) return false;
    ++g_exprDef->RefCount; out->Object = g_exprDef; return true;
}
static CDefinitionManager g_manager;
class CThing { public:
    unsigned char              _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;
    CDefPointer<CDefClassBase> GetDef() const { return PDef; }
};
class CTCExpression { public:
    virtual void v00() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CDefPointer<CExpressionDef> PDef;
    CDefinitionManager* GetDefinitionManager() { return &g_manager; }
    void OnCreate();
};
void CTCExpression::OnCreate()
{
    if (PDef.Object == 0)
    {
        CDefPointerOut<CExpressionUserDef> userDef;
        bool hasExpression = ParentThing->GetDef()->GetSubDef(&userDef) && userDef.Object->ExpressionDefIndex > 0;
        CExpressionUserDef* user = userDef.Object;
        if (hasExpression)
            GetDefinitionManager()->GetDef(user->ExpressionDefIndex, &PDef);
        if (user != 0)
            user->Release();
    }
}
int main()
{
    static CThing thing; static CDefClassBase classDef; static CExpressionUserDef userDef; static CExpressionDef exprDef;
    memset(&thing, 0, sizeof(thing)); classDef.RefCount = 1; thing.PDef.Object = &classDef; userDef.RefCount = 1; userDef.ExpressionDefIndex = 33; exprDef.RefCount = 1;
    g_userDef = &userDef; g_exprDef = &exprDef;
    CTCExpression tc; tc.ParentThing = &thing; tc.PDef.Object = 0;
    // 1: no def yet, user def with an expression: the expression def is fetched and kept
    tc.OnCreate();
    CHECK(g_getSubDef == 1 && g_getDef == 1 && g_askedIndex == 33); CHECK(tc.PDef.Object == &exprDef && exprDef.RefCount == 2);
    CHECK(userDef.RefCount == 1 && classDef.RefCount == 1 && g_destroyed == 0);
    // 2: already has a def: nothing happens
    tc.OnCreate(); CHECK(g_getSubDef == 1);
    // 3: user def without an expression index: no def lookup, user released
    tc.PDef.Object = 0; userDef.ExpressionDefIndex = 0; tc.OnCreate(); CHECK(g_getSubDef == 2 && g_getDef == 1 && tc.PDef.Object == 0); CHECK(userDef.RefCount == 1);
    // 4: no user def at all
    g_userDef = 0; tc.OnCreate(); CHECK(g_getSubDef == 3 && g_getDef == 1 && g_destroyed == 0);
    // 5: a user def held only by the lookup is destroyed
    g_userDef = &userDef; userDef.RefCount = 0; userDef.ExpressionDefIndex = 5; tc.OnCreate(); CHECK(g_destroyed == 1 && g_askedIndex == 5);
    if (fails == 0) printf("007eefe0_TEST PASS\n"); else printf("007eefe0_TEST FAIL\n");
    return fails;
}