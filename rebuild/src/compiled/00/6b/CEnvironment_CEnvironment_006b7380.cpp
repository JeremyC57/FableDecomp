// CEnvironment::~CEnvironment() -- destroy the environment's members in reverse order (rain-splash particle pointer, spell-effect interpolator and interface, theme resource pallette, interpolated draw params, both game-tick states, priority-stack channels), then the CBase part
// Leaf: ??1CEnvironment@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B7380, 263 bytes, thiscall, plain `ret`, no EH frame.
// Identity: the catalog's "CGameWorld_Destructor" is a descriptive auto label. The only caller is CEnvironment's scalar
//   deleting destructor 0x006B7360 (landed, referenced from the vtable at 0x0125D53C). Members are Ego_r.pdb CEnvironment
//   in declaration order; the engine header CEnvironment.h carries the PDB's 16-byte debug vectors, so its offsets disagree
//   with retail's 12-byte STLport vectors from +0x58 on and this is a local model: PriorityStackChannels +0x48,
//   GameTickState +0x54, PrevGameTickState +0xC74 (0xC20 bytes each: CBlendedEnvironmentTheme[6] of 0x1F4 + 0x68),
//   InterpolatedEnvironmentParams +0x1894 (ThemeChannels vector at +0x54), ThemeSet +0x1954, SpellEffectInterface +0x1960,
//   SpellEffectInterpolator +0x1964, PRainSplashParticle +0x1968.
// Callees: ~CBaseIntelligentPointer 0x00A01C10, ~CScreenEffectPriorityStack 0x008814C0 (the interpolator's member at +0x90),
//   operator delete 0x00BFE9BC (both scoped pointers), std::__destroy_aux 0x006B6FF0 (the pallette's day sets), 0x006B6610
//   (the theme channels) and 0x006B6F50 (the priority stacks), free 0x00BFEA14 (the three vector blocks), the vector
//   destructor iterator 0x00401400 with ~CIDrawEnvironmentTextureSet 0x006B5B00 (each blended theme's TextureSets[3]),
//   CBase::~CBase 0x0099A300.
// Shape: the compiler-generated member destruction of an empty user destructor. CEnvironment is `__declspec(novtable)` --
//   retail stores no vtable of its own before the member destructors (GOTCHAS 24f). The interface's destructor is
//   trivial, so its `delete` is a bare operator delete; the interpolator's is not, so it is null-tested. Each STLport
//   `_Destroy(first, last)` passes an empty `__false_type` homed in the `push ecx` slot.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void operator delete(void* block);                          // 0x00BFE9BC

class CBase {
public:
    virtual ~CBase();                                       // 0x0099A300
};

class CThing;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    void* PData;                                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

template <class T> class CArray : public std::vector<T> { };

template <class T> class scoped_ptr {
    T* ptr;
public:
    ~scoped_ptr() { delete ptr; }
};

class CRGBFloatColour {
public:
    float R, G, B, A;
};
class C3DVector {
public:
    float X, Y, Z;
};

class CIDrawEnvironmentTextureSet {
public:
    CArray<long>  Textures;                                 // +0x00
    CArray<float> Blends;                                   // +0x0C
    ~CIDrawEnvironmentTextureSet();                         // 0x006B5B00
};

class CBlendedEnvironmentTheme {                            // 0x1F4 bytes
public:
    float                       TimeOfDay;                  // +0x000
    CRGBFloatColour             Colours[19];                // +0x004
    float                       Scalers[30];                // +0x134
    CIDrawEnvironmentTextureSet TextureSets[3];             // +0x1AC
};

class CEnvironmentGameTickState {                           // 0xC20 bytes
public:
    CBlendedEnvironmentTheme BlendedTheme[6];               // +0x000
    float     Time;                                         // +0xBB8
    float     WindSpeed;                                    // +0xBBC
    C3DVector WindDirection;                                // +0xBC0
    float     ThemeWindSpeed;                               // +0xBCC
    C3DVector ThemeWindDirection;                           // +0xBD0
    C3DVector SunVector;                                    // +0xBDC
    C3DVector MoonVector;                                   // +0xBE8
    C3DVector DiffuseVector;                                // +0xBF4
    C3DVector ShadowVector;                                 // +0xC00
    bool      OverrideWindDirection;                        // +0xC0C
    C3DVector OldWindDirection;                             // +0xC10 (PDB +0xC0D: packed in the debug build)
    float     OldWindSpeed;                                 // +0xC1C
};

