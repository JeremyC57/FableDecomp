// CFontManager::GetFont(const CCharString& name) -- look a font name up in the static symbol map, else the streaming one, and return the counted font bank from the matching data bank (static by default)
// Leaf: ?GetFont@CFontManager@@QAE?AV?$CCountedPointer@VCFontBank@@@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E2C80, 91 bytes, thiscall `ret 8` (hidden result + name).
// Identity: Ego_r.pdb ?GetFont@CFontManager@@QAE?AV?$CCountedPointer@VCFontBank@@@@ABVCCharString@@@Z (labels_rename1,
//   FINDINGS chunk 376); its callees are the landed twins GetStaticFont 0x009E2BA0 / GetStreamingFont 0x009E2C10. The
//   catalog label "CMessageEventFilter_CanBeSeenOrHeard operator()" is wrong.
// Callees: CSymbolMap::SymbolExists(const CCharString&) const @ 0x009E51C0, CSymbolMap::GetSymbolSLONG(const CCharString&)
//   const @ 0x009E5120 (Ego_r donor order: GetSymbolSLONG 0xB36AD0 precedes SymbolExists 0xB36B80, as 0x9E5120 < 0x9E51C0;
//   one argument, bool / long results), GetStaticFont @ 0x009E2BA0, GetStreamingFont @ 0x009E2C10.
// Layout (PDB CFontManager, retail offsets): PSymbols +0x14, PStreamingSymbols +0x1c (counted pointers, Data first).
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;                          // +0
    CCPPointerInfo* Info;                          // +4
    CCountedPointer(const CCountedPointer& o);
    ~CCountedPointer();
    T* operator->() const { return Data; }
};
class CCharString;
class CSymbolMap { public:
    long GetSymbolSLONG(const CCharString& name) const;   // 0x009E5120
    bool SymbolExists(const CCharString& name) const;     // 0x009E51C0
};
class CFontBank;
class CFontDataBank;
class CStreamingFontDataBank;
class CFontManager { public:
    void*                                   _vptr;
    CCountedPointer<CFontDataBank>          PFontDataBank;            // +0x04
    CCountedPointer<CStreamingFontDataBank> PStreamingFontDataBank;   // +0x0c
    CCountedPointer<CSymbolMap>             PSymbols;                 // +0x14
    CCountedPointer<CSymbolMap>             PStreamingSymbols;        // +0x1c
    CCountedPointer<CFontBank> GetStaticFont(unsigned long index);    // 0x009E2BA0
    CCountedPointer<CFontBank> GetStreamingFont(unsigned long index); // 0x009E2C10
    CCountedPointer<CFontBank> GetFont(const CCharString& name);
};

CCountedPointer<CFontBank> CFontManager::GetFont(const CCharString& name)
{
    if (PSymbols->SymbolExists(name))
        return GetStaticFont(PSymbols->GetSymbolSLONG(name));
    if (PStreamingSymbols->SymbolExists(name))
        return GetStreamingFont(PStreamingSymbols->GetSymbolSLONG(name));
    return GetStaticFont(PSymbols->GetSymbolSLONG(name));
}