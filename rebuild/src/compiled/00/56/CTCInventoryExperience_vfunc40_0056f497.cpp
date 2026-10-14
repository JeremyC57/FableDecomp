// CTCInventoryExperience::vfunc_40() -- calls the member method @ 0x0056F03D on `this`, then tail-jumps to the argument-less free function @ 0x005BC638
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056F497, oracle length 10 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  call 0x56f03d ; jmp 0x5bc638
// Tiny-body shape `PrepareThenFinish` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
void Finish_005bc638();                      // 0x005BC638: a free function with no arguments (tail-called)

struct CTCInventoryExperience {
    void Prepare();                        // 0x0056F03D
    void vfunc_40();
};

void CTCInventoryExperience::vfunc_40()
{
    Prepare();
    Finish_005bc638();
}