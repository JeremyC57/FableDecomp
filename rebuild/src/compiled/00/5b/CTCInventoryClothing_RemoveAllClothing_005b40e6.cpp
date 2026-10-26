// CTCInventoryClothing::RemoveAllClothing() -- with the mannequin flag dropped, selects the HERO_SUIT_NAKED item and dresses the owner in the selected clothes, then raises the flag again
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B40E6, length 87 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on)
// (a `push ecx` frame for the CCharString temporary).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 and ~CCharString @ 0x0099EAE0 on "HERO_SUIT_NAKED" @ 0x0124F510;
// the definition-manager accessor @ 0x006869D0 (`mov ecx,[ecx+4] ; jmp 0x004C7990`, the world's +0x20 manager, reached through the
// owner at +4); CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410; CTCInventoryBase::SelectItemWithDefIndex
// @ 0x005BDFB1; CTCInventoryClothing::SetThingAsWearingSelectedClothes(CThing*) @ 0x005B406D.
// Model: engine/CTCInventoryClothing.h (PDB) has UseMannequin at +0x180; the retail flag written here sits at +0x15C (narrower
// retail base and STLport containers), so a local view carries the PDB names at the retail offsets.
// Shape: the flag clear precedes the literal conversion (it is scheduled among the constructor's pushes); the name is an
// IMPLICIT conversion temporary of the const-reference argument (re-addressed with `lea eax,[ebp-4]` after its constructor),
// destroyed before SelectItemWithDefIndex, so the index is a named local (edi).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);             // 0x0099EBF0
    ~CCharString();                                              // 0x0099EAE0
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);     // 0x009AD410
};
class CThing;
struct CTCInventoryClothing_Retail {
    void*         __vftable;                                     // +0x000
    CThing*       Owner;                                         // +0x004
    unsigned char _pad_0x008[0x154];
    bool          UseMannequin;                                  // +0x15C
    CDefinitionManager* GetDefinitionManager();                  // 0x006869D0 (Owner->... out of line)
    void SelectItemWithDefIndex(long defIndex);                  // 0x005BDFB1 (CTCInventoryBase)
    void SetThingAsWearingSelectedClothes(CThing* thing);        // 0x005B406D
    void RemoveAllClothing();
};
void CTCInventoryClothing_Retail::RemoveAllClothing()
{
    UseMannequin = false;
    long suitIndex = GetDefinitionManager()->GetDefGlobalIndexFromName("HERO_SUIT_NAKED");
    SelectItemWithDefIndex(suitIndex);
    SetThingAsWearingSelectedClothes(Owner);
    UseMannequin = true;
}