// NParticleEngine::CPSCSpline::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info) -- vtable slot 17: persist the spline base, then the speed-scaling and bounce flags and the anim speed by name
// Leaf: ?DatabaseSerialise@CPSCSpline@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B1B2E0, 105 bytes (+7 int3 padding).
// Name: FableWin.pdb ?DatabaseSerialise@CPSCSpline@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z;
// slot 17 of the vtable 0x012A0934 (rtti-vtables.tsv).
// Callees: CPSCSplineBase::DatabaseSerialise @ 0x00B22670, CPersistContext::Transfer(name, bool&, const bool&)
// @ 0x004045C0 (x2), CPersistContext::Transfer(name, float&, const float&) @ 0x00410620 (both as the landed
// CPSCAttractor::DatabaseSerialise 0x00B18110).
// Data: the three member names @ 0x012A0918, 0x012A0908, 0x012A08F8 (read from Fable.exe .rdata: "ScaleSplineSpeed",
// "SplineBounce", "SplineAnimSpeed").
// Layout: as the landed CPSCSpline::WriteBinary 0x00B1AFF0: SplineBounce +0x39, ScaleSplineSpeed +0x3A,
// SplineAnimSpeed +0x3C.
// Shape: each member is transferred in place by reference; the three default-value temporaries (false,
// false, 0.0f) are homed in the dead `info` argument slot ([esp+10h]) after the base call. `ret 8`.
class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: bool 0x004045C0; float 0x00410620
};

namespace NParticleEngine {

class CParticleSystem;
class CEmitterDatabaseSerialiseInfo;

class CPSCBase { public:
    virtual void v00();
    CParticleSystem* ParticleSystem;        // +0x04
    bool UpdateStarted : 1;                 // +0x08
    bool Initialised : 1;
};

class CPSCSplineBase : public CPSCBase { public:
    unsigned char _body[0x34 - 0x0c];       // +0x0c SplineAnimSpeed, Spline, the two counted data pointers
    void DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info);   // 0x00B22670
};

class CPSCSpline : public CPSCSplineBase { public:        // vtable 0x012A0934
    float SplineCurrentPosition;            // +0x34
    bool  ReverseSpline;                    // +0x38
    bool  SplineBounce;                     // +0x39
    bool  ScaleSplineSpeed;                 // +0x3a
    float SplineAnimSpeed;                  // +0x3c
    virtual void DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info);
};

void CPSCSpline::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info)
{
    CPSCSplineBase::DatabaseSerialise(context, info);
    context.Transfer("ScaleSplineSpeed", ScaleSplineSpeed, false);    // 0x012A0918
    context.Transfer("SplineBounce", SplineBounce, false);            // 0x012A0908
    context.Transfer("SplineAnimSpeed", SplineAnimSpeed, 0.0f);       // 0x012A08F8
}

} // namespace NParticleEngine