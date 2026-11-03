#ifndef FABLE_ENGINE_CSYMBOLMAP_H
#define FABLE_ENGINE_CSYMBOLMAP_H
#include "engine/CBaseClassNonCopyable.h"
// Layout: original Ego_r TPIab1d; native ctor009e5200 and database ctor00ae8bf0
// prove base4, retail vector-map16, set12, trailing bool and total0x24.
class CSymbolMap:public CBaseClassNonCopyable {
public:CSymbolMap();virtual ~CSymbolMap();
protected:
 unsigned char SLONGMap[16]; // CVectorMap<CCharString,CSymbolInfo,CCompareString<CSymbolInfo> >
 unsigned char NamesUsed[12]; // std::set<CCharString,std::less<CCharString>,std::allocator<CCharString> >
 bool CheckForDuplicateSymbols;
};
FABLE_STATIC_ASSERT(sizeof(CSymbolMap)==0x24);
#endif
