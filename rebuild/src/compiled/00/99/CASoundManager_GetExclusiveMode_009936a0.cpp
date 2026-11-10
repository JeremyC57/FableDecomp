// CASoundManager::GetExclusiveMode() -- base CASoundManager::GetExclusiveMode: read the flag
// Name (agentSND, 2026-11-10): vtable slot 13 (+0x34); spelling from the Fable Anniversary 360 PDB publics (CSoundManagerUN / CASoundManager at the same vtable position) with the TLC slot evidence in SLOTS.tsv; was T::Get (gapscan_sub_009936a0).
// Leaf: GetExclusiveMode
// Member getter at this+0x20. __fastcall this=ecx. pack(1) keeps the field at
// the retail byte offset (natural alignment would shift an odd one).
#pragma pack(push,1)
struct CASoundManager {
    char pad[0x20];
    bool ExclusiveMode;
    bool GetExclusiveMode() const;
};
#pragma pack(pop)
bool CASoundManager::GetExclusiveMode() const { return this->ExclusiveMode; }