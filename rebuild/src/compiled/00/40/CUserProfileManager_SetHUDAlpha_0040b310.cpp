// CUserProfileManager::SetHUDAlpha(float value, bool save) -- store the option (and its new-profile copy while
// UseNewProfileValues is set), push the live values to the engine, then save the profile when asked
// Leaf: ?SetHUDAlpha@CUserProfileManager@@QAEXM_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040B310, thiscall, `ret 8`.
// Identity: PDB name set ?SetHUDAlpha@CUserProfileManager@@QAEXM_N@Z; the stored field is HUDAlpha (retail +0x20) and its
//   new-profile copy HUDAlphaNewProfile (retail +0xE0) by struct_layouts_egor (retail = PDB below the first STL vector at
//   +0x54, PDB - 8 after the two AssignedInputs vectors, PDB - 4 in the new-profile block).
// Callees: CUserProfileManager::ApplyValues() @ 0x00407370 (protected in the PDB name set), CUserProfileManager::Save()
//   @ 0x0040A740 (builds the profile path and writes the file).
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x13];
    float         HUDAlpha;                  // +0x20
    unsigned char _pad24[0xBC];
    float         HUDAlphaNewProfile;        // +0xE0
    void SetHUDAlpha(float value, bool save);
    void Save();                                 // 0x0040A740
protected:
    void ApplyValues();                          // 0x00407370
};

void CUserProfileManager::SetHUDAlpha(float value, bool save)
{
    if (UseNewProfileValues)
        HUDAlphaNewProfile = value;
    HUDAlpha = value;
    ApplyValues();
    if (save)
        Save();
}
