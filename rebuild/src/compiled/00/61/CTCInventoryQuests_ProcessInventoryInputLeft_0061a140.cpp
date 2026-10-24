// CTCInventoryQuests::ProcessInventoryInputLeft() -- outside the test-quest list mode, with at least one category, the base inventory's left-input handling runs
// Leaf: ProcessInventoryInputLeft
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0061A140, 32 bytes. rtti-vtables: CTCInventoryQuests 0x012585B4 slot 41.
// Callees: CTCInventoryBase category count @ 0x005BCE2F (`(Categories.last - first) / 0x2C`; Categories at retail +0x20,
//          manifest label "CWorldMap::DrawGetNoMaps" is propagated), the base slot-41 handler @ 0x005BC649 (tail jump;
//          also CTCInventoryClothing's slot 41, landed as CTCInventoryBase::ProcessButtonAReleased).
//
// Shape: `mov al,[esi+157h] ; test ; jne out ; call count ; test eax,eax ; jle out ; mov ecx,esi ; pop esi ; jmp base`
// -- an `&&` guard with the qualified base call as a void tail call. +0x157 is ActiveTestQuestMode (PDB +0x177).
// Local model: no engine header for CTCInventoryQuests / CTCInventoryBase.
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    long GetNumberOfCategories();             // 0x005BCE2F
    void ProcessInventoryInputLeft();         // 0x005BC649
};
struct CTCInventoryQuests : CTCInventoryBase_Retail {
    unsigned char _pad_0x130[0x157 - 0x130];
    bool ActiveTestQuestMode;                 // +0x157
    void ProcessInventoryInputLeft();
};
void CTCInventoryQuests::ProcessInventoryInputLeft()
{
    if (!ActiveTestQuestMode && GetNumberOfCategories() > 0)
        CTCInventoryBase_Retail::ProcessInventoryInputLeft();
}