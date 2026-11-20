// Behaviour test for 0x008C2D60 CCountedPointer<NCreatureMode::CModeJackOfBladesScream>::DeleteData(void*) (NF157, 2026-11-20: rewritten on
// the shared engine/CCountedPointer.h for the PDB leaf; the old test called the free stand-in DeleteData(IData*)): a null pointer is ignored,
// a live object is destroyed once through its virtual destructor (the scalar deleting destructor, slot 0, flag 1).
#include <stdio.h>
#include "engine/CCountedPointer.h"

static int g_calls = 0;
namespace NCreatureMode {
class CModeJackOfBladesScream {                          // the mode class has no shared header; the test needs only its virtual destructor
public:
    virtual ~CModeJackOfBladesScream() { g_calls++; }
};
}

int main()
{
    CCountedPointer<NCreatureMode::CModeJackOfBladesScream>::DeleteData(new NCreatureMode::CModeJackOfBladesScream);
    CCountedPointer<NCreatureMode::CModeJackOfBladesScream>::DeleteData(0);
    if (g_calls == 1)
        printf("DELETEDATA_OK\n");
    else
        printf("DELETEDATA_FAIL calls=%d\n", g_calls);
    return g_calls == 1 ? 0 : 1;
}
