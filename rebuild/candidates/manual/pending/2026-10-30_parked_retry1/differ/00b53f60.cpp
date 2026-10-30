// Grade: DIFFER(418v411) (retry1) -- not a lane gap: VC7.1 tracks same-TU callee register use, so `this` can stay in ecx once both callees are in the TU. UpdateBoxPositions 0x00B52990 (landed, retail_asm) tail-ends in `call 0x00B51AD0` (RebuildBoxes) with ecx untouched, and RebuildBoxes itself (~260 instructions, more inline-asm floor expansions, no calls) never writes ecx; with only UpdateBoxPositions in the TU (this retry) ecx is still clobbered by the unknown RebuildBoxes, so `this` stays in esi | next: reconstruct RebuildBoxes 0x00B51AD0 and put both callee bodies ahead of FrameStart (chunk-585 same-TU lever)
// CEngineWeatherRenderer::FrameStart(const CEngineComponentParams&) -- move the particle boxes while it rains or snows, interpolate the rain/snow/mist time and the wind vector between their history samples by the frame blend, and fetch the mist volume texture from the graphic data bank (forced immediate load) when mist is on
// Leaf: FrameStart
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B53F60, length 416 bytes (body 0x199), `ret 4`.
// Callees: CEngineWeatherRenderer::UpdateBoxPositions 0x00B52990; CGraphicDataBank::Reload(long) 0x009FD100 (the bank at
//   +0x954 of the resource manager @ 0x01436E98, its load mode at +0x26c and its 0x2c-byte entry array at +0x1e0); the entry
//   getter 0x009FFB20 (CVolumeTexture**, 0); the entry index at +0xf0 of the global @ 0x01436E24; the static load flag
//   @ 0x013D28E8 (saved, forced on, restored).
// Layout: engine/CEngineWeatherRenderer.h for `this` (MistVolumeTexture +0x110, Settings +0x114); PDB
//   CEngineWeatherSettings: Raining +0x4bc, Snowing +0x4bd, Mist +0x4be, Rain/Snow/MistTimePassed +0x4c0.., their
//   [2] histories +0x4cc.., WindVector +0x4e4, WindVectorHistory[2] +0x4f0. CEngineComponentParams blend +0x3c (retail body).
#include "engine/CEngineWeatherRenderer.h"

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
    C3DVector operator+(const C3DVector& other) const { return C3DVector(X + other.X, Y + other.Y, Z + other.Z); }
    C3DVector operator*(float scale) const { return C3DVector(X * scale, Y * scale, Z * scale); }
};

struct CEngineWeatherSettingsView {                // CEngineWeatherSettings (PDB, 0x558 bytes)
    unsigned char _pad000[0x4bc];
    bool          Raining;                         // +0x4bc
    bool          Snowing;                         // +0x4bd
    bool          Mist;                            // +0x4be
    unsigned char _pad4bf;
    float         RainTimePassed;                  // +0x4c0
    float         SnowTimePassed;                  // +0x4c4
    float         MistTimePassed;                  // +0x4c8
    float         RainTimePassedHistory[2];        // +0x4cc
    float         SnowTimePassedHistory[2];        // +0x4d4
    float         MistTimePassedHistory[2];        // +0x4dc
    C3DVector     WindVector;                      // +0x4e4
    C3DVector     WindVectorHistory[2];            // +0x4f0
};

struct CEngineComponentParams {
    unsigned char _pad[0x3c];
    float         Blend;                           // +0x3c
};

template <class T> struct CIndexedArray {
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    T* Get(unsigned long index) { return index < Size() ? &First[index] : 0; }
};

struct CGraphicDataEntry {                         // 0x2c bytes
    unsigned char _pad[0x2c];
    void GetVolumeTexture(CVolumeTexture** texture, long flags);   // 0x009FFB20
};

struct CGraphicDataBank {
    unsigned char                    _pad000[0x1e0];
    CIndexedArray<CGraphicDataEntry> Entries;      // +0x1e0
    unsigned char                    _pad1ec[0x26c - 0x1ec];
    long                             LoadMode;     // +0x26c
    void Reload(long mode);                        // 0x009FD100
};

struct CEngineResourceManager {
    unsigned char     _pad[0x954];
    CGraphicDataBank* m_pDataBank;                 // +0x954
};
extern CEngineResourceManager* g_pEngineResourceManager_01436e98;   // 0x01436E98

struct SWeatherGraphics_01436e24 {
    unsigned char _pad[0xf0];
    unsigned long MistVolumeTextureIndex;          // +0xf0
};
extern SWeatherGraphics_01436e24* g_pWeatherGraphics_01436e24;      // 0x01436E24
extern bool g_bForceImmediateLoad_013d28e8;                        // 0x013D28E8

struct CEngineWeatherRenderer_Methods : CEngineWeatherRenderer {
    CEngineWeatherSettingsView& GetSettings() { return *reinterpret_cast<CEngineWeatherSettingsView*>(Settings); }
    void UpdateBoxPositions();                     // 0x00B52990
    void FrameStart(const CEngineComponentParams& params);
};

void CEngineWeatherRenderer_Methods::FrameStart(const CEngineComponentParams& params)
{
    CEngineWeatherSettingsView& settings = GetSettings();
    if (settings.Raining || settings.Snowing)
        UpdateBoxPositions();
    float blend = params.Blend;
    settings.RainTimePassed = (settings.RainTimePassedHistory[1] - settings.RainTimePassedHistory[0]) * blend + settings.RainTimePassedHistory[0];
    settings.SnowTimePassed = (settings.SnowTimePassedHistory[1] - settings.SnowTimePassedHistory[0]) * blend + settings.SnowTimePassedHistory[0];
    settings.MistTimePassed = (settings.MistTimePassedHistory[1] - settings.MistTimePassedHistory[0]) * blend + settings.MistTimePassedHistory[0];
    settings.WindVector = (settings.WindVectorHistory[1] - settings.WindVectorHistory[0]) * blend + settings.WindVectorHistory[0];
    CVolumeTexture** texture = &MistVolumeTexture;
    *texture = 0;
    if (settings.Mist) {
        bool savedForce = g_bForceImmediateLoad_013d28e8;
        CGraphicDataBank* bank = g_pEngineResourceManager_01436e98->m_pDataBank;
        long savedMode = bank->LoadMode;
        unsigned long index = g_pWeatherGraphics_01436e24->MistVolumeTextureIndex;
        bank->Reload(0);
        g_bForceImmediateLoad_013d28e8 = true;
        bank->Entries.Get(index)->GetVolumeTexture(texture, 0);
        bank->Reload(savedMode);
        g_bForceImmediateLoad_013d28e8 = savedForce;
    }
}
