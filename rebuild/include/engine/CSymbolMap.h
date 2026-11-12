#ifndef FABLE_ENGINE_CSYMBOLMAP_H
#define FABLE_ENGINE_CSYMBOLMAP_H
// CSymbolMap (Ego_r TPI, RTTI base CBaseClassNonCopyable): SLONGMap +0x04 (CVectorMap<CCharString,CSymbolInfo,...>, 0x10),
// NamesUsed +0x14 (std::set<CCharString>, 0x0C), CheckForDuplicateSymbols +0x20; sizeof 0x24 (ctor 0x009E5200; the
// CParticleEmitterDatabase ctor 0x00AE8BF0 places UsingSymbolMap at +0x24). Containers opaque (no row reads them).
#include "engine/CBaseClassNonCopyable.h"
#ifdef FABLE_RETAIL_CSYMBOLMAP
#include "fable_string.h"
#endif
#if defined(FABLE_RETAIL_CSYMBOLMAP) && defined(FABLE_RETAIL_CSYMBOLMAP_SLONGMAP)
#ifndef FABLE_RETAIL_CARRAY_DEALLOCATEUNUSEDMEMORY
#define FABLE_RETAIL_CARRAY_DEALLOCATEUNUSEDMEMORY
#endif
#include "engine/CVectorMap.h"
template<class V> class CCompareString { };
#endif
class CSymbolMap : public CBaseClassNonCopyable {
public:
    CSymbolMap();
    virtual ~CSymbolMap();
#if defined(FABLE_RETAIL_CSYMBOLMAP) && defined(FABLE_RETAIL_CSYMBOLMAP_CURRENTFILENAME)
    // agentFAM09 opt-in (row 0x01224190, its dynamic initializer): Ego_r.pdb / FableWin.pdb `dynamic initializer for
    // 'CSymbolMap::CurrentFilename'' (default CWideString at 0x013CAA34, after the TU's GNullName); protected static per
    // Anniv360 Test PDB (third source) ?CurrentFilename@CSymbolMap@@1VCWideString@@A. Static member: no layout change.
protected:
    static CWideString CurrentFilename;                                  // 0x013CAA34
public:
#endif
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
#ifdef FABLE_RETAIL_CSYMBOLMAP_FINDSYMBOL
    // Nested opt-in (chunk 1020, row 0x009E81E0 via CPersistTraits<float>::TransferIn 0x0040F7E0): Ego_r.pdb / FableWin.pdb
    // ?FindSymbol@CSymbolMap@@QBE_NABVCCharString@@AAJ@Z = 0x009E5170 (Ego_r donor order GetSymbolSLONG 0x00B36AD0, FindSymbol 0x00B36B20,
    // SymbolExists 0x00B36B80 = retail 0x009E5120, 0x009E5170, 0x009E51C0). Landed local models spell it `Lookup(name, long*)`.
    bool FindSymbol(const CCharString& name, long& value) const;
#endif
#endif
#if defined(FABLE_RETAIL_CSYMBOLMAP) && defined(FABLE_RETAIL_CSYMBOLMAP_SLONGMAP)
    // Chunk 767 (row 0x009E5350): Ego_r.pdb ?DeallocateUnusedMemory@CSymbolMap@@QAEXXZ (donor 0x00B376D0, after Clear 0x00B376B0) and the
    // typed SLONGMap (Ego_r CVectorMap<CCharString,CSymbolMap::CSymbolInfo,CCompareString<CSymbolMap::CSymbolInfo> >, the 11 PDB
    // manglings) whose CArray base DeallocateUnusedMemory retail calls (0x009E7C70). CCompareString is empty (an Ego_r functor).
    void DeallocateUnusedMemory();                               // 0x009E5350
protected:
    CVectorMap<CCharString,CSymbolInfo,CCompareString<CSymbolInfo> > SLONGMap;   // +0x04
#else
protected:
    unsigned char SLONGMap[16];      // CVectorMap<CCharString,CSymbolInfo,CCompareString<CSymbolInfo> >
#endif
    unsigned char NamesUsed[12];     // std::set<CCharString,std::less<CCharString>,std::allocator<CCharString> >
    bool CheckForDuplicateSymbols;
};
FABLE_STATIC_ASSERT(sizeof(CSymbolMap)==0x24);
#endif
