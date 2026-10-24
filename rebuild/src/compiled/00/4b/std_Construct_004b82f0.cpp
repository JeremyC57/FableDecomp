// std::_Construct(CQuestManager::CQuestCard*, const CQuestManager::CQuestCard&) -- placement copy of a quest card: two CCharStrings, a flag and a feat index
// Retail: Fable.exe @ 0x004B82F0, 43 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CCharString copy ctor @ 0x0099EC30 (x2).
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N004b82f0 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct CQuestCard {                              // CQuestManager::CQuestCard (PDB, sizeof 0x10)
    CCharString DefName;                         // +0x00
    CCharString ScriptName;                      // +0x04
    bool        Replayable;                      // +0x08
    long        FeatIndex;                       // +0x0C
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CQuestCard* p, const CQuestCard& value)
{
    if (p != 0)
        new (p) CQuestCard(value);
}
}