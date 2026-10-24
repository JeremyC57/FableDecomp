// std::_Construct(CQuestManager::CActivateQuestInfo*, const CQuestManager::CActivateQuestInfo&) -- placement copy of a quest-activation record: name string, script-info pointer and two flags
// Retail: Fable.exe @ 0x004B8130, 37 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CCharString copy ctor @ 0x0099EC30.
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N004b8130 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct CScriptInfo;
struct CActivateQuestInfo {                      // CQuestManager::CActivateQuestInfo (PDB, sizeof 0xC)
    CCharString  QuestName;                      // +0x00
    CScriptInfo* PScriptInfo;                    // +0x04
    bool         GameScript;                     // +0x08
    bool         LoadResources;                  // +0x09
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CActivateQuestInfo* p, const CActivateQuestInfo& value)
{
    if (p != 0)
        new (p) CActivateQuestInfo(value);
}
}