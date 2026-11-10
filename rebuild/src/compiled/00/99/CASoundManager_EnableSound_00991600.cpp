// CASoundManager::EnableSound(bool enable) -- store the sound-enabled flag when it changes; switching it off stops every sound (slot 16 StopAllSounds)
// Name (agentSND, 2026-11-10): see EnableMusic 0x009915E0 and NAME_FIXES.tsv. Was the free stand-in CWorld_SetMiniMapEnabled (catalog
//   CWorld::SetMiniMapEnabled).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00991600, `ret 4`.
// Leaf: EnableSound
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
    void EnableSound(bool enable);
};

void CASoundManager::EnableSound(bool enable)
{
    if (SoundEnabled != enable) {
        SoundEnabled = enable;
        if (!enable)
            StopAllSounds();
    }
}