class CIDrawEnvironmentParams {                             // 0xC0 bytes
public:
    class CThemeChannel {                                   // 0x1F0 bytes
    public:
        CRGBFloatColour             Colours[19];            // +0x000
        float                       Scalers[30];            // +0x130
        CIDrawEnvironmentTextureSet TextureSets[3];         // +0x1A8
    };
    float     Time;                                         // +0x00
    C3DVector WindDirection;                                // +0x04
    float     WindSpeed;                                    // +0x10
    C3DVector ThemeWindDirection;                           // +0x14
    float     ThemeWindSpeed;                               // +0x20
    C3DVector SunVector;                                    // +0x24
    C3DVector MoonVector;                                   // +0x30
    C3DVector DiffuseVector;                                // +0x3C
    C3DVector ShadowVector;                                 // +0x48
    CArray<CThemeChannel> ThemeChannels;                    // +0x54
    unsigned char ScreenEffectParams[0x60];                 // +0x60 CIDrawEnvironmentScreenEffectParams (plain floats)
};

namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry {                 // 48 bytes (destroyed inside 0x006B6F50)
public:
    unsigned char _data[0x30];
};
class CEnvironmentThemePriorityStack {
public:
    CArray<CEnvironmentThemePriorityStackEntry> Entries;    // +0x00
};
class CEnvironmentThemeSetDay {                             // 12 bytes (destroyed inside 0x006B6FF0)
public:
    unsigned char _data[0xC];
};
}

class CEnvironmentThemeResourcePallette {
public:
    CArray<NEnvironmentInternal::CEnvironmentThemeSetDay> DaySets;   // +0x00
};

class CScreenEffectPriorityStack {
public:
    ~CScreenEffectPriorityStack();                          // 0x008814C0
};

class CEnvironmentSpellEffectInterface {                    // trivially destructible
public:
    unsigned char _data[0xF8];
};

class CEnvironmentSpellEffectInterpolator {
public:
    unsigned char _pad00[0x90];
    CScreenEffectPriorityStack ScreenEffectPriorityStack;   // +0x90
};

class CGameDefinitionManager;

class __declspec(novtable) CEnvironment : public CBase {
public:
    CGameDefinitionManager* DefinitionManager;              // +0x04
    float  SunlightAttenuatorAngleFadeStart;                // +0x08
    float  SunlightAttenuatorAngleFadeEnd;                  // +0x0C
    long   RegionTheme;                                     // +0x10
    double LightningStartTime;                              // +0x18
    float  LightningFadeInDuration;                         // +0x20
    float  LightningFadeOutDuration;                        // +0x24
    float  LightningFlashDuration;                          // +0x28
    float  LightningRainThreshold;                          // +0x2C
    long   LightningTheme;                                  // +0x30
    float  LightningCurrentFlashChance;                     // +0x34
    unsigned long LightningRandomSeed;                      // +0x38
    bool   ForcedColours;                                   // +0x3C
    bool   ForcedNonColourValues;                           // +0x3D
    bool   ForcedEnvironmentTheme;                          // +0x3E
    float  DiffuseClampAngle;                               // +0x40
    bool   PassedFirstUpdate;                               // +0x44
    bool   RainEnabled;                                     // +0x45
    CArray<NEnvironmentInternal::CEnvironmentThemePriorityStack> PriorityStackChannels;   // +0x48
    CEnvironmentGameTickState GameTickState;                // +0x54
    CEnvironmentGameTickState PrevGameTickState;            // +0xC74
    CIDrawEnvironmentParams InterpolatedEnvironmentParams;  // +0x1894
    CEnvironmentThemeResourcePallette ThemeSet;             // +0x1954
    scoped_ptr<CEnvironmentSpellEffectInterface> SpellEffectInterface;         // +0x1960
    scoped_ptr<CEnvironmentSpellEffectInterpolator> SpellEffectInterpolator;   // +0x1964
    CIntelligentPointer<CThing> PRainSplashParticle;        // +0x1968
    virtual ~CEnvironment();
};

CEnvironment::~CEnvironment()
{
}