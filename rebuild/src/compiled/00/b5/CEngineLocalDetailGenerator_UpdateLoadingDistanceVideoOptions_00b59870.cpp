// CEngineLocalDetailGenerator::UpdateLoadingDistanceVideoOptions(const CEngineVideoOptionsDef& def) -- rebuild the 8-entry runtime fade-distance modifier table from the video options: reset every entry to (0, 1), set the draw-distance (bit 1) and repeated-mesh (bit 2) entries from the def, then make each combined entry the per-field maximum of its single-bit entries
// Leaf: ?UpdateLoadingDistanceVideoOptions@CEngineLocalDetailGenerator@@QAEXABVCEngineVideoOptionsDef@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B59870, length 321 bytes, `ret 4`. No callees. Single caller CEngine::SetVideoOptions
//   (0x00B24817). Grade: MATCH (first grade).
// Name: Ego_r.pdb + FableWin.pdb `?UpdateLoadingDistanceVideoOptions@CEngineLocalDetailGenerator@@QAEXABVCEngineVideoOptionsDef@@@Z`
//   (the def is a const reference; the landed caller binds a PAV pointer spelling).
// Layout: CEngineLocalDetailGenerator::RuntimeFadeDistanceModiferTable (CRuntimeFadeDistanceModiferTableEntry[8], PDB name and
//   member names) sits at +0x3C in retail (Ego_r +0x74: four Dinkumware CArrays and the vector/bool members ahead of it shrink in
//   retail); CEngineVideoOptionsDef members at the landed retail offsets (0x004340D0: DrawDistanceMultiplier +0x48,
//   DrawDistanceMinimum +0x4C, RepeatedMeshDrawDistanceFactor +0x54). No engine/ header exists for either class.
// Shape: the reset loop unrolls into sixteen stores of two hoisted registers; the bit walk is the engine's branch-free
//   highest-set-bit binary search (the landed 0x00BE2A80 helper) inlined, with the bit count homed in the loop counter.
class CEngineVideoOptionsDef { public:
    unsigned char _pad00[0x48];
    float DrawDistanceMultiplier;          // +0x48
    float DrawDistanceMinimum;             // +0x4C
    float DrawDistanceMaximum;             // +0x50
    float RepeatedMeshDrawDistanceFactor;  // +0x54
};
class CRuntimeFadeDistanceModiferTableEntry { public:
    float DistanceClampMin;
    float DistanceFactor;
};
class CEngineLocalDetailGenerator { public:
    unsigned char _pad00[0x3c];
    CRuntimeFadeDistanceModiferTableEntry RuntimeFadeDistanceModiferTable[8];   // +0x3C
    void UpdateLoadingDistanceVideoOptions(const CEngineVideoOptionsDef& def);
};

// the engine's branch-free highest-set-bit search (descriptive name, as landed at 0x00BE2A80; inline only, no symbol)
__forceinline long HighestSetBit(unsigned long value)
{
    if (value == 0)
        return -1;
    if (value >= 0x80000000)
        return 31;
    ++value;
    long r = ((long)((1 << 16) - value) >> 31) & 16;
    r += ((long)((1 << (r + 8)) - value) >> 31) & 8;
    r += ((long)((1 << (r + 4)) - value) >> 31) & 4;
    r += ((long)((1 << (r + 2)) - value) >> 31) & 2;
    r += ((long)((1 << (r + 1)) - value) >> 31) & 1;
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