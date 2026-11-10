// CASoundManager::SetExclusiveMode() -- base CASoundManager::SetExclusiveMode: store the flag
// Name (agentSND, 2026-11-10): vtable slot 12 (+0x30); spelling from the Fable Anniversary 360 PDB publics (CSoundManagerUN / CASoundManager at the same vtable position) with the TLC slot evidence in SLOTS.tsv; was T::Set (gapscan_sub_00993690).
// Leaf: SetExclusiveMode
// Byte-member setter at this+0x20 from a stack arg. __fastcall this=ecx (ret 4).
#pragma pack(push,1)
struct CASoundManager {
    char pad[0x20];
    bool ExclusiveMode;
    void SetExclusiveMode(bool value);
};
#pragma pack(pop)
void CASoundManager::SetExclusiveMode(bool value) { this->ExclusiveMode = value; }