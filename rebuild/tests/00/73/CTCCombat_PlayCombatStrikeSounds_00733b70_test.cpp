// Behaviour test for CTCCombat::PlayCombatStrikeSounds (0x00733B70): the row body verbatim over models of its callees. Kills: any
// of the four conditions dropped or inverted, the conditions not short-circuited in order (a later lookup made when an earlier one
// failed), the wrong sound name or map, the null-pair test dropped, wrong PlayAttachedSample flags or init defaults, the hero-combat
// def or the on-hit sound looked up when not needed.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void check(bool v, int line) { if (!v) { printf("FAIL line %d\n", line); exit(1); } }
#define CHECK(c) check((c), __LINE__)
static int n_gettc, n_subdef, n_peektc, n_combatdef, n_onhit, n_pair, n_play;
static bool has_sound, has_def, has_exp, strike_flag, has_pair;
struct CCharString { const char* s; };
struct CSoundPair { int id; };
struct CSoundInit { float Volume, Pitch, Pan; long NoPlays, PlaybackType; unsigned long Gain, PitchBits; long OverrideID;
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), PlaybackType(1), Gain(0), PitchBits(0), OverrideID(-1) {} };
static CSoundPair the_pair;
struct CSoundMap {
    int tag;
    const CSoundPair* GetPSoundPair(const CCharString& n) const { ++n_pair; CHECK(tag == 77 && strcmp(n.s, "HIT_SOUND") == 0); return has_pair ? &the_pair : 0; }
};
struct CEntitySoundDef { CSoundMap SoundMap; };
static CEntitySoundDef the_def;
enum ENullDef { NULL_DEF };
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(ENullDef) : Object(0) {}
    T* operator->() const { CHECK(Object != 0); return Object; }
};
struct CHeroCombatDef { bool MeleeCombatPlayStrikeSounds; };
static CHeroCombatDef the_combat_def;
struct CTCSoundPlayer {
    const CSoundPair* played; bool a, b, c; CSoundInit init;
    unsigned long PlayAttachedSample(const CSoundPair& p, const CSoundInit& i, bool x, bool y, bool z) { ++n_play; played = &p; init = i; a = x; b = y; c = z; return 0; }
};
static CTCSoundPlayer the_player;
struct CTCHeroExperience { CCharString GetOnHitSound() const { ++n_onhit; CCharString s = { "HIT_SOUND" }; return s; } };
static CTCHeroExperience the_exp;
struct CThingBaseDef {
    bool GetSubDef(CDefPointer<const CEntitySoundDef>* out) const { ++n_subdef; if (!has_def) return false; out->Object = &the_def; return true; }
};
static CThingBaseDef the_base_def;
struct CThing {
    bool GetTC(CTCSoundPlayer** p) { ++n_gettc; if (!has_sound) return false; *p = &the_player; return true; }
    bool PeekTC(const CTCHeroExperience** p) const { ++n_peektc; if (!has_exp) return false; *p = &the_exp; return true; }
    const CThingBaseDef& PeekBaseDef() const { return the_base_def; }
};
static CThing the_thing;
struct CHeroCombatDefPtr { const CHeroCombatDef* operator->() const { return &the_combat_def; } };
class CTCCombat {
public:
    CThing& GetParentThing() { return the_thing; }
    CHeroCombatDefPtr GetPHeroCombatDef() const { ++n_combatdef; the_combat_def.MeleeCombatPlayStrikeSounds = strike_flag; return CHeroCombatDefPtr(); }
    void PlayCombatStrikeSounds();
};
// ---- row body (verbatim) ----
void CTCCombat::PlayCombatStrikeSounds()
{
    CThing& thing = GetParentThing();
    CTCSoundPlayer* ptc_sound;
    CDefPointer<const CEntitySoundDef> psound_def(NULL_DEF);
    const CTCHeroExperience* ptc_hero_exp = 0;
    if (thing.GetTC(&ptc_sound) && thing.PeekBaseDef().GetSubDef(&psound_def) && GetParentThing().PeekTC(&ptc_hero_exp) &&
        GetPHeroCombatDef()->MeleeCombatPlayStrikeSounds) {
        const CCharString& sound_symbol_name = ptc_hero_exp->GetOnHitSound();
        const CSoundPair* psound_pair = psound_def->SoundMap.GetPSoundPair(sound_symbol_name);
        if (psound_pair)
            ptc_sound->PlayAttachedSample(*psound_pair, CSoundInit(), true, false, false);
    }
}
// ---- end row body ----
int main()
{
    the_def.SoundMap.tag = 77;
    long cases = 0;
    for (int m = 0; m != 32; ++m) {
        has_sound = (m & 1) != 0; has_def = (m & 2) != 0; has_exp = (m & 4) != 0; strike_flag = (m & 8) != 0; has_pair = (m & 16) != 0;
        n_gettc = n_subdef = n_peektc = n_combatdef = n_onhit = n_pair = n_play = 0; the_player.played = 0;
        CTCCombat c; c.PlayCombatStrikeSounds();
        CHECK(n_gettc == 1);
        CHECK(n_subdef == (has_sound ? 1 : 0));
        CHECK(n_peektc == (has_sound && has_def ? 1 : 0));
        CHECK(n_combatdef == (has_sound && has_def && has_exp ? 1 : 0));
        bool go = has_sound && has_def && has_exp && strike_flag;
        CHECK(n_onhit == (go ? 1 : 0) && n_pair == (go ? 1 : 0));
        CHECK(n_play == (go && has_pair ? 1 : 0));
        if (n_play) {
            CHECK(the_player.played == &the_pair && the_player.a && !the_player.b && !the_player.c);
            CHECK(the_player.init.Volume == 1.0f && the_player.init.Pitch == 1.0f && the_player.init.Pan == 0.0f && the_player.init.NoPlays == 1);
            CHECK(the_player.init.PlaybackType == 1 && the_player.init.Gain == 0 && the_player.init.PitchBits == 0 && the_player.init.OverrideID == -1);
        }
        ++cases;
    }
    CHECK(cases == 32);
    puts("00733b70_TEST PASS");
    return 0;
}