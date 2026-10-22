// CEnvironment::CEnvironment(CGameDefinitionManager* defs) -- build the environment: priority channels, tick states, theme pallette, spell-effect interface / interpolator and rain-splash pointer; seed the lightning and sunlight values from the environment defs, build the themes, then give each of the six channels a five-entry stack and blend the day theme into both tick states
// Leaf: ??0CEnvironment
// Layout: retail CEnvironment (not Ego_r.pdb): CBaseObject polymorphic root, PriorityStackChannels a 12-byte vector at +0x48, tick states
//          (pack(1), 0xc20 bytes: 6 x 0x1f4 themes + wind state, override direction at +0xc0d) at +0x54 / +0xc74, theme pallette +0x1954,
//          spell-effect scoped_ptrs +0x1960/+0x1964, rain-splash CIntelligentPointer +0x1968 (matches differ 006b3ff0 / 006b3030 views).
// Shape: the push_back overflow 0x006B7130 needs a same-TU noinline body that never touches the tag (else the tag keeps the dead defs
//          arg word live and the induction spill takes a new frame word); scoped_ptr members with inline px(0) ctors give the store order.
#include <stdlib.h>
inline void* operator new(size_t, void* where) { return where; }
void __cdecl operator delete(void* block);                     // 0x00BFE9BC
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CBaseObject { virtual void v000(); CBaseObject(); };   // 0x0099A2F0
struct CBaseIntelligentPointer {
    virtual void v000();
    void* m_pItem;
    CBaseIntelligentPointer();                                 // 0x00A01B10
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    virtual void v000();
};
struct CThing;
struct CRGBFloatColour { float R, G, B, A; };
struct CBlendedEnvironmentTheme {
    unsigned char _b[0x1f4];
    CBlendedEnvironmentTheme();                                // 0x008864A0
};
#pragma pack(push, 1)
struct CEnvironmentGameTickState {
    CBlendedEnvironmentTheme BlendedTheme[6];                  // +0x000
    float         Time;                                        // +0xbb8
    float         WindSpeed;                                   // +0xbbc
    C3DVector     WindDirection;                               // +0xbc0
    float         ThemeWindSpeed;                              // +0xbcc
    C3DVector     ThemeWindDirection;                          // +0xbd0
    unsigned char _padbdc[0x30];
    bool          WindOverridden;                              // +0xc0c
    C3DVector     OverrideWindDirection;                       // +0xc0d
    unsigned char _padc19[3];
    long          ThunderState;                                // +0xc1c
};
#pragma pack(pop)
struct CEnvironmentThemePriorityStackEntry {
    unsigned char _b0[8];
    void* Start; void* Finish; void* End;
    unsigned char _b14[0x1c];
    ~CEnvironmentThemePriorityStackEntry() { if (Start) free(Start); }
    void Reset(long theme);                                    // 0x00887330
};
inline void _Destroy(CEnvironmentThemePriorityStackEntry* first, CEnvironmentThemePriorityStackEntry* last)
{
    for (; first != last; ++first)
        first->~CEnvironmentThemePriorityStackEntry();
}
struct CEnvironmentThemePriorityStack {
    CEnvironmentThemePriorityStackEntry* Start;
    CEnvironmentThemePriorityStackEntry* Finish;
    CEnvironmentThemePriorityStackEntry* End;
    explicit CEnvironmentThemePriorityStack(long count);       // 0x008883C0
    CEnvironmentThemePriorityStack(const CEnvironmentThemePriorityStack& other);   // 0x006B6840
    ~CEnvironmentThemePriorityStack()
    {
        _Destroy(Start, Finish);
        if (Start)
            free(Start);
    }
};
struct __false_type { __false_type() { } };
struct CPriorityStackArray {
    CEnvironmentThemePriorityStack* _M_start;
    CEnvironmentThemePriorityStack* _M_finish;
    CEnvironmentThemePriorityStack* _M_end_of_storage;
    CPriorityStackArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    void _M_insert_overflow(CEnvironmentThemePriorityStack* pos, const CEnvironmentThemePriorityStack& x,
                            const __false_type&, unsigned int fill_len, bool atend);   // 0x006B7130
    void push_back(const CEnvironmentThemePriorityStack& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CEnvironmentThemePriorityStack(x);
            ++_M_finish;
        }
        else {
            __false_type tag;
            _M_insert_overflow(_M_finish, x, tag, 1, true);
        }
    }
};
// same-TU stand-in for the overflow insert (0x006B7130): reads the range first, never touches the tag
#pragma auto_inline(off)
__declspec(noinline) void CPriorityStackArray::_M_insert_overflow(CEnvironmentThemePriorityStack* pos, const CEnvironmentThemePriorityStack& x,
                                                                  const __false_type&, unsigned int fill_len, bool atend)
{
    CEnvironmentThemePriorityStack* start = _M_start;
    CEnvironmentThemePriorityStack* finish = _M_finish;
    unsigned int size = finish - start;
    unsigned int len = size + (size > fill_len ? size : fill_len);
    CEnvironmentThemePriorityStack* buffer = (CEnvironmentThemePriorityStack*)malloc(len * sizeof(CEnvironmentThemePriorityStack));
    CEnvironmentThemePriorityStack* cur = buffer;
    for (CEnvironmentThemePriorityStack* p = start; p != pos; ++p, ++cur)
        new (cur) CEnvironmentThemePriorityStack(*p);
    for (unsigned int n = 0; n < fill_len; ++n, ++cur)
        new (cur) CEnvironmentThemePriorityStack(x);
    if (!atend)
        for (CEnvironmentThemePriorityStack* p = pos; p != finish; ++p, ++cur)
            new (cur) CEnvironmentThemePriorityStack(*p);
    if (start)
        free(start);
    _M_start = buffer;
    _M_finish = cur;
    _M_end_of_storage = buffer + len;
}
#pragma auto_inline(on)
struct CVectorRaw {
    void* Start; void* Finish; void* End;
    CVectorRaw() : Start(0), Finish(0), End(0) { }
};
struct CEnvironmentThemeSetDay {
    void GetBlendedTheme(CBlendedEnvironmentTheme* theme, float hour);   // 0x00888260
};
struct CEnvironmentThemeResourcePallette {
    CEnvironmentThemeSetDay* Day; void* Finish; void* End;
    CEnvironmentThemeResourcePallette() : Day(0), Finish(0), End(0) { }
};
struct CEnvironment;
template <class T> struct scoped_ptr {
    T* px;
    scoped_ptr() : px(0) { }
};
struct CEnvironmentSpellEffectInterface {
    unsigned char _b[0x104];
    CEnvironmentSpellEffectInterface(CEnvironment* environment);   // 0x008852E0
};
struct CEnvironmentSpellEffectSubState { ~CEnvironmentSpellEffectSubState(); };   // 0x008814C0
struct CEnvironmentSpellEffectInterpolator {
    unsigned char _b[0x90];
    CEnvironmentSpellEffectSubState State;                     // +0x90
    unsigned char _b91[0xcf];
    CEnvironmentSpellEffectInterpolator(CEnvironmentSpellEffectInterface* iface, CEnvironment* environment);   // 0x006B84B0
};
struct CEnvironmentDefs {
    unsigned char _pad[0x98];
    float DiffuseClampAngle;                                   // +0x98
    float SunlightAttenuatorAngleFadeStart;                    // +0x9c
    float SunlightAttenuatorAngleFadeEnd;                      // +0xa0
    unsigned char _pada4[0x60];
    float LightningFadeInDuration;                             // +0x104
    float LightningFadeOutDuration;                            // +0x108
    float LightningFlashDuration;                              // +0x10c
    float LightningRainThreshold;                              // +0x110
    long  LightningTheme;                                      // +0x114
};
struct CGameDefinitionManager { unsigned char _pad[0xd8]; CEnvironmentDefs* Environment; };
struct CEnvironment : CBaseObject {
    virtual void v001();
    CGameDefinitionManager* DefinitionManager;                 // +0x04
    float SunlightAttenuatorAngleFadeStart;                    // +0x08
    float SunlightAttenuatorAngleFadeEnd;                      // +0x0c
    long  RegionTheme;                                         // +0x10
    long  _pad14;
    double LightningStartTime;                                 // +0x18
    float LightningFadeInDuration;                             // +0x20
    float LightningFadeOutDuration;                            // +0x24
    float LightningFlashDuration;                              // +0x28
    float LightningRainThreshold;                              // +0x2c
    long  LightningTheme;                                      // +0x30
    float LightningCurrentFlashChance;                         // +0x34
    unsigned long LightningRandomSeed;                         // +0x38
    bool ForcedColours;                                        // +0x3c
    bool ForcedNonColourValues;                                // +0x3d
    bool ForcedEnvironmentTheme;                               // +0x3e
    float DiffuseClampAngle;                                   // +0x40
    bool PassedFirstUpdate;                                    // +0x44
    bool RainEnabled;                                          // +0x45
    CPriorityStackArray PriorityStackChannels;                 // +0x48
    CEnvironmentGameTickState GameTickState;                   // +0x54
    CEnvironmentGameTickState PrevGameTickState;               // +0xc74
    unsigned char _pad1894[0x54];
    CVectorRaw InterpolatedTextureSets;                        // +0x18e8
    unsigned char _pad18f4[0x60];
    CEnvironmentThemeResourcePallette ThemeSet;                // +0x1954
    scoped_ptr<CEnvironmentSpellEffectInterface> SpellEffectInterface;    // +0x1960
    scoped_ptr<CEnvironmentSpellEffectInterpolator> SpellEffectInterpolator;   // +0x1964
    CIntelligentPointer<CThing> PRainSplashParticle;           // +0x1968
    CEnvironment(CGameDefinitionManager* defs);
    void BuildThemes();                                        // 0x006B3350
};

