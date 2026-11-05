// Leaf: ?CInputProcessJumpingAndRolling_Update@@YIXPAVCInputProcessJumpingAndRolling@@HAAVCProcessedInput@@@Z
#pragma optimize("s",on)

class CInputProcessJumpingAndRolling { public:
    char pad[0x4c];
    char field_4c;
};

class CProcessedInput;

extern "C" char __stdcall JR_Helper(int a, int b);

void __fastcall CInputProcessJumpingAndRolling_Update(CInputProcessJumpingAndRolling* process, int edx_ignored, CProcessedInput& in)
{
    char r = JR_Helper(0x3c, 2);
    if (!r)
        process->field_4c = r;
}