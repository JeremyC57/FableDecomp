// Naming wave 35: the row now defines ?Main@CHKC_Maze@CQ_WizardBattleScript@NScript@@EAEXXZ (it was landed as ?Main@@YIXPAUCQ_SunnyvaleMasterScript@@@Z).
// This behaviour model keeps the old spelling; the linker maps it onto the row's symbol (same thiscall/ecx ABI, same layout).
#pragma comment(linker, "/alternatename:?Main@@YIXPAUCQ_SunnyvaleMasterScript@@@Z=?Main@CHKC_Maze@CQ_WizardBattleScript@NScript@@EAEXXZ")
#include <cstdio>

struct IStep {
    struct VT { void* pad[7]; void (__fastcall *slot1c)(IStep*); };
    VT* vtbl;
};
struct CQ_SunnyvaleMasterScript {
    void* field0;
    IStep* field4;
};

static int g_calls = 0;
static int g_advance_left = 3;

void __fastcall CQ_SunnyvaleMasterScript_Advance_impl(CQ_SunnyvaleMasterScript*);
class CEntityScriptBase { public: bool IsActiveThreadTerminating() const; };   // 0x00F35B30 (the row calls it so)

static void __fastcall step_fn(IStep*) { g_calls++; }

bool CEntityScriptBase::IsActiveThreadTerminating() const {
    // return false, false, true  -> loop exits after 3rd body run
    return --g_advance_left <= 0;
}

void __fastcall Main(CQ_SunnyvaleMasterScript* self);

int main() {
    IStep::VT vt;
    vt.slot1c = step_fn;
    IStep step;
    step.vtbl = &vt;
    CQ_SunnyvaleMasterScript o;
    o.field0 = 0;
    o.field4 = &step;
    Main(&o);
    // Advance returns false twice then true -> body runs 3 times
    if (g_calls == 3) printf("00e1ba20_TEST PASS\n");
    else printf("FAIL calls=%d\n", g_calls);
    return 0;
}