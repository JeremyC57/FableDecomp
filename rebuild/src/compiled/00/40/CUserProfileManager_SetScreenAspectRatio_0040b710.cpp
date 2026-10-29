// CUserProfileManager::SetScreenAspectRatio(unsigned long value, bool save) -- store the option (and its new-profile copy while
// UseNewProfileValues is set), push the live values to the engine, then save the profile when asked
// Leaf: ?SetScreenAspectRatio@CUserProfileManager@@QAEXK_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040B710, thiscall, `ret 8`.
// Identity: PDB name set ?SetScreenAspectRatio@CUserProfileManager@@QAEXK_N@Z; the stored field is ScreenAspectRatio (retail +0x74) and its
//   new-profile copy ScreenAspectRatioNewProfile (retail +0x128) by struct_layouts_egor (retail = PDB below the first STL vector at
//   +0x54, PDB - 8 after the two AssignedInputs vectors, PDB - 4 in the new-profile block).
// Callees: CUserProfileManager::ApplyValues() @ 0x00407370 (protected in the PDB name set), CUserProfileManager::Save()
//   @ 0x0040A740 (builds the profile path and writes the file).
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x67];
    unsigned long ScreenAspectRatio;         // +0x74
    unsigned char _pad78[0xB0];
    unsigned long ScreenAspectRatioNewProfile; // +0x128
    void SetScreenAspectRatio(unsigned long value, bool save);
    void Save();                                 // 0x0040A740
protected:
    void ApplyValues();                          // 0x00407370
};

void CUserProfileManager::SetScreenAspectRatio(unsigned long value, bool save)
{
    if (UseNewProfileValues)
        ScreenAspectRatioNewProfile = value;
    ScreenAspectRatio = value;
    ApplyValues();
    if (save)
        Save();
}
