// CTCInventoryWeapons::OnItemSelected(long, bool*) -- slot 33: clears the caller's flag, and when the def index is an augmentation, makes it the active weapon (no menu entry)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CBCEA, 40 bytes. rtti-vtables: CTCInventoryWeapons 0x01253FF4 slot 33
// (the same slot is labelled CTCInventory::OnItemSelected at 0x00583E26; the real name is unknown, catalog label kept).
// Callees: the free `__fastcall` IsAugmentation(long) @ 0x005D97F0 (argument in ecx),
//          CTCInventoryWeapons::SetThingAsActiveWeapon(long, NUISystem::CComponent*) @ 0x005CB2B0.
//
// Shape: the out-flag pointer is loaded before `push esi` and the def index into ecx after it, then the byte store
// `mov byte ptr [eax],0` precedes the call (`*handled = false;` first); the def index is re-pushed from its slot for
// SetThingAsActiveWeapon (`push 0 ; push dword ptr [esp+0Ch]`). Size TU (`#pragma optimize("s", on)`). No member is
// touched, so the model is the bare class.
#pragma optimize("s", on)
namespace NUISystem { class CComponent; }
bool __fastcall IsAugmentation(long defIndex);   // 0x005D97F0
class CTCInventoryWeapons { public:
    void SetThingAsActiveWeapon(long defIndex, NUISystem::CComponent* menuEntry);   // 0x005CB2B0
    void OnItemSelected(long defIndex, bool* handled);
};
void CTCInventoryWeapons::OnItemSelected(long defIndex, bool* handled)
{
    *handled = false;
    if (IsAugmentation(defIndex))
        SetThingAsActiveWeapon(defIndex, 0);
}