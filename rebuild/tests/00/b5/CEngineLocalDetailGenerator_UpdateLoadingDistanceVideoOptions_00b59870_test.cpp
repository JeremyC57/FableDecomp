#include <stdio.h>
class CEngineVideoOptionsDef { public:
    unsigned char _pad00[0x48];
    float DrawDistanceMultiplier;
    float DrawDistanceMinimum;
    float DrawDistanceMaximum;
    float RepeatedMeshDrawDistanceFactor;
};
class CRuntimeFadeDistanceModiferTableEntry { public:
    float DistanceClampMin;
    float DistanceFactor;
};
class CEngineLocalDetailGenerator { public:
    unsigned char _pad00[0x3c];
    CRuntimeFadeDistanceModiferTableEntry RuntimeFadeDistanceModiferTable[8];
    void UpdateLoadingDistanceVideoOptions(const CEngineVideoOptionsDef& def);
};
static long HighestSetBit(unsigned long value)
{
    if (value == 0)
        return -1;
    if (value >= 0x80000000)
        return 31;
    ++value;
    long r = ((long)((1 << 16) - (long)value) >> 31) & 16;
    r += ((long)((1 << (r + 8)) - (long)value) >> 31) & 8;
    r += ((long)((1 << (r + 4)) - (long)value) >> 31) & 4;
    r += ((long)((1 << (r + 2)) - (long)value) >> 31) & 2;
    r += ((long)((1 << (r + 1)) - (long)value) >> 31) & 1;
    return r;
}
void CEngineLocalDetailGenerator::UpdateLoadingDistanceVideoOptions(const CEngineVideoOptionsDef& def)
{
    for (long i = 0; i < 8; ++i) {
        RuntimeFadeDistanceModiferTable[i].DistanceClampMin = 0.0f;
        RuntimeFadeDistanceModiferTable[i].DistanceFactor = 1.0f;
    }
    RuntimeFadeDistanceModiferTable[1].DistanceClampMin = def.DrawDistanceMinimum;
    RuntimeFadeDistanceModiferTable[1].DistanceFactor = def.DrawDistanceMultiplier;
    RuntimeFadeDistanceModiferTable[2].DistanceClampMin = 0.0f;
    RuntimeFadeDistanceModiferTable[2].DistanceFactor = def.RepeatedMeshDrawDistanceFactor;
    RuntimeFadeDistanceModiferTable[4].DistanceClampMin = 0.0f;
    RuntimeFadeDistanceModiferTable[4].DistanceFactor = 1.0f;
    for (long j = 0; j < 8; ++j) {
        for (unsigned long bits = j; bits != 0; ) {
            long bit = 1 << HighestSetBit(bits);
            if (RuntimeFadeDistanceModiferTable[j].DistanceClampMin < RuntimeFadeDistanceModiferTable[bit].DistanceClampMin)
                RuntimeFadeDistanceModiferTable[j].DistanceClampMin = RuntimeFadeDistanceModiferTable[bit].DistanceClampMin;
            if (RuntimeFadeDistanceModiferTable[j].DistanceFactor < RuntimeFadeDistanceModiferTable[bit].DistanceFactor)
                RuntimeFadeDistanceModiferTable[j].DistanceFactor = RuntimeFadeDistanceModiferTable[bit].DistanceFactor;
            bits -= bit;
        }
    }
}
int main()
{
    static CEngineVideoOptionsDef def;
    def.DrawDistanceMultiplier = 2.0f;
    def.DrawDistanceMinimum = 30.0f;
    def.RepeatedMeshDrawDistanceFactor = 0.5f;
    static CEngineLocalDetailGenerator gen;
    gen.UpdateLoadingDistanceVideoOptions(def);
    const CRuntimeFadeDistanceModiferTableEntry* t = gen.RuntimeFadeDistanceModiferTable;
    int ok = 1;
    if (HighestSetBit(1) != 0 || HighestSetBit(6) != 2 || HighestSetBit(0) != -1 || HighestSetBit(0x80000001u) != 31) ok = 0;
    if (t[0].DistanceClampMin != 0.0f || t[0].DistanceFactor != 1.0f) ok = 0;
    if (t[1].DistanceClampMin != 30.0f || t[1].DistanceFactor != 2.0f) ok = 0;
    if (t[2].DistanceClampMin != 0.0f || t[2].DistanceFactor != 0.5f) ok = 0;
    if (t[6].DistanceClampMin != 0.0f || t[6].DistanceFactor != 1.0f) ok = 0;
    if (t[3].DistanceClampMin != 30.0f || t[3].DistanceFactor != 2.0f) ok = 0;
    if (t[4].DistanceClampMin != 0.0f || t[4].DistanceFactor != 1.0f) ok = 0;
    if (t[7].DistanceClampMin != 30.0f || t[7].DistanceFactor != 2.0f) ok = 0;
    if (!ok) { printf("00b59870_TEST FAIL\n"); return 1; }
    printf("00b59870_TEST PASS\n");
    return 0;
}