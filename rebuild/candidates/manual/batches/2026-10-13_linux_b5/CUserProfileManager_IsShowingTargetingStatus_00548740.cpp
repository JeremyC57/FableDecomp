// CUserProfileManager::IsShowingTargetingStatus()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00548740, oracle length 18 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov al,[ecx+0Ch] ; test al,al ; je C ; mov al,[ecx+134h] ; ret ; C: mov al,[ecx+6Ch] ; ret
// Same template as the landed profile getters (0x0054A530 etc.): UseNewProfileValues (+0x0C) picks
// the pending new-profile copy, else the current value. PDB +0x74 ShowTargetingStatus is +0x6C in
// retail (two 4-byte-smaller vectors before it); its NewProfile twin sits at retail +0x134. The
// manifest leaf `IsCameraResetting` is the propagated label this family carries.
struct CUserProfileManager_Retail {
    unsigned char _pad_0x00[0x0C];
    bool          UseNewProfileValues;              // +0x0C (PDB)
    unsigned char _pad_0x0D[0x6C - 0x0D];
    bool          ShowTargetingStatus;              // +0x6C (PDB +0x74)
    unsigned char _pad_0x6D[0x134 - 0x6D];
    bool          ShowTargetingStatusNewProfile;    // +0x134
    bool IsShowingTargetingStatus();
};

bool CUserProfileManager_Retail::IsShowingTargetingStatus()
{
    if (this->UseNewProfileValues)
        return this->ShowTargetingStatusNewProfile;
    return this->ShowTargetingStatus;
}
