// CStreamingFontDataBankEntry::CanUnload @ 0x00A5F230
// __fastcall bool CanUnload(CStreamingFontDataBankEntry* this)

// Leaf: ?CanUnload@CStreamingFontDataBankEntry@@UBE_NXZ
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CStreamingFontDataBankEntry.h under FABLE_RETAIL_CSTREAMINGFONTDATABANKENTRY_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CStreamingFontDataBankEntry_Methods::CanUnload); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CSTREAMINGFONTDATABANKENTRY_VIRTUAL_MODEL
#include "engine/CStreamingFontDataBankEntry.h"  // retyped onto the PDB layout; byte parity re-verified
enum EFontLoadState { FONT_LOADED = 1 };

class CCPPointerInfo { public:
    EFontLoadState state; // [+0]
};

bool CStreamingFontDataBankEntry::CanUnload() const {
    if (this->Font_Data != 0 && this->Font_Info != 0)
    {
        int s = this->Font_Info->state;
        if (s == FONT_LOADED)
            return s == FONT_LOADED;
    }
    return false;
}
