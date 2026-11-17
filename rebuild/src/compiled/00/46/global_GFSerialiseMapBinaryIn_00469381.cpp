// GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, map<long, CBlendedParticleEffectSet>& out) -- clear the map, read the count, then read each long key and blended particle-effect set and insert the pair
// Leaf: ??$GFSerialiseMapBinaryIn@VCBlendedParticleEffectSet@@JU?$less@J@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@JVCBlendedParticleEffectSet@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJVCBlendedParticleEffectSet@@@std@@@3@@std@@@Z
// Inline-budget: __declspec(noinline) pair() (overlay stlport/fable_containers.h opt-in FABLE_RETAIL_STLP_PAIR_DEFAULT_CTOR_NOINLINE) -- retail
//   calls pair<long, CBlendedParticleEffectSet>() 0x00469435 out of line; VC7.1 /Os expands it (also with /d2inlS5; NF61 grades).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00469381, length 180 bytes (__fastcall stream, out).
// Identity: the catalog label <CDefString, EObjectAugmentationType> is propagated; the map here is <long, CBlendedParticleEffectSet>.
// Callees: map::clear 0x00450FAC, map::insert 0x004695B3, pair default ctor 0x00469435 (out of line), CBlendedParticleEffectSet::TransferBinaryIn 0x0046944C, long traits (out of line).
// Shape (family recipe): `#pragma optimize("y", off)` + `("s", on)` gives retail's frame-pointer size TU; the key and value traits are
//   empty objects declared at function scope; an out-of-line traits call gets a same-TU noinline stand-in under auto_inline(off)
//   (it only reads the stream and writes through `out`) so VC7.1 homes the empty traits object in a live slot as retail does.
// Model: CMemoryDataInputStream from engine/CMemoryDataInputStream.h with a methods-only reader adapter (the inline Read:
//   64-bit range check, memcpy from the chunk, else ReadWithSrcChunkOverflow 0x00993CA0).
// Shape: the pair default constructor is out of line in retail (declared, not defined); the count read is an inline ReadCount.
// NF61 (round 7, q10563): moved onto the shared body engine/CPersistTraits.h FABLE_RETAIL_GFSERIALISEMAPBINARYIN (the landed 0x00632B90
//   / NF61 0x0045DF5E pattern: retail stream models, CPersistTraits, stlport/fable_map.h map with STLP_TREE_CLEAR_OUTOFLINE); the
//   local std::map / CPersistTraits / stream-reader models are gone. The insert now binds NF58's _Rb_tree::insert_unique and the
//   self leaf is the PDB-form spelling (CMemoryDataInputStream&, 4-argument STLport map).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_PAIR_DEFAULT_CTOR_NOINLINE
#include "stlport/fable_containers.h"   // first, so the overlay copy (with the opt-in) is the one every later header reaches
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS
#define FABLE_RETAIL_GFSERIALISEMAPBINARYIN
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY   // retail: no DefaultVal store for the key traits, which shares the map-pointer word (the generic traits body is visible)
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_map.h"
class CBlendedParticleEffectSet {   // Ego_r.pdb, 16 bytes; no shared header defines it (engine/CMaterialDef.h only forward-declares it; NF58 0x00469639)
public:
    long Particle1Index;    // +0x0
    float Particle1Value;   // +0x4
    long Particle2Index;    // +0x8
    float Particle2Value;   // +0xC
    // Ego_r.pdb ??0CBlendedParticleEffectSet@@QAE@XZ: every member zero (retail pair<long, CBlendedParticleEffectSet>() 0x00469435 stores
    // 0 / 0.0f / 0 / 0.0f after the key)
    CBlendedParticleEffectSet() : Particle1Index(0), Particle1Value(0.0f), Particle2Index(0), Particle2Value(0.0f) {}
    void TransferBinaryIn(CMemoryDataInputStream& stream);   // 0x0046944C, Ego_r.pdb ?TransferBinaryIn@CBlendedParticleEffectSet@@QAEXAAVCMemoryDataInputStream@@@Z
};
#include "engine/CPersistTraits.h"
// CPersistTraits<CBlendedParticleEffectSet>::TransferBinaryIn: the value reads itself (inline in retail: the direct call of
// CBlendedParticleEffectSet::TransferBinaryIn 0x0046944C, as the header's CDefString / CWoundMorphs traits forward).
template<> inline void CPersistTraits<CBlendedParticleEffectSet>::TransferBinaryIn(CMemoryDataInputStream& is, CBlendedParticleEffectSet& value) { value.TransferBinaryIn(is); }

template void FABLE_FASTCALL GFSerialiseMapBinaryIn<CBlendedParticleEffectSet, long, std::less<long> >(CMemoryDataInputStream& is, std::map<long, CBlendedParticleEffectSet, std::less<long>, std::allocator<std::pair<const long, CBlendedParticleEffectSet > > >& m);
