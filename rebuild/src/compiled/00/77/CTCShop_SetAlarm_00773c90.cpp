// CTCShop::SetAlarm(bool on) -- switch the shop alarm: on starts the looping SND_SHOPALARM_01 sample on the thing's sound player (once) and keeps its handle; off stops that sample and the attached sounds and clears the handle
// Leaf: SetAlarm
// Retail: Fable.exe (TLC, VC7.1) @ 0x00773C90, 297 bytes, `ret 4`.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (interface 0x10 = CTCSoundPlayer, twice), CCharString(const char*, long)
//   @ 0x0099EBF0 ("SND_SHOPALARM_01" @ 0x0126A934), CASoundBank::GetSoundIndexFromSymbol(const CCharString&) @ 0x00A01920 (on
//   g_pMainGameComponent @ 0x013B86A0 ->PSampleBank +0x10), ~CCharString @ 0x0099EAE0, CSoundInit ctor @ 0x0041C930,
//   CTCSoundPlayer::PlayAttachedSample(const CSoundPair&, const CSoundInit&, bool, long, long) @ 0x0066B280,
//   CTCSoundPlayer::StopSample(unsigned long) @ 0x0066AD90 (manifest "CGameScriptThing::SetToKillOnLevelUnload" is
//   propagated), CTCSoundPlayer::StopAllAttachedSounds @ 0x0066AEB0.
// Layout: Ego_r.pdb CTCShop Alarm +0x60 -> retail +0x54 (12-byte vectors, 12-byte map); CThing interface bitset +0x20 (0x10 =
//   bit 16), map +0x44; CSoundInit 0x20 bytes (the landed 0x0041C930 record: loop count +0x0C, flags +0x14).
// Shape: the alarm handle is read once per arm; the symbol string is an implicit conversion temporary (built before the
//   bank pointer is loaded) homed in the dead `on` slot, as is the lookup key; the bank comes through an inline getter (a
//   direct member read loads it before the string address); the sound pair is built from the index twice.
class CTCBase;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CASoundBank { public:
public:
    long GetSoundIndexFromSymbol(const CCharString&) const;   // 0x00A01920
};
class CMainGameComponent { public:
    unsigned char _pad00[0x10];
    CASoundBank*  PSampleBank;                          // +0x10
    CASoundBank* GetSampleBank() const { return PSampleBank; }
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
class CSoundPair { public:
    long First;
    long Second;
    CSoundPair(long first, long second) : First(first), Second(second) {}
};
class CSoundInit { public:                                     // 0x20 bytes (ctor 0x0041C930)
    float Volume;                                       // +0x00
    float Pitch;                                        // +0x04
    long  Delay;                                        // +0x08
    long  LoopCount;                                    // +0x0c
    long  Priority;                                     // +0x10
    long  Flags;                                        // +0x14
    long  Pan;                                          // +0x18
    long  Id;                                           // +0x1c
    CSoundInit();                                       // 0x0041C930
};
class CTCSoundPlayer { public:
    void StopSample_0066ad90(unsigned long sample);              // 0x0066AD90
    void StopAllAttachedSounds();                       // 0x0066AEB0
public:
    unsigned long PlayAttachedSample(const CSoundPair&, const CSoundInit&, bool, bool, bool);   // 0x0066B280
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
class CTCShop { public:
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x54 - 0x08];
    unsigned long Alarm;                                // +0x54
    void SetAlarm(bool on);
};

void CTCShop::SetAlarm(bool on)
{
    if (on) {
        if (Alarm == 0 && ParentThing->HasTC(0x10)) {
            CTCSoundPlayer* player = ParentThing->GetTC<CTCSoundPlayer>(0x10);
            long sound = g_pMainGameComponent->GetSampleBank()->GetSoundIndexFromSymbol("SND_SHOPALARM_01");
            if (sound) {
                CSoundInit init;
                init.Flags = 0x8010;
                init.LoopCount = -1;
                Alarm = player->PlayAttachedSample(CSoundPair(sound, sound), init, true, 0, 0);
            }
        }
    } else {
        unsigned long alarm = Alarm;
        if (alarm && ParentThing->HasTC(0x10)) {
            CTCSoundPlayer* player = ParentThing->GetTC<CTCSoundPlayer>(0x10);
            player->StopSample_0066ad90(alarm);
            player->StopAllAttachedSounds();
        }
        Alarm = 0;
    }
}