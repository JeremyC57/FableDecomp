#include <cstdio>
#include <cstdlib>
// BEGIN exact source snapshot SHA256 b05de23b7e2252d57422d4d6c74acc019ed6551c96cd6e74f0079eaa79dd171c
// 00B1A700 NParticleEngine::CPSCLight::vfunc_18, 361 retail bytes.
// The vtable maps this entry to CPSCLight slot 18. No generated CPSCLight header is
// present, so this local model follows struct_layouts_egor.tsv (112-byte CPSCLight).
// The copy order and bit units are confirmed by adjacent DatabaseCopy code and retail
// loads. LightUserPointsArray is the PDB CArray<C3DVector>, a 12-byte STLport vector;
// its reserve is 0x00920FF0 and resize is 0x0075C040. Timer state, primitive handle,
// and StartedTimer are intentionally not copied.
namespace NParticleEngine {
struct CParticleSystem;
struct CPSCBase {
    virtual void v000() {}
    CParticleSystem* ParticleSystem;
    bool UpdateStarted : 1;
    bool Initialised : 1;
};
struct CRGBColour { unsigned int Colour; };
struct C3DVector { float X, Y, Z; C3DVector() {} };
struct CVector3D {
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void reserve(unsigned int count);
    void resize(unsigned int count, const C3DVector& value = C3DVector());
    C3DVector& operator[](unsigned int i) const { return _M_start[i]; }
};
struct CPSCLight : CPSCBase {
    long LightLifeTimer;
    long LightRespawnDelayTimer;
    bool LightIsAlive;
    unsigned int LightPositionParam;
    float LightLifeSecs;
    float LightRespawnDelaySecs;
    float LightStartTime;
    float LightTimelineSecs;
    float LightStartRenderWorldRadius;
    float LightEndRenderWorldRadius;
    long LightAttenuationFactor;
    long LightFadeInEndInteger;
    long LightFadeOutBeginInteger;
    float LightWorldRadiusFadeMinimum;
    CRGBColour LightColourFadeMinimum;
    bool LightWorldRadiusFadeEnable : 1;
    bool LightUseLifeSecs : 1;
    bool LightEnabled : 1;
    bool LightRespawns : 1;
    bool LightColourFadeEnable : 1;
    bool LightUseStartColour : 1;
    bool LightUseMidColour : 1;
    bool LightUseEndColour : 1;
    bool LightUseFadeColour : 1;
    bool LightUseTimelineSecs : 1;
    bool StartedTimer : 1;
    CRGBColour LightStartColour;
    CRGBColour LightMidColour;
    CRGBColour LightEndColour;
    unsigned int PrimHandleData;
    unsigned int PrimHandleInfo;
    CVector3D LightUserPointsArray;
    void DatabaseCopy(const CPSCBase& other);
};
void CPSCLight::DatabaseCopy(const CPSCBase& other)
{
    const CPSCLight* src = (const CPSCLight*)&other;
    LightEnabled = src->LightEnabled;
    LightPositionParam = src->LightPositionParam;
    LightUseLifeSecs = src->LightUseLifeSecs;
    LightLifeSecs = src->LightLifeSecs;
    LightRespawns = src->LightRespawns;
    LightRespawnDelaySecs = src->LightRespawnDelaySecs;
    LightStartRenderWorldRadius = src->LightStartRenderWorldRadius;
    LightEndRenderWorldRadius = src->LightEndRenderWorldRadius;
    LightAttenuationFactor = src->LightAttenuationFactor;
    LightFadeInEndInteger = src->LightFadeInEndInteger;
    LightFadeOutBeginInteger = src->LightFadeOutBeginInteger;
    LightWorldRadiusFadeEnable = src->LightWorldRadiusFadeEnable;
    LightWorldRadiusFadeMinimum = src->LightWorldRadiusFadeMinimum;
    LightUseFadeColour = src->LightUseFadeColour;
    LightColourFadeEnable = src->LightColourFadeEnable;
    LightColourFadeMinimum = src->LightColourFadeMinimum;
    LightUseStartColour = src->LightUseStartColour;
    LightUseMidColour = src->LightUseMidColour;
    LightUseEndColour = src->LightUseEndColour;
    LightStartColour = src->LightStartColour;
    LightMidColour = src->LightMidColour;
    LightEndColour = src->LightEndColour;
    LightStartTime = src->LightStartTime;
    LightTimelineSecs = src->LightTimelineSecs;
    LightUseTimelineSecs = src->LightUseTimelineSecs;
    unsigned int count = src->LightUserPointsArray.size();
    LightUserPointsArray.reserve(count);
    LightUserPointsArray.resize(count);
    for (unsigned int i = 0; i < count; ++i)
        LightUserPointsArray[i] = src->LightUserPointsArray[i];
}
}

