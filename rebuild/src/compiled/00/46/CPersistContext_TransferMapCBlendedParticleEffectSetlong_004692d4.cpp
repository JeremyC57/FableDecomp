// CPersistContext::TransferMap<CBlendedParticleEffectSet, long>(map<long, CBlendedParticleEffectSet>& map) -- unnamed binary-only map transfer: CRC check, then the binary map reader (Mode 2) or writer (Mode 3)
// Leaf: ??$TransferMap@VCBlendedParticleEffectSet@@J@CPersistContext@@QAEXAAV?$map@
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x004692D4, 53 bytes, `ret 4`; ecx = the context, one pushed map address; no frame.
// Identity (V = CBlendedParticleEffectSet, K = long): CMaterialDef::Transfer +0x8C (map<long,CBlendedParticleEffectSet>).
//   Ego_r.pdb / FableWin.pdb name the named form ??$TransferMap@VCBlendedParticleEffectSet@@J@CPersistContext@@QAEXPBDAAV?$map@... for this map, and
//   both stream helpers this row calls: GFSerialiseMapBinaryOut<CBlendedParticleEffectSet,long,less> 0x00469309 (mode 3, the save stream) and
//   GFSerialiseMapBinaryIn<CBlendedParticleEffectSet,long,less> 0x00469381 (mode 2, the load stream), __fastcall (stream ecx, map edx). The one-argument
//   parameter list is the established descriptive spelling of the unnamed retail-only transfer family (engine/CPersistContext.h
//   `TransferMap(map&)`, the sibling of the landed `Transfer(T&)` rows 0x00431020 / 0x00431143): no PDB carries it (descriptive, NOTES).
// Body: CheckCRC("") (0x00404500, the pooled empty literal 0x0122D70E), then the Mode switch (`dec;dec;je / dec;jne`).
// Replaces a pad-model `CEngineLightingManager::UpdateShadowScene` stand-in (fake profiler callee for CheckCRC, free helper names).
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "engine/CPersistContext.h"

class CBlendedParticleEffectSet;
template <class V, class K, class C> void FABLE_FASTCALL GFSerialiseMapBinaryIn(CMemoryDataInputStream& is, std::map<K, V, C, std::allocator<std::pair<const K, V> > >& m);
template <class V, class K, class C> void FABLE_FASTCALL GFSerialiseMapBinaryOut(CDataOutputStream& os, const std::map<K, V, C, std::allocator<std::pair<const K, V> > >& m);

template <> void CPersistContext::TransferMap<CBlendedParticleEffectSet, long>(std::map<long, CBlendedParticleEffectSet, std::less<long>, std::allocator<std::pair<const long, CBlendedParticleEffectSet> > >& map)
{
    CheckCRC("");
    switch (Mode) {
    case MODE_LOAD_BINARY:
        GFSerialiseMapBinaryIn(*PLoadStream, map);
        break;
    case MODE_SAVE_BINARY:
        GFSerialiseMapBinaryOut(*PSaveStream, map);
        break;
    }
}
