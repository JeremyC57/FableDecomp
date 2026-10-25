// Behaviour test for 0x008B7CF0 CCreatureAction_UnsheatheWeapons::ShouldPlayUnsheatheWeaponAction(const CThingCreatureBase&,
// EUnsheatheWeaponType, bool&, bool&)
// The model mirrors the source: types other than 0/1/3 need UnsheatheWeaponsAutomatically (the def reference is taken and
// released); the wanted weapon def is the default one for 2/3, melee for 4, ranged for 5 and none otherwise; a negative def
// or no carrying component refuses; with no wanted weapon a live thing in hand is sheathed (STANDARD_SHEATHE plays and
// sheathes first when the creature has it, else STANDARD_UNSHEATHE's existence is reported); with a wanted weapon the
// action is refused only when that weapon is already live in hand, otherwise STANDARD_UNSHEATHE's existence is reported.
#include <cstdio>
#include <cstring>

static int g_def_refs = 0;

struct Thing { bool dead; unsigned short def_index; };
struct Creature {
    bool unsheathe_automatically;
    bool use_melee_as_default;
    long melee_def, ranged_def;
    bool has_carrying;
    const Thing* in_hand;
    bool has_sheathe_anim, has_unsheathe_anim;
};

static bool AnimExists(const Creature& c, const char* name)
{
    if (std::strcmp(name, "STANDARD_SHEATHE") == 0) return c.has_sheathe_anim;
    if (std::strcmp(name, "STANDARD_UNSHEATHE") == 0) return c.has_unsheathe_anim;
    return false;
}

static bool ShouldPlay(const Creature& c, int type, bool& play_animation, bool& sheathe_first)
{
    if (type != 0 && type != 1 && type != 3) {
        ++g_def_refs;                       // GetPBaseDef() temporary
        bool automatic = c.unsheathe_automatically;
        --g_def_refs;
        if (!automatic) return false;
    }
    long weapon_def;
    switch (type) {
    case 2: case 3: weapon_def = c.use_melee_as_default ? c.melee_def : c.ranged_def; break;
    case 5: weapon_def = c.ranged_def; break;
    case 4: weapon_def = c.melee_def; break;
    default: weapon_def = 0; break;
    }
    if (weapon_def < 0 || !c.has_carrying) return false;
    const Thing* in_hand = c.in_hand;
    if (weapon_def <= 0) {
        if (in_hand == 0 || in_hand->dead) return false;
        if (AnimExists(c, "STANDARD_SHEATHE")) { play_animation = true; sheathe_first = true; return true; }
        play_animation = AnimExists(c, "STANDARD_UNSHEATHE");
        sheathe_first = false;
        return true;
    }
    if (in_hand != 0 && !in_hand->dead && in_hand->def_index == weapon_def) return false;
    play_animation = AnimExists(c, "STANDARD_UNSHEATHE");
    sheathe_first = false;
    return true;
}

int main()
{
    int fail = 0;
    const Thing sword = { false, 7 };
    const Thing bow = { false, 9 };
    const Thing dead_sword = { true, 7 };
    const Creature base = { true, true, 7, 9, true, 0, true, true };
    Creature c = base;
    bool play = false, first = true;

    if (!ShouldPlay(c, 4, play, first) || !play || first) fail |= 1;         // melee wanted, nothing in hand
    c.in_hand = &sword;
    play = true; first = true;
    if (ShouldPlay(c, 4, play, first)) fail |= 2;                             // melee already in hand
    if (!ShouldPlay(c, 5, play, first) || !play || first) fail |= 4;         // ranged wanted, sword in hand
    c.in_hand = &bow;
    if (ShouldPlay(c, 5, play, first)) fail |= 8;
    if (ShouldPlay(c, 2, play, first) != true) fail |= 16;                    // default = melee (7) vs bow in hand
    c.use_melee_as_default = false;
    if (ShouldPlay(c, 3, play, first)) fail |= 32;                            // default = ranged, bow in hand
    c = base; c.in_hand = &sword;
    play = false; first = false;
    if (!ShouldPlay(c, 0, play, first) || !play || !first) fail |= 64;       // sheathe with the animation
    c.has_sheathe_anim = false;
    play = true; first = true;
    if (!ShouldPlay(c, 1, play, first) || !play || first) fail |= 128;       // falls back to the unsheathe report
    c.has_unsheathe_anim = false;
    if (!ShouldPlay(c, 0, play, first) || play || first) fail |= 256;
    c.in_hand = &dead_sword;
    if (ShouldPlay(c, 0, play, first)) fail |= 512;                           // nothing live to sheathe
    c = base; c.unsheathe_automatically = false;
    if (ShouldPlay(c, 4, play, first)) fail |= 1024;                          // automatic unsheathing off
    if (!ShouldPlay(c, 3, play, first)) fail |= 2048;                         // type 3 skips that check
    c = base; c.has_carrying = false;
    if (ShouldPlay(c, 4, play, first)) fail |= 4096;
    c = base; c.melee_def = -1;
    if (ShouldPlay(c, 4, play, first)) fail |= 8192;
    c = base; c.in_hand = &dead_sword;
    if (!ShouldPlay(c, 4, play, first)) fail |= 16384;                        // dead sword does not count
    if (ShouldPlay(c, 7, play, first)) fail |= 32768;                         // out of range: no weapon, nothing live
    if (g_def_refs != 0) fail |= 65536;
    if (fail) { std::printf("008b7cf0_TEST FAIL %d\n", fail); return 1; }
    std::printf("008b7cf0_TEST PASS\n");
    return 0;
}