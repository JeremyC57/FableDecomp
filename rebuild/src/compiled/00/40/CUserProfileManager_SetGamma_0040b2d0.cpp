// CUserProfileManager::SetGamma(float value, bool save) -- store the option (and its new-profile copy while UseNewProfileValues is set), push the live values to the engine, then save the profile when asked
// Leaf: ?SetGamma@CUserProfileManager@@QAEXM_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040B2D0, 64 bytes (+ padding), thiscall, `ret 8`.
// Identity: PDB name set ?SetGamma@CUserProfileManager@@QAEXM_N@Z; the stored field is Gamma (retail +0x1C, struct_layouts_egor
//   +0x1C) and its new-profile copy GammaNewProfile (retail +0xDC, struct_layouts_egor +0xE0); the retail offsets are the
//   PDB ones less the 4-byte STLport shrink of each container in front (none before +0x54; two vectors before +0x74;
//   0x4 bytes before the new-profile block).
// Label: the manifest's "SetAntialiasing" is propagated across the byte-identical setters.
// Callees: CUserProfileManager::ApplyValues() @ 0x00407370 (protected in the PDB name set), CUserProfileManager::Save()
//   @ 0x0040A740.
class CUserProfileManager {
public:
    unsigned char _pad00[0xC];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0xF];
    float         Gamma;                     // +0x1C
    unsigned char _pad20[0xBC];
    float         GammaNewProfile;           // +0xDC
    void SetGamma(float value, bool save);
    void Save();                                 // 0x0040A740
protected:
    void ApplyValues();                          // 0x00407370
};
void CUserProfileManager::SetGamma(float value, bool save)
{
    if (UseNewProfileValues)
        GammaNewProfile = value;
    Gamma = value;
    ApplyValues();
    if (save)
        Save();
}