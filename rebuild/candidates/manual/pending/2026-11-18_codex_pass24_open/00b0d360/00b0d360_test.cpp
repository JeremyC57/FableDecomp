// Standalone C++03 fixture for particle update persistence and transfer defaults.
#include <cstdio>
#include <map>
#include <string>
#include <sstream>

class CCharString {
public:
    std::string text;
    unsigned long GetCRC() const {
        unsigned long h = 0;
        for (std::string::size_type i = 0; i < text.size(); ++i)
            h = h * 31 + (unsigned char)text[i];
        return h;
    }
};
static CCharString EmptyCCharString() { return CCharString(); }

struct C3DVector {
    float x, y, z;
    C3DVector(float px, float py, float pz) : x(px), y(py), z(pz) {}
    C3DVector() : x(0), y(0), z(0) {}
    bool operator==(const C3DVector& v) const { return x == v.x && y == v.y && z == v.z; }
};
static std::istream& operator>>(std::istream& stream, C3DVector& v) { return stream >> v.x >> v.y >> v.z; }
static std::ostream& operator<<(std::ostream& stream, const C3DVector& v) { return stream << v.x << ' ' << v.y << ' ' << v.z; }

struct CPersistContext {
    bool saving;
    std::map<std::string, std::string> records;
    explicit CPersistContext(bool mode) : saving(mode) {}
    bool IsSaving() const { return saving; }
    template<class T> void Transfer(const char* name, T& value, const T& fallback) {
        if (saving) {
            std::ostringstream out;
            out << value;
            records[name] = out.str();
        } else {
            std::map<std::string, std::string>::const_iterator entry = records.find(name);
            if (entry == records.end()) {
                value = fallback;
            } else {
                std::istringstream in(entry->second);
                in >> value;
            }
        }
    }
    void Transfer(const char* name, CCharString& value, const CCharString& fallback) {
        if (saving) records[name] = value.text;
        else if (records.count(name)) value.text = records[name];
        else value = fallback;
    }
};

namespace NParticleEngine {
struct CEmitterDatabaseSerialiseInfo {};

struct CPSCUpdateNormal {
    bool UseParticleLifeSecs : 1;
    bool UseSystemLifeSecs : 1;
    bool UseAccelerationParam : 1;
    bool UseAllAttractors : 1;
    bool UseRandomInitialRotation : 1;
    bool UseRandomRotationAxis : 1;
    bool ParticleCollideWithGround : 1;
    bool ParticleCollideWithAnything : 1;
    bool ParticleDieOnCollision : 1;
    bool SystemAlphaFadeEnable : 1;
    bool EmissionFadeEnable : 1;
    bool RandomisePosEnable : 1;
    bool RandomisePosDistVaryEnable : 1;
    bool StayWithEmitter : 1;
    bool SetOrientationFromDirection : 1;
    bool SetOrientationFromGame : 1;
    bool ParticleCreateDecal : 1;
    bool ParticleCreateDecalEmitter : 1;
    float ParticleLifeSecs;
    float SystemLifeSecs;
    float WindFactor;
    float GravityFactor;
    float AirResistance;
    float ParticleAccelerationScale;
    float InitialRotationX;
    float InitialRotationY;
    float InitialRotationZ;
    float RotationMinAngleSpeed;
    float RotationMaxAngleSpeed;
    float ParticleBounce;
    float SystemAlphaFadeMinimum;
    float EmissionFadeMinimum;
    float RandomisePosDistVaryScale;
    C3DVector ParticleSystemOffset;
    C3DVector ParticleAcceleration;
    C3DVector RotationAxis;
    C3DVector RandomisePosSpeed;
    C3DVector RandomisePosScale;
    unsigned int FadeInEndInteger : 10;
    unsigned int FadeOutBeginInteger : 10;
    unsigned long AccelerationParam, OrientFromGameParam;
    CCharString DecalEmitterName;
    void DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info);
};
}

