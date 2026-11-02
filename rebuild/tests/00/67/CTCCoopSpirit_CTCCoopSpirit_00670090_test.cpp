// Behaviour test for 0x00670090 ~CTCCoopSpirit: kill the particle emitter when it is alive, clear Master, then PParticleEmitter,
// PCoopSpiritDef and ~CTCBase.
#include <cstdio>
#include <cstring>
static char g_log[32];
static void Log(const char* s) { std::strcat(g_log, s); }
class CThing { public: bool Killed; bool Immediately; CThing() : Killed(false), Immediately(false) { } void Kill(bool now) { Killed = true; Immediately = now; Log("K"); } };
class CTCBase { public: virtual ~CTCBase() { Log("B"); } };
class CIntelligentPointer { public: CIntelligentPointer() : P(0) { } virtual ~CIntelligentPointer() { Log("P"); } CThing* Get() const { return P; } CThing* operator->() const { return P; } CThing* P; };
class CPlayer { public: };
class CTCCoopSpirit : public CTCBase {
public:
    CTCCoopSpirit() : Master(0) { }
    virtual ~CTCCoopSpirit()
    {
        if (PParticleEmitter.Get())
            PParticleEmitter->Kill(true);
        Master = 0;
        if (Master == 0) Log("M");
    }
    long NoMovementTimer, Score, OffscreenCounter;
    float MovementVector[3];
    CPlayer* Master;
    const void* PCoopSpiritDef;
    CIntelligentPointer PParticleEmitter;
};
int main()
{
    CThing emitter;
    CPlayer player;
    g_log[0] = 0;
    CTCCoopSpirit* spirit = new CTCCoopSpirit;
    spirit->Master = &player;
    spirit->PParticleEmitter.P = &emitter;
    delete (CTCBase*)spirit;
    bool ok = std::strcmp(g_log, "KMPB") == 0 && emitter.Killed && emitter.Immediately;
    g_log[0] = 0;
    spirit = new CTCCoopSpirit;
    delete (CTCBase*)spirit;
    ok = ok && std::strcmp(g_log, "MPB") == 0;
    std::printf(ok ? "00670090_TEST PASS\n" : "00670090_TEST FAIL\n");
    return ok ? 0 : 1;
}