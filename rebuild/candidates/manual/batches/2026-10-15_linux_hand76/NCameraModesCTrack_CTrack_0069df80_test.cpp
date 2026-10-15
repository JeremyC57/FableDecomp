#include <cstdio>
#include <cstring>
#include <new>
struct CGameCameraManager; struct CCameraModeDef; struct CBaseObject; struct CThing;
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
struct C3DVector { float x, y, z; };
namespace NCameraModes {
struct CBase {
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
class CTrackPath;
struct CTrack : CCameraPointModeBase {
    CTrackPath* PTrack; float CurrentFractionAlongTrack; bool ZTargeting;
    virtual void v00() { }
    CTrack(CGameCameraManager* manager, const CCameraModeDef* def, const CThing* point)
        : CCameraPointModeBase(manager, def, point), ZTargeting(false) { }
};
}
int main()
{
    static unsigned char buf[sizeof(NCameraModes::CTrack)];
    std::memset(buf, 0xFF, sizeof(buf));
    static long thingStorage, managerStorage, defStorage;
    NCameraModes::CTrack* s = new (buf) NCameraModes::CTrack(
        (CGameCameraManager*)&managerStorage, (const CCameraModeDef*)&defStorage, (const CThing*)&thingStorage);
    bool ok = g_manager == (CGameCameraManager*)&managerStorage && g_def == (const CCameraModeDef*)&defStorage
        && s->PCameraPoint.m_pItem == (CBaseObject*)&thingStorage && !s->ZTargeting;
    if (!ok) { std::printf("0069df80_TEST FAIL\n"); return 1; }
    std::printf("0069df80_TEST PASS\n");
    return 0;
}
