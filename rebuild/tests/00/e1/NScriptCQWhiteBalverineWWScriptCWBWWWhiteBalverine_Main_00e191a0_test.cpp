// Naming wave 35: the row now defines ?Main@CWBWW_WhiteBalverine@CQ_WhiteBalverineWWScript@NScript@@EAEXXZ (it was landed as ?CQ_SunnyvaleMasterScript__Main@@YIXPAUCQ_SunnyvaleMasterScript@@@Z).
// This behaviour model keeps the old spelling; the linker maps it onto the row's symbol (same thiscall/ecx ABI, same layout).
#pragma comment(linker, "/alternatename:?CQ_SunnyvaleMasterScript__Main@@YIXPAUCQ_SunnyvaleMasterScript@@@Z=?Main@CWBWW_WhiteBalverine@CQ_WhiteBalverineWWScript@NScript@@EAEXXZ")
#include <cstdio>

struct IThing;
struct IThingVtbl {
    void* slot00; void* slot04; void* slot08; void* slot0c;
    void* slot10; void* slot14; void* slot18;
    char (__fastcall *slot1c)(IThing* self);
};
struct IThing { IThingVtbl* vtbl; };
struct CQ_SunnyvaleMasterScript { void* field0; IThing* field4; };

static int g_vcalls = 0;
static char __fastcall thing_slot1c(IThing*) { g_vcalls++; return 0; }

static int g_helper_calls = 0;
class CEntityScriptBase { public: bool IsActiveThreadTerminating() const; };   // 0x00F35B30 (the row calls it so)
bool CEntityScriptBase::IsActiveThreadTerminating() const {
    g_helper_calls++;
    // return nonzero on 3rd call so loop runs 3 iterations
    return (g_helper_calls >= 3) ? 1 : 0;
}

void __fastcall CQ_SunnyvaleMasterScript__Main(CQ_SunnyvaleMasterScript* self);

int main() {
    IThingVtbl vt; vt.slot1c = &thing_slot1c;
    IThing thing; thing.vtbl = &vt;
    CQ_SunnyvaleMasterScript obj; obj.field0 = 0; obj.field4 = &thing;

    CQ_SunnyvaleMasterScript__Main(&obj);

    if (g_vcalls == 3 && g_helper_calls == 3)
        printf("00e191a0_TEST PASS\n");
    else
        printf("FAIL vcalls=%d helper=%d\n", g_vcalls, g_helper_calls);
    return 0;
}