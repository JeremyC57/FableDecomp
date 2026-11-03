#ifndef FABLE_ENGINE_CSLIDERANIMATOR_H
#define FABLE_ENGINE_CSLIDERANIMATOR_H
// CSliderAnimator (chunk 735, row 0x00577AC0): Ego_r.pdb class CSliderAnimator, 28 bytes: pSlider +0, StartValue +4,
// EndValue +8, MaxValue +0xC, a +0x10, Overall +0x14, Play +0x18, Finished +0x19 (struct_layouts_egor.tsv). The
// constructor is out of line at 0x0056E545 (landed as CRecord_0056e545: five dword arguments into +0/+4/+8/+0xC/+0x14, a =
// 0.0f, both flags false); the row passes the slider component and four floats. Update 0x0056EB48 / ExtractParenthesis
// 0x0056EAD6 are the other out-of-line members (Ego_r.pdb ?Update@CSliderAnimator@@QAEXXZ,
// ?ExtractParenthesis@CSliderAnimator@@QAE?AVCWideString@@PAVCText@NUISystem@@@Z).
#include "rebuild_abi.h"
namespace NUISystem { class CComponent; }
class CSliderAnimator {
public:
    CSliderAnimator(NUISystem::CComponent* slider, float startValue, float endValue, float maxValue, float overall);  // 0x0056E545
    void Update();                                      // 0x0056EB48
    NUISystem::CComponent* pSlider;                     // +0x00
    float StartValue;                                   // +0x04
    float EndValue;                                     // +0x08
    float MaxValue;                                     // +0x0c
    float a;                                            // +0x10
    float Overall;                                      // +0x14
    bool Play;                                          // +0x18
    bool Finished;                                      // +0x19
};
FABLE_STATIC_ASSERT(sizeof(CSliderAnimator) == 0x1c);
#endif
