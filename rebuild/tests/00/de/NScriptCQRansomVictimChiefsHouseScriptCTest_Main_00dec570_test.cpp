// Naming wave 35: the row now defines ?Main@CTest@CQ_RansomVictimChiefsHouseScript@NScript@@EAEXXZ (it was landed as ?CQ_SunnyvaleMasterScript__Main@@YIXPAUCQ_SunnyvaleMasterScript@@@Z).
// This behaviour model keeps the old spelling; the linker maps it onto the row's symbol (same thiscall/ecx ABI, same layout).
#pragma comment(linker, "/alternatename:?CQ_SunnyvaleMasterScript__Main@@YIXPAUCQ_SunnyvaleMasterScript@@@Z=?Main@CTest@CQ_RansomVictimChiefsHouseScript@NScript@@EAEXXZ")
#include <cstdio>

struct Sub;

struct SubVtbl {
    void (__fastcall *slot0)(Sub*);
    void (__fastcall *slot1)(Sub*);
    void (__fastcall *slot2)(Sub*);
    void (__fastcall *slot3)(Sub*);
    void (__fastcall *slot4)(Sub*);
    void (__fastcall *slot5)(Sub*);
    void (__fastcall *slot6)(Sub*);
    void (__fastcall *slot7)(Sub*);
};

struct Sub {
    SubVtbl* vtbl;
};

struct CQ_SunnyvaleMasterScript {
    void* f0;
    Sub*  f4;
};

class CEntityScriptBase { public: bool IsActiveThreadTerminating() const; };   // 0x00F35B30 (the row calls it so)

void __fastcall CQ_SunnyvaleMasterScript__Main(CQ_SunnyvaleMasterScript* self);

static int g_slotCalls = 0;
static int g_helperCalls = 0;

void __fastcall test_slot7(Sub*) { ++g_slotCalls; }

bool CEntityScriptBase::IsActiveThreadTerminating() const {
    ++g_helperCalls;
    // return false twice, then true -> 3 iterations
    return g_helperCalls >= 3;
}

int main()
{
    SubVtbl vt = {0,0,0,0,0,0,0, test_slot7};
    Sub theSub; theSub.vtbl = &vt;
    CQ_SunnyvaleMasterScript obj; obj.f0 = 0; obj.f4 = &theSub;

    CQ_SunnyvaleMasterScript__Main(&obj);

    if (g_slotCalls == 3 && g_helperCalls == 3)
        printf("00dec570_TEST PASS\n");
    else
        printf("FAIL slot=%d helper=%d\n", g_slotCalls, g_helperCalls);
    return 0;
}