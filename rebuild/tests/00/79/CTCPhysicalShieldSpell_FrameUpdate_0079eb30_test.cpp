// Behaviour test for CTCPhysicalShieldSpell::FrameUpdate (0x0079EB30): unpaused frames decrement a positive duration and redraw the
// shield; at duration 0 the shield stops being pausable and, once the fade-out timer reaches 0, plays the end sound (player flag
// from the owner's type), the END effect, and removes component 0x93.
#include <cstdio>
#include <cstring>
static int g_log[16]; static int g_n = 0; static bool g_player = false; static long g_soundId = 0; static long g_removed = 0;
static const char* g_bone = 0; static const char* g_type = 0; static long g_effectFlags = 0; static int g_strings = 0;
class CCharString { public:
    const char* PStringData;
    CCharString(const char* text, long length) : PStringData(text) { ++g_strings; }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_strings; }
    ~CCharString() { --g_strings; }
};
struct CDefStringTable {
    CCharString GetString(long id) { g_soundId = id; return CCharString("SND", -1); }
};
CDefStringTable g_DefStringTable;
class CThing { public:
    unsigned char Type;
    void RemoveTC(long type) { g_log[g_n++] = 4; g_removed = type; }
};
class CThingCreatureBase : public CThing { public: };
class CSpecialAbilitiesPhysicalShieldDef { public: long EndSound; };
class CTCPhysicalShieldSpell { public:
    CThing* ParentThing;
    const CSpecialAbilitiesPhysicalShieldDef* PDef;
    long    CurShieldFadeOutTimer;
    long    DurationTimer;
    bool    Pausable;
    bool    Paused;
    unsigned long PlaySound(const CCharString& sound, bool player) { g_log[g_n++] = 2; g_player = player; return std::strcmp(sound.PStringData, "SND") == 0 ? 1 : 0; }
    void PlayEffect(long effect, const CCharString& boneName, const CCharString& type, CThing* target, long flags)
    { g_log[g_n++] = 3; g_bone = boneName.PStringData; g_type = type.PStringData; g_effectFlags = flags + effect + (target == ParentThing ? 0 : 100); }
    void PlayShieldEffects(CThingCreatureBase* creature) { g_log[g_n++] = creature == ParentThing ? 1 : 9; }
    void FrameUpdate();
};

void CTCPhysicalShieldSpell::FrameUpdate()
{
    CThing* owner = ParentThing;
    if (!Paused) {
        if (DurationTimer > 0)
            --DurationTimer;
        PlayShieldEffects((CThingCreatureBase*)owner);
    }
    if (DurationTimer == 0) {
        Pausable = false;
        if (--CurShieldFadeOutTimer <= 0) {
            {
                CCharString sound = g_DefStringTable.GetString(PDef->EndSound);
                PlaySound(sound, owner->Type == 2);
            }
            {
                CCharString type("", -1);
                CCharString boneName("END", -1);
                PlayEffect(0, boneName, type, owner, 8);
            }
            owner->RemoveTC(0x93);
        }
    }
}

int main()
{
    int bad = 0;
    CThing owner; owner.Type = 2;
    CSpecialAbilitiesPhysicalShieldDef def; def.EndSound = 77;
    CTCPhysicalShieldSpell s; s.ParentThing = &owner; s.PDef = &def; s.CurShieldFadeOutTimer = 2; s.DurationTimer = 2;
    s.Pausable = true; s.Paused = false;
    s.FrameUpdate();                                    // 2 -> 1, drawn
    if (s.DurationTimer != 1 || g_n != 1 || g_log[0] != 1 || !s.Pausable) bad |= 1;
    s.FrameUpdate();                                    // 1 -> 0, drawn, fade 2 -> 1
    if (s.DurationTimer != 0 || g_n != 2 || s.Pausable || s.CurShieldFadeOutTimer != 1) bad |= 2;
    s.Paused = true; s.Pausable = true;
    s.FrameUpdate();                                    // paused: no draw, fade 1 -> 0 -> end
    if (g_n != 5 || g_log[2] != 2 || g_log[3] != 3 || g_log[4] != 4 || s.Pausable) bad |= 4;
    if (g_soundId != 77 || !g_player || std::strcmp(g_bone, "END") != 0 || std::strcmp(g_type, "") != 0 || g_effectFlags != 8) bad |= 8;
    if (g_removed != 0x93 || g_strings != 0 || s.CurShieldFadeOutTimer != 0) bad |= 16;
    owner.Type = 1; s.Paused = false; s.DurationTimer = 0; s.CurShieldFadeOutTimer = 1;
    s.FrameUpdate();                                    // duration 0 stays 0; draw then end with a non-player owner
    if (g_n != 9 || g_log[5] != 1 || g_player) bad |= 32;
    s.DurationTimer = -1; s.CurShieldFadeOutTimer = 5;
    s.FrameUpdate();                                    // untimed shield: draw only
    if (g_n != 10 || s.DurationTimer != -1 || s.CurShieldFadeOutTimer != 5) bad |= 64;
    if (bad) { std::printf("0079eb30_TEST FAIL\n"); return 1; }
    std::printf("0079eb30_TEST PASS\n");
    return 0;
}