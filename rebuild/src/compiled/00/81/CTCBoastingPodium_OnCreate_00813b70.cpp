// CTCBoastingPodium::OnCreate() -- passes the member at +0x28 by address to the method @ 0x008148C0 of the object at +0x70 of the object at +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00813B70, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  lea eax, [ecx + 0x28] ; mov ecx, dword ptr [ecx + 4] ; mov ecx, dword ptr [ecx + 0x70] ; push eax ; call 0x8148c0 ; ret 
// Tiny-body shape `RegisterWith` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct SEntry { long m_value; };

struct CRegistry {
    void Register_008148c0(SEntry* entry);          // 0x008148C0
};

struct COwner {
    unsigned char _pad[0x70];
    CRegistry* m_pRegistry;                // +0x70
};

class CTCBoastingPodium { public:
    unsigned char _pad0[0x4];
    COwner* m_pOwner;                      // +0x4
    unsigned char _pad1[0x20];
    SEntry m_entry;                        // +0x28
    void OnCreate();
};

void CTCBoastingPodium::OnCreate()
{
    m_pOwner->m_pRegistry->Register_008148c0(&m_entry);
}