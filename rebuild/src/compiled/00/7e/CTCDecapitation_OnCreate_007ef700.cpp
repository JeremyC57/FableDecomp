// CTCDecapitation::OnCreate() -- passes the member at +0x10 by address to the method @ 0x007EFC50 of the object at +0x70 of the object at +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EF700, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  lea eax, [ecx + 0x10] ; mov ecx, dword ptr [ecx + 4] ; mov ecx, dword ptr [ecx + 0x70] ; push eax ; call 0x7efc50 ; ret 
// Tiny-body shape `RegisterWith` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct SEntry { long m_value; };

struct CRegistry {
    void Register_007efc50(SEntry* entry);          // 0x007EFC50
};

struct COwner {
    unsigned char _pad[0x70];
    CRegistry* m_pRegistry;                // +0x70
};

struct CTCDecapitation {
    unsigned char _pad0[0x4];
    COwner* m_pOwner;                      // +0x4
    unsigned char _pad1[0x8];
    SEntry m_entry;                        // +0x10
    void OnCreate();
};

void CTCDecapitation::OnCreate()
{
    m_pOwner->m_pRegistry->Register_007efc50(&m_entry);
}