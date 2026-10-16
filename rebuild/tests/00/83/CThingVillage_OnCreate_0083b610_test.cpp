#include <cstdio>
#include <cstring>
struct CCharString {
    char Text[40];
    CCharString(const char* t, long = -1) { std::strcpy(Text, t); }
};
struct CTCBase; struct CTCParamsBase;
#pragma pack(push, 2)
struct CEngineGraphic { unsigned long BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
#pragma pack(pop)
struct CTCGraphicAppearance { CEngineGraphic Graphic; };
static char g_added[2][40]; static int g_nAdded = 0; static bool g_baseOk = true;
static CTCGraphicAppearance g_app;
static CEngineGraphic g_current = { 7, 0.5f, 2.0f, true, 3 };
struct CThingGameObject {
    CTCGraphicAppearance* PTCGraphicAppearance;
    float MaxHealth;
    CTCBase* AddTC(const CCharString& name, bool a2, CTCParamsBase* p)
    {
        if (!a2 && !p && g_nAdded < 2) std::strcpy(g_added[g_nAdded++], name.Text);
        PTCGraphicAppearance = &g_app;
        return 0;
    }
    bool OnCreate() { return g_baseOk; }
};
struct CThingVillage : CThingGameObject {
    const CEngineGraphic* GetCurrentGraphic() { return &g_current; }
    bool OnCreate();
};
bool CThingVillage::OnCreate()
{
    {
        CCharString name("CTCGraphicAppearance");
        AddTC(name, false, 0);
    }
    {
        CCharString name("CTCGraphicAppearanceStaticMesh");
        AddTC(name, false, 0);
    }
    CTCGraphicAppearance* appearance = PTCGraphicAppearance;
    appearance->Graphic = *GetCurrentGraphic();
    if (CThingGameObject::OnCreate()) {
        MaxHealth = 100.0f;
        return true;
    }
    return false;
}
int main()
{
    CThingVillage v; v.PTCGraphicAppearance = 0; v.MaxHealth = 0.0f;
    if (!v.OnCreate() || g_nAdded != 2 || std::strcmp(g_added[0], "CTCGraphicAppearance") || std::strcmp(g_added[1], "CTCGraphicAppearanceStaticMesh")
        || g_app.Graphic.BankIndex != 7 || g_app.Graphic.Type != 3 || !g_app.Graphic.AdditiveAlpha || v.MaxHealth != 100.0f) { std::printf("0083b610_TEST FAIL\n"); return 1; }
    g_baseOk = false; v.MaxHealth = 5.0f; g_nAdded = 0;
    if (v.OnCreate() || v.MaxHealth != 5.0f) { std::printf("0083b610_TEST FAIL\n"); return 1; }
    std::printf("0083b610_TEST PASS\n");
    return 0;
}