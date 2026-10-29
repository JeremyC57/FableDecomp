// CUserProfileManager::SetAntialiasing(unsigned long value, bool save) -- store the option (and its new-profile copy while
// UseNewProfileValues is set), push the live values to the engine, then save the profile when asked
// Leaf: ?SetAntialiasing@CUserProfileManager@@QAEXK_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040B4D0, thiscall, `ret 8`.
// Identity: PDB name set ?SetAntialiasing@CUserProfileManager@@QAEXK_N@Z; the stored field is Antialiasing (retail +0x38) and its
//   new-profile copy AntialiasingNewProfile (retail +0xF4) by struct_layouts_egor (retail = PDB below the first STL vector at
//   +0x54, PDB - 8 after the two AssignedInputs vectors, PDB - 4 in the new-profile block).
// Callees: CUserProfileManager::ApplyValues() @ 0x00407370 (protected in the PDB name set), CUserProfileManager::Save()
//   @ 0x0040A740 (builds the profile path and writes the file).
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x2B];
    unsigned long Antialiasing;              // +0x38
    unsigned char _pad3C[0xB8];
    unsigned long AntialiasingNewProfile;    // +0xF4
    void SetAntialiasing(unsigned long value, bool save);
    void Save();                                 // 0x0040A740
protected:
    void ApplyValues();                          // 0x00407370
};

void CUserProfileManager::SetAntialiasing(unsigned long value, bool save)
{
    if (UseNewProfileValues)
        AntialiasingNewProfile = value;
    Antialiasing = value;
    ApplyValues();
    if (save)
        Save();
}
