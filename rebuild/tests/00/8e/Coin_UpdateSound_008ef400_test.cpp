// Behaviour test (0x008ef400): Coin::UpdateSound stops the slide sound when the coin is at rest, otherwise starts "TAVERN_GAME_COIN_SLIDE" if none is playing and sets pitch 1 and volume (muted ? 0 : 1); a coin without a thing does nothing
#include <cstdio>
#include <cstring>
#include <cmath>
static const unsigned int g_InvalidSoundHandle = 0xFFFFFFFFu;
struct CTCSoundPlayer {
    int Stops; int Plays; unsigned int NextHandle; char LastName[64]; float Pitch; float Volume; unsigned int LastSound;
    void StopAttachedSound(unsigned int) { ++Stops; }
    unsigned int PlaySound(const char* name, bool, int, int, int) { ++Plays; std::strcpy(LastName, name); return NextHandle; }
    void SetSpecificSamplePitch(unsigned int s, float p) { LastSound = s; Pitch = p; }
    void SetSpecificSampleVolume(unsigned int s, float v) { LastSound = s; Volume = v; }
};
struct CThing { CTCSoundPlayer* Player; };
struct C2DVector { float X, Y; float GetLengthSquared() const { return X * X + Y * Y; } };
struct Coin {
    C2DVector Velocity; CThing* Thing; unsigned int Sound;
    void UpdateSound(bool muted)
    {
        if (Thing == 0) return;
        CTCSoundPlayer* player = Thing->Player;
        if (Velocity.GetLengthSquared() == 0.0f) {
            if (Sound != g_InvalidSoundHandle) { player->StopAttachedSound(Sound); Sound = g_InvalidSoundHandle; }
            return;
        }
        if (Sound == g_InvalidSoundHandle) Sound = player->PlaySound("TAVERN_GAME_COIN_SLIDE", false, 0, 0, -1);
        player->SetSpecificSamplePitch(Sound, 1.0f);
        player->SetSpecificSampleVolume(Sound, muted ? 0.0f : 1.0f);
    }
};
int main()
{
    CTCSoundPlayer p; p.Stops = 0; p.Plays = 0; p.NextHandle = 17; p.LastName[0] = 0; p.Pitch = 0.0f; p.Volume = -1.0f; p.LastSound = 0;
    CThing t; t.Player = &p;
    Coin c; c.Velocity.X = 0.0f; c.Velocity.Y = 0.0f; c.Thing = 0; c.Sound = g_InvalidSoundHandle;
    c.UpdateSound(false);
    if (p.Plays != 0 || p.Stops != 0) { std::printf("008ef400_TEST FAIL\n"); return 1; }
    c.Thing = &t; c.Velocity.X = 0.5f;
    c.UpdateSound(true);
    if (p.Plays != 1 || std::strcmp(p.LastName, "TAVERN_GAME_COIN_SLIDE") != 0 || c.Sound != 17 || std::fabs(p.Pitch - 1.0f) > 1e-6f || std::fabs(p.Volume) > 1e-6f) { std::printf("008ef400_TEST FAIL\n"); return 1; }
    c.UpdateSound(false);
    if (p.Plays != 1 || std::fabs(p.Volume - 1.0f) > 1e-6f || p.LastSound != 17) { std::printf("008ef400_TEST FAIL\n"); return 1; }
    c.Velocity.X = 0.0f;
    c.UpdateSound(false);
    if (p.Stops != 1 || c.Sound != g_InvalidSoundHandle) { std::printf("008ef400_TEST FAIL\n"); return 1; }
    c.UpdateSound(false);
    if (p.Stops != 1) { std::printf("008ef400_TEST FAIL\n"); return 1; }
    std::printf("008ef400_TEST PASS\n");
    return 0;
}