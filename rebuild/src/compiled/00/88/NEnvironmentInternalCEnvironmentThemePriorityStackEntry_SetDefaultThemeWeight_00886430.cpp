// NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::SetDefaultThemeWeight(float, bool) -- clamp and lock the base weight, optionally rebalance the theme weights
// Leaf: ?SetDefaultThemeWeight@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@QAEXM_N@Z
// Retail: Fable.exe @ 0x00886430, 70 bytes, thiscall `ret 8`.
// Identity (proven): Ego_r.pdb ?SetDefaultThemeWeight@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@QAEXM_N@Z is the
// class's only (float, bool) member; the body writes Ego_r.pdb BaseWeight (+0, clamped to [0, 1]: pool constants 0x0122DEDC
// = 0.0f, 0x0122DED8 = 1.0f) and BaseWeightLocked (+4 = true) and calls RebalanceWeights 0x00886200 (bound, landed) when the
// flag is set. It sits between the class's TransitionToDefaultTheme (0x00886400) and CBlendedEnvironmentTheme's constructor.
// Label correction: catalog "_gapscan::sub_00886430".
// Callees: 0x00886200 ?RebalanceWeights@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@QAEXXZ.
// Layout: Ego_r.pdb CEnvironmentThemePriorityStackEntry (BaseWeight +0, BaseWeightLocked +4, Themes +8 ...); only the first
// two members are used. No engine header: local model.
// Shape: `x < 0 ? 0 : (x > 1 ? 1 : x)` on the FPU (`test ah,5 ; jp` then `test ah,41h ; jne`), the constants reloaded
// from the float pool.
namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry {
public:
    void SetDefaultThemeWeight(float weight, bool rebalance);
    void RebalanceWeights();                                // 0x00886200

    float BaseWeight;                                       // +0x00
    bool BaseWeightLocked;                                  // +0x04
};

void CEnvironmentThemePriorityStackEntry::SetDefaultThemeWeight(float weight, bool rebalance)
{
    if (weight < 0.0f)
        weight = 0.0f;
    else if (weight > 1.0f)
        weight = 1.0f;
    BaseWeight = weight;
    BaseWeightLocked = true;
    if (rebalance)
        RebalanceWeights();
}
}