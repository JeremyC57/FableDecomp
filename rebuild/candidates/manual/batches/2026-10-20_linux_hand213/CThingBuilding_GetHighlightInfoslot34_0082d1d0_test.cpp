// Behaviour test for CThingBuilding::GetHighlightInfo_slot34 (0x0082D1D0): the record takes the owning player's def
// colour (through a counted def copy that is released) when the player is local, else opaque black; alpha is the
// normalised time clamped to 0..1; scale 1 and valid; the call reports false.
#include <cstdio>
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() { }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
static int g_deletes = 0;
struct CDefPointeeBase {
    virtual void v00() {}
    virtual void Delete() { ++g_deletes; }
    long RefCount;
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Delete(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(T* object) : Object(object) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CPlayerDef : CDefPointeeBase { unsigned char _pad08[0x40 - 0x08]; CRGBColour Colour; };
static bool g_local = false;
struct CPlayer { unsigned char _pad00[0x21c]; CDefPointer<CPlayerDef> PlayerDef; CPlayer(CPlayerDef* def) : PlayerDef(def) {} bool IsLocal() { return g_local; } };
static CPlayer* g_player; static long g_askedPlayer = -99;
struct CPlayerManager { CPlayer* GetPlayer(long playerNumber) { g_askedPlayer = playerNumber; return g_player; } };
static CPlayerManager g_manager; static float g_time = 0.5f;
struct SThingHighlightInfo { long _unused; float Scale; float Alpha; CRGBColour Colour; bool Valid; };
struct CThingBuilding {
    unsigned char _pad00[0x90]; signed char PlayerNumber;
    CPlayerManager* GetPlayerManager() { return &g_manager; }
    float GetNormalisedTime() { return g_time; }
    bool GetHighlightInfo_slot34(SThingHighlightInfo* info);
};
bool CThingBuilding::GetHighlightInfo_slot34(SThingHighlightInfo* info)
{
    CRGBColour colour;
    CPlayer* player = GetPlayerManager()->GetPlayer(PlayerNumber);
    if (player->IsLocal() == true) {
        CDefPointer<CPlayerDef> def(player->PlayerDef);
        colour = def->Colour;
    } else {
        colour = CRGBColour(0, 0, 0, 255);
    }
    float time = GetNormalisedTime();
    info->Alpha = time;
    if (time < 0.0f)
        info->Alpha = 0.0f;
    else if (time > 1.0f)
        info->Alpha = 1.0f;
    info->Colour = colour;
    info->Valid = true;
    info->Scale = 1.0f;
    return false;
}
static int fail(const char* why) { std::printf("0082d1d0_TEST FAIL: %s\n", why); return 1; }
int main()
{
    static CPlayerDef def; def.RefCount = 1; def.Colour = CRGBColour(10, 20, 30, 40);
    static CPlayer player(&def); g_player = &player;
    static CThingBuilding building; building.PlayerNumber = 2;
    SThingHighlightInfo info; info._unused = 7; info.Valid = false;
    // 1. remote player: opaque black, alpha = time, scale 1
    g_local = false; g_time = 0.25f;
    if (building.GetHighlightInfo_slot34(&info)) return fail("return value");
    if (g_askedPlayer != 2 || info.Colour.R != 0 || info.Colour.G != 0 || info.Colour.B != 0 || info.Colour.A != 255) return fail("black colour");
    if (info.Alpha != 0.25f || info.Scale != 1.0f || !info.Valid || info._unused != 7) return fail("record words");
    // 2. local player: the def colour (copied through a counted copy, reference count restored, nothing deleted)
    g_local = true; g_time = -0.5f; building.GetHighlightInfo_slot34(&info);
    if (info.Colour.R != 10 || info.Colour.G != 20 || info.Colour.B != 30 || info.Colour.A != 40) return fail("def colour");
    if (def.RefCount != 1 || g_deletes != 0) return fail("def reference");
    if (info.Alpha != 0.0f) return fail("clamp below");
    g_time = 1.5f; building.GetHighlightInfo_slot34(&info); if (info.Alpha != 1.0f) return fail("clamp above");
    g_time = 1.0f; building.GetHighlightInfo_slot34(&info); if (info.Alpha != 1.0f) return fail("one stays one");
    // 3. the copy's release deletes a def whose last reference it held
    def.RefCount = 0; building.GetHighlightInfo_slot34(&info); if (g_deletes != 1) return fail("last reference deleted");
    std::printf("0082d1d0_TEST PASS\n");
    return 0;
}
