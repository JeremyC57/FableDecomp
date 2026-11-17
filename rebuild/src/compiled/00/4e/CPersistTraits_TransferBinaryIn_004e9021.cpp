// CPersistTraits<CWeaponTrailGraphicSet>::TransferBinaryIn(CMemoryDataInputStream&, CWeaponTrailGraphicSet&) -- read the Attack and Knockdown trail-graphic indices
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCWeaponTrailGraphicSet@@@@QAEXAAVCMemoryDataInputStream@@AAVCWeaponTrailGraphicSet@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E9021, 185 bytes, `ret 8` (thiscall; the empty traits `this` is unused).
//   0x00469D47 and 0x004E9021 are the same instantiation emitted in two TUs (identical bytes but for the call).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (x2, the slow path of the inlined Read).
// NF64 (round 7, q10678): re-authored on engine/CPersistTraits.h + engine/CMemoryDataInputStream.h (the primary template's
//   TransferBinaryIn(CMemoryDataInputStream&, T&)), as the landed CPersistTraits<CComboMultiplierData> row 0x006DBE00. Ego_r.pdb
//   0x00459A5E and FableWin.pdb 0x01C0C6E4 both give ?TransferBinaryIn@?$CPersistTraits@VCWeaponTrailGraphicSet@@@@QAEXAAVCMemoryDataInputStream@@
//   AAVCWeaponTrailGraphicSet@@@Z; the landed caller 0x004E9156 already binds it. Was CPersistTraits<...>::TransferBinaryIn(CDataInputStream*,
//   CWeaponTrailGraphicSet*) over a local CDataInputStreamReader. PDB CWeaponTrailGraphicSet (8 bytes): Attack +0, Knockdown +4 (longs);
//   no engine header for the set (local model).
// Size-optimised TU: #pragma optimize("s", on) (ebp frame).
#pragma optimize("s", on)
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
class CWeaponTrailGraphicSet { public:
    long Attack;            // +0x00
    long Knockdown;         // +0x04
};
#include "engine/CPersistTraits.h"

template<> void CPersistTraits<CWeaponTrailGraphicSet>::TransferBinaryIn(CMemoryDataInputStream& is, CWeaponTrailGraphicSet& value)
{
    value.Attack = is.ReadSLONG();
    value.Knockdown = is.ReadSLONG();
}
