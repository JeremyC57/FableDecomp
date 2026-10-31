// CTCInventoryQuests::MoveToNextTestQuest() -- in the test-quest list mode, selects the next displayed test quest (wrapping to the first and scrolling the window of 12 rows along)
// Grade: DIFFER(168v168) -- callee-saved permutation: retail this=esi, start=ebp (loaded straight after the call), index=edi; ours this=edi, start loaded into ecx and copied to esi for the dtor, index=ebp.
// Tried (chunk 628, on top of the five earlier levers): named `selected` index local, early-return guard, named first/last in the dtor + named first in size().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0061A9D0, length 176 bytes (+ int3 padding); speed TU (no frame pointer).
// Callees: CTCInventoryQuests::GetTestQuestsToDisplay(vector<CTestQuest>&) @ 0x0061A8A0, the vector's element destroy helper
// `__fastcall _Destroy(first, last, const __false_type&)` @ 0x004ABD90 (manifest label is the byte-twin pair<CCharString,CPersistInfo>
// instantiation; CTestQuest is 28 bytes too) and free @ 0x00BFEA14 for the local vector's destructor.
// Layout: retail ActiveTestQuestMode +0x157, TestQuestSelectedIndex +0x158, TestQuestTopIndex +0x15C (PDB +0x177/+0x178/+0x17C: 0x20 of
// 16-byte debug containers before them).
// Shape: the local vector is three zero words (one zero register shared with the mode test), filled by the out-of-line getter; the
// element count is the 28-byte reciprocal multiply (0x92492493, `sar 4`); the wrap test is unsigned (`jb`) because size() is unsigned
// and the wrap arm is the `if` body; the destructor's empty tag temporary lives in the spare frame byte [esp+13h].
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
    void MoveToNextTestQuest();
};
void CTCInventoryQuests::MoveToNextTestQuest()
{
    if (ActiveTestQuestMode) {
        CTestQuestVector quests;
        GetTestQuestsToDisplay(quests);
        if (TestQuestSelectedIndex >= quests.size() - 1) {
            TestQuestSelectedIndex = 0;
            TestQuestTopIndex = 0;
        } else {
            ++TestQuestSelectedIndex;
            if (TestQuestSelectedIndex - TestQuestTopIndex >= 12)
                ++TestQuestTopIndex;
        }
    }
}
