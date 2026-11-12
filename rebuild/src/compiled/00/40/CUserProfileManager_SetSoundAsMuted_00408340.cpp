class CUserProfileManager { public:
    char pad[0x31];
    char muted;
    void SetSoundAsMuted(char v);
protected:
    void ApplyValues();   // 0x00407370 ?ApplyValues@CUserProfileManager@@IAEXXZ (Ego_r.pdb / FableWin.pdb; NF28: was the free stand-in SoundMutedChanged())
};
void CUserProfileManager::SetSoundAsMuted(char v)
{
    if (this->muted != v) {
        this->muted = v;
        ApplyValues();
    }
}