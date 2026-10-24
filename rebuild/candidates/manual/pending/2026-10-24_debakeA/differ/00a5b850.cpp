// Grade: DIFFER(20v25) -- IMPOSSIBLE (no intrinsic in VC7.1): retail executes `cpuid` in line (`push ebx ; mov eax,1 ; cpuid ; test edx,2000000h ; je ; mov byte ptr [013D2880h],1 ; pop ebx`). VC7.1 has no __cpuid intrinsic (added in VC8), so the instruction only comes from inline assembly (the landed source is an inline-assembly block); IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE) is the nearest compiled equivalent (an import call). One grade.
// NVectorMath::GFInitVectorMath() -- note whether the CPU has SSE (CPUID leaf 1, EDX bit 25)
// Leaf: ?GFInitVectorMath@NVectorMath
extern "C" __declspec(dllimport) int __stdcall IsProcessorFeaturePresent(unsigned long feature);
namespace NVectorMath {
extern bool GHasSSE;                         // 0x013D2880
void GFInitVectorMath()
{
    if (IsProcessorFeaturePresent(6))        // PF_XMMI_INSTRUCTIONS_AVAILABLE; VC7.1 has no __cpuid (VC8)
        GHasSSE = true;
}
}
