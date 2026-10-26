#include <cstdio>
class CGameCameraManager { public: int id; };
class CCameraModeDef { public: float FOV; };
template <class T> class CDefPointer { public: T* Def; };
static int g_ctor = 0;
struct CBase {
    CGameCameraManager* mgr; const CCameraModeDef* def;
    CBase(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& d) : mgr(&manager), def(d.Def) { ++g_ctor; }
    virtual ~CBase() { }
};
struct CZTargetNew : CBase {
    CZTargetNew(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& d) : CBase(manager, d) { }
};
struct CZTargetNewInit {
    CBase* Alloc(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& d) const
    {
        return new CZTargetNew(manager, d);
    }
};
int main()
{
    CGameCameraManager mgr; mgr.id = 2;
    CCameraModeDef def; def.FOV = 60.0f;
    CDefPointer<const CCameraModeDef> dp; dp.Def = &def;
    CZTargetNewInit init;
    CBase* m = init.Alloc(mgr, dp);
    int bad = (!m || m->mgr != &mgr || m->def != &def || g_ctor != 1);
    delete m;
    if (bad) { std::printf("00873200_TEST FAIL\n"); return 1; }
    std::printf("00873200_TEST PASS\n");
    return 0;
}