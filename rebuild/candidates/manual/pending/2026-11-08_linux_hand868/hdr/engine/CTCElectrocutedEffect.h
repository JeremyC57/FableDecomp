#ifndef FABLE_ENGINE_CTCELECTROCUTEDEFFECT_H
#define FABLE_ENGINE_CTCELECTROCUTEDEFFECT_H
// CTCElectrocutedEffect (chunk 868, row 0x007A1B20): only the member the will-lightning beam calls is modelled. labels_*.tsv
// 0x007CE7A0 CTCElectrocutedEffect::Reset (Ego_r.pdb ?Reset@CTCElectrocutedEffect@@QAEXXZ). Ego_r.pdb layout (sizeof 0x38 in the
// PDB, 0x34 in retail with the 12-byte STLport vector): PDef +0x0c, CurTime +0x10, FadeInTime +0x14, FadeOutTimeStart +0x18,
// FadeOutTime +0x1c, PEffect +0x20, TintList +0x28; the beam only holds the component pointer (GetTC / AddTC "CTCElectrocutedEffect").
class CTCElectrocutedEffect {
public:
    void Reset();                                            // 0x007CE7A0
};
#endif
