// CSymbolMap::DeallocateUnusedMemory() -- shrink the symbol map's SLONG table storage to fit
// Leaf: ?DeallocateUnusedMemory@CSymbolMap@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E5350, 8 bytes: `add ecx,4 ; jmp 0x009E7C70` (SLONGMap.DeallocateUnusedMemory(), the CArray base
//   member of the CVectorMap at +4, tail call).
// Identity (two sources): (1) Ego_r.pdb ?DeallocateUnusedMemory@CSymbolMap@@QAEXXZ (donor 0x00B376D0 right after Clear), and the target is
//   the already-bound ?DeallocateUnusedMemory@?$CArray@U?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@std@@@@QAEXXZ on the Ego_r member
//   SLONGMap (+0x04, CVectorMap<CCharString,CSymbolInfo,CCompareString<CSymbolInfo> > : CArray<pair<CCharString,CSymbolInfo> >);
//   (2) its only caller is CDefinitionManager::RemoveUnusedMemory 0x009AE605.
// Callees: ?DeallocateUnusedMemory@?$CArray@U?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@std@@@@QAEXXZ 0x009E7C70.
// Header: engine/CSymbolMap.h (FABLE_RETAIL_CSYMBOLMAP, _SLONGMAP new), engine/CVectorMap.h, engine/CArray.h
//   (FABLE_RETAIL_CARRAY_DEALLOCATEUNUSEDMEMORY new).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_SLONGMAP
#include "fable_string.h"
#include "engine/CSymbolMap.h"
void CSymbolMap::DeallocateUnusedMemory()
{
    SLONGMap.DeallocateUnusedMemory();
}