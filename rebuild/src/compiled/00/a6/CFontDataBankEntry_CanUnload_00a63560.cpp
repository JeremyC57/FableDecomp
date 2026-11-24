// CFontDataBankEntry_Methods::CanUnload  @ retail 0x00a63560
// __fastcall bool CanUnload(CFontDataBankEntry* this)

// Leaf: ?CanUnload@CFontDataBankEntry@@UBE_NXZ
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CFontDataBankEntry.h under FABLE_RETAIL_CFONTDATABANKENTRY_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CFontDataBankEntry_Methods::CanUnload); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CFONTDATABANKENTRY_VIRTUAL_MODEL
#include "engine/CFontDataBankEntry.h"  // retyped onto the PDB layout; byte parity re-verified
enum EFontLoadState { FONT_LOADED = 1 };

class CCPPointerInfo { public:
    EFontLoadState state; // [+0]
};

bool CFontDataBankEntry::CanUnload() const {
    if (this->Font_Data != 0 && this->Font_Info != 0)
    {
        int s = this->Font_Info->state;
        if (s == FONT_LOADED)
            return s == FONT_LOADED;
    }
    return false;
}
