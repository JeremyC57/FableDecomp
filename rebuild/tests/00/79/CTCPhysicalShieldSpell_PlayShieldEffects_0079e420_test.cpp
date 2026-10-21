// CTCPhysicalShieldSpell::PlayShieldEffects(CThingCreatureBase* creature) -- advance the shield wobble (bouncing between 0 and 1 at the def's wobble rate) and highlight the creature's appearance with a width between the def's minimum and the level's maximum
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079E420, 211 bytes (`ret 4`; PDB ?PlayShieldEffects@CTCPhysicalShieldSpell@@IAEXPAVCThingCreatureBase@@@Z).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, NGraphicAppearance::CTCGraphicAppearance::
//   SetAsHighlighted(float, float, const CRGBColour&, bool, bool, EAppearanceMeshEffectPriority, void*) @ 0x004C04C0.
//   Float pool: 1.0f @ 0x0122DED8, 0.0f @ 0x0122DEDC. Interface 0x5b = CTCGraphicAppearance (bitset word 2 bit 27).
// Shape: the appearance comes from the out-parameter GetInterface whose entry pick is one end/hit ternary
//   (`*out = (it == last || k < it->type ? last : it)->component`): the component loads straight into ecx before the
//   argument pushes and the bool goes through eax. The if-form pick (or a HasTC test + GetTC) loads it after the pushes.
struct CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)(it == map->last || type < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing { };
template <class T> struct vector {              // retail STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
struct CDefString { long m_index; };
struct CSpecialAbilitiesPhysicalShieldDef {     // retail layout: PDB offset - 4, 12-byte vectors
    unsigned char _pad00[0x3c];                 // CDefClassBase body
    CDefString InitSound;                       // +0x3c
    CDefString PowerUpSound[3];                 // +0x40
    CDefString CastSound;                       // +0x4c
    CDefString LoopingSound[4];                 // +0x50
    CDefString EndSound;                        // +0x60
    float Health[4];                            // +0x64
    float LevelUpTimings[3];                    // +0x74
    float CamDist[4];                           // +0x80
    float CamHeight[4];                         // +0x90
    float CamFOV[4];                            // +0xa0
    float CamSwitchTime[4];                     // +0xb0
    float CastCamDist;                          // +0xc0
    float CastCamHeight;                        // +0xc4
    float CastCamFOV;                           // +0xc8
    float CastCamSwitchTime;                    // +0xcc
    float MinWidth;                             // +0xd0
    vector<float> MaxWidth;                     // +0xd4
    float BeenHitExtraWidth;                    // +0xe0
    float BeenHitFadeOffTimeSecs;               // +0xe4
    float WobbleRate;                           // +0xe8
    vector<float> ColorStartLevel1;             // +0xec
    vector<float> ColorStartLevel2;             // +0xf8
    vector<float> ColorStartLevel3;             // +0x104
    vector<float> ColorStartLevel4;             // +0x110
    vector<float> ColorEnd;                     // +0x11c
    vector<float> ColorHit;                     // +0x128
    float FadeOutPhysicalShieldTimeSecs;        // +0x134
    vector<float> ManaDamage;                   // +0x138
    bool  ShieldCoversCarriedItems;             // +0x144
};
template <class T> struct CDefPointer {
    const T* Object;
    const T* operator->() const { return Object; }
};
struct CRGBColour { unsigned char B, G, R, A; };
struct CIntelligentPointer {                    // CIntelligentPointer<CThing> (8 bytes)
    void* __vftable;
    void* m_pItem;
};
struct CTCHeroStats {
    unsigned char _pad00[0x58];
    long Stamina;                               // +0x58 (PDB +0x60; retail CTCHeroStats sits 8 bytes below the PDB)
    void ModifyStaminaBy(long amount);          // 0x0057B1DB
};
namespace NGraphicAppearance {
    enum EAppearanceMeshEffectPriority { PRIORITY_4 = 4 };
    struct CTCGraphicAppearance {
        void SetAsHighlighted(float start, float width, const CRGBColour& colour, bool includeCarried, bool flag,
                              EAppearanceMeshEffectPriority priority, void* owner);   // 0x004C04C0
    };
}
struct CTCPhysicalShieldSpell {
    void*   __vftable;                          // +0x00
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
    CDefPointer<CSpecialAbilitiesPhysicalShieldDef> PDef;   // +0x0c
    long    SpellLevel;                         // +0x10
    long    ShieldFadeOutTimer;                 // +0x14
    long    CurShieldFadeOutTimer;              // +0x18
    long    DurationTimer;                      // +0x1c
    float   ShieldHealth;                       // +0x20
    float   CurWobbleTimer;                     // +0x24
    float   WobbleDelta;                        // +0x28
    CRGBColour ShieldColor;                     // +0x2c
    CIntelligentPointer ShieldEffect;           // +0x30
    CIntelligentPointer SwirlyEffect;           // +0x38
    unsigned long LoopingSound;                 // +0x40
    bool    Pausable;                           // +0x44
    bool    Paused;                             // +0x45
    bool    CutSceneCancelled;                  // +0x46
    void TakeDamage(float damage);
    void PlayShieldEffects(CThingCreatureBase* creature);
};

void CTCPhysicalShieldSpell::PlayShieldEffects(CThingCreatureBase* creature)
{
    CurWobbleTimer += WobbleDelta;
    if (CurWobbleTimer > 1.0f) {
        CurWobbleTimer = 1.0f;
        WobbleDelta = -PDef->WobbleRate;
    } else if (CurWobbleTimer < 0.0f) {
        CurWobbleTimer = 0.0f;
        WobbleDelta = PDef->WobbleRate;
    }
    const CSpecialAbilitiesPhysicalShieldDef* def = PDef.Object;
    float width = (def->MaxWidth[SpellLevel] - def->MinWidth) * CurWobbleTimer + def->MinWidth;
    NGraphicAppearance::CTCGraphicAppearance* appearance;
    if (creature->GetInterface(0x5b, &appearance)) {
        appearance->SetAsHighlighted(0.0f, width, ShieldColor, def->ShieldCoversCarriedItems, true,
                                     NGraphicAppearance::PRIORITY_4, this);
    }
}
// ---- harness ----
#include <cstdio>
#include <cmath>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
static int g_calls = 0; static float g_start = -1, g_width = -1; static const CRGBColour* g_colour = 0;
static bool g_carried = false, g_flag = false; static long g_priority = 0; static void* g_owner = 0; static const void* g_appThis = 0;
void NGraphicAppearance::CTCGraphicAppearance::SetAsHighlighted(float start, float width, const CRGBColour& colour, bool includeCarried,
    bool flag, EAppearanceMeshEffectPriority priority, void* owner)
{ ++g_calls; g_start = start; g_width = width; g_colour = &colour; g_carried = includeCarried; g_flag = flag; g_priority = priority; g_owner = owner; g_appThis = this; }
static bool Near(float a, float b) { return fabs(a - b) < 1e-4f; }
static CSpecialAbilitiesPhysicalShieldDef g_def;
static float g_maxWidth[4] = { 2.0f, 3.0f, 5.0f, 9.0f };
static CThingCreatureBase g_creature;
static NGraphicAppearance::CTCGraphicAppearance g_app;
static CInterfaceMap::SEntry g_entries[2];
int main()
{
    g_def.MinWidth = 1.0f; g_def.WobbleRate = 0.25f; g_def.ShieldCoversCarriedItems = true;
    g_def.MaxWidth._M_start = g_maxWidth; g_def.MaxWidth._M_finish = g_maxWidth + 4; g_def.MaxWidth._M_end_of_storage = g_maxWidth + 4;
    g_entries[0].type = 0x5b; g_entries[0].component = (CTCBase*)&g_app;
    g_entries[1].type = 0x60; g_entries[1].component = 0;
    g_creature.m_interfaces.first = g_entries; g_creature.m_interfaces.last = g_entries + 2;
    g_creature.m_presentBits[0x5b >> 5] = 1UL << (0x5b & 31);
    CTCPhysicalShieldSpell s;
    s.PDef.Object = &g_def; s.SpellLevel = 2; s.CurWobbleTimer = 0.5f; s.WobbleDelta = 0.25f;
    s.PlayShieldEffects(&g_creature);               // 0.75: width (5-1)*0.75+1 = 4
    CHECK(Near(s.CurWobbleTimer, 0.75f) && Near(s.WobbleDelta, 0.25f));
    CHECK(g_calls == 1 && g_start == 0.0f && Near(g_width, 4.0f) && g_colour == &s.ShieldColor && g_carried && g_flag);
    CHECK(g_priority == 4 && g_owner == &s && g_appThis == &g_app);
    s.CurWobbleTimer = 0.9f;                        // overshoot: clamp to 1 and reverse
    s.PlayShieldEffects(&g_creature);
    CHECK(s.CurWobbleTimer == 1.0f && Near(s.WobbleDelta, -0.25f) && Near(g_width, 5.0f));
    s.CurWobbleTimer = 0.1f;                        // undershoot: clamp to 0 and reverse
    g_def.ShieldCoversCarriedItems = false;
    s.PlayShieldEffects(&g_creature);
    CHECK(s.CurWobbleTimer == 0.0f && Near(s.WobbleDelta, 0.25f) && Near(g_width, 1.0f) && !g_carried);
    g_creature.m_presentBits[0x5b >> 5] = 0;        // no appearance: no highlight
    s.PlayShieldEffects(&g_creature);
    CHECK(g_calls == 3 && Near(s.CurWobbleTimer, 0.25f));
    if (fails == 0) printf("0079e420_TEST PASS\n");
    return fails;
}