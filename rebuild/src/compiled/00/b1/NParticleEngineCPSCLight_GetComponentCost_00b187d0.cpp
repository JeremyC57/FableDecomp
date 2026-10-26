// NParticleEngine::CPSCLight::GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float, float) -- vtable slot 20: an enabled light adds one light and its (system-scaled) larger render radius to the cost
// Leaf: ?GetComponentCost@CPSCLight@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B187D0, 80 bytes.
// Name: FableWin.pdb ?GetComponentCost@CPSCLight@NParticleEngine@@UAEXAAVCParticleSystemCost@2@ABVCParticleSystem@2@MM@Z;
// slot 20 of the vtable (rtti-vtables.tsv: slot 20 of CPSCSpline/CPSCOrbit/CPSCUpdateNormal is their landed
// GetComponentCost; `ret 10h`, four arguments).
// Callees: CParticleSystem::ScaleParticleDimensions(float&) @ 0x00AF8F80 (landed non-const; called through a
// const_cast on the const system).
// Constants: 1.0f @ 0x0122DED8 (compiler literal).
// Layout: the landed AddToBoundingSphere 0x00B18AF0 (retail offsets equal the PDB): LightStartRenderWorldRadius +0x2C,
// LightEndRenderWorldRadius +0x30, the +0x48 flag bits (LightEnabled bit 2); CParticleSystemCost (as the landed
// CPSCUpdateNormal::GetComponentCost 0x00B0C7B0): LightCount +8, LightCost +0x14.
// Shape: the radius is a named float `start > end ? start : end` (each arm an integer copy) homed in the dead
// `cost` argument slot (the cost pointer lives in esi), its address passed to the scaler. `ret 10h`.
namespace NParticleEngine {

class CParticleSystemCost { public:
    float PolyCount;                        // +0x00
    float PixelCount;                       // +0x04
    float LightCount;                       // +0x08
    float RenderCost;                       // +0x0c
    float SetupCost;                        // +0x10
    float LightCost;                        // +0x14
};

class CParticleSystem { public:
    void ScaleParticleDimensions(float& size);                  // 0x00AF8F80
};

class CPSCBase { public:
    virtual ~CPSCBase();
    CParticleSystem* ParticleSystem;            // +0x04
    bool             UpdateStarted : 1;         // +0x08 bit 0
    bool             Initialised : 1;           // +0x08 bit 1
};

class CPSCLight : public CPSCBase { public:
    long          LightLifeTimer;               // +0x0c
    long          LightRespawnDelayTimer;       // +0x10
    bool          LightIsAlive;                 // +0x14
    unsigned long LightPositionParam;           // +0x18
    float         LightLifeSecs;                // +0x1c
    float         LightRespawnDelaySecs;        // +0x20
    float         LightStartTime;               // +0x24
    float         LightTimelineSecs;            // +0x28
    float         LightStartRenderWorldRadius;  // +0x2c
    float         LightEndRenderWorldRadius;    // +0x30
    long          LightAttenuationFactor;       // +0x34
    long          LightFadeInEndInteger;        // +0x38
    long          LightFadeOutBeginInteger;     // +0x3c
    float         LightWorldRadiusFadeMinimum;  // +0x40
    unsigned long LightColourFadeMinimum;       // +0x44
    bool          LightWorldRadiusFadeEnable : 1;   // +0x48 bit 0
    bool          LightUseLifeSecs : 1;             //        bit 1
    bool          LightEnabled : 1;                 //        bit 2
    bool          LightRespawns : 1;
    bool          LightColourFadeEnable : 1;
    bool          LightUseStartColour : 1;
    bool          LightUseMidColour : 1;
    bool          LightUseEndColour : 1;
    bool          LightUseFadeColour : 1;           // +0x49 bit 0
    bool          LightUseTimelineSecs : 1;
    bool          StartedTimer : 1;
    unsigned long LightStartColour;             // +0x4c CRGBColour
    unsigned long LightMidColour;               // +0x50
    unsigned long LightEndColour;               // +0x54
    virtual void GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float, float);
};

void CPSCLight::GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float, float)
{
    if (LightEnabled) {
        cost.LightCount += 1.0f;
        float radius = LightStartRenderWorldRadius > LightEndRenderWorldRadius ? LightStartRenderWorldRadius
                                                                                : LightEndRenderWorldRadius;
        const_cast<CParticleSystem&>(system).ScaleParticleDimensions(radius);
        cost.LightCost += radius;
    }
}

} // namespace NParticleEngine