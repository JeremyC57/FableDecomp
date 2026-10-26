// Test for 0x0084D090 NCameraModes::CBase::PreparePrimitivesForRendering: the base mode's slot-5 virtual does nothing; a
// derived mode's override is dispatched through the same slot.
#include <stdio.h>
class CEnginePrimitivePrepareInfo { public: int Calls; };
namespace NCameraModes {
class CBase {
public:
    virtual void v00() { }
    virtual void v01() { }
    virtual void v02() { }
    virtual void v03() { }
    virtual void v04() { }
    virtual void PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) const;
};
}
class CTestMode : public NCameraModes::CBase {
public:
    virtual void PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) const { ++const_cast<CEnginePrimitivePrepareInfo&>(info).Calls; }
};
int main()
{
    CEnginePrimitivePrepareInfo info; info.Calls = 0;
    NCameraModes::CBase b; CTestMode t;
    const NCameraModes::CBase* p = &b;
    p->PreparePrimitivesForRendering(info);
    if (info.Calls != 0) { printf("0084d090_TEST FAIL base\n"); return 1; }
    p = &t;
    p->PreparePrimitivesForRendering(info);
    if (info.Calls != 1) { printf("0084d090_TEST FAIL derived\n"); return 1; }
    printf("0084d090_TEST PASS\n");
    return 0;
}
