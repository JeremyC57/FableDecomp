// CGameScriptInterface::RadialBlurFadeTo(float time, C3DVector center, float a, C3DVector areaCenter, float b, float c, float d) const -- script call: starts a positioned radial-blur fade on the environment's spell-effect interpolator at priority 1
// Leaf: ?RadialBlurFadeTo@CGameScriptInterface@@UBEPAXMVC3DVector@@M0MMM@Z
// Retail: Fable.exe @ 0x008900D0, 57 bytes, thiscall `ret 2Ch` (the two vectors by value).
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 668 holds this body; slot 668 is
//   ?RadialBlurFadeTo@CGameScriptInterface@@UBEPAXMVC3DVector@@M0MMM@Z (ghidra_out/cgsi_slots_aligned.tsv).
// Chain: World (+0x04)->GetEnvironment() (inline, CWorld::PEnvironment +0x18; FableWin.pdb ?GetEnvironment@CWorld@@QAEAAVCEnvironment@@XZ)
//   .GetSpellEffectInterpolator() (inline, CEnvironment::SpellEffectInterpolator retail +0x1964, Ego_r.pdb +0x1A90;
//   ?GetSpellEffectInterpolator@CEnvironment@@QAEAAVCEnvironmentSpellEffectInterpolator@@XZ), priority 1 (enumerator
//   name not in the name sets). Callees (FableWin.pdb): 0x006B8730
//   ?RadialBlurFadeTo@CEnvironmentSpellEffectInterpolator@@QAEPAXMW4ScreenEffectPriority@NScreenEffect@@MMMMMM@Z and 0x006B8550
//   ?RadialBlurFadeTo@CEnvironmentSpellEffectInterpolator@@QAEPAXMW4ScreenEffectPriority@NScreenEffect@@ABVC3DVector@@M1MMM@Z
//   (told apart by their callers: 0x006B8730 is called by the all-float overload, 0x006B8550 by the C3DVector one; the
//   manifest labels 0x006B8550 as an STLport _Ucopy, which is wrong). Parameter names are descriptive.
class C3DVector { public: float X, Y, Z; };
namespace NScreenEffect { enum ScreenEffectPriority { }; }
class CEnvironmentSpellEffectInterpolator {
public:
    void* RadialBlurFadeTo(float time, NScreenEffect::ScreenEffectPriority priority, float a, float b, float c, float d, float e, float f);   // 0x006B8730
    void* RadialBlurFadeTo(float time, NScreenEffect::ScreenEffectPriority priority, const C3DVector& center, float a, const C3DVector& areaCenter, float b, float c, float d);   // 0x006B8550
};
class CEnvironment {
public:
    unsigned char _pad00[0x1964];
    CEnvironmentSpellEffectInterpolator* SpellEffectInterpolator;   // +0x1964 scoped_ptr (Ego_r.pdb +0x1A90)
    CEnvironmentSpellEffectInterpolator& GetSpellEffectInterpolator() { return *SpellEffectInterpolator; }
};
class CWorld {
public:
    unsigned char _pad00[0x18];
    CEnvironment* PEnvironment;                          // +0x18 scoped_ptr<CEnvironment> (Ego_r.pdb +0x18)
    CEnvironment& GetEnvironment() { return *PEnvironment; }
};
class CGameScriptInterface {
public:
    virtual void* RadialBlurFadeTo(float time, C3DVector center, float a, C3DVector areaCenter, float b, float c, float d) const;
    CWorld* World;                                       // +0x04
};
void* CGameScriptInterface::RadialBlurFadeTo(float time, C3DVector center, float a, C3DVector areaCenter, float b, float c, float d) const
{
    return World->GetEnvironment().GetSpellEffectInterpolator().RadialBlurFadeTo(time, (NScreenEffect::ScreenEffectPriority)1, center, a, areaCenter, b, c, d);
}