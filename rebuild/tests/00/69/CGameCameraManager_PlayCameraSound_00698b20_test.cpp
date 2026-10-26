#include <cstdio>
class CCharString { public: const char* text; };
class CThing; class CBaseObject; class CCameraManagerDef;
static CThing* g_thing = 0; static const CCameraManagerDef* g_def = 0; static const CCharString* g_sound = 0;
struct CBaseIntelligentPointer { CBaseObject* m_pItem; CBaseObject* GetPItem() const { return m_pItem; } };
template <class T> class CDefPointer { public: T* m_pDef; T* Get() const { return m_pDef; } };
static void PlayCameraSoundThroughObservedThing(CThing* thing, const CCameraManagerDef* def, const CCharString& sound)
{ g_thing = thing; g_def = def; g_sound = &sound; }
class CGameCameraManager { public:
    CBaseIntelligentPointer PObservedThing;
    CDefPointer<const CCameraManagerDef> PDef;
    void PlayCameraSound(const CCharString& sound)
    { PlayCameraSoundThroughObservedThing((CThing*)PObservedThing.GetPItem(), PDef.Get(), sound); }
};
int main()
{
    static long thing, def;
    CGameCameraManager m; m.PObservedThing.m_pItem = (CBaseObject*)&thing; m.PDef.m_pDef = (const CCameraManagerDef*)&def;
    CCharString s; s.text = "CAM_WHOOSH";
    m.PlayCameraSound(s);
    if (g_thing != (CThing*)&thing || g_def != (const CCameraManagerDef*)&def || g_sound != &s) { std::printf("00698b20_TEST FAIL\n"); return 1; }
    std::printf("00698b20_TEST PASS\n");
    return 0;
}