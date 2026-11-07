// Behaviour test for 0x0075E4C0 CTCCameraPointScriptedSpline::OnSerialseLeaf: the save arm writes every key camera under
// "KeyCameras[i].<Field>" with its position relative to the thing; the load arm rebuilds the keys from those fields
// (position not re-offset) and leaves SelectedKeyCamera at end(); TimeToPlay / Tension default to 1.0 / 0.8.
#include <cstdio>
#include <cstring>
#include <cmath>

struct Vec { float X, Y, Z; };
struct Key { Vec Pos, Look; float FOV, Roll, Shuttle, Duration, Pause, Speed; char Event[16]; };

struct Entry { char name[48]; float f[3]; char s[16]; };
struct Context {
    bool saving;
    Entry e[200];
    int n;
    Entry* find(const char* name) { for (int i = 0; i < n; ++i) if (!std::strcmp(e[i].name, name)) return &e[i]; return 0; }
    Entry* put(const char* name) { Entry* p = find(name); if (!p) { p = &e[n++]; std::strcpy(p->name, name); } return p; }
    void Transfer(const char* name, float& v, float def) {
        if (saving) put(name)->f[0] = v; else { Entry* p = find(name); v = p ? p->f[0] : def; }
    }
    void Transfer(const char* name, Vec& v) {
        if (saving) { Entry* p = put(name); p->f[0] = v.X; p->f[1] = v.Y; p->f[2] = v.Z; }
        else { Entry* p = find(name); if (p) { v.X = p->f[0]; v.Y = p->f[1]; v.Z = p->f[2]; } else { v.X = v.Y = v.Z = 0; } }
    }
    void Transfer(const char* name, char* s) {
        if (saving) std::strcpy(put(name)->s, s); else { Entry* p = find(name); std::strcpy(s, p ? p->s : ""); }
    }
};

struct Spline {
    Vec thing;
    Key keys[8];
    int count;
    int selected;
    float TimeToPlay, Tension;
    void Serialise(Context& c) {
        c.Transfer("TimeToPlay", TimeToPlay, 1.0f);
        c.Transfer("Tension", Tension, 0.8f);
        char prefix[32], name[48];
        if (c.saving) {
            float num = (float)count;
            c.Transfer("NumKeyCameras", num, 0.0f);
            for (int i = 0; i < count; ++i) {
                std::sprintf(prefix, "KeyCameras[%d].", i);
                Key& k = keys[i];
                Vec pos = { k.Pos.X - thing.X, k.Pos.Y - thing.Y, k.Pos.Z - thing.Z };
                std::sprintf(name, "%sPosition", prefix); c.Transfer(name, pos);
                std::sprintf(name, "%sLookDirection", prefix); c.Transfer(name, k.Look);
                std::sprintf(name, "%sFOV", prefix); c.Transfer(name, k.FOV, 0.0f);
                std::sprintf(name, "%sShuttleSpeed", prefix); c.Transfer(name, k.Shuttle, 0.0f);
                std::sprintf(name, "%sDuration", prefix); c.Transfer(name, k.Duration, 0.0f);
                std::sprintf(name, "%sPauseTime", prefix); c.Transfer(name, k.Pause, 0.0f);
                std::sprintf(name, "%sEvent", prefix); c.Transfer(name, k.Event);
                std::sprintf(name, "%sAnimationSpeed", prefix); c.Transfer(name, k.Speed, 0.0f);
                std::sprintf(name, "%sRollAngle", prefix); c.Transfer(name, k.Roll, 0.0f);
            }
        } else {
            float num = 0.0f;
            c.Transfer("NumKeyCameras", num, 0.0f);
            for (int i = 0; i < (int)num; ++i) {
                std::sprintf(prefix, "KeyCameras[%d].", i);
                Key k;
                std::sprintf(name, "%sPosition", prefix); c.Transfer(name, k.Pos);
                std::sprintf(name, "%sLookDirection", prefix); c.Transfer(name, k.Look);
                std::sprintf(name, "%sFOV", prefix); c.Transfer(name, k.FOV, 0.0f);
                std::sprintf(name, "%sShuttleSpeed", prefix); c.Transfer(name, k.Shuttle, 0.0f);
                std::sprintf(name, "%sDuration", prefix); c.Transfer(name, k.Duration, 0.0f);
                std::sprintf(name, "%sPauseTime", prefix); c.Transfer(name, k.Pause, 0.0f);
                std::sprintf(name, "%sEvent", prefix); c.Transfer(name, k.Event);
                std::sprintf(name, "%sAnimationSpeed", prefix); c.Transfer(name, k.Speed, 0.0f);
                std::sprintf(name, "%sRollAngle", prefix); c.Transfer(name, k.Roll, 0.0f);
                keys[count++] = k;
                selected = count;
            }
        }
    }
};

static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    static Context save; save.saving = true; save.n = 0;
    static Spline a; a.thing.X = 10; a.thing.Y = 20; a.thing.Z = 30; a.count = 2; a.TimeToPlay = 3.0f; a.Tension = 0.5f;
    for (int i = 0; i < 2; ++i) {
        Key& k = a.keys[i];
        k.Pos.X = 11.0f + i; k.Pos.Y = 22.0f; k.Pos.Z = 33.0f; k.Look.X = 0; k.Look.Y = 1; k.Look.Z = 0;
        k.FOV = 60.0f + i; k.Roll = 0.25f; k.Shuttle = 2.0f; k.Duration = 4.0f + i; k.Pause = 0.5f; k.Speed = 1.5f;
        std::sprintf(k.Event, "EV%d", i);
    }
    a.Serialise(save);
    Entry* p = save.find("KeyCameras[1].Position");
    if (!p || !near(p->f[0], 2.0f) || !near(p->f[1], 2.0f) || !near(p->f[2], 3.0f)) { std::printf("0075e4c0_TEST FAIL\n"); return 1; }
    static Context load; load = save; load.saving = false;
    static Spline b; b.count = 0; b.selected = -1;
    b.Serialise(load);
    if (b.count != 2 || b.selected != 2 || !near(b.TimeToPlay, 3.0f) || !near(b.Tension, 0.5f)) { std::printf("0075e4c0_TEST FAIL\n"); return 1; }
    if (!near(b.keys[1].Pos.X, 2.0f) || !near(b.keys[1].FOV, 61.0f) || !near(b.keys[0].Roll, 0.25f) || std::strcmp(b.keys[1].Event, "EV1")) { std::printf("0075e4c0_TEST FAIL\n"); return 1; }
    static Context empty; empty.saving = false; empty.n = 0;
    static Spline c; c.count = 0;
    c.Serialise(empty);
    if (!near(c.TimeToPlay, 1.0f) || !near(c.Tension, 0.8f) || c.count != 0) { std::printf("0075e4c0_TEST FAIL\n"); return 1; }
    std::printf("0075e4c0_TEST PASS\n");
    return 0;
}