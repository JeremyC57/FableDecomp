#ifndef FABLE_ENGINE_CSYMBOLMAP_H
#define FABLE_ENGINE_CSYMBOLMAP_H
// CSymbolMap (Ego_r TPI, RTTI base CBaseClassNonCopyable): SLONGMap +0x04 (CVectorMap<CCharString,CSymbolInfo,...>, 0x10),
// NamesUsed +0x14 (std::set<CCharString>, 0x0C), CheckForDuplicateSymbols +0x20; sizeof 0x24 (ctor 0x009E5200; the
// CParticleEmitterDatabase ctor 0x00AE8BF0 places UsingSymbolMap at +0x24). Containers opaque (no row reads them).
#include "engine/CBaseClassNonCopyable.h"
class CSymbolMap : public CBaseClassNonCopyable {
public:
    CSymbolMap();
    virtual ~CSymbolMap();
protected:
    unsigned char SLONGMap[16];      // CVectorMap<CCharString,CSymbolInfo,CCompareString<CSymbolInfo> >
    unsigned char NamesUsed[12];     // std::set<CCharString,std::less<CCharString>,std::allocator<CCharString> >
    bool CheckForDuplicateSymbols;
};
FABLE_STATIC_ASSERT(sizeof(CSymbolMap)==0x24);
#endif
