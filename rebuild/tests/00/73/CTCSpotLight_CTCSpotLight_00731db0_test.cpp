// Behaviour test for CTCSpotLight::~CTCSpotLight (0x00731DB0): clear LightPrimitive, release the light def, release the shared
// light info (delete func + block at the last reference), then ~CTCBase.
#include <cstdio>
#include <cstring>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
struct CInfo { unsigned long RefCount; int Deleted; };
struct CCountedModel {
    void* Data; CInfo* Info;
    ~CCountedModel() { if (Info) { if (--Info->RefCount == 0) { Info->Deleted++; Log("I"); } } Data = 0; Info = 0; }
};
struct CDefPointee { long RefCount; int Deleted; };
struct CDefModel { CDefPointee* Object; ~CDefModel() { if (Object) { if (--Object->RefCount == 0) { Object->Deleted++; Log("D"); } Object = 0; } } };
struct CTCBaseModel { virtual ~CTCBaseModel() { Log("B"); } };
struct CAppearanceComponentModel { virtual void OnAppearanceClear() { } ~CAppearanceComponentModel() { Log("A"); } };
static void* g_seenPrimitive = (void*)1;
struct CTCSpotLightModel : CTCBaseModel, CAppearanceComponentModel {
    CCountedModel PLightInfo;
    CDefModel PLightDef;
    bool Overridden;
    void* LightPrimitive;
    virtual ~CTCSpotLightModel() { LightPrimitive = 0; g_seenPrimitive = LightPrimitive; }
};
int main()
{
    CInfo info = { 1, 0 };
    CDefPointee def = { 2, 0 };
    {
        CTCSpotLightModel l;
        l.PLightInfo.Data = &info; l.PLightInfo.Info = &info;
        l.PLightDef.Object = &def;
        l.LightPrimitive = &def;
    }
    if (g_seenPrimitive != 0 || std::strcmp(g_log, "IAB") != 0 || info.Deleted != 1 || def.RefCount != 1 || def.Deleted) {
        std::printf("00731db0_TEST FAIL\n"); return 1;
    }
    std::printf("00731db0_TEST PASS\n");
    return 0;
}