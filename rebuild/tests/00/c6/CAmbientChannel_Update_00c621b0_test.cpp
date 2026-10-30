// Behaviour test for CAmbientChannel::Update (0x00C621B0): a silent source releases the channel's voice; with no voice a
// new one is played (volume, owner id, priority 1, layer registration, owner position) and its handle kept; with a voice
// the position and volume are pushed to the sound system.
#include <cstdio>

struct Vec { float X, Y, Z; };
struct Params { int loopCount; float volume; int priority; Vec position; int registered; };
struct Owner { int id; Vec position; };
struct Source { Owner* owner; float volume; int layerAdds; };
struct System {
    int played, released, moved, volumes; float lastVolume; Vec lastPos; int lastHandle;
    int Play(unsigned int sound, const Params& p) { ++played; lastVolume = p.volume; lastPos = p.position; return (int)sound + 100; }
    void Release(int h) { ++released; lastHandle = h; }
    void SetPosition(int h, const Vec& v) { ++moved; lastHandle = h; lastPos = v; }
    void SetVolume(int h, float v) { ++volumes; lastHandle = h; lastVolume = v; }
};
static System g_sys;

struct Channel {
    unsigned int soundId; int handle;
    void Update(Source* source)
    {
        float volume = source->volume;
        if (volume <= 0.0f) {
            if (handle >= 0) { g_sys.Release(handle); handle = -1; }
            return;
        }
        if (handle < 0) {
            Params p; p.volume = volume; p.loopCount = source->owner->id; p.priority = 1;
            ++source->layerAdds; p.registered = 1;
            p.position = source->owner->position;
            handle = g_sys.Play(soundId, p);
        } else {
            Vec position = source->owner->position;
            g_sys.SetPosition(handle, position);
            g_sys.SetVolume(handle, volume);
        }
    }
};

int main()
{
    int ok = 1;
    Owner owner; owner.id = 9; owner.position.X = 1.0f; owner.position.Y = 2.0f; owner.position.Z = 3.0f;
    Source src; src.owner = &owner; src.volume = 0.5f; src.layerAdds = 0;
    Channel ch; ch.soundId = 7; ch.handle = -1;
    ch.Update(&src);
    ok &= ch.handle == 107 && g_sys.played == 1 && g_sys.lastVolume == 0.5f && src.layerAdds == 1 && g_sys.lastPos.Z == 3.0f;
    owner.position.X = 4.0f; src.volume = 0.25f;
    ch.Update(&src);
    ok &= g_sys.played == 1 && g_sys.moved == 1 && g_sys.volumes == 1 && g_sys.lastVolume == 0.25f && g_sys.lastPos.X == 4.0f;
    src.volume = 0.0f;
    ch.Update(&src);
    ok &= g_sys.released == 1 && g_sys.lastHandle == 107 && ch.handle == -1;
    ch.Update(&src);
    ok &= g_sys.released == 1;
    if (!ok) { std::printf("00c621b0_TEST FAIL\n"); return 1; }
    std::printf("00c621b0_TEST PASS\n");
    return 0;
}