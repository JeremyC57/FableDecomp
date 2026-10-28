// CCpuInfo::GetCyrixModelName() -- names a Cyrix processor from its CPUID family / model: family 4 "MediaGX"; family 5 model 2 "6x86", model 4 "GXm"; family 6 model 0 "6x86MX", model 5 "Cyrix III"; everything else "<unknown>"
// Grade: DIFFER(75v75) -- register choice only: the family-5 block loads Model into ecx (`mov ecx,[ecx+0Ch] ; cmp ecx,2/4`) where retail uses eax; everything else is byte-identical
// Leaf: ?GetCyrixModelName@CCpuInfo@@QBEPBDXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C52C40, 75 bytes, thiscall, `ret`. No callees; string literals "MediaGX" 0x012B8900,
//   "6x86" 0x012B890C, "GXm" 0x012B8908, "6x86MX" 0x012B8920, "Cyrix III" 0x012B8914, "<unknown>" 0x012B8830.
// Identity: the CPU-identification helper set of the landed CCpuInfo::GetIntelModelName 0x00C52AD0 and
//   GetCentaurModelName 0x00C52C90 (this row sits between the AMD 0x00C52BB0 and Centaur namers; only caller 0x00C53164
//   in the CPU identification). Descriptive (no PDB covers this helper). Label correction: the manifest row
//   "CEngineInternalPrimitiveParticleGroup::GetRenderLayerMask" is propagated.
// Layout: the landed CCpuInfo model (Family +4, Stepping +8, Model +0xc).
// Shape: the three-case switch lowers to `sub eax,4 ; je ; dec eax ; je ; dec eax ; jne`; the model tests are if-chains.
struct CCpuInfo {
    unsigned char _pad[4];
    long Family;                                         // +4
    unsigned long Stepping;                              // +8
    long Model;                                          // +0xc
    const char* GetCyrixModelName() const;
};

const char* CCpuInfo::GetCyrixModelName() const
{
    switch (Family) {
    case 4:
        return "MediaGX";
    case 5:
        if (Model == 2)
            return "6x86";
        if (Model == 4)
            return "GXm";
        break;
    case 6:
        if (Model == 0)
            return "6x86MX";
        if (Model == 5)
            return "Cyrix III";
        break;
    }
    return "<unknown>";
}
