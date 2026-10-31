// Grade: DIFFER(180v176) (sec1b 2026-10-31) -- branch order: the vector (index <= 0) block written first gives retail's fall-through layout (84->15 difflines); left: the start pointer is in ecx then copied to esi for the free where retail keeps it in edi (same gap as 0x0061A9D0; tried count local, last/first dtor locals)
// CTCInventoryQuests::MoveToPreviousTestQuest() -- in the test-quest list mode, selects the previous displayed test quest (scrolling the 12-row window back, or wrapping to the last quest with the window ending on it)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0061AA80, length 176 bytes; speed TU (no frame pointer).
// Callees: CTCInventoryQuests::GetTestQuestsToDisplay(vector<CTestQuest>&) @ 0x0061A8A0, the vector's element destroy helper
// `__fastcall _Destroy(first, last, const __false_type&)` @ 0x004ABD90 (manifest label is the byte-twin pair<CCharString,CPersistInfo>
// instantiation; CTestQuest is 28 bytes too) and free @ 0x00BFEA14 for the local vector's destructor.
// Layout: retail ActiveTestQuestMode +0x157, TestQuestSelectedIndex +0x158, TestQuestTopIndex +0x15C (PDB +0x177/+0x178/+0x17C).
// Shape: the step-back arm (no vector) sits after the epilogue with its own return; the wrap arm builds the local vector (three zero
// words through the shared zero register), takes the 28-byte reciprocal count, stores `count - 1` and `count - 12` and clamps the
// window start at 0 through the stored value; the destructor's empty tag lives in the spare frame byte [esp+13h].
#include <stddef.h>
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
struct __false_type { __false_type() { } };
struct CTestQuest {                                // 28 bytes
    unsigned char _body[0x1c];
};
void __fastcall _Destroy(CTestQuest* first, CTestQuest* last, const __false_type& tag);   // 0x004ABD90
struct CTestQuestVector {                          // std::vector<CTestQuest> (12 bytes)
    CTestQuest* _M_start;
    CTestQuest* _M_finish;
    CTestQuest* _M_end_of_storage;
    CTestQuestVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CTestQuestVector()                            // the start pointer is one register-held local across the destroy call
    {
        CTestQuest* first = _M_start;
        _Destroy(first, _M_finish, __false_type());
        if (first)
            free(first);
    }
    size_t size() const { return _M_finish - _M_start; }
};
struct CTCInventoryQuests {
    unsigned char _pad00[0x157];
    bool ActiveTestQuestMode;                      // +0x157
    long TestQuestSelectedIndex;                   // +0x158
    long TestQuestTopIndex;                        // +0x15c
    void GetTestQuestsToDisplay(CTestQuestVector& quests);   // 0x0061A8A0
    void MoveToPreviousTestQuest();
};
void CTCInventoryQuests::MoveToPreviousTestQuest()
{
    if (ActiveTestQuestMode) {
        if (TestQuestSelectedIndex <= 0) {
            CTestQuestVector quests;
            GetTestQuestsToDisplay(quests);
            TestQuestSelectedIndex = quests.size() - 1;
            TestQuestTopIndex = TestQuestSelectedIndex - 11;
            if (TestQuestTopIndex < 0)
                TestQuestTopIndex = 0;
        } else {
            --TestQuestSelectedIndex;
            if (TestQuestSelectedIndex < TestQuestTopIndex)
                --TestQuestTopIndex;
        }
    }
}
