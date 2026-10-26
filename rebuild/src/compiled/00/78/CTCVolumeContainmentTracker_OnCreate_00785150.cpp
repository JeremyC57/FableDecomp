// CTCVolumeContainmentTracker::OnCreate() -- passes the member at +0x1c by address to the method @ 0x00785010 of the object at +0x70 of the object at +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00785150, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  lea eax, [ecx + 0x1c] ; mov ecx, dword ptr [ecx + 4] ; mov ecx, dword ptr [ecx + 0x70] ; push eax ; call 0x785010 ; ret 
// Tiny-body shape `RegisterWith` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct SEntry { long m_value; };

struct CRegistry {
    void Register_00785010(SEntry* entry);          // 0x00785010
};

struct COwner {
    unsigned char _pad[0x70];
    CRegistry* m_pRegistry;                // +0x70
};

class CTCVolumeContainmentTracker { public:
    unsigned char _pad0[0x4];
    COwner* m_pOwner;                      // +0x4
    unsigned char _pad1[0x14];
    SEntry m_entry;                        // +0x1c
    void OnCreate();
};

void CTCVolumeContainmentTracker::OnCreate()
{
    m_pOwner->m_pRegistry->Register_00785010(&m_entry);
}