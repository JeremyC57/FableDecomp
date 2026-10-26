// CThingAICreature::vfunc_85(void*) -- calls the member method @ 0x00663BA0 with the argument and the constant 9
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008300B0, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [esp + 4] ; push 9 ; push eax ; call 0x663ba0 ; ret 4
// Tiny-body shape `ForwardWithConstant` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
class CThingAICreature { public:
    void Method(void* value, long kind);   // 0x00663BA0
    void vfunc_85(void* value);
};

void CThingAICreature::vfunc_85(void* value)
{
    Method(value, 9);
}