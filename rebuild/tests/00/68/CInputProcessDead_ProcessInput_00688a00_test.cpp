// Behaviour model for CInputProcessDead::ProcessInput.
// The retail function is an empty no-op (`ret 8`): it must not modify the
// output object regardless of the incoming event. We model that and confirm
// the output is left untouched.
#include <stdio.h>

class CInputEvent { public: int code; };
class CProcessedInput { public: int flags; };
class CInputProcessDead { public: int dummy; };

void __fastcall ProcessInput(CInputProcessDead* process,
                             int edx_dummy,
                             const CInputEvent& ev,
                             CProcessedInput& out)
{
    (void)process;
    (void)edx_dummy;
    (void)ev;
    (void)out;
}

int main()
{
    CInputProcessDead obj; obj.dummy = 7;
    CInputEvent ev; ev.code = 12345;
    CProcessedInput out; out.flags = 0xABCD;

    ProcessInput(&obj, 0, ev, out);

    // No-op: output must be unchanged by the dead-input handler.
    if (out.flags == 0xABCD)
        printf("BEHAVIOUR_PASS\n");
    else
        printf("BEHAVIOUR_FAIL\n");
    return 0;
}