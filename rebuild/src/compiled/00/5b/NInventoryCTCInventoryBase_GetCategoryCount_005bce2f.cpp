#pragma optimize("s",on)
// NInventory::CTCInventoryBase::GetCategoryCount() const -- the number of categories in the Categories vector
// Leaf: ?GetCategoryCount@CTCInventoryBase@NInventory@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BCE2F, 13 bytes:
//   mov eax,[ecx+24h] ; sub eax,[ecx+20h] ; push 2Ch ; cdq ; pop ecx ; idiv ecx ; ret
// Name: Ego_r.exe ?GetCategoryCount@CTCInventoryBase@NInventory@@QBEJXZ @ 0x0061FD2E is the same 13-byte, 7-instruction
// body; the element size 0x2C is the retail NInventory::CCategory (Ego_r.pdb 48 bytes with a 16-byte vector, retail
// STLport vectors are 12) and Categories sits at retail +0x20 (Ego_r.pdb +0x24, PDB minus 4 for the Components vector).
// The function lies in the CTCInventoryBase run (0x005BC4C0..0x005BFD5E) and its three callers are inventory screens
// (CTCInventoryTrade::InitialiseSellableItems 0x0059D820, CTCInventoryQuests::ProcessInventoryInputLeft/Right
// 0x0061A120 / 0x0061A140). The old label "CWorldMap::DrawGetNoMaps" was propagated.
namespace NInventory {

class CCategory {                                      // retail 0x2c bytes
    unsigned char _body[0x2c];
};

class CTCInventoryBase {
public:
    virtual ~CTCInventoryBase();
    unsigned char _pad_0x04[0x18];
    long          InventorySelectionState;             // +0x1c
    CCategory*    CategoriesFirst;                     // +0x20 Categories (STLport vector<CCategory>)
    CCategory*    CategoriesLast;                      // +0x24
    CCategory*    CategoriesEnd;                       // +0x28
    long GetCategoryCount() const;
};

long CTCInventoryBase::GetCategoryCount() const
{
    return CategoriesLast - CategoriesFirst;
}

} // namespace NInventory
