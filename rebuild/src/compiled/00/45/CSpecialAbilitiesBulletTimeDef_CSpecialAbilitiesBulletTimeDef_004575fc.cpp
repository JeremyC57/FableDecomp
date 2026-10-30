// CSpecialAbilitiesBulletTimeDef::~CSpecialAbilitiesBulletTimeDef() -- free the seven screen-filter float vectors (last declared first), then the CParentDefClassBase part
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004575FC, length 130 bytes.
// Leaf: ??1CSpecialAbilitiesBulletTimeDef
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (`pop ecx` after each free).
// Label correction: the catalog "~CPatchTesselationEdgeStrip" is propagated over byte-similar def destructors. The only
// caller is 0x004575E3 inside the deleting destructor 0x004575E0, which is slot 0 of CSpecialAbilitiesBulletTimeDef's vtable
// 0x01234264 (slot 20, 0x004575DA, returns the retail size 0x18c).
// Callees: free @ 0x00BFEA14 (x7, the inline vector<float> destructors), the CParentDefClassBase destructor @ 0x00430300
// (manifest label "CTCInventoryItem::OnDie" is propagated; tail jump).
// Layout: Ego_r.pdb CSpecialAbilitiesBulletTimeDef minus 4 (12-byte retail containers): the seven ScreenFilter vectors at
// +0x130 .. +0x178, 12 bytes apart.
// Shape: an empty destructor; the members die in reverse declaration order through a `__forceinline` vector<float>
// destructor (a size TU keeps a plain inline one out of line), then the out-of-line base destructor as a sibling call. As
// in the landed 0x00459ABE the base is modelled without its vptr: the def vtable is not re-stored here.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* memory);        // 0x00BFEA14
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
struct CFloatVector {                              // retail std::vector<float> (12 bytes)
    float* m_first;
    float* m_last;
    float* m_end;
    __forceinline ~CFloatVector() { if (m_first) free(m_first); }
};
// Model: the retail def chain from engine/CParentDefClassBase.h; the derived def is `__declspec(novtable)` (no vtable
//   store of its own in the destructor, as 0x0047747C) and its CDefString members are the 4-byte TablePos class.
class CDefString { public:                         // a def-name table position word (4 bytes)
    long TablePos;
};
class C3DVector { public: float X, Y, Z; };
class __declspec(novtable) CSpecialAbilitiesBulletTimeDef : public CParentDefClassBase { public:   // vtable 0x01234264, retail 0x18c bytes
    virtual void Transfer(CPersistContext& context);              // ?Transfer@CSpecialAbilitiesBulletTimeDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase* other);                // ?Copy@CSpecialAbilitiesBulletTimeDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CSpecialAbilitiesBulletTimeDef@@UAEKXZ
    float        SlowTimeDuration[4];              // +0x3c
    long         BulletTimeSlowdownMultiplier[4];  // +0x4c
    float        LevelUpTimings[3];                // +0x5c
    float        CamDist[4];                       // +0x68
    float        CamHeight[4];                     // +0x78
    float        CamFOV[4];                        // +0x88
    float        CamSwitchTime[4];                 // +0x98
    float        CastCamDist;                      // +0xa8
    float        CastCamHeight;                    // +0xac
    float        CastCamFOV;                       // +0xb0
    float        CastCamSwitchTime;                // +0xb4
    CDefString   ChargeSound;                      // +0xb8
    CDefString   PowerUp1Sound;                    // +0xbc
    CDefString   PowerUp2Sound;                    // +0xc0
    CDefString   PowerUp3Sound;                    // +0xc4
    CDefString   InitSound;                        // +0xc8
    CDefString   LoopSound;                        // +0xcc
    CDefString   EndSound;                         // +0xd0
    C3DVector    RadialBlurThresholdCol;           // +0xd4
    float        RadialBlurLength;                 // +0xe0
    C3DVector    RadialBlurIntensityCol;           // +0xe4
    float        RadialBlurFade;                   // +0xf0
    float        RadialBlurRange;                  // +0xf4
    float        RadialBlurTwist;                  // +0xf8
    float        RadialBlurFadeTimeSecs;           // +0xfc
    C3DVector    DamageRadialBlurThresholdCol;     // +0x100
    float        DamageRadialBlurLength;           // +0x10c
    C3DVector    DamageRadialBlurIntensityCol;     // +0x110
    float        DamageRadialBlurFade;             // +0x11c
    float        DamageRadialBlurRange;            // +0x120
    float        DamageRadialBlurTwist;            // +0x124
    float        DamageRadialBlurFadeTimeSecs;     // +0x128
    float        HeroCombatSlowness;               // +0x12c
    CFloatVector ScreenFilterAlpha;                // +0x130
    CFloatVector ScreenFilterSaturation;           // +0x13c
    CFloatVector ScreenFilterContrast;             // +0x148
    CFloatVector ScreenFilterBrightness;           // +0x154
    CFloatVector ScreenFilterTintR;                // +0x160
    CFloatVector ScreenFilterTintG;                // +0x16c
    CFloatVector ScreenFilterTintB;                // +0x178
    float        ScreenFilterFadeInSecs;           // +0x184
    float        ScreenFilterFadeOutSecs;          // +0x188
    ~CSpecialAbilitiesBulletTimeDef();
};
CSpecialAbilitiesBulletTimeDef::~CSpecialAbilitiesBulletTimeDef()
{
}