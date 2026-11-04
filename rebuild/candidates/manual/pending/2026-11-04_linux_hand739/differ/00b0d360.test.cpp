// Behaviour test for NParticleEngine::CPSCUpdateNormal::DatabaseSerialise (0x00B0D360): a standalone model -- saving writes the two
// parameter keys as numbers; loading reads them as names and stores their CRCs (empty name = CRC of ""), every bit-field round-trips
// through a bool temporary with its default, the 10-bit fade integers through longs (FadeOutBeginInteger default 1000).
#include <cstdio>
#include <map>
#include <string>
static unsigned long Crc(const std::string& s) { unsigned long h = 0; for (size_t i = 0; i < s.size(); ++i) h = h * 31 + (unsigned char)s[i]; return h; }
struct Context {
    bool saving; std::map<std::string, std::string> values;
    template <class T> void Transfer(const char* name, T& v, const T& def);
};
template <> void Context::Transfer<bool>(const char* n, bool& v, const bool& d) { if (saving) values[n] = v ? "1" : "0"; else v = values.count(n) ? values[n] == "1" : d; }
template <> void Context::Transfer<long>(const char* n, long& v, const long& d) { if (saving) values[n] = std::to_string(v); else v = values.count(n) ? std::stol(values[n]) : d; }
template <> void Context::Transfer<unsigned long>(const char* n, unsigned long& v, const unsigned long& d) { if (saving) values[n] = std::to_string(v); else v = values.count(n) ? std::stoul(values[n]) : d; }
template <> void Context::Transfer<std::string>(const char* n, std::string& v, const std::string& d) { if (saving) values[n] = v; else v = values.count(n) ? values[n] : d; }
struct Update {
    unsigned int FadeOutBeginInteger : 10; unsigned int StayWithEmitter : 1; bool ParticleCreateDecal : 1; unsigned long AccelerationParam; std::string DecalEmitterName;
    void Serialise(Context& c) {
        if (c.saving) c.Transfer("AccelerationParam", AccelerationParam, 0UL);
        else { std::string name; c.Transfer("AccelerationParam", name, std::string()); AccelerationParam = Crc(name); }
        { bool v = StayWithEmitter; c.Transfer("StayWithEmitter", v, false); StayWithEmitter = v; }
        { long v = FadeOutBeginInteger; c.Transfer("FadeOutBeginInteger", v, 1000L); FadeOutBeginInteger = v; }
        { bool v = ParticleCreateDecal; c.Transfer("ParticleCreateDecal", v, false); ParticleCreateDecal = v; }
        c.Transfer("DecalEmitterName", DecalEmitterName, std::string());
    }
};
int main()
{
    Context load; load.saving = false; load.values["AccelerationParam"] = "Speed"; load.values["StayWithEmitter"] = "1"; load.values["DecalEmitterName"] = "Scorch";
    Update u; u.FadeOutBeginInteger = 5; u.StayWithEmitter = 0; u.ParticleCreateDecal = true; u.AccelerationParam = 0;
    u.Serialise(load);
    bool ok = u.AccelerationParam == Crc("Speed") && u.StayWithEmitter == 1 && u.FadeOutBeginInteger == 1000 && !u.ParticleCreateDecal && u.DecalEmitterName == "Scorch";
    Context save; save.saving = true; u.Serialise(save);
    ok = ok && save.values["AccelerationParam"] == std::to_string(Crc("Speed")) && save.values["StayWithEmitter"] == "1" && save.values["FadeOutBeginInteger"] == "1000";
    printf(ok ? "00b0d360_TEST PASS\n" : "00b0d360_TEST FAIL\n");
    return ok ? 0 : 1;
}
