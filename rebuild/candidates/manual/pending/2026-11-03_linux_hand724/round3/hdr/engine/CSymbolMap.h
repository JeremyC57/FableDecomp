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
#ifdef FABLE_RETAIL_CSYMBOLMAP
    // Chunk 724 round 3 (row 0x00B1E380 CPSCDecalRenderer::DatabaseSerialise): the nested symbol record (Ego_r TPI
    // CSymbolMap::CSymbolInfo, `VCSymbolInfo@CSymbolMap@@` in both PDB name sets; size 0xC: GroupName +0, EntryName +4, Val +8 --
    // the caller copies EntryName from `add ebp,4`) and the const lookups, out of line at their catalog addresses.
    class CSymbolInfo {
    public:
        CCharString GroupName;
        CCharString EntryName;
        long Val;
    };
    const CSymbolInfo* FindFirstSymbolWithVal(const CCharString&,long) const;   // 0x009E50C0 ?FindFirstSymbolWithVal@CSymbolMap@@QBEPBVCSymbolInfo@1@ABVCCharString@@J@Z
    long GetSymbolSLONG(const CCharString&) const;                              // 0x009E5120 ?GetSymbolSLONG@CSymbolMap@@QBEJABVCCharString@@@Z
    bool SymbolExists(const CCharString&) const;                                // 0x009E51C0 ?SymbolExists@CSymbolMap@@QBE_NABVCCharString@@@Z
#endif
    CSymbolMap();
    virtual ~CSymbolMap();
protected:
    unsigned char SLONGMap[16];      // CVectorMap<CCharString,CSymbolInfo,CCompareString<CSymbolInfo> >
    unsigned char NamesUsed[12];     // std::set<CCharString,std::less<CCharString>,std::allocator<CCharString> >
    bool CheckForDuplicateSymbols;
};
FABLE_STATIC_ASSERT(sizeof(CSymbolMap)==0x24);
#endif
