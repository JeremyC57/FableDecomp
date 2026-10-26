// CTCHeroStats::GetStaminaPercentage() const -- the hero's stamina as a fraction of its maximum (0 when the maximum is not positive)
// Leaf: ?GetStaminaPercentage@CTCHeroStats@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057A57A, 33 bytes, thiscall `ret`, float in st(0). Name from FableWin.pdb
//   (`?GetStaminaPercentage@CTCHeroStats@@QBEMXZ`); sibling of the landed GetMoralityPercentage 0x0057A690. Label
//   correction: the catalog "WillEnergy_ComputePercentage" is a guess.
// Layout: retail CTCHeroStats Stamina +0x58, MaxStamina +0x5C (Ego_r.pdb +0x60/+0x64: the two containers ahead of
//   them are 4 bytes smaller each in retail; as the landed SetMaxStamina 0x0057A730), hence the retail view here
//   rather than engine/CTCHeroStats.h.
// Shape: `float max = (float)MaxStamina; if (max > 0.0f) { float fraction = (float)Stamina; fraction /= max;
//   return fraction; } return 0.0f;` -- the maximum converted once and kept on the x87 stack (`fcom`, not
//   `fcomp`), `> 0` as `test ah,41h ; jne`, the in-place `/=` giving retail's `fild ; fdiv st,st(1) ; fstp st(1)`
//   (`return Stamina / max` folds to `fidivr`, a named quotient local to `fdivrp`), the 0.0f pool constant
//   0x0122DEDC (`fld`, never `fldz`) on both the compare and the zero return. Size TU.
// Layout (supersedes any local-model note above): engine/CTCHeroStats.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCHeroStats CTCHeroStatsDonor
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
#pragma optimize("s", on)

class CTCHeroStats : public CTCHeroStatsDonor {
public:
    float GetStaminaPercentage() const;
};

float CTCHeroStats::GetStaminaPercentage() const
{
    float max = (float)MaxStamina;
    if (max > 0.0f) {
        float fraction = (float)Stamina;
        fraction /= max;
        return fraction;
    }
    return 0.0f;
}