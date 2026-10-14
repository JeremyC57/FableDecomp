// CTCHeroCentre::vfunc_4() -- passes the member at +0xc by address to the method @ 0x0077E430 of the object at +0x70 of the object at +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077D3D0, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  lea eax, [ecx + 0xc] ; mov ecx, dword ptr [ecx + 4] ; mov ecx, dword ptr [ecx + 0x70] ; push eax ; call 0x77e430 ; ret 
// Tiny-body shape `RegisterWith` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct SEntry { long m_value; };

struct CRegistry {
    void Register(SEntry* entry);          // 0x0077E430
};

struct COwner {
    unsigned char _pad[0x70];
    CRegistry* m_pRegistry;                // +0x70
};

struct CTCHeroCentre {
    unsigned char _pad0[0x4];
    COwner* m_pOwner;                      // +0x4
    unsigned char _pad1[0x4];
    SEntry m_entry;                        // +0xc
    void vfunc_4();
};

void CTCHeroCentre::vfunc_4()
{
    m_pOwner->m_pRegistry->Register(&m_entry);
}