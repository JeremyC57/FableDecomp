#include <cstdio>
#include <cstring>
// behaviour test: saving writes nothing for a dependent emitter, otherwise the flag and the type's name (empty for an unknown id);
// loading reads the flag and the name, maps it back to the type id and kills the thing when the id is 0 (not while compiling
// front-end definitions)
class CPersistContext { public:
    long Mode; bool savedFlag; char savedName[32]; int writes;
    bool loadFlag; const char* loadName;
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char*, bool& value, const bool& def) { if (IsSaving()) { savedFlag = value; ++writes; } else value = loadFlag ? true : def; }
    void TransferName(const char*, char* value) { if (IsSaving()) { std::strcpy(savedName, value); ++writes; } else std::strcpy(value, loadName); }
};
static const char* g_names[3] = { "", "SPARKS", "SMOKE" };
static long LookupName(const char* name) { for (long i = 1; i < 3; ++i) if (std::strcmp(g_names[i], name) == 0) return i; return 0; }
static bool g_compileFrontend = false;
class CTCDParticleEmitter { public:
    long ParticleTypeID; bool IndependantObject; bool killed;
    void OnSerialise(CPersistContext& context)
    {
        if (context.IsSaving()) {
            if (!IndependantObject)
                return;
            bool independant = IndependantObject;
            context.Transfer("IndependantObject", independant, false);
            char name[32] = "";
            if (ParticleTypeID > 0 && ParticleTypeID < 3)
                std::strcpy(name, g_names[ParticleTypeID]);
            context.TransferName("ParticleTypeName", name);
        } else {
            bool independant = false;
            context.Transfer("IndependantObject", independant, false);
            IndependantObject = independant;
            char name[32] = "";
            context.TransferName("ParticleTypeName", name);
            if (!g_compileFrontend) {
                ParticleTypeID = LookupName(name);
                if (ParticleTypeID == 0)
                    killed = true;
            }
        }
    }
};
int main()
{
    CPersistContext save = { 1, false, "", 0, false, 0 };
    CTCDParticleEmitter e = { 2, false, false };
    e.OnSerialise(save);
    bool ok = save.writes == 0;
    e.IndependantObject = true; e.OnSerialise(save);
    ok = ok && save.writes == 2 && save.savedFlag && std::strcmp(save.savedName, "SMOKE") == 0;
    CPersistContext load = { 0, false, "", 0, true, "SPARKS" };
    CTCDParticleEmitter f = { 0, false, false };
    f.OnSerialise(load);
    ok = ok && f.IndependantObject && f.ParticleTypeID == 1 && !f.killed;
    load.loadName = "GONE"; load.loadFlag = false; f.OnSerialise(load);
    ok = ok && !f.IndependantObject && f.ParticleTypeID == 0 && f.killed;
    g_compileFrontend = true; f.killed = false; f.ParticleTypeID = 7; f.OnSerialise(load);
    ok = ok && f.ParticleTypeID == 7 && !f.killed;
    if (!ok) { std::printf("006e0730_TEST FAIL\n"); return 1; }
    std::printf("006e0730_TEST PASS\n");
    return 0;
}