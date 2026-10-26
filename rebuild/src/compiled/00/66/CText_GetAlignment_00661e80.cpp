// Local model: retail CText holds Value.PStringData at +0x15c (STLport 4.6 layout, engine/CText.h), so the
// flag byte this function tests at +0x15c is not a CText member; the class attribution is suspect
// (flagged in the layout pass). Only the byte at +0x15c is modelled.
#include "rebuild_abi.h"
struct CTextFlagOwner { unsigned char _pad_0x000[0x15c]; unsigned char flags; };

struct CText_Methods : CTextFlagOwner {
    char FABLE_FASTCALL get();
};

char FABLE_FASTCALL CText_Methods::get()
{
    unsigned char v = this->flags;
    if (v & 4)
        return 1;
    v = (unsigned char)(v >> 3);
    v = (unsigned char)(v & 1);
    return (char)v;
}
