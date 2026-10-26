// Local model: retail CText holds Value.PStringData at +0x15c (STLport 4.6 layout, engine/CText.h), so the
// flag byte this function tests at +0x15c is not a CText member; the class attribution is suspect
// (flagged in the layout pass). Only the byte at +0x15c is modelled.
#include "rebuild_abi.h"
struct CTextFlagOwner { unsigned char _pad_0x000[0x15c]; unsigned char flags; };
#include <cstdio>

struct CText_Methods : CTextFlagOwner {
    char FABLE_FASTCALL get();
};

int main()
{
    CText_Methods a;
    a.flags = 0x04; if (a.get() != 1) { std::printf("BAD1\n"); return 1; }
    a.flags = 0x08; if (a.get() != 1) { std::printf("BAD2\n"); return 1; }
    a.flags = 0x00; if (a.get() != 0) { std::printf("BAD3\n"); return 1; }
    a.flags = 0x0c; if (a.get() != 1) { std::printf("BAD4\n"); return 1; }
    std::printf("GETALIGN_00661e80_OK\n");
    return 0;
}
