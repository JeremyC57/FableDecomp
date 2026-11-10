// CASoundManager::EnableMusic(bool enable) -- store the music-enabled flag when it changes; switching it off stops the music (slot 18 StopMusic(0))
// Name (agentSND, 2026-11-10): the twin of EnableSound 0x00991600 (flag +0x24, slot 16 StopAllSounds) and EnableAtmos 0x00991620 (flag
//   +0x26, slot 2 SetEnableAtmos); spellings from the Fable Anniversary 360 PDB publics ?EnableMusic@CASoundManager / ?EnableSound@ /
//   ?EnableAtmos@ (the same three setters in that order) with the TLC callers (cutscene verbs MuteSounds / EnableSounds, script
//   EnableSounds 0x0088F620, movie row 0x006286F0); see NAME_FIXES.tsv. Was CWorld::SetMiniMapEnabled (catalog).
// Retail: Fable.exe (TLC, VC7.1) @ 0x009915E0, `ret 4`.
// Leaf: EnableMusic
class CASoundManager {
public:
    virtual ~CASoundManager();
    virtual void Slot01();
    virtual void Slot02();
    virtual void Slot03();
    virtual void Slot04();
    virtual void Slot05();
    virtual void Slot06();
    virtual void Slot07();
    virtual void Slot08();
    virtual void Slot09();
    virtual void Slot10();
    virtual void Slot11();
    virtual void Slot12();
    virtual void Slot13();
    virtual void Slot14();
    virtual void Slot15();
    virtual void StopAllSounds();                                // slot 16 (+0x40)
    virtual void PlayMusic_Slot17();                             // slot 17 (+0x44) (engine/CASoundManager.h: PlayMusic)
    virtual void StopMusic(unsigned long fadeOutMS);             // slot 18 (+0x48)
    unsigned char _pad04[0x24 - 0x04];
    bool SoundEnabled;                                           // +0x24 (descriptive)
    bool MusicEnabled;                                           // +0x25 (descriptive)
    bool AtmosEnabled;                                           // +0x26 (descriptive)
    void EnableMusic(bool enable);
};

void CASoundManager::EnableMusic(bool enable)
{
    if (MusicEnabled == enable)
        return;
    MusicEnabled = enable;
    if (enable)
        return;
    StopMusic(0);
}
