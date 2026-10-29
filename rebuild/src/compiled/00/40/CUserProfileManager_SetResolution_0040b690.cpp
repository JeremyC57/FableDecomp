// CUserProfileManager::SetResolution(unsigned long value, bool save) -- store the option (and its new-profile copy while
// UseNewProfileValues is set), push the live values to the engine, then save the profile when asked
// Leaf: ?SetResolution@CUserProfileManager@@QAEXK_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040B690, thiscall, `ret 8`.
// Identity: PDB name set ?SetResolution@CUserProfileManager@@QAEXK_N@Z; the stored field is Resolution (retail +0x48) and its
//   new-profile copy ResolutionNewProfile (retail +0x104) by struct_layouts_egor (retail = PDB below the first STL vector at
//   +0x54, PDB - 8 after the two AssignedInputs vectors, PDB - 4 in the new-profile block).
// Callees: CUserProfileManager::ApplyValues() @ 0x00407370 (protected in the PDB name set), CUserProfileManager::Save()
//   @ 0x0040A740 (builds the profile path and writes the file).
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x3B];
    unsigned long Resolution;                // +0x48
    unsigned char _pad4C[0xB8];
    unsigned long ResolutionNewProfile;      // +0x104
    void SetResolution(unsigned long value, bool save);
    void Save();                                 // 0x0040A740
protected:
    void ApplyValues();                          // 0x00407370
};

void CUserProfileManager::SetResolution(unsigned long value, bool save)
{
    if (UseNewProfileValues)
        ResolutionNewProfile = value;
    Resolution = value;
    ApplyValues();
    if (save)
        Save();
}
