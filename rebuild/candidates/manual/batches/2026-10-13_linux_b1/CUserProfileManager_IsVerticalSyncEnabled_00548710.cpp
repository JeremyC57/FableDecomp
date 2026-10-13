// CUserProfileManager::IsVerticalSyncEnabled()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00548710, oracle length 18 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov al,[ecx+0Ch] ; test al,al ; je L ; mov al [ecx+0x10c] ; ret ; L: mov al [ecx+0x50] ; ret
// Manifest label `CUserProfileManager::IsCameraResetting` is shared by a family of byte-shaped
// siblings and is NOT trusted for this address; the field pair is what identifies the getter.
// Ego_r.pdb CUserProfileManager: +0x0C bool UseNewProfileValues, +0x50 bool VerticalSyncEnabled;
// the "NewProfile" copy of every setting sits 0xBC bytes later in retail (+0x10c; the PDB
// places that block 4 bytes further, so the shipped build dropped one 4-byte field before it).
// While the options screen is editing a new profile the pending value is returned instead.

struct CUserProfileManager {
    unsigned char _pad_0x00[0x0C];
    bool          UseNewProfileValues;         // +0x0C (PDB)
    unsigned char _pad_0x0D[0x50 - 0x0D];
    bool          VerticalSyncEnabled;         // +0x50 (PDB)
    unsigned char _pad_after[0x10c - 0x50 - 1];
    bool          VerticalSyncEnabledNewProfile; // +0x10c (retail; PDB +0x110)
    bool IsVerticalSyncEnabled();
};

bool CUserProfileManager::IsVerticalSyncEnabled()
{
    if (this->UseNewProfileValues)
        return this->VerticalSyncEnabledNewProfile;
    return this->VerticalSyncEnabled;
}
