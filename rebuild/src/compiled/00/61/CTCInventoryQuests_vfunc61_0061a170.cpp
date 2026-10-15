// CTCInventoryQuests::vfunc_61() -- in test-quest list mode, toggles "show all test quests" and rewinds the selection and the top of the list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0061A170, 40 bytes (+8 int3). rtti-vtables: CTCInventoryQuests 0x012585B4 slot 61.
//
// Shape: `mov dl,[ecx+157h] ; xor eax,eax ; cmp dl,al ; je ; cmp [ecx+160h],al ; sete dl ; mov [ecx+160h],dl ;
// mov [ecx+158h],eax ; mov [ecx+15Ch],eax ; ret` -- the zero register is hoisted above the test and shared by the
// bool negation (`!b` = `cmp b,0 ; sete`) and the two index resets. Members from the PDB (CTCInventoryQuests:
// ActiveTestQuestMode +0x177, TestQuestSelectedIndex +0x178, TestQuestTopIndex +0x17C, TestQuestShowAll +0x180), each
// 0x20 lower in retail (eight 12-byte vectors before them). The real method name is unknown (catalog label kept).
// Local model (no engine header).
struct CTCInventoryQuests {
    unsigned char _pad_0x00[0x157];
    bool ActiveTestQuestMode;                 // +0x157
    long TestQuestSelectedIndex;              // +0x158
    long TestQuestTopIndex;                   // +0x15C
    bool TestQuestShowAll;                    // +0x160
    void vfunc_61();
};
void CTCInventoryQuests::vfunc_61()
{
    if (ActiveTestQuestMode) {
        TestQuestShowAll = !TestQuestShowAll;
        TestQuestSelectedIndex = 0;
        TestQuestTopIndex = 0;
    }
}