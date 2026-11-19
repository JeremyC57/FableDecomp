def tfirst(s):
    a=s.index('FABLE_FORCEINLINE void CMatrix4x4::AlignedPreScale(')
    b=s.index('FABLE_FORCEINLINE void CMatrix4x4::AlignedPreScaleTransposed(')
    c=s.index('void C3DAnimationManager::ApplyBoneMorphScaling(const C3DVector* bone_morphs')
    return s[:a]+s[b:c]+s[a:b]+s[c:]
def after(s):
    a=s.index('FABLE_FORCEINLINE void CMatrix4x4::AlignedPreScale(')
    c=s.index('void C3DAnimationManager::ApplyBoneMorphScaling(const C3DVector* bone_morphs')
    return s[:a]+s[c:]+'\n'+s[a:c]
def idx(s):
    return s.replace('m(0, 0) *= scale.X; m(0, 1) *= scale.X; m(0, 2) *= scale.X;','m(0, 0) *= scale[0]; m(0, 1) *= scale[0]; m(0, 2) *= scale[0];')
VARIANTS=[('tfirst',tfirst),('after',after)]
