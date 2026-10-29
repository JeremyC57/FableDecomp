// CUserProfileManager::SetHUDTooltipEnabled(bool value, bool save) -- store the option (and its new-profile copy while
// UseNewProfileValues is set), push the live values to the engine, then save the profile when asked
// Leaf: ?SetHUDTooltipEnabled@CUserProfileManager@@QAEX_N0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040B290, thiscall, `ret 8`.
// Identity: PDB name set ?SetHUDTooltipEnabled@CUserProfileManager@@QAEX_N0@Z; the stored field is HUDTooltipEnabled (retail +0x19) and its
//   new-profile copy HUDTooltipEnabledNewProfile (retail +0xD9) by struct_layouts_egor (retail = PDB below the first STL vector at
//   +0x54, PDB - 8 after the two AssignedInputs vectors, PDB - 4 in the new-profile block).
// Callees: CUserProfileManager::ApplyValues() @ 0x00407370 (protected in the PDB name set), CUserProfileManager::Save()
//   @ 0x0040A740 (builds the profile path and writes the file).
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0xC];
    bool          HUDTooltipEnabled;         // +0x19
    unsigned char _pad1A[0xBF];
    bool          HUDTooltipEnabledNewProfile; // +0xD9
    void SetHUDTooltipEnabled(bool value, bool save);
    void Save();                                 // 0x0040A740
protected:
    void ApplyValues();                          // 0x00407370
};

void CUserProfileManager::SetHUDTooltipEnabled(bool value, bool save)
{
    if (UseNewProfileValues)
        HUDTooltipEnabledNewProfile = value;
    HUDTooltipEnabled = value;
    ApplyValues();
    if (save)
        Save();
}
