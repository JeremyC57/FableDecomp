// Behaviour test for 0x00C0B420 CAudioParamBlock::GetGain: MultiplyMaskedParams(value, m_gains, mask, 16) clamped to [0.0f, 1.0f] (retail's pooled
// __real@00000000 0x0122DEDC and __real@3f800000 0x0122DED8).
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object (and
// defines its callees under the row's spellings); it asserts the value of the float literal the row now spells (a mutated literal fails).
#include <cstdio>
struct CRampedParam { unsigned char _body[0x34]; };
struct CAudioParamBlock {
    void* m_vtable;
    CRampedParam m_gains[16];
    float m_pitchFactors[8];
    float GetGain(float value, unsigned short mask) const;
};
static float g_ret = 0.0f; static const CRampedParam* g_params = 0; static unsigned long g_mask = 0, g_count = 0; static float g_value = 0.0f;
float __fastcall MultiplyMaskedParams(float value, const CRampedParam* params, unsigned long mask, unsigned long count)
{ g_value = value; g_params = params; g_mask = mask; g_count = count; return g_ret; }
int main()
{
    static CAudioParamBlock b;
    g_ret = -2.0f; float lo = b.GetGain(0.75f, 0x8001);
    if (lo != 0.0f || g_params != b.m_gains || g_mask != 0x8001 || g_count != 16 || g_value != 0.75f) { std::printf("BAD lo %f\n", lo); return 1; }
    g_ret = 3.0f; float hi = b.GetGain(1.0f, 3);
    g_ret = 0.375f; float mid = b.GetGain(1.0f, 3);
    if (hi != 1.0f || mid != 0.375f) { std::printf("BAD %f %f\n", hi, mid); return 1; }
    std::printf("00c0b420_TEST PASS\n");
    return 0;
}
