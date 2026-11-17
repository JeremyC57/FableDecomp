// GFSerialiseMapBinaryIn(CMemoryDataInputStream&, std::map<EObjectAugmentationType,CWeaponTrailGraphicSet>&) -- [instantiation <CWeaponTrailGraphicSet,EObjectAugmentationType,std::less<EObjectAugmentationType> >] binary persist read of the weapon-trail graphic map: clear it, read the count, then per entry read key and value through their persist traits and insert the pair
// Leaf: ??$GFSerialiseMapBinaryIn@VCWeaponTrailGraphicSet@@W4EObjectAugmentationType@@U?$less@W4EObjectAugmentationType@@@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@W4EObjectAugmentationType@@VCWeaponTrailGraphicSet@@U?$less@W4EObjectAugmentationType@@@std@@V?$allocator@U?$pair@$$CBW4EObjectAugmentationType@@VCWeaponTrailGraphicSet@@@std@@@4@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E9156, 189 bytes, `__fastcall` (ecx = stream, edx = map), plain `ret`; ebp frame of a
//   size TU (`#pragma optimize("y", off)` + `("s", on)`: out-of-line clear, `movsd` 4-byte read, `and/or` immediates).
// Callees: map::clear @ 0x004DB9EC, CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0, CPersistTraits<EObjectAugmentationType>::
//   TransferBinaryIn @ 0x00452D8E, CPersistTraits<CWeaponTrailGraphicSet>::TransferBinaryIn @ 0x004E9021, map insert @ 0x004E9213.
// Identity: FableWin.pdb ??$GFSerialiseMapBinaryIn@VCWeaponTrailGraphicSet@@W4EObjectAugmentationType@@...; the only caller is
//   0x004E9106 (a CPersistContext map transfer, load-stream arm). The catalog's list<COpinionTransientOffset> label is wrong.
// Layout: Ego_r.pdb CWeaponTrailGraphicSet {long Attack, long Knockdown} (8 bytes); the loop pair is {key, Attack, Knockdown}.
// Shape: both traits objects live at function scope: the key traits' DefaultVal is value-initialised once (`and [ebp-0Ch],0`
//   first thing), the value traits get a frame word but no stores, so they are modelled as an empty specialization (the
//   PDB gives CPersistTraits<CWeaponTrailGraphicSet> an 8-byte DefaultVal, whose -1/-1 default construction retail lacks).
//   The loop pair is value-initialised each pass (key 0, the set's ctor -1/-1: `and ; or ; or`), the map pointer is
//   spilled for the insert, and the insert result shares the count's dead frame word.
// NF61 (round 7, q10569): moved onto the shared body engine/CPersistTraits.h FABLE_RETAIL_GFSERIALISEMAPBINARYIN (the landed 0x00632B90
//   / NF61 0x0045DF5E pattern: retail stream models, CPersistTraits, stlport/fable_map.h map with STLP_TREE_CLEAR_OUTOFLINE); the
//   local std::map / CPersistTraits / stream-reader models are gone. The insert now binds NF58's _Rb_tree::insert_unique and the
//   self leaf is the PDB-form spelling (CMemoryDataInputStream&, 4-argument STLport map).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS
#define FABLE_RETAIL_GFSERIALISEMAPBINARYIN
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWEAPONDEF_WEAPONTRAILS
#define FABLE_RETAIL_CWEAPONTRAILGRAPHICSET_CLASS
#define FABLE_RETAIL_CWEAPONTRAILGRAPHICSET_CTOR
#include "engine/CWeaponDef.h"
#include "engine/CPersistTraits.h"
// CPersistTraits<CWeaponTrailGraphicSet>::TransferBinaryIn (Ego_r.pdb ?TransferBinaryIn@?$CPersistTraits@VCWeaponTrailGraphicSet@@@@QAEXAAVCMemoryDataInputStream@@AAVCWeaponTrailGraphicSet@@@Z;
// retail COMDAT 0x004E9021, landed: two inline 4-byte reads into Attack then Knockdown): its body, visible in this TU but kept out of line
// as retail calls it (EXTRA_RULES same-TU body): VC7.1 then knows the call never reads the traits' DefaultVal, so the -1 / -1 default
// construction of that empty traits object is dead, as in retail (no stores to [ebp-14h]); the key traits keeps its `and [ebp-0Ch],0`.
#pragma auto_inline(off)
template<>
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline void CPersistTraits<CWeaponTrailGraphicSet>::TransferBinaryIn(CMemoryDataInputStream& is, CWeaponTrailGraphicSet& value)
{
    value.Attack = is.ReadSLONG();
    value.Knockdown = is.ReadSLONG();
}
#pragma auto_inline(on)

template void FABLE_FASTCALL GFSerialiseMapBinaryIn<CWeaponTrailGraphicSet, EObjectAugmentationType, std::less<EObjectAugmentationType> >(CMemoryDataInputStream& is, std::map<EObjectAugmentationType, CWeaponTrailGraphicSet, std::less<EObjectAugmentationType>, std::allocator<std::pair<const EObjectAugmentationType, CWeaponTrailGraphicSet > > >& m);
