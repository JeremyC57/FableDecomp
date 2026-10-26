#include <cstdio>
#include <cstring>
#include <new>
class CGameCameraManager; class CCameraModeDef; class CBaseObject; class CThing;
static CGameCameraManager* g_manager = 0; static const CCameraModeDef* g_def = 0;
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() { }
    CIntelligentPointer(const CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
class C3DVector { public: float x, y, z; };
namespace NCameraModes {
class CBase { public:
    virtual void v00() { }
    unsigned char _body[0xa0];
    CBase(CGameCameraManager* manager, const CCameraModeDef* def) { g_manager = manager; g_def = def; }
};
struct CCameraPointModeBase : CBase {
    CIntelligentPointer PCameraPoint; C3DVector Pos; float FOV; bool CutInto; bool CutOutOf;
    virtual void v00() { }
    CCameraPointModeBase(CGameCameraManager* manager, const CCameraModeDef* def, const CThing* point)
        : CBase(manager, def), PCameraPoint(point) { }
};
struct CSnapToPoint : CCameraPointModeBase {
    bool TrackThing; C3DVector LookDirection;
    virtual void v00() { }
    CSnapToPoint(CGameCameraManager* manager, const CCameraModeDef* def, const CThing* point)
        : CCameraPointModeBase(manager, def, point), TrackThing(true) { }
};
}
int main()
{
    static unsigned char buf[sizeof(NCameraModes::CSnapToPoint)];
    std::memset(buf, 0, sizeof(buf));
    static long thingStorage, managerStorage, defStorage;
    NCameraModes::CSnapToPoint* s = new (buf) NCameraModes::CSnapToPoint(
        (CGameCameraManager*)&managerStorage, (const CCameraModeDef*)&defStorage, (const CThing*)&thingStorage);
    bool ok = g_manager == (CGameCameraManager*)&managerStorage && g_def == (const CCameraModeDef*)&defStorage
        && s->PCameraPoint.m_pItem == (CBaseObject*)&thingStorage && s->TrackThing;
    if (!ok) { std::printf("0056e330_TEST FAIL\n"); return 1; }
    std::printf("0056e330_TEST PASS\n");
    return 0;
}