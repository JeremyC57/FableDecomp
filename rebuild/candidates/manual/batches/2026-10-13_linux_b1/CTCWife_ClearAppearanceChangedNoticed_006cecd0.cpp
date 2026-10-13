// CTCWife::ClearAppearanceChangedNoticed(EWifeAppearanceCategories*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CECD0, oracle length 20 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[esp+4] ; mov edx,[eax] ; mov dword ptr [ecx+edx*4+0A4h],0 ; ret 4
// engine/CTCWife.h (PDB): +0xAC long FrameNoticedChangeInAppearance[16]. The retail store lands
// at +0xA4 + category*4, i.e. FrameNoticedChangeInAppearance[category - 2]: the first two
// EWifeAppearanceCategories values are not tracked, so the enum is rebased by 2 when indexing.
// Clears the "frame the wife last noticed a change" entry for one appearance category.
#include "engine/CTCWife.h"

typedef long EWifeAppearanceCategories;

struct CTCWife_Methods : CTCWife {
    void ClearAppearanceChangedNoticed(EWifeAppearanceCategories* category);
};

void CTCWife_Methods::ClearAppearanceChangedNoticed(EWifeAppearanceCategories* category)
{
    this->FrameNoticedChangeInAppearance[*category - 2] = 0;
}
