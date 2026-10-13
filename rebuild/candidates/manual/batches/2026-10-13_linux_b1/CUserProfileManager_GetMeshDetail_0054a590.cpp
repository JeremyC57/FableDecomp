// CUserProfileManager::GetMeshDetail()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054A590, oracle length 18 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov al,[ecx+0Ch] ; test al,al ; je L ; fld [ecx+0x100] ; ret ; L: fld [ecx+0x44] ; ret
// Manifest label `CUserProfileManager::GetCameraSensitivity` is shared by a family of byte-shaped
// siblings and is NOT trusted for this address; the field pair is what identifies the getter.
// Ego_r.pdb CUserProfileManager: +0x0C bool UseNewProfileValues, +0x44 float MeshDetail;
// the "NewProfile" copy of every setting sits 0xBC bytes later in retail (+0x100; the PDB
// places that block 4 bytes further, so the shipped build dropped one 4-byte field before it).
// While the options screen is editing a new profile the pending value is returned instead.

struct CUserProfileManager {
    unsigned char _pad_0x00[0x0C];
    bool          UseNewProfileValues;         // +0x0C (PDB)
    unsigned char _pad_0x0D[0x44 - 0x0D];
    float         MeshDetail;                  // +0x44 (PDB)
    unsigned char _pad_after[0x100 - 0x44 - 4];
    float         MeshDetailNewProfile;        // +0x100 (retail; PDB +0x104)
    float GetMeshDetail();
};

float CUserProfileManager::GetMeshDetail()
{
    if (this->UseNewProfileValues)
        return this->MeshDetailNewProfile;
    return this->MeshDetail;
}
