#pragma optimize("s",on)
// Leaf: ?vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z
typedef unsigned int uint;

// NF158 (q5208): the +8 member is the shared engine/CCountedPointer.h CCountedPointer<NInventory::CPerItemData>, released through its
//   destructor 0x005C04BF (Ego_r.pdb / FableWin.pdb ??1?$CCountedPointer@VCPerItemData@NInventory@@@@QAE@XZ, the self row's leaf);
//   was the free stand-in Sub_dtor(void*) on `(char*)self + 8`. The row's own class / leaf are unchanged (identity not settled).
#include "engine/CCountedPointer.h"
namespace NInventory { class CPerItemData; }
class CLandscapeBackgroundPatch { public:
    char pad[8];
    CCountedPointer<NInventory::CPerItemData> sub;
};

void* __fastcall vector_deleting_destructor(CLandscapeBackgroundPatch* self, int /*edx*/, uint flags)
{
    self->sub.~CCountedPointer();
    if (flags & 1)
        operator delete(self);
    return self;
}