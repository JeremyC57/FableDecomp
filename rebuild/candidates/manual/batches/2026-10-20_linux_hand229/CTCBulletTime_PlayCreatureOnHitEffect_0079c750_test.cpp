// Behaviour test for CTCBulletTime::PlayCreatureOnHitEffect (0x0079C750).
#include <cstdio>

struct C3DVector { float X, Y, Z; };
class CThing { public: C3DVector pos; const C3DVector& GetPosition() const { return pos; } };
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { CThing* item = o.GetPItem(); SetPItem(item); return *this; }
};
struct CRadialBlurParams { C3DVector ThresholdCol; float Length; C3DVector IntensityCol; float Fade, Range, Twist, FadeTimeSecs; };
struct CSpecialAbilitiesBulletTimeDef { CRadialBlurParams RadialBlur, DamageRadialBlur; };
template <class T> struct CDefPointer { T* Object; T* operator->() const { return Object; } };
static float g_length = 0, g_fadeOut = 0; static void* g_fadeKey = 0; static int g_next = 100;
struct CRadialBlurManager {
    void* AddRadialBlur(long, long, const C3DVector&, float length, const C3DVector&, float, float, float)
    { g_length = length; return (void*)(long)(g_next++); }
    void FadeOutRadialBlur(float seconds, void* key) { g_fadeOut = seconds; g_fadeKey = key; }
};
static const C3DVector* g_centre = 0;
struct CEnvironmentSpellEffectInterface { void SetRadialBlurCenterWorldPos(const C3DVector& p, bool) { g_centre = &p; } };
struct CWorldEffects { CEnvironmentSpellEffectInterface* SpellEffects; CRadialBlurManager* RadialBlurs; };
struct CWorld { CWorldEffects* PEffects; };
class CTCBulletTime {
public:
    CThing* ParentThing; CWorld* world;
    CDefPointer<const CSpecialAbilitiesBulletTimeDef> PDef;
    CIntelligentPointer<CThing> HitCreatureBlur; void* RadialBlurKey;
    CTCBulletTime() : HitCreatureBlur(0) {}
    CWorld* GetWorld() { return world; }
    void PlayCreatureOnHitEffect(CThing* creature)
    {
        HitCreatureBlur = CIntelligentPointer<CThing>(creature);
        CEnvironmentSpellEffectInterface* spellEffects = GetWorld()->PEffects->SpellEffects;
        CRadialBlurManager* blurs = GetWorld()->PEffects->RadialBlurs;
        CThing* parent = ParentThing;
        if (HitCreatureBlur.GetPItem() == parent) {
            RadialBlurKey = blurs->AddRadialBlur(0, 0, PDef->DamageRadialBlur.ThresholdCol, PDef->DamageRadialBlur.Length,
                                                 PDef->DamageRadialBlur.IntensityCol, PDef->DamageRadialBlur.Fade,
                                                 PDef->DamageRadialBlur.Range, PDef->DamageRadialBlur.Twist);
            blurs->FadeOutRadialBlur(PDef->DamageRadialBlur.FadeTimeSecs, RadialBlurKey);
        } else {
            RadialBlurKey = blurs->AddRadialBlur(0, 0, PDef->RadialBlur.ThresholdCol, PDef->RadialBlur.Length,
                                                 PDef->RadialBlur.IntensityCol, PDef->RadialBlur.Fade,
                                                 PDef->RadialBlur.Range, PDef->RadialBlur.Twist);
            blurs->FadeOutRadialBlur(PDef->RadialBlur.FadeTimeSecs, RadialBlurKey);
        }
        spellEffects->SetRadialBlurCenterWorldPos(HitCreatureBlur.GetPItem()->GetPosition(), true);
    }
};

int main()
{
    int ok = 1;
    CSpecialAbilitiesBulletTimeDef def;
    def.RadialBlur.Length = 1.0f; def.RadialBlur.FadeTimeSecs = 0.25f;
    def.DamageRadialBlur.Length = 2.0f; def.DamageRadialBlur.FadeTimeSecs = 0.75f;
    CEnvironmentSpellEffectInterface spell; CRadialBlurManager blurs; CWorldEffects fx; fx.SpellEffects = &spell; fx.RadialBlurs = &blurs;
    CWorld world; world.PEffects = &fx;
    CThing hero, enemy;
    CTCBulletTime bt; bt.ParentThing = &hero; bt.world = &world; bt.PDef.Object = &def; bt.RadialBlurKey = 0;
    bt.PlayCreatureOnHitEffect(&enemy);
    if (bt.HitCreatureBlur.GetPItem() != &enemy || g_length != 1.0f || g_fadeOut != 0.25f || g_fadeKey != bt.RadialBlurKey) ok = 0;
    if (g_centre != &enemy.pos) ok = 0;
    bt.PlayCreatureOnHitEffect(&hero);
    if (g_length != 2.0f || g_fadeOut != 0.75f || g_fadeKey != bt.RadialBlurKey || g_centre != &hero.pos) ok = 0;
    if (!ok) { std::printf("0079c750_TEST FAIL\n"); return 1; }
    std::printf("0079c750_TEST PASS\n");
    return 0;
}
