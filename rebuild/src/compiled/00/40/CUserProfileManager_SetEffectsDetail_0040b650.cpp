// CUserProfileManager::SetEffectsDetail(float value, bool save) -- store the option (and its new-profile copy while
// UseNewProfileValues is set), push the live values to the engine, then save the profile when asked
// Leaf: ?SetEffectsDetail@CUserProfileManager@@QAEXM_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040B650, thiscall, `ret 8`.
// Identity: PDB name set ?SetEffectsDetail@CUserProfileManager@@QAEXM_N@Z; the stored field is EffectsDetail (retail +0x70) and its
//   new-profile copy EffectsDetailNewProfile (retail +0x124) by struct_layouts_egor (retail = PDB below the first STL vector at
//   +0x54, PDB - 8 after the two AssignedInputs vectors, PDB - 4 in the new-profile block).
// Callees: CUserProfileManager::ApplyValues() @ 0x00407370 (protected in the PDB name set), CUserProfileManager::Save()
//   @ 0x0040A740 (builds the profile path and writes the file).
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x63];
    float         EffectsDetail;             // +0x70
    unsigned char _pad74[0xB0];
    float         EffectsDetailNewProfile;   // +0x124
    void SetEffectsDetail(float value, bool save);
    void Save();                                 // 0x0040A740
protected:
    void ApplyValues();                          // 0x00407370
};

void CUserProfileManager::SetEffectsDetail(float value, bool save)
{
    if (UseNewProfileValues)
        EffectsDetailNewProfile = value;
    EffectsDetail = value;
    ApplyValues();
    if (save)
        Save();
}
