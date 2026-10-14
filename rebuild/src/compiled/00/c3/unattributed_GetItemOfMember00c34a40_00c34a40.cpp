// CClass_00c34a40::GetItemOfMember_00c34a40(long) -- calls the method @ 0x00C42AC0 on the member at +0x4 (the argument is unused)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C34A40, oracle length 11 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  add ecx, 4 ; call 0xc42ac0 ; ret 4
// Tiny-body shape `GetItemOfMember` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CBaseIntelligentPointer {
    void* GetPItem();                      // 0x00C42AC0
};

struct CClass_00c34a40 {
    unsigned char _pad[0x4];
    CBaseIntelligentPointer m_pointer;     // +0x4
    void* GetItemOfMember_00c34a40(long unused);
};

void* CClass_00c34a40::GetItemOfMember_00c34a40(long unused)
{
    return m_pointer.GetPItem();
}