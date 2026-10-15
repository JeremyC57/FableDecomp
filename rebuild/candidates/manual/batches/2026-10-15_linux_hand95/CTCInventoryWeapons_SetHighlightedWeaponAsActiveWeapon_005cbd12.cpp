// CTCInventoryWeapons::SetHighlightedWeaponAsActiveWeapon() -- makes the highlighted inventory item (when there is one) the active weapon, with no menu entry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CBD12, 24 bytes.
// Callees: CTCInventoryBase::GetHighlightedItem @ 0x005BCAE3 (landed; the selected category's highlighted item def index,
//          0 when there are no categories), CTCInventoryWeapons::SetThingAsActiveWeapon(long, NUISystem::CComponent*)
//          @ 0x005CB2B0.
//
// Shape: `call ; test eax,eax ; jle ; push 0 ; push eax ; mov ecx,esi ; call` -- a signed `> 0` guard on the call
// result held in eax (a named local), the null menu entry pushed first. Size TU (`#pragma optimize("s", on)`, the
// weapons module). No member is touched. Local model.
#pragma optimize("s", on)
namespace NUISystem { struct CComponent; }
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    long GetHighlightedItem();                // 0x005BCAE3
};
struct CTCInventoryWeapons : CTCInventoryBase_Retail {
    void SetThingAsActiveWeapon(long defIndex, NUISystem::CComponent* menuEntry);   // 0x005CB2B0
    void SetHighlightedWeaponAsActiveWeapon();
};
void CTCInventoryWeapons::SetHighlightedWeaponAsActiveWeapon()
{
    long item = GetHighlightedItem();
    if (item > 0)
        SetThingAsActiveWeapon(item, 0);
}
