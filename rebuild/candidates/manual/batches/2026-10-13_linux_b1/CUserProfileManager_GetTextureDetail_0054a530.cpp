// CUserProfileManager::GetTextureDetail()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054A530, oracle length 18 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov al,[ecx+0Ch] ; test al,al ; je L ; fld [ecx+0xf8] ; ret ; L: fld [ecx+0x3c] ; ret
// Manifest label `CUserProfileManager::GetCameraSensitivity` is shared by a family of byte-shaped
// siblings and is NOT trusted for this address; the field pair is what identifies the getter.
// Ego_r.pdb CUserProfileManager: +0x0C bool UseNewProfileValues, +0x3c float TextureDetail;
// the "NewProfile" copy of every setting sits 0xBC bytes later in retail (+0xf8; the PDB
// places that block 4 bytes further, so the shipped build dropped one 4-byte field before it).
// While the options screen is editing a new profile the pending value is returned instead.

struct CUserProfileManager {
    unsigned char _pad_0x00[0x0C];
    bool          UseNewProfileValues;         // +0x0C (PDB)
    unsigned char _pad_0x0D[0x3c - 0x0D];
    float         TextureDetail;               // +0x3c (PDB)
    unsigned char _pad_after[0xf8 - 0x3c - 4];
    float         TextureDetailNewProfile;     // +0xf8 (retail; PDB +0xfc)
    float GetTextureDetail();
};

float CUserProfileManager::GetTextureDetail()
{
    if (this->UseNewProfileValues)
        return this->TextureDetailNewProfile;
    return this->TextureDetail;
}
