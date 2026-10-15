// CUserProfileManager::SetAntialiasing(unsigned long, bool) -- store the value at +0x48 (and its new-profile copy at +0x104 while new-profile values are in use), apply the values, optionally save
// Retail: Fable.exe @ 0x0040B690, 49 bytes (+ int3 padding), `ret 8`.
// Callees: CUserProfileManager::ApplyValues @ 0x00407370, CUserProfileManager::Save @ 0x0040A740.
// Label note: by the Ego_r.pdb layout (retail = PDB below the first STL vector, PDB - 4 in the new-profile block)
// +0x48 / +0x104 are Resolution / ResolutionNewProfile, so this is the resolution setter; the manifest's
// "SetAntialiasing" is shared by the whole setter family. Same shape as the landed 0x0040B6D0 (+0x4c / +0x108).
// Shape: the flag byte is loaded and tested before the argument load (`mov al ; test al,al ; mov eax,[esp+8]`);
// the bool argument is tested as a byte.
struct CUserProfileManager {
    void* _vtables[2];                  // +0x00 / +0x04
    void* ProfileName;                  // +0x08 (CWideString)
    bool UseNewProfileValues;           // +0x0c
    unsigned char _pad0d[0x48 - 0x0d];
    unsigned long Resolution;           // +0x48
    unsigned char _pad4c[0x104 - 0x4c];
    unsigned long ResolutionNewProfile; // +0x104
    void ApplyValues();                 // 0x00407370
    void Save();                        // 0x0040A740
    void SetAntialiasing(unsigned long value, bool save);
};

void CUserProfileManager::SetAntialiasing(unsigned long value, bool save)
{
    if (UseNewProfileValues)
        ResolutionNewProfile = value;
    Resolution = value;
    ApplyValues();
    if (save)
        Save();
}
