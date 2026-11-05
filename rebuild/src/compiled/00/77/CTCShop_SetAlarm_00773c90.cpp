// CTCShop::SetAlarm(bool on) -- switch the shop alarm: on starts the looping SND_SHOPALARM_01 sample on the thing's sound player (once) and keeps its handle; off stops that sample and the attached sounds and clears the handle
// Leaf: SetAlarm
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00773C90, 297 bytes, `ret 4`.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (interface 0x10 = CTCSoundPlayer, twice), CCharString(const char*, long)
//   @ 0x0099EBF0 ("SND_SHOPALARM_01" @ 0x0126A934), CASoundBank::GetSoundIndexFromSymbol(const CCharString&) @ 0x00A01920 (on
//   CMainGameComponent::Get() @ 0x013B86A0 ->PSampleBank +0x10), ~CCharString @ 0x0099EAE0, CSoundInit ctor @ 0x0041C930,
//   CTCSoundPlayer::PlayAttachedSample(const CSoundPair&, const CSoundInit&, bool, long, long) @ 0x0066B280,
//   CTCSoundPlayer::StopSample(unsigned long) @ 0x0066AD90 (manifest "CGameScriptThing::SetToKillOnLevelUnload" is
//   propagated), CTCSoundPlayer::StopAllAttachedSounds @ 0x0066AEB0.
// Layout: Ego_r.pdb CTCShop Alarm +0x60 -> retail +0x54 (12-byte vectors, 12-byte map); CThing interface bitset +0x20 (0x10 =
//   bit 16), map +0x44; CSoundInit 0x20 bytes (the landed 0x0041C930 record: loop count +0x0C, flags +0x14).
// Shape: the alarm handle is read once per arm; the symbol string is an implicit conversion temporary (built before the
//   bank pointer is loaded) homed in the dead `on` slot, as is the lookup key; the bank comes through an inline getter (a
//   direct member read loads it before the string address); the sound pair is built from the index twice.
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
class CASoundBank { public:
public:
    long GetSoundIndexFromSymbol(const CCharString&) const;   // 0x00A01920
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad00[0x10];
    CASoundBank*  PSampleBank;                          // +0x10
    CASoundBank* GetSampleBank() const { return PSampleBank; }
};
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

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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
        if (Alarm == 0 && ParentThing->HasTC(TCI_SOUND_PLAYER)) {
            CTCSoundPlayer* player = ((CTCSoundPlayer*)&ParentThing->GetTC(TCI_SOUND_PLAYER));
            long sound = CMainGameComponent::Get()->GetSampleBank()->GetSoundIndexFromSymbol("SND_SHOPALARM_01");
            if (sound) {
                CSoundInit init;
                init.Flags = 0x8010;
                init.LoopCount = -1;
                Alarm = player->PlayAttachedSample(CSoundPair(sound, sound), init, true, 0, 0);
            }
        }
    } else {
        unsigned long alarm = Alarm;
        if (alarm && ParentThing->HasTC(TCI_SOUND_PLAYER)) {
            CTCSoundPlayer* player = ((CTCSoundPlayer*)&ParentThing->GetTC(TCI_SOUND_PLAYER));
            player->StopSample_0066ad90(alarm);
            player->StopAllAttachedSounds();
        }
        Alarm = 0;
    }
}