// Behaviour test for 0x004457F0 CGamePlayerInterface::Update(): links the row's object (the test defines no body of its own) and
// checks that Update clears HasCalledInputUpdateThisFrame (+0x894) and touches no neighbouring byte. Plain C++ on the shared header.
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDISKFILEWIN32
class CEnginePrimitivePrepareInfo;
#include "engine/CGamePlayerInterface.h"

static unsigned char Storage[sizeof(CGamePlayerInterface) + 8];

int main()
{
    int fails = 0;
    memset(Storage, 0xFF, sizeof(Storage));
    CGamePlayerInterface* gpi = reinterpret_cast<CGamePlayerInterface*>(Storage);
    gpi->Update();
    if (Storage[0x894] != 0) { printf("FAIL flag=%d\n", (int)Storage[0x894]); ++fails; }
    if (Storage[0x893] != 0xFF || Storage[0x895] != 0xFF) { printf("FAIL neighbour clobbered\n"); ++fails; }
    for (unsigned i = 0; i < sizeof(Storage); ++i)
        if (i != 0x894 && Storage[i] != 0xFF) { printf("FAIL byte %u changed\n", i); ++fails; break; }
    Storage[0x894] = 1;
    gpi->Update();
    if (Storage[0x894] != 0) { printf("FAIL second call\n"); ++fails; }
    if (fails) return 1;
    printf("OK_0x004457f0\n");
    return 0;
}
