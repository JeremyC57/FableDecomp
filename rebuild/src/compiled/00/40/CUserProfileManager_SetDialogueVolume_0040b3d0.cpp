// CUserProfileManager::SetDialogueVolume(float value, bool save) -- store the option (and its new-profile copy while
// UseNewProfileValues is set), push the live values to the engine, then save the profile when asked
// Leaf: ?SetDialogueVolume@CUserProfileManager@@QAEXM_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040B3D0, thiscall, `ret 8`.
// Identity: PDB name set ?SetDialogueVolume@CUserProfileManager@@QAEXM_N@Z; the stored field is DialogueVolume (retail +0x2C) and its
//   new-profile copy DialogueVolumeNewProfile (retail +0xEC) by struct_layouts_egor (retail = PDB below the first STL vector at
//   +0x54, PDB - 8 after the two AssignedInputs vectors, PDB - 4 in the new-profile block).
// Callees: CUserProfileManager::ApplyValues() @ 0x00407370 (protected in the PDB name set), CUserProfileManager::Save()
//   @ 0x0040A740 (builds the profile path and writes the file).
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x1F];
    float         DialogueVolume;            // +0x2C
    unsigned char _pad30[0xBC];
    float         DialogueVolumeNewProfile;  // +0xEC
    void SetDialogueVolume(float value, bool save);
    void Save();                                 // 0x0040A740
protected:
    void ApplyValues();                          // 0x00407370
};

void CUserProfileManager::SetDialogueVolume(float value, bool save)
{
    if (UseNewProfileValues)
        DialogueVolumeNewProfile = value;
    DialogueVolume = value;
    ApplyValues();
    if (save)
        Save();
}