// END exact source snapshot
namespace NParticleEngine {
void CVector3D::reserve(unsigned int count)
{
    if (count <= (unsigned int)(_M_end_of_storage - _M_start)) return;
    C3DVector* p = new C3DVector[count];
    unsigned int old = size();
    for (unsigned int i = 0; i < old; ++i) p[i] = _M_start[i];
    delete [] _M_start;
    _M_start = p;
    _M_finish = p + old;
    _M_end_of_storage = p + count;
}
void CVector3D::resize(unsigned int count, const C3DVector& value)
{
    reserve(count);
    while (size() < count) {
        C3DVector& p = *_M_finish++;
        p.X = 0.0f; p.Y = 0.0f; p.Z = 0.0f;
    }
    _M_finish = _M_start + count;
    (void)value;
}
}
int main()
{
    static NParticleEngine::CPSCLight src;
    static NParticleEngine::CPSCLight dst;
    src.LightUserPointsArray._M_start = new NParticleEngine::C3DVector[2];
    src.LightUserPointsArray._M_finish = src.LightUserPointsArray._M_start + 2;
    src.LightUserPointsArray._M_end_of_storage = src.LightUserPointsArray._M_finish;
    dst.LightUserPointsArray._M_start = new NParticleEngine::C3DVector[2];
    dst.LightUserPointsArray._M_finish = dst.LightUserPointsArray._M_start + 1;
    dst.LightUserPointsArray._M_end_of_storage = dst.LightUserPointsArray._M_start + 2;
    src.LightUserPointsArray._M_start[0].X = 1.0f; src.LightUserPointsArray._M_start[0].Y = 2.0f; src.LightUserPointsArray._M_start[0].Z = 3.0f;
    src.LightUserPointsArray._M_start[1].X = 4.0f; src.LightUserPointsArray._M_start[1].Y = 5.0f; src.LightUserPointsArray._M_start[1].Z = 6.0f;
    dst.LightUserPointsArray._M_start[0].X = -1.0f; dst.LightUserPointsArray._M_start[0].Y = -2.0f; dst.LightUserPointsArray._M_start[0].Z = -3.0f;
    src.LightWorldRadiusFadeEnable = false; src.LightUseLifeSecs = false; src.LightEnabled = false;
    src.LightRespawns = false; src.LightColourFadeEnable = false; src.LightUseStartColour = false;
    src.LightUseMidColour = false; src.LightUseEndColour = false; src.LightUseFadeColour = false;
    src.LightUseTimelineSecs = false; src.StartedTimer = false;
    dst.LightWorldRadiusFadeEnable = true; dst.LightUseLifeSecs = true; dst.LightEnabled = true;
    dst.LightRespawns = true; dst.LightColourFadeEnable = true; dst.LightUseStartColour = true;
    dst.LightUseMidColour = true; dst.LightUseEndColour = true; dst.LightUseFadeColour = true;
    dst.LightUseTimelineSecs = true;
    src.LightEnabled = true;
    src.LightPositionParam = 19;
    src.LightUseLifeSecs = true;
    src.LightLifeSecs = 5.0f;
    src.LightRespawns = true;
    src.LightRespawnDelaySecs = 2.0f;
    src.LightStartRenderWorldRadius = 3.0f;
    src.LightEndRenderWorldRadius = 6.0f;
    src.LightAttenuationFactor = 7;
    src.LightFadeInEndInteger = 8;
    src.LightFadeOutBeginInteger = 9;
    src.LightWorldRadiusFadeEnable = true;
    src.LightWorldRadiusFadeMinimum = 0.25f;
    src.LightUseFadeColour = true;
    src.LightColourFadeEnable = true;
    src.LightColourFadeMinimum.Colour = 0x10203040;
    src.LightUseStartColour = true;
    src.LightUseMidColour = true;
    src.LightUseEndColour = true;
    src.LightStartColour.Colour = 0x11223344;
    src.LightMidColour.Colour = 0x55667788;
    src.LightEndColour.Colour = 0x99aabbcc;
    src.LightStartTime = 1.0f;
    src.LightTimelineSecs = 4.0f;
    src.LightUseTimelineSecs = true;
    src.LightUseFadeColour = false; src.LightUseMidColour = false; src.LightUseEndColour = false;
    src.LightLifeTimer = 0; src.LightRespawnDelayTimer = 1; src.LightIsAlive = false;
    dst.LightLifeTimer = 123; dst.LightRespawnDelayTimer = 456; dst.LightIsAlive = true;
    dst.StartedTimer = true; dst.UpdateStarted = true; dst.Initialised = true;
    dst.DatabaseCopy((const NParticleEngine::CPSCBase&)src);
    if (!dst.LightEnabled || dst.LightPositionParam != 19 || !dst.LightUseLifeSecs ||
        dst.LightLifeSecs != 5.0f || !dst.LightRespawns || dst.LightRespawnDelaySecs != 2.0f ||
        dst.LightStartRenderWorldRadius != 3.0f || dst.LightEndRenderWorldRadius != 6.0f ||
        dst.LightAttenuationFactor != 7 || dst.LightFadeInEndInteger != 8 ||
        dst.LightFadeOutBeginInteger != 9 || !dst.LightWorldRadiusFadeEnable ||
        dst.LightWorldRadiusFadeMinimum != 0.25f || dst.LightUseFadeColour ||
        !dst.LightColourFadeEnable || dst.LightColourFadeMinimum.Colour != 0x10203040 ||
        !dst.LightUseStartColour || dst.LightUseMidColour || dst.LightUseEndColour ||
        dst.LightStartColour.Colour != 0x11223344 || dst.LightMidColour.Colour != 0x55667788 ||
        dst.LightEndColour.Colour != 0x99aabbcc || dst.LightStartTime != 1.0f ||
        dst.LightTimelineSecs != 4.0f || !dst.LightUseTimelineSecs ||
        dst.LightLifeTimer != 123 || dst.LightRespawnDelayTimer != 456 || !dst.LightIsAlive ||
        !dst.StartedTimer || !dst.UpdateStarted || !dst.Initialised ||
        (unsigned int)(dst.LightUserPointsArray._M_finish - dst.LightUserPointsArray._M_start) != 2 ||
        dst.LightUserPointsArray._M_start[0].X != 1.0f || dst.LightUserPointsArray._M_start[0].Y != 2.0f ||
        dst.LightUserPointsArray._M_start[0].Z != 3.0f || dst.LightUserPointsArray._M_start[1].X != 4.0f ||
        dst.LightUserPointsArray._M_start[1].Y != 5.0f || dst.LightUserPointsArray._M_start[1].Z != 6.0f) {
        std::printf("00b1a700_TEST FAIL\n"); return 1;
    }
    src.LightUserPointsArray._M_finish = src.LightUserPointsArray._M_start;
    dst.DatabaseCopy((const NParticleEngine::CPSCBase&)src);
    if (dst.LightPositionParam != 19 || !dst.LightUseLifeSecs ||
        dst.LightUserPointsArray._M_finish != dst.LightUserPointsArray._M_start) {
        std::printf("00b1a700_TEST FAIL\n"); return 1;
    }
    std::printf("00b1a700_TEST PASS\n");
    return 0;
}