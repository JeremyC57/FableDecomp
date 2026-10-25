// CUserProfileManager::SetShadowDetail(float value, bool save) -- store the option (and its new-profile copy while UseNewProfileValues is set), push the live values to the engine, then save the profile when asked
// Leaf: ?SetShadowDetail@CUserProfileManager@@QAEXM_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040B590, 64 bytes (+ padding), thiscall, `ret 8`.
// Identity: PDB name set ?SetShadowDetail@CUserProfileManager@@QAEXM_N@Z; the stored field is ShadowDetail (retail +0x40, struct_layouts_egor
//   +0x40) and its new-profile copy ShadowDetailNewProfile (retail +0xFC, struct_layouts_egor +0x100); the retail offsets are the
//   PDB ones less the 4-byte STLport shrink of each container in front (none before +0x54; two vectors before +0x74;
//   0x4 bytes before the new-profile block).
// Label: the manifest's "SetAntialiasing" is propagated across the byte-identical setters.
// Callees: CUserProfileManager::ApplyValues() @ 0x00407370 (protected in the PDB name set), CUserProfileManager::Save()
//   @ 0x0040A740.
class CUserProfileManager {
public:
    unsigned char _pad00[0xC];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x33];
    float         ShadowDetail;              // +0x40
    unsigned char _pad44[0xB8];
    float         ShadowDetailNewProfile;    // +0xFC
    void SetShadowDetail(float value, bool save);
    void Save();                                 // 0x0040A740
protected:
    void ApplyValues();                          // 0x00407370
};
void CUserProfileManager::SetShadowDetail(float value, bool save)
{
    if (UseNewProfileValues)
        ShadowDetailNewProfile = value;
    ShadowDetail = value;
    ApplyValues();
    if (save)
        Save();
}