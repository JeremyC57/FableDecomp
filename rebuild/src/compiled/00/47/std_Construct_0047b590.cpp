// std::_Construct(CHeroLogBook::CQuestObjectiveDesc*, const CHeroLogBook::CQuestObjectiveDesc&) -- placement copy of a quest objective: id, description string, completed flag
// Retail: Fable.exe @ 0x0047B590, 35 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CCharString copy ctor @ 0x0099EC30.
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N0047b590 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct CQuestObjectiveDesc {                     // CHeroLogBook::CQuestObjectiveDesc (PDB, sizeof 0xC)
    unsigned long SubObjectiveID;                // +0x00
    CCharString   TextDescription;               // +0x04
    bool          QuestObjectiveCompleted;       // +0x08
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CQuestObjectiveDesc* p, const CQuestObjectiveDesc& value)
{
    if (p != 0)
        new (p) CQuestObjectiveDesc(value);
}
}