// Recovered method body, with game dependencies supplied by the fixture.
namespace NParticleEngine {
#define TRANSFER_BOOL_BIT(name, member) { bool temp = member; context.Transfer(name, temp, false); member = temp; }
void CPSCUpdateNormal::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info)
{
    if (context.IsSaving()) {
        context.Transfer("AccelerationParam", AccelerationParam, 0UL);
        context.Transfer("OrientFromGameParam", OrientFromGameParam, 0UL);
    } else {
        CCharString temp;
        context.Transfer("AccelerationParam", temp, EmptyCCharString());
        AccelerationParam = temp.GetCRC();
        context.Transfer("OrientFromGameParam", temp, EmptyCCharString());
        OrientFromGameParam = temp.GetCRC();
    }
    TRANSFER_BOOL_BIT("UseParticleLifeSecs", UseParticleLifeSecs);
    context.Transfer("ParticleLifeSecs", ParticleLifeSecs, 0.0f);
    TRANSFER_BOOL_BIT("UseSystemLifeSecs", UseSystemLifeSecs);
    context.Transfer("SystemLifeSecs", SystemLifeSecs, 0.0f);
    context.Transfer("ParticleSystemOffset", ParticleSystemOffset, C3DVector(0.0f, 0.0f, 0.0f));
    context.Transfer("WindFactor", WindFactor, 0.0f);
    context.Transfer("GravityFactor", GravityFactor, 0.0f);
    context.Transfer("AirResistance", AirResistance, 0.0f);
    context.Transfer("ParticleAcceleration", ParticleAcceleration, C3DVector(0.0f, 0.0f, 0.0f));
    context.Transfer("ParticleAccelerationScale", ParticleAccelerationScale, 1.0f);
    TRANSFER_BOOL_BIT("UseAccelerationParam", UseAccelerationParam);
    TRANSFER_BOOL_BIT("UseAllAttractors", UseAllAttractors);
    TRANSFER_BOOL_BIT("UseRandomInitialRotation", UseRandomInitialRotation);
    context.Transfer("InitialRotationX", InitialRotationX, 0.0f);
    context.Transfer("InitialRotationY", InitialRotationY, 0.0f);
    context.Transfer("InitialRotationZ", InitialRotationZ, 0.0f);
    TRANSFER_BOOL_BIT("UseRandomRotationAxis", UseRandomRotationAxis);
    context.Transfer("RotationAxis", RotationAxis, C3DVector(0.0f, 0.0f, 0.0f));
    context.Transfer("RotationMinAngleSpeed", RotationMinAngleSpeed, 0.0f);
    context.Transfer("RotationMaxAngleSpeed", RotationMaxAngleSpeed, 0.0f);
    TRANSFER_BOOL_BIT("ParticleCollideWithGround", ParticleCollideWithGround);
    TRANSFER_BOOL_BIT("ParticleCollideWithAnything", ParticleCollideWithAnything);
    context.Transfer("ParticleBounce", ParticleBounce, 0.0f);
    TRANSFER_BOOL_BIT("ParticleDieOnCollision", ParticleDieOnCollision);
    { long temp = FadeInEndInteger; context.Transfer("FadeInEndInteger", temp, 0L); FadeInEndInteger = temp; }
    { long temp = FadeOutBeginInteger; context.Transfer("FadeOutBeginInteger", temp, 1000L); FadeOutBeginInteger = temp; }
    TRANSFER_BOOL_BIT("SystemAlphaFadeEnable", SystemAlphaFadeEnable);
    context.Transfer("SystemAlphaFadeMinimum", SystemAlphaFadeMinimum, 0.0f);
    TRANSFER_BOOL_BIT("EmissionFadeEnable", EmissionFadeEnable);
    context.Transfer("EmissionFadeMinimum", EmissionFadeMinimum, 0.0f);
    TRANSFER_BOOL_BIT("RandomisePosEnable", RandomisePosEnable);
    context.Transfer("RandomisePosSpeed", RandomisePosSpeed, C3DVector(0.0f, 0.0f, 0.0f));
    context.Transfer("RandomisePosScale", RandomisePosScale, C3DVector(0.0f, 0.0f, 0.0f));
    {
        bool temp = RandomisePosDistVaryEnable;
        context.Transfer("RandomisePosDistVaryEnable", temp, false);
        RandomisePosDistVaryEnable = temp;
        const float defaultScale = 0.0f;
        context.Transfer("RandomisePosDistVaryScale", RandomisePosDistVaryScale, defaultScale);
    }
    TRANSFER_BOOL_BIT("StayWithEmitter", StayWithEmitter);
    TRANSFER_BOOL_BIT("SetOrientationFromDirection", SetOrientationFromDirection);
    TRANSFER_BOOL_BIT("SetOrientationFromGame", SetOrientationFromGame);
    TRANSFER_BOOL_BIT("ParticleCreateDecal", ParticleCreateDecal);
    TRANSFER_BOOL_BIT("ParticleCreateDecalEmitter", ParticleCreateDecalEmitter);
    context.Transfer("DecalEmitterName", DecalEmitterName, EmptyCCharString());
}
}

using namespace NParticleEngine;

