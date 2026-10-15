// CQuestManager::OnQuestFinishScreenDisplayed(const CCharString&) -- if the quest is waiting between its start and finish screens, drop it from that list (decrementing the positive waiting count) and report true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B0160, length 75 bytes.
// Callees: the SGI __find for CCharString ranges @ 0x004B8E40 (`__fastcall`; manifest "VectorFind"),
// vector<CCharString>::erase(iterator) @ 0x004B8770 (manifest "vector_pop_back_shift").
// Identity: FableWin.pdb ?OnQuestFinishScreenDisplayed@CQuestManager@@QAE_NABVCCharString@@@Z.
// Layout (retail): BetweenStartAndEndScreenQuests (vector<CCharString>) at +0x4c (PDB +0x6c),
// NumberOfQuestsBetweenStartAndFinishScreens at +0x98 (PDB +0xf4).
// Shape: `last` is read once into a local (edi) and compared after the call; the vector's address is a named
// pointer (ebx: `lea ebx,[esi+4Ch]`) declared after it (callee-saved registers go in reverse declaration order);
// the found path is the `if` body so the `return false` tail sits at the end; the empty iterator-category tag is a const-reference temporary that VC homes
// in the quest argument's slot once its value is in ecx (`lea eax,[esp+10h]`).
struct CCharString {
    void* PStringData;
};

struct SRandomAccessIteratorTag {                  // empty tag: passed by const reference, homed in a dead slot
    SRandomAccessIteratorTag() { }
};

// SGI __find(first, last, value, tag) for CCharString ranges (`__fastcall`: first in ecx, last in edx)
CCharString* __fastcall FindString_004b8e40(CCharString* first, CCharString* last, const CCharString& value, const SRandomAccessIteratorTag& tag);   // 0x004B8E40

struct CCharStringVector {                         // retail vector<CCharString>: three words
    CCharString* m_pFirst;
    CCharString* m_pLast;
    CCharString* m_pEnd;
    void         push_back(const CCharString& value);   // 0x0044BFF0
    CCharString* erase(CCharString* where);             // 0x004B8770
};

struct CQuestManager {
    unsigned char     _pad_0x00[0x4c];
    CCharStringVector BetweenStartAndEndScreenQuests;              // +0x4c
    unsigned char     _pad_0x58[0x40];
    long              NumberOfQuestsBetweenStartAndFinishScreens;  // +0x98
    bool OnQuestFinishScreenDisplayed(const CCharString& quest);
};

bool CQuestManager::OnQuestFinishScreenDisplayed(const CCharString& quest)
{
    CCharString* last = BetweenStartAndEndScreenQuests.m_pLast;
    CCharStringVector* quests = &BetweenStartAndEndScreenQuests;
    CCharString* it = FindString_004b8e40(quests->m_pFirst, last, quest, SRandomAccessIteratorTag());
    if (it != last) {
        if (NumberOfQuestsBetweenStartAndFinishScreens > 0)
            --NumberOfQuestsBetweenStartAndFinishScreens;
        quests->erase(it);
        return true;
    }
    return false;
}