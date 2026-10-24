// std::_Construct(std::pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*, const std::pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>&) -- placement copy of a named quest-persist record: key string, then quest-name and persist-script strings
// Retail: Fable.exe @ 0x004B8160, 45 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CCharString copy ctor @ 0x0099EC30 (x3).
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N004b8160 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct CPersistInfo {                            // NQuestManager::CRuntimeQuestPersistence::CPersistInfo (PDB, sizeof 8)
    CCharString QuestName;                       // +0x00
    CCharString PersistScript;                   // +0x04
};
struct CNamedPersistInfo {                       // std::pair<CCharString, CPersistInfo>
    CCharString  first;                          // +0x00
    CPersistInfo second;                         // +0x04
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CNamedPersistInfo* p, const CNamedPersistInfo& value)
{
    if (p != 0)
        new (p) CNamedPersistInfo(value);
}
}