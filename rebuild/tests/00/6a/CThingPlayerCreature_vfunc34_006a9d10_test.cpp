// behaviour test for CThingPlayerCreature::vfunc_34 (0x006A9D10): player colour or opaque black, clamped value, enabled, 0.7 scale, def ref balanced
#include <cstdio>
static int g_destroyed = 0;
struct CDefPointeeBase { long RefCount; virtual void Destroy() { ++g_destroyed; } virtual ~CDefPointeeBase() {}
    void AddRef() { ++RefCount; } void Release() { if (--RefCount == 0) Destroy(); } };
template <class T> struct CDefPointer { T* Object; CDefPointer(T* o) : Object(o) {}
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { if (Object) Object->Release(); } T* operator->() const { return Object; } };
struct CRGBColour { unsigned char B, G, R, A; CRGBColour() {} CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
struct CPlayerDef : CDefPointeeBase { CRGBColour Colour; };
struct CPlayer { bool m_init; CDefPointer<CPlayerDef> PlayerDef; CPlayer(CPlayerDef* d) : PlayerDef(d) {} bool IsInitialised() { return m_init; } };
struct CPlayerManager { CPlayer* m_players[2]; CPlayer* GetPlayer_00449910(long n) { return m_players[n]; } };
struct CThingHighlightInfo { float Scale; float Value; CRGBColour Colour; bool Enabled; };
static CPlayerManager g_manager;
class CThingPlayerCreature { public:
    signed char PlayerNumber; float m_value;
    CPlayerManager* GetPlayerManager_004c79f0() { return &g_manager; }
    float GetNormalisedValue_006a4e40() { return m_value; }
    bool vfunc_34(CThingHighlightInfo* info)
    {
        CPlayer* player = GetPlayerManager_004c79f0()->GetPlayer_00449910(PlayerNumber);
        CRGBColour colour;
        if (player->IsInitialised() == true) { CDefPointer<CPlayerDef> def(player->PlayerDef); colour = def->Colour; }
        else colour = CRGBColour(0, 0, 0, 255);
        float value = GetNormalisedValue_006a4e40();
        info->Value = value;
        if (value < 0.0f) info->Value = 0.0f; else if (value > 1.0f) info->Value = 1.0f;
        info->Colour = colour; info->Enabled = true; info->Scale = 0.7f;
        return false;
    } };
int main()
{
    CPlayerDef def; def.RefCount = 1; def.Colour = CRGBColour(10, 20, 30, 40);
    CPlayer p0(&def); p0.m_init = true; CPlayer p1(&def); p1.m_init = false;
    g_manager.m_players[0] = &p0; g_manager.m_players[1] = &p1;
    CThingPlayerCreature c; c.PlayerNumber = 0; c.m_value = 1.5f;
    CThingHighlightInfo info; info.Enabled = false;
    int bad = c.vfunc_34(&info);
    if (info.Value != 1.0f || info.Colour.R != 10 || info.Colour.B != 30 || info.Colour.A != 40 || !info.Enabled || info.Scale != 0.7f) bad = 1;
    if (def.RefCount != 1 || g_destroyed != 0) bad = 1;
    c.PlayerNumber = 1; c.m_value = -2.0f; c.vfunc_34(&info);
    if (info.Value != 0.0f || info.Colour.R != 0 || info.Colour.G != 0 || info.Colour.B != 0 || info.Colour.A != 255) bad = 1;
    c.m_value = 0.25f; c.vfunc_34(&info); if (info.Value != 0.25f) bad = 1;
    def.RefCount = 100; p0.PlayerDef.Object = 0; p1.PlayerDef.Object = 0;
    if (bad) { std::printf("006a9d10_TEST FAIL\n"); return 1; }
    std::printf("006a9d10_TEST PASS\n");
    return 0;
}