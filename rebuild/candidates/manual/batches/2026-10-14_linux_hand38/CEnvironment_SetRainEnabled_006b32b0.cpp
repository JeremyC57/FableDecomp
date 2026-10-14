// CEnvironment::SetRainEnabled(bool enabled) -- push the rain override to the weather renderer (slot 80 of the renderer reached through the engine globals: -1.0f when enabled, 0.0f when not) and record the flag in RainEnabled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B32B0, length 60 bytes.
// Callees: virtual slot 80 (+0x140) of the object at g_pEngineGlobals(0x013B86A0)->+0x28->+0x2c (a float argument pushed as an immediate:
// 0xBF800000 / 0). Shape: the global is read before the prologue, the chain and the vtable are loaded before the branch, the flag byte
// is kept in bl across the call and stored on both paths. The shape that reproduces the single vtable read before the branch is an
// if/else whose arms each reach the renderer through the SAME inline accessor chain (VC7.1 hoists the whole common prefix of the two
// arms, vtable fetch included); a pre-computed local keeps the vtable load inside each arm and a ternary argument builds a stack
// temporary. RainEnabled +0x45 as in
// engine/CEnvironment.h; the global chain is modelled descriptively (the same global feeds CViewBase::GetDisplayEngine).
struct IWeatherOverride_006b32b0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void SetRainOverride(float strength);  // slot 80 (+0x140)
};

struct CEngineRenderers_006b32b0 {
    unsigned char             _pad[0x2c];
    IWeatherOverride_006b32b0* m_pWeather;         // +0x2c
};

struct CEngineGlobals_006b32b0 {
    unsigned char              _pad[0x28];
    CEngineRenderers_006b32b0* m_pRenderers;       // +0x28
};

extern CEngineGlobals_006b32b0* g_pEngineGlobals_013b86a0;   // 0x013B86A0

static inline IWeatherOverride_006b32b0* GetWeatherOverride_006b32b0()
{
    return g_pEngineGlobals_013b86a0->m_pRenderers->m_pWeather;
}

struct CEnvironment {
    unsigned char _pad0[0x45];
    bool          RainEnabled;                     // +0x45
    void SetRainEnabled(bool enabled);
};

void CEnvironment::SetRainEnabled(bool enabled)
{
    if (enabled)
        GetWeatherOverride_006b32b0()->SetRainOverride(-1.0f);
    else
        GetWeatherOverride_006b32b0()->SetRainOverride(0.0f);
    RainEnabled = enabled;
}
