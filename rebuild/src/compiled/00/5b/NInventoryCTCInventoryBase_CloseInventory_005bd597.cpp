#pragma optimize("s", on)
// NInventory::CTCInventoryBase::CloseInventory() -- clears InventoryOpen, notifies the global GUI owner, walks the categories (an inlined empty per-category hook) and resets the two animation def indices
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD597, length 68 bytes; size-optimised TU (`push 14h ; pop ecx` divisor, pass-through pushes).
// Callees: the global object at 0x013B8790 (method @ 0x004374B0, manifest "CPlayerGui::Open", propagated). CTCInventoryBase virtual slot 31 (vtable 0x01253DBC); the manifest label "CCategory::SetQuestPersistentFlagForItems" is propagated and rejected.
// Retail CTCInventoryBase: the PDB layout minus 4 bytes per vector before each member -- Categories
// (vector<CCategory>, 0x2C-byte elements) at +0x20, InventoryOpen at +0x58 (PDB +0x60),
// InventoryAnimThingDefIndex at +0x124 and RHSetInvItemDefIndex at +0x128 (PDB +0x138/+0x13C).
// The category loop survives with an empty body: size() is re-read every iteration.
struct CPlayerGuiCalls {
    void Close();                                // 0x004374B0
};
extern CPlayerGuiCalls* g_pPlayerGui;            // 0x013B8790

struct CCategory_Retail {
    unsigned char _body[0x2C];
    void CloseInventory() { }                 // inlined to nothing; the loop remains
};

struct CCategoryVector {                         // retail std::vector<CCategory>
    CCategory_Retail* first;
    CCategory_Retail* last;
    CCategory_Retail* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CCategory_Retail& operator[](unsigned long n) { return first[n]; }
};

struct CTCInventoryBase_Retail {
    unsigned char   _pad_0x00[0x20];
    CCategoryVector Categories;                  // +0x20
    unsigned char   _pad_0x2c[0x2C];
    bool            InventoryOpen;               // +0x58
    unsigned char   _pad_0x59[0xCB];
    long            InventoryAnimThingDefIndex;  // +0x124
    long            RHSetInvItemDefIndex;        // +0x128
    void CloseInventory();
};

void CTCInventoryBase_Retail::CloseInventory()
{
    InventoryOpen = false;
    g_pPlayerGui->Close();
    for (unsigned long i = 0; i < Categories.size(); ++i)
        Categories[i].CloseInventory();
    InventoryAnimThingDefIndex = 0;
    RHSetInvItemDefIndex = 0;
}