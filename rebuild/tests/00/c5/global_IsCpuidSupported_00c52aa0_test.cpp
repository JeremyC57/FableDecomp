#include <cstdio>
// behaviour test of IsCpuidSupported (0x00C52AA0): the EFLAGS.ID (bit 21) toggle probe
#ifdef _MSC_VER
#pragma optimize("y", off)
unsigned char IsCpuidSupported()
{
    long supported = 0;
    __asm {
        pushfd
        pop eax
        mov ecx, eax
        xor eax, 200000h
        push eax
        popfd
        pushfd
        pop eax
        xor eax, ecx
        je done
        mov supported, 1
    done:
    }
    return supported != 0;
}
int main()
{
    // every CPU the lane runs on implements CPUID, so the toggled bit must stick
    bool ok = IsCpuidSupported() == 1;
    std::printf(ok ? "00c52aa0_TEST PASS\n" : "00c52aa0_TEST FAIL\n");
    return ok ? 0 : 1;
}
#else
// the same logic over a modelled EFLAGS register: a write of bit 21 either sticks (CPUID present) or is dropped (386/early 486)
struct ModelCpu { unsigned long eflags; bool idBitWritable; };
static unsigned long ReadFlags(const ModelCpu& cpu) { return cpu.eflags; }
static void WriteFlags(ModelCpu& cpu, unsigned long value)
{
    if (cpu.idBitWritable) cpu.eflags = value;
    else cpu.eflags = (value & ~0x200000ul) | (cpu.eflags & 0x200000ul);
}
unsigned char IsCpuidSupported(ModelCpu& cpu)
{
    long supported = 0;
    unsigned long original = ReadFlags(cpu);
    WriteFlags(cpu, original ^ 0x200000ul);
    if ((ReadFlags(cpu) ^ original) != 0)
        supported = 1;
    return supported != 0;
}
int main()
{
    ModelCpu modern = { 0x246, true };
    ModelCpu old = { 0x246, false };
    ModelCpu modernSet = { 0x200246, true };
    bool ok = IsCpuidSupported(modern) == 1 && IsCpuidSupported(old) == 0 && IsCpuidSupported(modernSet) == 1;
    std::printf(ok ? "00c52aa0_TEST PASS\n" : "00c52aa0_TEST FAIL\n");
    return ok ? 0 : 1;
}
#endif