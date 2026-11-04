#ifndef FABLE_ENGINE_CSYMBOLMAP_H
#define FABLE_ENGINE_CSYMBOLMAP_H
// CSymbolMap (Ego_r TPI, RTTI base CBaseClassNonCopyable): SLONGMap +0x04 (CVectorMap<CCharString,CSymbolInfo,...>, 0x10),
// NamesUsed +0x14 (std::set<CCharString>, 0x0C), CheckForDuplicateSymbols +0x20; sizeof 0x24 (ctor 0x009E5200; the
// CParticleEmitterDatabase ctor 0x00AE8BF0 places UsingSymbolMap at +0x24). Containers opaque (no row reads them).
#include "engine/CBaseClassNonCopyable.h"
#ifdef FABLE_RETAIL_CSYMBOLMAP
// Retail opt-in (chunk 738, CPSCRenderSprite::DatabaseSerialise 0x00AFBCE0): the symbol record (Ego_r TPI struct CSymbolInfo, 12 bytes:
// GroupName +0, EntryName +4, Val +8) and three out-of-line const lookups, spelled as both PDB name sets and the landed bodies:
// ?FindFirstSymbolWithVal@CSymbolMap@@QBEPBUCSymbolInfo@@ABVCCharString@@J@Z 0x009E50C0, ?GetSymbolSLONG@CSymbolMap@@QBEJABVCCharString@@@Z
// 0x009E5120, ?SymbolExists@CSymbolMap@@QBE_NABVCCharString@@@Z 0x009E51C0.
#include "fable_string.h"
struct CSymbolInfo {
    CCharString GroupName;
    CCharString EntryName;
    long Val;
};
#endif
class CSymbolMap : public CBaseClassNonCopyable {
public:
    CSymbolMap();
    virtual ~CSymbolMap();
#ifdef FABLE_RETAIL_CSYMBOLMAP
    const CSymbolInfo* FindFirstSymbolWithVal(const CCharString& group, long value) const;   // 0x009E50C0
    long GetSymbolSLONG(const CCharString& name) const;                                       // 0x009E5120
    bool SymbolExists(const CCharString& name) const;                                         // 0x009E51C0
#endif
protected:
    unsigned char SLONGMap[16];      // CVectorMap<CCharString,CSymbolInfo,CCompareString<CSymbolInfo> >
    unsigned char NamesUsed[12];     // std::set<CCharString,std::less<CCharString>,std::allocator<CCharString> >
    bool CheckForDuplicateSymbols;
};
FABLE_STATIC_ASSERT(sizeof(CSymbolMap)==0x24);
#endif
