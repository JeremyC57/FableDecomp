// Behaviour test for 0x008D5000 CCreatureAction_FireMissileWeapon::FrameUpdate (NF64 round 7): the same shared headers as the row;
// the base CCreatureActionBase::FrameUpdate (?FrameUpdate@CCreatureActionBase@@MAEXXZ) is stubbed here to record the call.
// No object is constructed: the qualified (non-virtual) call runs the row's body on a zeroed buffer.
#include <stdio.h>
#include <string.h>
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_FIREMISSILEWEAPON
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureAction_FireMissileWeapon.h"

static int g_base_called = 0;
void CCreatureActionBase::FrameUpdate() { g_base_called = 1; }

struct CTestAction : CCreatureAction_FireMissileWeapon {
    void Run() { CCreatureAction_FireMissileWeapon::FrameUpdate(); }
    long& Timer() { return *(long*)((unsigned char*)this + 0xC0); }   // DelayTimer (private; retail +0xC0, as the landed test)
};

int main()
{
    static unsigned char buffer[sizeof(CCreatureAction_FireMissileWeapon) + 16];
    memset(buffer, 0, sizeof(buffer));
    CTestAction* a = (CTestAction*)buffer;
    a->Timer() = 3;
    a->Run();
    if (a->Timer() != 2 || g_base_called) { printf("008d5000_TEST FAIL dec\n"); return 1; }
    a->Run();
    if (a->Timer() != 1 || g_base_called) { printf("008d5000_TEST FAIL dec2\n"); return 1; }
    a->Timer() = 0;
    a->Run();
    if (!g_base_called || a->Timer() != 0) { printf("008d5000_TEST FAIL base\n"); return 1; }
    printf("OK_008d5000\n");
    return 0;
}
