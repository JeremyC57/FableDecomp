// Grade: DIFFER(751v758) -- code shape matches (channel loop, colour rep movsd, unrolled scaler copy, texture-set operator= pairs, rain-splash keep-alive); retail's frame is 0x244 (GetPosition return temp above the theme plus 0x20 more), VC7.1 here packs it into 0x210, and the RainEnabled zeroing store is laid out of line
// CEnvironment::Update(float time) -- per-frame environment update: remember the previous tick state, blend each priority channel's theme into the tick state (colours / scalers / texture sets unless forced), keep the rain-splash particle alive while it rains, then the view / wind / thunder updates
// Leaf: ?Update@CEnvironment
// Layout: retail CEnvironment (not the Ego_r.pdb one): CArray<CEnvironmentThemePriorityStack> is a 12-byte vector at +0x48, the
// tick states sit at +0x54 / +0xc74 (6 x 0x1f4-byte CBlendedEnvironmentTheme + time at +0xbb8), the theme pallette at +0x1954,
// the spell-effect interpolator at +0x1964 and the rain-splash CIntelligentPointer<CThing> at +0x1968.
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CVectorLong {                                           // STLport vector<long>, 12 bytes
    long* Start; long* Finish; long* End;
    CVectorLong& operator=(const CVectorLong& other);          // 0x00454886
};
struct CVectorTexture {
    void* Start; void* Finish; void* End;
    CVectorTexture& operator=(const CVectorTexture& other);    // 0x0045EF2D
};
struct CIDrawEnvironmentTextureSet {
    CVectorLong    Textures;
    CVectorTexture Params;
};
struct CRGBFloatColour { float R, G, B, A; };
struct CBlendedEnvironmentTheme {
    float                       TimeOfDay;                     // +0x000
    CRGBFloatColour             Colours[19];                   // +0x004
    float                       Scalers[30];                   // +0x134
    CIDrawEnvironmentTextureSet TextureSets[3];                // +0x1ac
    CBlendedEnvironmentTheme();                                // 0x008864A0
    ~CBlendedEnvironmentTheme();                               // 0x006B5D80
};
struct CEnvironmentThemeResourcePallette { unsigned char _pad[0xc]; };
struct CEnvironmentGameTickState {
    CBlendedEnvironmentTheme BlendedTheme[6];                  // +0x000
    float                    Time;                             // +0xbb8
    unsigned char            _padbbc[0x64];
    CEnvironmentGameTickState& operator=(const CEnvironmentGameTickState& other);   // 0x006B63C0
};
struct CEnvironmentThemePriorityStack {
    unsigned char _pad[0xc];
    void ProcessTransitions();                                 // 0x00887B00
    void CalculateTheme(CBlendedEnvironmentTheme* theme, float time, CEnvironmentThemeResourcePallette* pallette);   // 0x008889C0
};
struct CEnvironmentSpellEffectInterpolator { void FrameUpdate(); };   // 0x006B8640
struct CThing {
    unsigned char _pad00[0x28];
    unsigned long Flags28;                                     // +0x28
    unsigned char _pad2c[0x30];
    struct CTCDParticleEmitter* Emitter;                       // +0x5c
    unsigned char _pad60[0x31];
    unsigned char Dead;                                        // +0x91 bit 0
    void Kill(bool now);                                       // 0x004C9B80
};
struct CTCDParticleEmitter { void AttachToCamera(long kind, float offset); };   // 0x006E0DB0
struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v000();
    CBaseObject* m_pItem;
    CBaseIntelligentPointer();                                 // 0x00A01B10
    ~CBaseIntelligentPointer();                                // 0x00A01C10
    CBaseObject* GetPItem() const;                             // 0x00A01B50
    void SetPItem(CBaseObject* item);                          // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    virtual void v000();
    T* Get() const { return (T*)GetPItem(); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached);   // 0x006E0880
