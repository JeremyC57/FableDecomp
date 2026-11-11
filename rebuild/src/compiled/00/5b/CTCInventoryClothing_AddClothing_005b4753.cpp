// CTCInventoryClothing::AddClothing(long item) -- adds a clothing item to the owner thing, then selects it in the inventory
// Leaf: ?AddClothing@CTCInventoryClothing@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B4753, 30 bytes, `ret 4`. Size-optimised TU (`optimize("s", on)`).
// Callees: CTCInventoryClothing::AddClothingToThing(long, CThing*) @ 0x005B43A2 (Ego_r.pdb
//   ?AddClothingToThing@CTCInventoryClothing@@QAEXJPAVCThing@@@Z; ecx is still `this`, so VC7.1 reloads nothing),
//   NInventory::CTCInventoryBase::SelectItemWithDefIndex(long) @ 0x005BDFB1 (Ego_r.pdb, protected).
// agentNF15: the free __stdcall stand-in Sub_5b43a2 and the member stand-in Sub_5bdfb1 replaced by the Ego_r.pdb members
//   (same bytes: AddClothingToThing is a thiscall whose `this` is already in ecx); the thing at +4 is CTCBase::ParentThing.
#pragma optimize("s",on)
class CThing;
namespace NInventory {
class CTCInventoryBase { public:                        // CTCBase: vptr +0, ParentThing +4 (declarations only)
    void*   __vftable;                                  // +0
    CThing* ParentThing;                                // +4
protected:
    void SelectItemWithDefIndex(long defIndex);         // 0x005BDFB1 ?SelectItemWithDefIndex@CTCInventoryBase@NInventory@@IAEXJ@Z
};
}
class CTCInventoryClothing : public NInventory::CTCInventoryBase { public:
    void AddClothing(long item);                        // Ego_r.pdb ?AddClothing@CTCInventoryClothing@@QAEXJ@Z
    void AddClothingToThing(long item, CThing* thing);  // 0x005B43A2
};

void CTCInventoryClothing::AddClothing(long item)
{
    AddClothingToThing(item, ParentThing);
    SelectItemWithDefIndex(item);
}
