// SSymbolMapEntry::`scalar deleting destructor'(unsigned int) -- destroys a pair<CCharString, CSymbolMap::CSymbolInfo> (second.EntryName, second.GroupName, first) and frees it for flag bit 0
// Leaf: ??_GSSymbolMapEntry
// Retail: Fable.exe 0x006FB730, 52 bytes. No vtable store: a non-polymorphic record. The manifest label
// "CThingSwitch::`vector_deleting_destructor'" is propagated over the byte-identical deleting destructors;
// the body sits among the std::pair<CCharString, CSymbolMap::CSymbolInfo> helpers (0x006FB460, 0x006FB8D0)
// and matches that pair's Ego_r.pdb layout (first +0; CSymbolInfo GroupName +4, EntryName +8, Val +0xc).
// Callees: CCharString::~CCharString 0x0099EAE0 (three times), operator delete 0x00BFE9BC.
// Shape: VC7.1 emits a non-virtual `??_G` for a non-polymorphic class that is `delete`d (the `Destroy`
// helper below); the inner CSymbolInfo's implicit destructor is inlined with its address in edi
// (`lea edi,[esi+4]`, `lea ecx,[edi+4]`).
struct CCharString {
    void* m_pData;
    ~CCharString();                                      // 0x0099EAE0
};

struct CSymbolInfo {                                     // CSymbolMap::CSymbolInfo
    CCharString GroupName;                               // +0x00
    CCharString EntryName;                               // +0x04
    long        Val;                                     // +0x08
};

struct SSymbolMapEntry {                                 // std::pair<CCharString, CSymbolMap::CSymbolInfo>
    CCharString first;                                   // +0x00
    CSymbolInfo second;                                  // +0x04
};

void DestroySymbolMapEntry(SSymbolMapEntry* entry)
{
    delete entry;
}
