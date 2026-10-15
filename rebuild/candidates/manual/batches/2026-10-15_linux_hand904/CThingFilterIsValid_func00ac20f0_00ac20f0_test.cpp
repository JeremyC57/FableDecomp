#include <cstdio>
#include <cstring>
// Behaviour of func_00ac20f0 (0x00AC20F0): returns the address of the static MPRT descriptor.
struct SMprtDescriptor_Retail { char m_tag[8]; void* m_entries[1]; };
SMprtDescriptor_Retail g_MprtDescriptor = { "MPRT", { 0 } };
struct CDescriptorOwner_Retail { SMprtDescriptor_Retail* func_00ac20f0() { return &g_MprtDescriptor; } };
int main()
{
    CDescriptorOwner_Retail o; SMprtDescriptor_Retail* d = o.func_00ac20f0();
    if (d != &g_MprtDescriptor || std::strcmp(d->m_tag, "MPRT") != 0) { std::printf("00ac20f0_TEST FAIL\n"); return 1; }
    std::printf("00ac20f0_TEST PASS\n"); return 0;
}
