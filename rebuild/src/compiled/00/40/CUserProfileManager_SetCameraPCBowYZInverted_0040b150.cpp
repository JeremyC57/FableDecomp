// CUserProfileManager::SetCameraPCBowYZInverted(bool value, bool save) -- store the option (and its new-profile copy while
// UseNewProfileValues is set), push the live values to the engine, then save the profile when asked
// Leaf: ?SetCameraPCBowYZInverted@CUserProfileManager@@QAEX_N0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040B150, thiscall, `ret 8`.
// Identity: PDB name set ?SetCameraPCBowYZInverted@CUserProfileManager@@QAEX_N0@Z; the stored field is CameraPCBowYZInverted (retail +0x14) and its
//   new-profile copy CameraPCBowYZInvertedNewProfile (retail +0xD4) by struct_layouts_egor (retail = PDB below the first STL vector at
//   +0x54, PDB - 8 after the two AssignedInputs vectors, PDB - 4 in the new-profile block).
// Callees: CUserProfileManager::ApplyValues() @ 0x00407370 (protected in the PDB name set), CUserProfileManager::Save()
//   @ 0x0040A740 (builds the profile path and writes the file).
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x7];
    bool          CameraPCBowYZInverted;     // +0x14
    unsigned char _pad15[0xBF];
    bool          CameraPCBowYZInvertedNewProfile; // +0xD4
    void SetCameraPCBowYZInverted(bool value, bool save);
    void Save();                                 // 0x0040A740
protected:
    void ApplyValues();                          // 0x00407370
};

void CUserProfileManager::SetCameraPCBowYZInverted(bool value, bool save)
{
    if (UseNewProfileValues)
        CameraPCBowYZInvertedNewProfile = value;
    CameraPCBowYZInverted = value;
    ApplyValues();
    if (save)
        Save();
}
