#include <cstdio>
#include <cstring>
struct Ctx { long Mode; int floats, strings; bool baseOk;
    void Transfer(const char*, float* v, const float& d) { ++floats; if (Mode == 0) *v = d; }
    void Transfer(const char*, int*) { ++strings; } };
struct Switch { float TriggerRadius; int EnvironmentDef; float Time; bool Triggered; int Name;
    bool Serialise(Ctx* c) {
        if (c->baseOk) {
            if (c->Mode != 1 && c->Mode != 3) Triggered = false;
            c->Transfer("TriggerRadius", &TriggerRadius, 0.0f);
            c->Transfer("TimeToChangeEnvironmentDef", &Time, 0.0f);
            c->Transfer("TriggerableObjectScriptName", &Name);
            c->Transfer("EnvironmentDef", &EnvironmentDef);
            return true;
        }
        return false; } };
int main()
{
    Switch s = { 5, 0, 2, true, 0 };
    Ctx c = { 1, 0, 0, true };
    bool ok = s.Serialise(&c) && s.Triggered && c.floats == 2 && c.strings == 2 && s.TriggerRadius == 5;
    c.Mode = 0; ok = ok && s.Serialise(&c) && !s.Triggered && s.TriggerRadius == 0;
    c.baseOk = false; c.floats = 0; ok = ok && !s.Serialise(&c) && c.floats == 0;
    if (!ok) { std::printf("0083c230_TEST FAIL\n"); return 1; }
    std::printf("0083c230_TEST PASS\n");
    return 0;
}