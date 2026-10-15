// CTCInventoryClothing::OnItemDeselected(long, long) -- rebuilds the selected-clothes set and, when the mannequin is in use and exists, re-dresses the mannequin
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B4A6E, length 39 bytes; size TU. Virtual slot 35 of the CTCInventoryClothing vtable
// 0x0124356C (`ret 8`: two arguments this override ignores; the base slot is a stub @ 0x0057A230).
// Callees: CTCInventoryClothing::SetUpSelectedClothes @ 0x005B4795, CTCInventoryClothing::SetThingAsWearingSelectedClothes(CThing*) @ 0x005B406D.
// Model: engine/CTCInventoryClothing.h (PDB) has PMannequin +0x17C / UseMannequin +0x180; the retail body reads +0x158 / +0x15C,
// so a local view carries the PDB names at the retail offsets.
// Shape: `UseMannequin && PMannequin` -- the byte flag first (`cmp byte ptr`), the pointer loaded once into eax and pushed.
#pragma optimize("s", on)
struct CThing;
struct CTCInventoryClothing_Retail {
    unsigned char _pad_0x000[0x158];
    CThing*       PMannequin;                                    // +0x158
    bool          UseMannequin;                                  // +0x15C
    void SetUpSelectedClothes();                                 // 0x005B4795
    void SetThingAsWearingSelectedClothes(CThing* thing);        // 0x005B406D
    void OnItemDeselected(long unusedA, long unusedB);
};
void CTCInventoryClothing_Retail::OnItemDeselected(long unusedA, long unusedB)
{
    SetUpSelectedClothes();
    if (UseMannequin && PMannequin)
        SetThingAsWearingSelectedClothes(PMannequin);
}
