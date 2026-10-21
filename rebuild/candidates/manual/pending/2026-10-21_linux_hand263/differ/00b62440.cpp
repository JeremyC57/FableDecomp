// Grade: DIFFER(389v405) -- layer-0 wind scaling: retail builds the C3DVector through stack temps (y,z spilled, copied into the result, result copied into the local; 0x1c frame); ours elides one copy (0x18 frame)
// CEngineSkyRenderer::WorldFrameUpdate(bool) -- when the world updated, scroll both cloud layers: clamp each layer's theme cloud speed (scaled by the multiplier and the frame time) to [min, max], push the current offset into the history and advance it along the wind, then keep both histories in range
// Leaf: WorldFrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B62440, length 416 bytes (body 0x195), `ret 4`.
// Callees: virtual slot 1 of the environment provider at +0x198 of the engine state @ 0x01436E8C, virtual slot 1 of that
//   (the theme), CEngineSkyRenderer::KeepHistoryInRange(C2DVector*, float) const @ 0x00B61AA0.
// Layout: `this` is engine/CEngineSkyRenderer.h (CloudSpeedMultiplier +0x18, CloudMaxSpeed +0x1c, CloudMinSpeed +0x20,
//   CloudVectorHistory C2DVector[2][2] +0x16c). Theme settings (+0x54 of the theme): layer 0 cloud speed +0x130, layer 1
//   +0x134. Frame state @ 0x01436E48: wind direction C3DVector +0x61c, frame time +0x640 (offsets read from the retail body).
#include "engine/CEngineSkyRenderer.h"

struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector& operator+=(const C2DVector& other) { X += other.X; Y += other.Y; return *this; }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float scale) const
    {
        C3DVector result;
        result.X = X * scale;
        result.Y = Y * scale;
        result.Z = Z * scale;
        return result;
    }
};

struct SSkyThemeSettings {
    unsigned char _pad[0x130];
    float         CloudSpeed[2];                   // +0x130
};

struct CEnvironmentTheme {
    unsigned char      _pad[0x54];
    SSkyThemeSettings* m_pSettings;                // +0x54
};

struct CEnvironment {
    virtual void v00();
    virtual CEnvironmentTheme* GetTheme();         // slot 1
};

struct CEnvironmentProvider {
    virtual void v00();
    virtual CEnvironment* GetCurrentEnvironment(); // slot 1
};

struct SEngineState_01436e8c {
    unsigned char         _pad[0x198];
    CEnvironmentProvider* m_pEnvironmentProvider;  // +0x198
};
extern SEngineState_01436e8c* g_pEngineState_01436e8c;   // 0x01436E8C

struct SFrameState_01436e48 {
    unsigned char _pad[0x61c];
    C3DVector     WindDirection;                   // +0x61c
    unsigned char _pad628[0x640 - 0x628];
    float         FrameTime;                       // +0x640
};
extern SFrameState_01436e48* g_pFrameState_01436e48;     // 0x01436E48

inline float FClamp(float value, float minValue, float maxValue)
{
    if (value < minValue)
        return minValue;
    return value > maxValue ? maxValue : value;
}

struct CEngineSkyRenderer_Methods : CEngineSkyRenderer {
    C2DVector* History(long layer) { return (C2DVector*)(CloudVectorHistory + layer * 2 * sizeof(C2DVector)); }
    void KeepHistoryInRange(C2DVector* history, float range) const;
    void WorldFrameUpdate(bool worldUpdated);
};

void CEngineSkyRenderer_Methods::WorldFrameUpdate(bool worldUpdated)
{
    if (worldUpdated) {
        const CEnvironmentTheme* theme = g_pEngineState_01436e8c->m_pEnvironmentProvider->GetCurrentEnvironment()->GetTheme();
        float speed = FClamp(g_pFrameState_01436e48->FrameTime * (CloudSpeedMultiplier * theme->m_pSettings->CloudSpeed[1]),
                             CloudMinSpeed, CloudMaxSpeed);
        C2DVector delta(g_pFrameState_01436e48->WindDirection.X * speed, g_pFrameState_01436e48->WindDirection.Y * speed);
        History(1)[1] = History(1)[0];
        History(1)[0] += delta;
        speed = FClamp(g_pFrameState_01436e48->FrameTime * (CloudSpeedMultiplier * theme->m_pSettings->CloudSpeed[0]),
                       CloudMinSpeed, CloudMaxSpeed);
        C3DVector wind = g_pFrameState_01436e48->WindDirection * speed;
        History(0)[1] = History(0)[0];
        History(0)[0] += C2DVector(wind.X, wind.Y);
        KeepHistoryInRange(History(1), 1.0f);
        KeepHistoryInRange(History(0), 1.0f);
    }
}
