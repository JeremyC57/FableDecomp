// CGameScriptInterface::DisplacementMonochromeEffectColourFadeTo(float, const CRGBFloatColour&) const -- starts the displacement-monochrome screen effect's colour fade at script priority
// Leaf: DisplacementMonochromeEffectColourFadeTo@CGameScriptInterface
// Retail: Fable.exe @ 0x008901C0, 32 bytes, `ret 8`. Calls CEnvironmentSpellEffectInterpolator::DisplacementMonochromeEffectColourFadeTo
// (float, NScreenEffect::ScreenEffectPriority, const CRGBFloatColour&) @ 0x006B8370 (catalogued as a std::_Ucopy) with priority 1 and
// returns its handle (Ego_r.pdb: both return void*).
// Shape: the pointer chain goes through inline const getters; direct member reads load the arguments before World.
// Layout: CGameScriptInterface World +0x04 (engine/CGameScriptInterface.h); CWorld PEnvironment +0x18 (Ego_r.pdb, scoped_ptr);
// retail CEnvironment SpellEffectInterpolator +0x1964 (Ego_r.pdb +0x1a90; the 0x12c gap is the retail STL shrink before it).
class CRGBFloatColour;
namespace NScreenEffect { enum ScreenEffectPriority { SCREEN_EFFECT_PRIORITY_SCRIPT = 1, SCREEN_EFFECT_PRIORITY_FORCE_DWORD = 0x7fffffff }; }
class CEnvironmentSpellEffectInterpolator {
public:
    void* DisplacementMonochromeEffectColourFadeTo(float time, NScreenEffect::ScreenEffectPriority priority, const CRGBFloatColour& colour);   // 0x006B8370
};
class CEnvironment {
public:
    unsigned char                        _pad00[0x1964];
    CEnvironmentSpellEffectInterpolator* SpellEffectInterpolator;   // +0x1964
    CEnvironmentSpellEffectInterpolator* GetSpellEffectInterpolator() const { return SpellEffectInterpolator; }
};
class CWorld {
public:
    unsigned char _pad00[0x18];
    CEnvironment* PEnvironment;                          // +0x18
    CEnvironment* GetEnvironment() const { return PEnvironment; }
};
class CGameScriptInterface {
public:
    virtual void* DisplacementMonochromeEffectColourFadeTo(float time, const CRGBFloatColour& colour) const;
    CWorld* World;                                       // +0x04
    CWorld* GetWorld() const { return World; }
};

void* CGameScriptInterface::DisplacementMonochromeEffectColourFadeTo(float time, const CRGBFloatColour& colour) const
{
    return GetWorld()->GetEnvironment()->GetSpellEffectInterpolator()->DisplacementMonochromeEffectColourFadeTo(time, NScreenEffect::SCREEN_EFFECT_PRIORITY_SCRIPT, colour);
}