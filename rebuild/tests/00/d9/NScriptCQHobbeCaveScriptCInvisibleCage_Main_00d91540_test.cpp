// Naming wave 35: the row now defines ?Main@CInvisibleCage@CQ_HobbeCaveScript@NScript@@EAEXXZ (it was landed as ?CQ_SunnyvaleMasterScript__Main@@YIXPAUCQ_SunnyvaleMasterScript@@@Z).
// This behaviour model keeps the old spelling; the linker maps it onto the row's symbol (same thiscall/ecx ABI, same layout).
#pragma comment(linker, "/alternatename:?CQ_SunnyvaleMasterScript__Main@@YIXPAUCQ_SunnyvaleMasterScript@@@Z=?Main@CInvisibleCage@CQ_HobbeCaveScript@NScript@@EAEXXZ")
#include <cstdio>

struct IStep {
    struct VT { void (__fastcall *slot0)(IStep*); void (__fastcall *slot1)(IStep*);
                void (__fastcall *slot2)(IStep*); void (__fastcall *slot3)(IStep*);
                void (__fastcall *slot4)(IStep*); void (__fastcall *slot5)(IStep*);
                void (__fastcall *slot6)(IStep*); void (__fastcall *slot7)(IStep*); };
    VT* vt;
};

struct CQ_SunnyvaleMasterScript {
    void* f0;
    IStep* f4;
};

class CEntityScriptBase { public: bool IsActiveThreadTerminating() const; };   // 0x00F35B30 (the row calls it so)

void __fastcall CQ_SunnyvaleMasterScript__Main(CQ_SunnyvaleMasterScript* self);

static int g_stepCalls = 0;
static int g_helperCalls = 0;

void __fastcall step7(IStep*) { g_stepCalls++; }

bool CEntityScriptBase::IsActiveThreadTerminating() const {
    g_helperCalls++;
    return g_helperCalls >= 3; // returns true (stop) on 3rd call; loop runs 3 times
}

int main()
{
    IStep::VT vt = {0,0,0,0,0,0,0, step7};
    IStep step; step.vt = &vt;
    CQ_SunnyvaleMasterScript obj; obj.f0 = 0; obj.f4 = &step;

    CQ_SunnyvaleMasterScript__Main(&obj);

    if (g_stepCalls == 3 && g_helperCalls == 3)
        printf("00d91540_TEST PASS\n");
    else
        printf("FAIL steps=%d helpers=%d\n", g_stepCalls, g_helperCalls);
    return 0;
}