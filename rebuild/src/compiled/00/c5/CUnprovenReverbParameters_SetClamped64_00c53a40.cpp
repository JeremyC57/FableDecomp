// CUnprovenReverbParameters::SetClamped64(float value) -- Clamp an audio parameter and mark its dirty bit.
// Leaf: ?SetClamped64@CUnprovenReverbParameters@@QAEXM@Z
// Identity: Own callers 00c53ef3 / 00c5437d / 00c5465a; values clamped to[0.1f,100.0f] and corresponding dirty mask 0x1. Descriptive parameter identity only.
// Layout: no active shared header. Caller passes its subobject+0x290 to all three methods; common flags at0, float parameters40/48/100. Remaining members are not modeled. Libpk has no original PDB class name evidence here.
class CUnprovenReverbParameters{public:unsigned long DirtyFlags;unsigned char Unmodeled04[36];float Float28;unsigned char Unmodeled2C[4];float Float30;unsigned char Unmodeled34[48];float Float64;void SetClamped28(float);void SetClamped30(float);void SetClamped64(float);};
void CUnprovenReverbParameters::SetClamped64(float value){Float64=value<0.1f?0.1f:(value>100.0f?100.0f:value);DirtyFlags|=0x1;}