struct CCameraView { virtual void v000(); virtual C3DVector GetPosition(float t) const; };   // slot 1
struct CEngineDisplay { unsigned char _pad[0x34]; CCameraView* View; };
struct CEngine { unsigned char _pad[0x24]; CEngineDisplay* Display; };
extern CEngine* g_pEngine;                                     // 0x013B86A0
struct CEffectDefs { unsigned char _pad[0x100]; long RainSplashEffect; };
struct CGameDefinitionManager { unsigned char _pad[0xd8]; CEffectDefs* Effects; };
struct CEnvironment {
    virtual void v000();
    CGameDefinitionManager* DefinitionManager;                 // +0x04
    unsigned char _pad08[0x34];
    bool ForcedColours;                                        // +0x3c
    bool ForcedNonColourValues;                                // +0x3d
    bool ForcedEnvironmentTheme;
    unsigned char _pad3f[5];
    bool PassedFirstUpdate;                                    // +0x44
    bool RainEnabled;                                          // +0x45
    CEnvironmentThemePriorityStack* Channels;                  // +0x48
    CEnvironmentThemePriorityStack* ChannelsEnd;
    CEnvironmentThemePriorityStack* ChannelsCapacity;
    CEnvironmentGameTickState GameTickState;                   // +0x54
    CEnvironmentGameTickState PrevGameTickState;               // +0xc74
    unsigned char _pad1894[0xc0];
    CEnvironmentThemeResourcePallette ThemeSet;                // +0x1954
    void* SpellEffectInterface;                                // +0x1960
    CEnvironmentSpellEffectInterpolator* SpellEffectInterpolator;   // +0x1964
    CIntelligentPointer<CThing> PRainSplashParticle;           // +0x1968
    void UpdateViewMatrix();                                   // 0x006B2CA0
    void UpdateWind();                                         // 0x006B3030
    void UpdateThunder();                                      // 0x006B3B80
    void Update(float time);
};

void CEnvironment::Update(float time)
{
    PrevGameTickState = GameTickState;
    SpellEffectInterpolator->FrameUpdate();
    GameTickState.Time = time;
    CBlendedEnvironmentTheme theme;
    for (long i = 0; i < 6; ++i) {
        Channels[i].ProcessTransitions();
        Channels[i].CalculateTheme(&theme, time, &ThemeSet);
        if (!ForcedColours)
            memcpy(GameTickState.BlendedTheme[i].Colours, theme.Colours, sizeof(theme.Colours));
        if (!ForcedNonColourValues) {
            for (long s = 0; s < 30; ++s)
                GameTickState.BlendedTheme[i].Scalers[s] = theme.Scalers[s];
            for (long t = 0; t < 3; ++t)
                GameTickState.BlendedTheme[i].TextureSets[t] = theme.TextureSets[t];
        }
    }
    float rain = GameTickState.BlendedTheme[0].Scalers[15];
    if (!RainEnabled)
        rain = 0.0f;
    if (rain > 0.0001f && (!PRainSplashParticle.Get() || (PRainSplashParticle.Get()->Dead & 1))) {
        C3DVector position;
        position = g_pEngine->Display->View->GetPosition(1.0f);
        PRainSplashParticle = CIntelligentPointer<CThing>(CreateSpecialEffect(DefinitionManager->Effects->RainSplashEffect, position, false));
        if (PRainSplashParticle.Get() && !(PRainSplashParticle.Get()->Dead & 1)) {
            CTCDParticleEmitter* emitter = 0;
            CThing* particle = PRainSplashParticle.Get();
            if (particle->Flags28 & 0x800)
                emitter = particle->Emitter;
            emitter->AttachToCamera(8, 0.0f);
        }
    } else if (rain <= 0.0001f && PRainSplashParticle.Get() && !(PRainSplashParticle.Get()->Dead & 1)) {
        PRainSplashParticle.Get()->Kill(false);
    }
    UpdateViewMatrix();
    UpdateWind();
    UpdateThunder();
    if (!PassedFirstUpdate) {
        PrevGameTickState = GameTickState;
        PassedFirstUpdate = true;
    }
}
