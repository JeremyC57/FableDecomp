#ifndef FABLE_LIBPK_CSOUNDSYSTEM_H
#define FABLE_LIBPK_CSOUNDSYSTEM_H
// libpk sound system interface (agentC40, rows 0x00A3E520 / 0x00A3E830): the object CSoundManagerLH keeps at +0x40 (created by the
// factory 0x00C096E0; vtable 0x012B1E24). The audio library has no PDB and no RTTI names: every class and method name here is
// DESCRIPTIVE, spelled as the landed CSoundManagerLH rows spell them (CSoundSystem, CSoundPlayer, CSoundEventManager, GetEventManager);
// slots no landed row calls keep the rows' vNN spelling. A virtual's name never reaches the bytes (calls go through the vtable).
// Slot evidence: system slot 5 0x00C0A570 `mov eax,[ecx+60h] ; ret` (the music player: slot 1 play 0x00A3E4B0, slot 2 stop
// 0x00A3E520, slot 6 state 0x00A3E480), slot 6 0x00C0A590 `mov eax,[ecx+38h] ; ret` (the event manager: slot 1 0x00A3E830).
class CSoundPlayer {
public:
    virtual void v00();
    virtual void v01();                                                  // slot 1 (+0x04) play a music file (0x00A3E4B0)
    virtual void Stop(unsigned long fadeOutMS);                          // slot 2 (+0x08)
};
class CSoundEventManager {
public:
    virtual void v00();
    virtual void SetMonitorAllowedPostStopTailDurationMS(unsigned long duration);   // slot 1 (+0x04)
};
class CSoundSystem {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();                                                  // slot 4 (+0x10) initialise from the voice configuration (0x00A3EF00)
    virtual CSoundPlayer* GetPlayer();                                   // slot 5 (+0x14) 0x00C0A570
    virtual CSoundEventManager* GetEventManager();                       // slot 6 (+0x18) 0x00C0A590
};
#endif
