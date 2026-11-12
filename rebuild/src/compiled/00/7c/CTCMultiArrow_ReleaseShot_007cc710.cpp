// CTCMultiArrow::ReleaseShot() -- one shot fired: when a whole volley has gone, remove (and kill) the newest ammo effect, or
// count down the pending ammo effects when none is showing; clean up after the last shot.
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CC710, 180 bytes (plain `ret`; Ego_r.pdb ?ReleaseShot@CTCMultiArrow@@QAEXXZ).
// Leaf: ?ReleaseShot@CTCMultiArrow@@QAEXXZ
// Callees: CBaseIntelligentPointer() @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90 (the copy of the back element,
//   CIntelligentPointer<CThing> vtable 0x01238C6C), CThing::Kill(bool) @ 0x004C9B80, the element's virtual destructor (slot 0,
//   `push 0`) in pop_back, ~CBaseIntelligentPointer @ 0x00A01C10, FinalCleanup() @ 0x007CC410.
// Layout: Ego_r.pdb CTCMultiArrow (shared header engine/CTCMultiArrow.h, VSLOTS3 block): NumberOfShots +0x10, NumberInVolley
//   +0x14, NumOfVolleys +0x18, CreateAmmoEffectCount +0x24, AmmoEffects +0x30 (STLport vector of CIntelligentPointer<CThing>).
// Shape: the port onto std::vector fixed the empty() test (pointer compare); the CIntelligentPointer COPY/GET ORIGINAL_CHAIN
//   spellings (SetPointer(item.GetPointer()), GetPointer()) put the volley count in ebx and the end pointer in edi as retail.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTCMULTIARROW_VSLOTS2
#define FABLE_RETAIL_CTCMULTIARROW_VSLOTS3
#define FABLE_RETAIL_CTCMULTIARROW_RELEASESHOT
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CTCMultiArrow.h"

void CTCMultiArrow::ReleaseShot()
{
    --NumberOfShots;
    long volley = NumberOfShots / NumberInVolley;
    if (volley < NumOfVolleys) {
        if (AmmoEffects.empty())
            --CreateAmmoEffectCount;
        else {
            CIntelligentPointer<CThing> effect = AmmoEffects.back();
            if (effect.Get() && !effect->IsDead())
                effect->Kill(true);
            AmmoEffects.pop_back();
        }
        NumOfVolleys = volley;
    }
    if (NumberOfShots <= 0)
        FinalCleanup();
}