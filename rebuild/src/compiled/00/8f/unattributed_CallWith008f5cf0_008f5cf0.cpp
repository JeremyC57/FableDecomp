// CClass_008f5cf0::CallWith_008f5cf0() -- calls the member method @ 0x004C9B80 with the constant 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F5CF0, oracle length 8 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push 1 ; call 0x4c9b80 ; ret 
// Tiny-body shape `CallWith` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
class CThing { public:
    void Kill(bool a0);   // 0x004C9B80 (Ego_r.pdb ?Kill@CThing@@QAEX_N@Z)
};
struct CClass_008f5cf0 {
    void CallWith_008f5cf0();
};

void CClass_008f5cf0::CallWith_008f5cf0()
{
    ((CThing*)this)->Kill(1);
}