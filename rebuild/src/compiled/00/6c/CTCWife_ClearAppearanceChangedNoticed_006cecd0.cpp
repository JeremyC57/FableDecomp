// CTCWife::ClearAppearanceChangedNoticed(EWifeAppearanceCategories*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CECD0, oracle length 20 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
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