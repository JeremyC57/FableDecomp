// CTCInventoryMap::ConstructScreen(long) -- slot 77 of the map screen: rebuilds the map UI for the given argument with the two trailing options cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006116C0, 17 bytes (+15 int3). rtti-vtables: CTCInventoryMap primary
// vtable 0x01257A9C slot 77 (the only reference).
// Callee: CTCInventoryMap::BuildMapTeleportUI-labelled builder @ 0x0060C900 (`ret 0Ch`: three dword arguments, a 0x298-byte
//         frame; label from the manifest, treated as a hint).
//
// Shape: `mov eax,[esp+4] ; push 0 ; push 0 ; push eax ; call ; ret 4` -- a void forwarder that cannot tail-jump
// (the callee pops three slots, the caller one); the argument is loaded before the constant pushes. The method's real
// name is unknown (catalog label kept); the two trailing arguments are modelled as bools. Local model.
struct CTCInventoryMap {
    void BuildMapTeleportUI(long screen, bool forceRebuild, bool keepSelection);   // 0x0060C900
    void ConstructScreen(long screen);
};

void CTCInventoryMap::ConstructScreen(long screen)
{
    BuildMapTeleportUI(screen, false, false);
}