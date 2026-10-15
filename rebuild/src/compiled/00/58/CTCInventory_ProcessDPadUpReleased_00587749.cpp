// CTCInventory::ProcessDPadUpReleased() -- D-pad-up release on the inventory screen: confirms a pending quick-access assignment, else cancels a pending quick-access removal, else falls back to the base behaviour
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00587749, length 62 bytes.
// Callees: CTCInventory::SetHighlightedItemAsQuickAccessItem(long) @ 0x005850DF,
//          CTCInventory::SetQuickAccessAssign(bool) @ 0x00581A14,
//          CTCInventoryBase::ProcessDPadUpReleased() @ 0x005BC693 (the base version, reached by a
//          qualified call and therefore tail-jumped; it dispatches through vtable slot 38).
// Pragma: optimize("s", on) -- size TU (zero register shared by tests, pushes and stores; byte flags compared against it).
// Layout: engine/CTCInventory.h is the Ego_r.pdb layout (16-byte vectors, 12-byte lists); the shipped
// class has 12-byte vectors and a 4-byte list, so the two flag bytes the PDB puts at +0x204/+0x205
// sit at +0x1CC/+0x1CD and QuickAccessItems (PDB +0x168) starts at +0x148. The header cannot model
// `this`, so a local view carries the PDB names at the retail offsets.
// Shape: the shared zero (ebx) feeds the flag tests, both pushed arguments and the slot store; the
// second flag's address is CSE'd into eax for the test and the clear.

struct CLongVector {
    long* first;                                  // +0x00
    long* last;                                   // +0x04
    long* end_of_storage;                         // +0x08
    long& operator[](unsigned long n) { return first[n]; }
};

struct CTCInventory {
    unsigned char _pad_0x000[0x148];
    CLongVector   QuickAccessItems;               // +0x148
    unsigned char _pad_0x154[0x1CC - 0x154];
    bool          EnableQuickAccessAssign;        // +0x1CC
    bool          EnableQuickAccessRemove;        // +0x1CD

    void SetHighlightedItemAsQuickAccessItem(long slot);   // 0x005850DF
    void SetQuickAccessAssign(bool enable);                // 0x00581A14
    void BaseProcessDPadUpReleased();                      // 0x005BC693 CTCInventoryBase::ProcessDPadUpReleased
    void ProcessDPadUpReleased();
};

void CTCInventory::ProcessDPadUpReleased()
{
    if (EnableQuickAccessAssign) {
        SetHighlightedItemAsQuickAccessItem(0);
        SetQuickAccessAssign(false);
    } else if (EnableQuickAccessRemove) {
        QuickAccessItems[0] = 0;
        EnableQuickAccessRemove = false;
    } else {
        BaseProcessDPadUpReleased();
    }
}