// CTools::GetRollingAverage<C3DVector>(const C3DVector&, const C3DVector&, float) -- folds a new sample into a rolling average: (average * count + value) / (count + 1), returned by value
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084EAF0, length 101 bytes (`ret 10h`: hidden result, two vector
// references, the float count; `this` unused). The C3DVector twin of the landed C2DVector instantiation 0x0069BF30.
// Constant: 0x0122DED8 = 1.0f (twice: `count + 1.0f` and the reciprocal `1.0f / (count + 1.0f)`).
// Shape: inline C3DVector operators with a user constructor (hidden-pointer result); `operator/` multiplies by one
// reciprocal.
// Leaf: ??$GetRollingAverage@VC3DVector@@@CTools@NCameraModes@@QBE?AVC3DVector@@ABV2@0M@Z
// Name fix (agentNF5 2026-11-10): Ego_r.pdb / FableWin.pdb ??$GetRollingAverage@VC3DVector@@@CTools@NCameraModes@@QBE?AVC3DVector@@ABV2@0M@Z
//   (CTools lives in NCameraModes and the member template is const), was the top-level stand-in CTools (QAE); now the shared
//   engine/NCameraModes_CTools.h (FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_AVERAGE + _ROLLING_OUTOFLINE).
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_AVERAGE
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_OUTOFLINE
#include "engine/C3DVector.h"
#include "engine/NCameraModes_CTools.h"
namespace NCameraModes {
template <class T>
T CTools::GetRollingAverage(const T& current_val, const T& new_val, float weight) const
{
    return (current_val * weight + new_val) / (weight + 1.0f);
}
template C3DVector CTools::GetRollingAverage<C3DVector>(const C3DVector&, const C3DVector&, float) const;
}
