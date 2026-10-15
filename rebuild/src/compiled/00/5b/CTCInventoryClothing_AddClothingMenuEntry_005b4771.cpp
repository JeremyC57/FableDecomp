// CTCInventoryClothing::AddClothingMenuEntry(EClothingSuitPart part, NUISystem::CComponent* component) -- appends the (part, component) pair to ClothingMenuEntries
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B4771, length 36 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: std::vector<std::pair<EClothingSuitPart, NUISystem::CComponent*> >::push_back(const value_type&) @ 0x005BBA5B
// (manifest "Vector_PushBack_Uint64").
// Model: engine/CTCInventoryClothing.h (PDB) puts ClothingMenuEntries at +0x14C with a 16-byte vector; the retail vector is
// {first, last, end} at +0x134 (`add ecx,134h`), so a local view is used. The enum argument is a plain 4-byte long.
// Shape: the pair temporary is built in an 8-byte frame slot by its inline two-argument constructor and passed by address.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CComponent;
struct SClothingMenuEntry {                                      // std::pair<EClothingSuitPart, NUISystem::CComponent*>
    long        first;                                           // EClothingSuitPart
    CComponent* second;
    SClothingMenuEntry(long part, CComponent* component) : first(part), second(component) {}
};
struct CClothingMenuEntryVector {
    SClothingMenuEntry* first;
    SClothingMenuEntry* last;
    SClothingMenuEntry* end_of_storage;
    void push_back(const SClothingMenuEntry& entry);             // 0x005BBA5B
};
struct CTCInventoryClothing_Retail {
    unsigned char            _pad_0x000[0x134];
    CClothingMenuEntryVector ClothingMenuEntries;                // +0x134
    void AddClothingMenuEntry(long part, CComponent* component);
};
void CTCInventoryClothing_Retail::AddClothingMenuEntry(long part, CComponent* component)
{
    ClothingMenuEntries.push_back(SClothingMenuEntry(part, component));
}