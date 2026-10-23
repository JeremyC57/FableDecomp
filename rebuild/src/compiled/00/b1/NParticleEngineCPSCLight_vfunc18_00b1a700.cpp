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