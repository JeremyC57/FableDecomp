#ifndef FABLE_ENGINE_CSYMBOLMAP_H
#define FABLE_ENGINE_CSYMBOLMAP_H
// CSymbolMap (Ego_r TPI, RTTI base CBaseClassNonCopyable): SLONGMap +0x04 (CVectorMap<CCharString,CSymbolInfo,...>, 0x10),
// NamesUsed +0x14 (std::set<CCharString>, 0x0C), CheckForDuplicateSymbols +0x20; sizeof 0x24 (ctor 0x009E5200; the
// CParticleEmitterDatabase ctor 0x00AE8BF0 places UsingSymbolMap at +0x24). Containers opaque (no row reads them).
#include "engine/CBaseClassNonCopyable.h"
#ifdef FABLE_RETAIL_CSYMBOLMAP
#include "fable_string.h"
#endif
class CSymbolMap : public CBaseClassNonCopyable {
public:
    CSymbolMap();
    virtual ~CSymbolMap();
#ifdef FABLE_RETAIL_CSYMBOLMAP
    // Chunk 740 (row 0x00B027D0 CPSCRenderMesh::DatabaseSerialise): the nested symbol record (Ego_r TPI CSymbolMap::CSymbolInfo,
    // 12 bytes: GroupName +0, EntryName +4, Val +8; FableWin.pdb mangles it `VCSymbolInfo@1@`, nested and class-keyed) and the
    // three lookups retail calls out of line: FindFirstSymbolWithVal(group, value) 0x009E50C0
    // (?FindFirstSymbolWithVal@CSymbolMap@@QBEPBVCSymbolInfo@1@ABVCCharString@@J@Z), SymbolExists 0x009E51C0
    // (?SymbolExists@CSymbolMap@@QBE_NABVCCharString@@@Z), GetSymbolSLONG 0x009E5120 (?GetSymbolSLONG@CSymbolMap@@QBEJABVCCharString@@@Z).
    class CSymbolInfo {
    public:
        CCharString GroupName;       // +0x00
        CCharString EntryName;       // +0x04
        long        Val;             // +0x08
    };
    const CSymbolInfo* FindFirstSymbolWithVal(const CCharString& group, long value) const;
    bool SymbolExists(const CCharString& name) const;
    long GetSymbolSLONG(const CCharString& name) const;
#endif
protected:
    unsigned char SLONGMap[16];      // CVectorMap<CCharString,CSymbolInfo,CCompareString<CSymbolInfo> >
    unsigned char NamesUsed[12];     // std::set<CCharString,std::less<CCharString>,std::allocator<CCharString> >
    bool CheckForDuplicateSymbols;
};
FABLE_STATIC_ASSERT(sizeof(CSymbolMap)==0x24);
#endif
