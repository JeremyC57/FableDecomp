// CUserProfileManager::SetCameraSensitivity(float value, bool save) -- store the option (and its new-profile copy while
// UseNewProfileValues is set), push the live values to the engine, then save the profile when asked
// Leaf: ?SetCameraSensitivity@CUserProfileManager@@QAEXM_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040B550, thiscall, `ret 8`.
// Identity: PDB name set ?SetCameraSensitivity@CUserProfileManager@@QAEXM_N@Z; the stored field is CameraSensitivity (retail +0x7C) and its
//   new-profile copy CameraSensitivityNewProfile (retail +0x130) by struct_layouts_egor (retail = PDB below the first STL vector at
//   +0x54, PDB - 8 after the two AssignedInputs vectors, PDB - 4 in the new-profile block).
// Callees: CUserProfileManager::ApplyValues() @ 0x00407370 (protected in the PDB name set), CUserProfileManager::Save()
//   @ 0x0040A740 (builds the profile path and writes the file).
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x6F];
    float         CameraSensitivity;         // +0x7C
    unsigned char _pad80[0xB0];
    float         CameraSensitivityNewProfile; // +0x130
    void SetCameraSensitivity(float value, bool save);
    void Save();                                 // 0x0040A740
protected:
    void ApplyValues();                          // 0x00407370
};

void CUserProfileManager::SetCameraSensitivity(float value, bool save)
{
    if (UseNewProfileValues)
        CameraSensitivityNewProfile = value;
    CameraSensitivity = value;
    ApplyValues();
    if (save)
        Save();
}
