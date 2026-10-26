#include <cstdio>
class CGameCameraManager { public: int id; };
class CCameraModeDef { public: float StringLengthXY; float CageRadius; float HeightOffsetZ; float LookOffsetZ; float FOV; };
template <class T> class CDefPointer { public: T* Def; T* operator->() const { return Def; } };
struct CBase { virtual ~CBase() { } };
struct CSACCombat : CBase {
    CGameCameraManager* mgr; float sl, cr, ho, agbt, fov; bool agb;
    CSACCombat(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>&,
       float stringLength, float cageRadius, float heightOffset, bool autoGoBehind,
       float autoGoBehindTime, float defaultFOV)
        : mgr(&manager), sl(stringLength), cr(cageRadius), ho(heightOffset), agbt(autoGoBehindTime), fov(defaultFOV), agb(autoGoBehind) { }
};
struct CSACCombatInit {
    CBase* Alloc(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) const
    {
        return new CSACCombat(manager, def, def->StringLengthXY, def->CageRadius, def->HeightOffsetZ, false, 0.0f,
                      def->FOV * (1.0f / 360.0f));
    }
};
int main()
{
    CGameCameraManager mgr; mgr.id = 1;
    CCameraModeDef d; d.StringLengthXY = 4.0f; d.CageRadius = 2.0f; d.HeightOffsetZ = 1.5f; d.LookOffsetZ = 0.0f; d.FOV = 90.0f;
    CDefPointer<const CCameraModeDef> dp; dp.Def = &d;
    CSACCombatInit init;
    CSACCombat* m = (CSACCombat*)init.Alloc(mgr, dp);
    int bad = (m->mgr != &mgr || m->sl != 4.0f || m->cr != 2.0f || m->ho != 1.5f || m->agb || m->agbt != 0.0f || m->fov != 0.25f);
    delete m;
    if (bad) { std::printf("008731b0_TEST FAIL\n"); return 1; }
    std::printf("008731b0_TEST PASS\n");
    return 0;
}