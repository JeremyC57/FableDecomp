#include <stdio.h>

class CInputProcessJumpingAndRollingLayout { public:
    char pad[0x4c];
    char field_4c;
};

class CProcessedInput { public: int dummy; };

extern "C" char __stdcall JR_Helper(int a, int b);
// NF335 (NF324 Q2): the row now defines the PDB member CInputProcessJumpingAndRolling::Update
// (?Update@CInputProcessJumpingAndRolling@@UAEXAAVCProcessedInput@@@Z).
// The test calls it qualified through a pointer to this declaration-only model, cast from the test's object:
// no body and never constructed, so the test emits no vftable and no copy of the row's symbol.
class CInputProcessJumpingAndRolling { public:
    virtual void Update(CProcessedInput&);
};

static int g_a, g_b, g_calls;
static char g_ret;
extern "C" char __stdcall JR_Helper(int a, int b){ g_a=a; g_b=b; g_calls++; return g_ret; }

int main(){
    CInputProcessJumpingAndRollingLayout o;
    CProcessedInput in;
    // case: helper returns 0 -> field_4c set to 0
    o.field_4c = 0x55;
    g_ret = 0; g_calls = 0;
    ((CInputProcessJumpingAndRolling*)(&o))->CInputProcessJumpingAndRolling::Update(in);
    if (g_calls != 1) { printf("bad calls\n"); return 1; }
    if (g_a != 0x3c || g_b != 2) { printf("bad args %d %d\n", g_a, g_b); return 1; }
    if (o.field_4c != 0) { printf("field not cleared\n"); return 1; }
    // case: helper returns nonzero -> field untouched
    o.field_4c = 0x55;
    g_ret = 1;
    ((CInputProcessJumpingAndRolling*)(&o))->CInputProcessJumpingAndRolling::Update(in);
    if (o.field_4c != 0x55) { printf("field wrongly touched\n"); return 1; }
    printf("OK_006a40b0\n");
    return 0;
}