int main() {
    const CEmitterDatabaseSerialiseInfo info;
    CPSCUpdateNormal value = CPSCUpdateNormal();
    CPersistContext load(false), defaults(false), save(true);
    load.records["AccelerationParam"] = "wind";
    load.records["OrientFromGameParam"] = "roll";
    load.records["DecalEmitterName"] = "impact";
    load.records["FadeInEndInteger"] = "11";
    load.records["FadeOutBeginInteger"] = "732";
    load.records["UseParticleLifeSecs"] = "1";
    load.records["UseSystemLifeSecs"] = "1";
    load.records["UseAccelerationParam"] = "1";
    load.records["UseAllAttractors"] = "1";
    load.records["UseRandomInitialRotation"] = "1";
    load.records["UseRandomRotationAxis"] = "1";
    load.records["ParticleCollideWithGround"] = "1";
    load.records["ParticleCollideWithAnything"] = "1";
    load.records["ParticleDieOnCollision"] = "1";
    load.records["SystemAlphaFadeEnable"] = "1";
    load.records["EmissionFadeEnable"] = "1";
    load.records["RandomisePosEnable"] = "1";
    load.records["RandomisePosDistVaryEnable"] = "1";
    load.records["StayWithEmitter"] = "1";
    load.records["SetOrientationFromDirection"] = "1";
    load.records["SetOrientationFromGame"] = "1";
    load.records["ParticleCreateDecal"] = "1";
    load.records["ParticleCreateDecalEmitter"] = "1";
    load.records["ParticleLifeSecs"] = "2.25";
    load.records["SystemLifeSecs"] = "2.25";
    load.records["WindFactor"] = "2.25";
    load.records["GravityFactor"] = "2.25";
    load.records["AirResistance"] = "2.25";
    load.records["ParticleAccelerationScale"] = "2.25";
    load.records["InitialRotationX"] = "2.25";
    load.records["InitialRotationY"] = "2.25";
    load.records["InitialRotationZ"] = "2.25";
    load.records["RotationMinAngleSpeed"] = "2.25";
    load.records["RotationMaxAngleSpeed"] = "2.25";
    load.records["ParticleBounce"] = "2.25";
    load.records["SystemAlphaFadeMinimum"] = "2.25";
    load.records["EmissionFadeMinimum"] = "2.25";
    load.records["RandomisePosDistVaryScale"] = "2.25";
    load.records["ParticleSystemOffset"] = "3 2 1";
    load.records["ParticleAcceleration"] = "3 2 1";
    load.records["RotationAxis"] = "3 2 1";
    load.records["RandomisePosSpeed"] = "3 2 1";
    load.records["RandomisePosScale"] = "3 2 1";
    value.DatabaseSerialise(load, info);
    CCharString wind, roll;
    wind.text = "wind";
    roll.text = "roll";
    bool ok = value.AccelerationParam == wind.GetCRC() && value.OrientFromGameParam == roll.GetCRC()
        && value.DecalEmitterName.text == "impact" && value.FadeInEndInteger == 11 && value.FadeOutBeginInteger == 732;
    value.DatabaseSerialise(save, info);
    std::ostringstream wind_crc, roll_crc;
    wind_crc << wind.GetCRC();
    roll_crc << roll.GetCRC();
    ok = ok && save.records["AccelerationParam"] == wind_crc.str() && save.records["OrientFromGameParam"] == roll_crc.str()
        && save.records.size() == load.records.size() && save.records["DecalEmitterName"] == "impact";
    ok = ok && save.records["UseParticleLifeSecs"] == "1";
    ok = ok && save.records["UseSystemLifeSecs"] == "1";
    ok = ok && save.records["UseAccelerationParam"] == "1";
    ok = ok && save.records["UseAllAttractors"] == "1";
    ok = ok && save.records["UseRandomInitialRotation"] == "1";
    ok = ok && save.records["UseRandomRotationAxis"] == "1";
    ok = ok && save.records["ParticleCollideWithGround"] == "1";
    ok = ok && save.records["ParticleCollideWithAnything"] == "1";
    ok = ok && save.records["ParticleDieOnCollision"] == "1";
    ok = ok && save.records["SystemAlphaFadeEnable"] == "1";
    ok = ok && save.records["EmissionFadeEnable"] == "1";
    ok = ok && save.records["RandomisePosEnable"] == "1";
    ok = ok && save.records["RandomisePosDistVaryEnable"] == "1";
    ok = ok && save.records["StayWithEmitter"] == "1";
    ok = ok && save.records["SetOrientationFromDirection"] == "1";
    ok = ok && save.records["SetOrientationFromGame"] == "1";
    ok = ok && save.records["ParticleCreateDecal"] == "1";
    ok = ok && save.records["ParticleCreateDecalEmitter"] == "1";
    ok = ok && save.records["ParticleLifeSecs"] == "2.25";
    ok = ok && save.records["SystemLifeSecs"] == "2.25";
    ok = ok && save.records["WindFactor"] == "2.25";
    ok = ok && save.records["GravityFactor"] == "2.25";
    ok = ok && save.records["AirResistance"] == "2.25";
    ok = ok && save.records["ParticleAccelerationScale"] == "2.25";
    ok = ok && save.records["InitialRotationX"] == "2.25";
    ok = ok && save.records["InitialRotationY"] == "2.25";
    ok = ok && save.records["InitialRotationZ"] == "2.25";
    ok = ok && save.records["RotationMinAngleSpeed"] == "2.25";
    ok = ok && save.records["RotationMaxAngleSpeed"] == "2.25";
    ok = ok && save.records["ParticleBounce"] == "2.25";
    ok = ok && save.records["SystemAlphaFadeMinimum"] == "2.25";
    ok = ok && save.records["EmissionFadeMinimum"] == "2.25";
    ok = ok && save.records["RandomisePosDistVaryScale"] == "2.25";
    ok = ok && save.records["ParticleSystemOffset"] == "3 2 1";
    ok = ok && save.records["ParticleAcceleration"] == "3 2 1";
    ok = ok && save.records["RotationAxis"] == "3 2 1";
    ok = ok && save.records["RandomisePosSpeed"] == "3 2 1";
    ok = ok && save.records["RandomisePosScale"] == "3 2 1";
    value.DatabaseSerialise(defaults, info);
    ok = ok && !value.UseParticleLifeSecs;
    ok = ok && !value.UseSystemLifeSecs;
    ok = ok && !value.UseAccelerationParam;
    ok = ok && !value.UseAllAttractors;
    ok = ok && !value.UseRandomInitialRotation;
    ok = ok && !value.UseRandomRotationAxis;
    ok = ok && !value.ParticleCollideWithGround;
    ok = ok && !value.ParticleCollideWithAnything;
    ok = ok && !value.ParticleDieOnCollision;
    ok = ok && !value.SystemAlphaFadeEnable;
    ok = ok && !value.EmissionFadeEnable;
    ok = ok && !value.RandomisePosEnable;
    ok = ok && !value.RandomisePosDistVaryEnable;
    ok = ok && !value.StayWithEmitter;
    ok = ok && !value.SetOrientationFromDirection;
    ok = ok && !value.SetOrientationFromGame;
    ok = ok && !value.ParticleCreateDecal;
    ok = ok && !value.ParticleCreateDecalEmitter;
    ok = ok && value.ParticleLifeSecs == 0.0f;
    ok = ok && value.SystemLifeSecs == 0.0f;
    ok = ok && value.WindFactor == 0.0f;
    ok = ok && value.GravityFactor == 0.0f;
    ok = ok && value.AirResistance == 0.0f;
    ok = ok && value.ParticleAccelerationScale == 1.0f;
    ok = ok && value.InitialRotationX == 0.0f;
    ok = ok && value.InitialRotationY == 0.0f;
    ok = ok && value.InitialRotationZ == 0.0f;
    ok = ok && value.RotationMinAngleSpeed == 0.0f;
    ok = ok && value.RotationMaxAngleSpeed == 0.0f;
    ok = ok && value.ParticleBounce == 0.0f;
    ok = ok && value.SystemAlphaFadeMinimum == 0.0f;
    ok = ok && value.EmissionFadeMinimum == 0.0f;
    ok = ok && value.RandomisePosDistVaryScale == 0.0f;
    ok = ok && value.ParticleSystemOffset == C3DVector(0,0,0);
    ok = ok && value.ParticleAcceleration == C3DVector(0,0,0);
    ok = ok && value.RotationAxis == C3DVector(0,0,0);
    ok = ok && value.RandomisePosSpeed == C3DVector(0,0,0);
    ok = ok && value.RandomisePosScale == C3DVector(0,0,0);
    ok = ok && value.AccelerationParam == 0 && value.OrientFromGameParam == 0 && value.DecalEmitterName.text.empty()
        && value.FadeInEndInteger == 0 && value.FadeOutBeginInteger == 1000;
    std::puts(ok ? "00b0d360_TEST PASS" : "00b0d360_TEST FAIL");
    return ok ? 0 : 1;
}
