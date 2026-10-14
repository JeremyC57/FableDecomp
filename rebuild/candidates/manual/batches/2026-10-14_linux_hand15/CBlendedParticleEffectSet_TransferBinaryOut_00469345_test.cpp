#include <stdio.h>
static char g_kind[8]; static float g_val[8]; static int g_n;
struct CDataOutputStream {
    void WriteLong(long value) { g_kind[g_n] = 'L'; g_val[g_n++] = (float)value; }
    void WriteFloat(float value) { g_kind[g_n] = 'F'; g_val[g_n++] = value; }
};
struct CBlendedParticleEffectSet {
    long Particle1Index; float Particle1Value; long Particle2Index; float Particle2Value;
    void TransferBinaryOut(CDataOutputStream& out) const;
};
void CBlendedParticleEffectSet::TransferBinaryOut(CDataOutputStream& out) const
{
    out.WriteLong(Particle1Index); out.WriteLong(Particle2Index); out.WriteFloat(Particle1Value); out.WriteFloat(Particle2Value);
}
int main()
{
    CBlendedParticleEffectSet s; s.Particle1Index = 3; s.Particle1Value = 0.25f; s.Particle2Index = 9; s.Particle2Value = 0.75f;
    CDataOutputStream out; s.TransferBinaryOut(out);
    bool ok = g_n == 4 && g_kind[0] == 'L' && g_val[0] == 3.0f && g_kind[1] == 'L' && g_val[1] == 9.0f && g_kind[2] == 'F' && g_val[2] == 0.25f && g_kind[3] == 'F' && g_val[3] == 0.75f;
    if (!ok) { printf("00469345_TEST FAIL\n"); return 1; }
    printf("00469345_TEST PASS\n"); return 0;
}
