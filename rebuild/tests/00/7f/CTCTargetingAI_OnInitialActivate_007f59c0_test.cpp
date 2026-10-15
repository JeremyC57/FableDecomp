#include <cstdio>
// Behaviour test for 0x007F59C0: the owner's def is held across GetSubDef(&PSubDef), then released; a null def is skipped.
struct CSubDef { int id; };
static CSubDef g_sub; static long g_refDuring = -1; static int g_destroyed = 0;
struct CDefClassBase {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long refCount;
    void AddRef() { ++refCount; }
    void Release() { if (--refCount == 0) Destroy(); }
    bool GetSubDef(CSubDef** out) { g_refDuring = refCount; *out = &g_sub; return true; }
};
struct CDefClassPointer {
    CDefClassBase* Object;
    CDefClassPointer(CDefClassBase* p) : Object(p) {}
    CDefClassPointer(const CDefClassPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefClassPointer() { if (Object) Object->Release(); }
    CDefClassBase* operator->() const { return Object; }
};
struct CThing { CDefClassPointer PDef; CThing(CDefClassBase* d) : PDef(d) {} };
struct CTCBase { virtual void v00() {} CThing* ParentThing; };
struct CTCTargetingAI : CTCBase { CSubDef* PSubDef; void OnInitialActivate(); };
void CTCTargetingAI::OnInitialActivate()
{
    CDefClassPointer def(ParentThing->PDef);
    def->GetSubDef(&PSubDef);
}
int main()
{
    CDefClassBase d; d.refCount = 1;
    CThing t(&d);
    CTCTargetingAI ai; ai.ParentThing = &t; ai.PSubDef = 0;
    ai.OnInitialActivate();
    if (ai.PSubDef != &g_sub || g_refDuring != 2 || d.refCount != 1 || g_destroyed != 0) { std::printf("007f59c0_TEST FAIL\n"); return 1; }
    std::printf("007f59c0_TEST PASS\n");
    return 0;
}