CEnvironment::CEnvironment(CGameDefinitionManager* defs)
    : DefinitionManager(defs)
{
    LightningStartTime = -1.0;
    GameTickState.WindDirection = C3DVector(1.0f, 0.0f, 0.0f);
    GameTickState.ThemeWindDirection = C3DVector(1.0f, 0.0f, 0.0f);
    GameTickState.WindSpeed = 0.2f;
    GameTickState.ThemeWindSpeed = 0.2f;
    GameTickState.Time = 0.0f;
    GameTickState.WindOverridden = false;
    GameTickState.ThunderState = 0;
    RegionTheme = 0;
    LightningRandomSeed = 123;
    LightningCurrentFlashChance = 0.0f;
    GameTickState.OverrideWindDirection = C3DVector(1.0f, 0.0f, 0.0f);
    CEnvironmentDefs* environment = DefinitionManager->Environment;
    DiffuseClampAngle = environment->DiffuseClampAngle;
    SunlightAttenuatorAngleFadeStart = environment->SunlightAttenuatorAngleFadeStart;
    SunlightAttenuatorAngleFadeEnd = environment->SunlightAttenuatorAngleFadeEnd;
    LightningFadeInDuration = environment->LightningFadeInDuration;
    LightningFadeOutDuration = environment->LightningFadeOutDuration;
    LightningFlashDuration = environment->LightningFlashDuration;
    LightningRainThreshold = environment->LightningRainThreshold;
    LightningTheme = environment->LightningTheme;
    ForcedColours = false;
    ForcedNonColourValues = false;
    ForcedEnvironmentTheme = false;
    PassedFirstUpdate = false;
    RainEnabled = true;
    {
        CEnvironmentSpellEffectInterface* created = new CEnvironmentSpellEffectInterface(this);
        CEnvironmentSpellEffectInterface* old = SpellEffectInterface.px;
        SpellEffectInterface.px = created;
        operator delete(old);
    }
    {
        CEnvironmentSpellEffectInterpolator* created = new CEnvironmentSpellEffectInterpolator(SpellEffectInterface.px, this);
        CEnvironmentSpellEffectInterpolator* old = SpellEffectInterpolator.px;
        SpellEffectInterpolator.px = created;
        delete old;
    }
    BuildThemes();
    for (long i = 0; i < 6; ++i) {
        PriorityStackChannels.push_back(CEnvironmentThemePriorityStack(5));
        PriorityStackChannels._M_start[i].Start->Reset(0);
        ThemeSet.Day->GetBlendedTheme(&GameTickState.BlendedTheme[i], 12.0f);
        ThemeSet.Day->GetBlendedTheme(&PrevGameTickState.BlendedTheme[i], 12.0f);
    }
}
