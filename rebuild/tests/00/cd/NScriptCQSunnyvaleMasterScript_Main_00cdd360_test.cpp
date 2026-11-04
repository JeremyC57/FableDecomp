// Naming wave 35: the row now defines ?Main@CQ_SunnyvaleMasterScript@NScript@@UAEXXZ (it was landed as ?CQ_SunnyvaleMasterScript_Main@@YIXPAUCQ_SunnyvaleMasterScript@@@Z).
// This behaviour model keeps the old spelling; the linker maps it onto the row's symbol (same thiscall/ecx ABI, same layout).
#pragma comment(linker, "/alternatename:?CQ_SunnyvaleMasterScript_Main@@YIXPAUCQ_SunnyvaleMasterScript@@@Z=?Main@CQ_SunnyvaleMasterScript@NScript@@UAEXXZ")
// The model's stand-in for 0x00CB7940 (extern "C" Q_Sunny_cb7940) answers the row's CScriptBase::IsActiveThreadTerminating call.
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ=@Q_Sunny_cb7940@4")
#include <cstdio>

struct IVtblObj;
struct IVtbl {
    void (__fastcall *slot0)(IVtblObj*);
    void (__fastcall *slot1)(IVtblObj*);
    void (__fastcall *slot2)(IVtblObj*);
    void (__fastcall *slot3)(IVtblObj*);
    void (__fastcall *slot4)(IVtblObj*);
    void (__fastcall *slot5)(IVtblObj*);
    void (__fastcall *slot6)(IVtblObj*);
    void (__fastcall *slot7)(IVtblObj*);
};
struct IVtblObj { IVtbl* vtbl; };

struct CQ_SunnyvaleMasterScript {
    char pad[0x40];
    IVtblObj* f40;
};

static int g_ticks = 0;
static int g_helper_calls = 0;

void __fastcall my_slot7(IVtblObj*) { g_ticks++; }

extern "C" char __fastcall Q_Sunny_cb7940(CQ_SunnyvaleMasterScript*) {
    g_helper_calls++;
    // return 0 for first 2 calls (keep looping), nonzero on 3rd (exit)
    return (g_helper_calls >= 3) ? (char)1 : (char)0;
}

void __fastcall CQ_SunnyvaleMasterScript_Main(CQ_SunnyvaleMasterScript* self);

int main() {
    IVtbl vt = {0,0,0,0,0,0,0,my_slot7};
    IVtblObj obj; obj.vtbl = &vt;
    CQ_SunnyvaleMasterScript s; s.f40 = &obj;

    CQ_SunnyvaleMasterScript_Main(&s);

    // do-while: body runs, then check. exits when helper != 0 (3rd call).
    // So body (slot7) runs 3 times, helper runs 3 times.
    if (g_ticks == 3 && g_helper_calls == 3) {
        printf("00cdd360_TEST PASS\n");
    } else {
        printf("FAIL ticks=%d helper=%d\n", g_ticks, g_helper_calls);
    }
    return 0;
}