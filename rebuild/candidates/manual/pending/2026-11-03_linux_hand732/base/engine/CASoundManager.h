#ifndef FABLE_ENGINE_CASOUNDMANAGER_H
#define FABLE_ENGINE_CASOUNDMANAGER_H
// Retail CASoundManager (the sound-manager base reached through PSoundManager 0x013B8394; movie row 0x006286F0).
// RTTI (vtable 0x0129A574) CASoundManager : CBaseClassNonCopyable : CBaseClass; the live object is a CSoundManagerLH
// (vtable 0x0129D2C4, RTTI CSoundManagerLH : CASoundManager). 54 virtual slots; slot 0 the scalar deleting destructor
// 0x009937B0; slots 3, 6, 12, 13, 52, 53 have base bodies, the other 47 are _purecall (0x00BFEA08) here.
//
// NAMES ARE NOT PROVEN. The sound module is absent from Ego_r.pdb and FableWin.pdb (no CASoundManager / CSoundManagerLH
// member in either name set; TPI holds only a forward CASoundManager record), Fable.exe has no string naming any of its
// members, and refs/fse_api_manifest.json has none. Every member below therefore carries a descriptive name: slot N is
// `VirtualNN_<address of the body it dispatches to>` (CSoundManagerLH's for pure slots, the base body otherwise), except
// slot 3 (the landed CASoundManager row 0x00991670 binds FlushLoadingQueue_00991670) and slot 46 (SetGroupVolume, the
// landed caller 0x0042E17C's descriptive name: CSoundManagerLH 0x00A3E9D0 walks the set bits of the mask). Parameter
// lists are retail's where a call proves them (slot 2 takes the bool 0x00991620 forwards; slot 18 the 0 0x009915E0
// passes; slot 46 (mask, volume, fade) from 0x006286F0 / 0x0042E17C; slots 12 / 52 / 53 `ret 4`), `()` otherwise.
// This header is the vtable shape for grading; it is not landable as named until the owner accepts descriptive slot names.
//
// Non-virtual members (the CASoundManager TU, between slot 6 0x009915C0 and slot 3 0x00991670, on members the base
// constructor 0x00991910 initialises to true): three bool setters that store a flag and, on a change, call a slot.
// Size: CSoundManagerLH's constructor 0x00A3EF00 calls ??0CASoundManager (0x00991910) and stores its own first member at
// +0x4C. The base members (landed constructor model 0x00991910: init-block settings +0x04..+0x18, Volume +0x1C, Flag
// +0x20, Enabled24 / Enabled25 / Enabled26 +0x24..+0x26, ResourceBank +0x28, Banks +0x30, +0x3C, Busy +0x3D; descriptive
// names, no PDB) are kept opaque.
#include "engine/CBaseClassNonCopyable.h"
class CASoundManager : public CBaseClassNonCopyable {
public:
    virtual ~CASoundManager();                                           // slot  0 (+0x00) ??_G 0x009937B0
    virtual void Virtual01_00a3e430() = 0;                      // slot  1 (+0x04) pure here; CSoundManagerLH 0x00a3e430
    virtual void Virtual02_00a3e3e0(bool) = 0;                  // slot  2 (+0x08) pure here; CSoundManagerLH 0x00a3e3e0
    virtual void FlushLoadingQueue_00991670();                  // slot  3 (+0x0C) 0x00991670; CSoundManagerLH 0x00a3eb20
    virtual void Virtual04_00a3ecb0() = 0;                      // slot  4 (+0x10) pure here; CSoundManagerLH 0x00a3ecb0
    virtual void Virtual05_00a3eb60() = 0;                      // slot  5 (+0x14) pure here; CSoundManagerLH 0x00a3eb60
    virtual bool Virtual06_009915c0();                          // slot  6 (+0x18) 0x009915c0; CSoundManagerLH 0x00a3ec70
    virtual void Virtual07_00a3e350() = 0;                      // slot  7 (+0x1C) pure here; CSoundManagerLH 0x00a3e350
    virtual void Virtual08_00a3e380() = 0;                      // slot  8 (+0x20) pure here; CSoundManagerLH 0x00a3e380
    virtual void Virtual09_00a3e3a0() = 0;                      // slot  9 (+0x24) pure here; CSoundManagerLH 0x00a3e3a0
    virtual unsigned long Virtual10_00a3ec90() = 0;             // slot 10 (+0x28) pure here; CSoundManagerLH 0x00a3ec90
    virtual void Virtual11_00a3e3b0() = 0;                      // slot 11 (+0x2C) pure here; CSoundManagerLH 0x00a3e3b0
    virtual void Virtual12_00993690(bool);                      // slot 12 (+0x30) 0x00993690 (both vtables)
    virtual bool Virtual13_009936a0();                          // slot 13 (+0x34) 0x009936a0 (both vtables)
    virtual void Virtual14_00a3ebf0() = 0;                      // slot 14 (+0x38) pure here; CSoundManagerLH 0x00a3ebf0
    virtual void Virtual15_00a3ec30() = 0;                      // slot 15 (+0x3C) pure here; CSoundManagerLH 0x00a3ec30
    virtual void Virtual16_00a3ebc0() = 0;                      // slot 16 (+0x40) pure here; CSoundManagerLH 0x00a3ebc0
    virtual void Virtual17_00a3e4b0() = 0;                      // slot 17 (+0x44) pure here; CSoundManagerLH 0x00a3e4b0
    virtual void Virtual18_00a3e520(long) = 0;                  // slot 18 (+0x48) pure here; CSoundManagerLH 0x00a3e520
    virtual void Virtual19_00a3e560() = 0;                      // slot 19 (+0x4C) pure here; CSoundManagerLH 0x00a3e560
    virtual void Virtual20_00a3e580() = 0;                      // slot 20 (+0x50) pure here; CSoundManagerLH 0x00a3e580
    virtual void Virtual21_00a3e5a0() = 0;                      // slot 21 (+0x54) pure here; CSoundManagerLH 0x00a3e5a0
    virtual void Virtual22_00a3e5c0() = 0;                      // slot 22 (+0x58) pure here; CSoundManagerLH 0x00a3e5c0
    virtual void Virtual23_00a3e5e0() = 0;                      // slot 23 (+0x5C) pure here; CSoundManagerLH 0x00a3e5e0
    virtual void Virtual24_00a3e600() = 0;                      // slot 24 (+0x60) pure here; CSoundManagerLH 0x00a3e600
    virtual void Virtual25_00a3e620() = 0;                      // slot 25 (+0x64) pure here; CSoundManagerLH 0x00a3e620
    virtual void Virtual26_00a3e640() = 0;                      // slot 26 (+0x68) pure here; CSoundManagerLH 0x00a3e640
    virtual void Virtual27_00a3e660() = 0;                      // slot 27 (+0x6C) pure here; CSoundManagerLH 0x00a3e660
    virtual void Virtual28_00a3e680() = 0;                      // slot 28 (+0x70) pure here; CSoundManagerLH 0x00a3e680
    virtual void Virtual29_00a3e6a0() = 0;                      // slot 29 (+0x74) pure here; CSoundManagerLH 0x00a3e6a0
    virtual void Virtual30_00a3e760() = 0;                      // slot 30 (+0x78) pure here; CSoundManagerLH 0x00a3e760
    virtual void Virtual31_00a3e780() = 0;                      // slot 31 (+0x7C) pure here; CSoundManagerLH 0x00a3e780
    virtual void Virtual32_00a3e7a0() = 0;                      // slot 32 (+0x80) pure here; CSoundManagerLH 0x00a3e7a0
    virtual void Virtual33_00a3e7c0() = 0;                      // slot 33 (+0x84) pure here; CSoundManagerLH 0x00a3e7c0
    virtual void Virtual34_00a3e7e0() = 0;                      // slot 34 (+0x88) pure here; CSoundManagerLH 0x00a3e7e0
    virtual void Virtual35_00a3e830() = 0;                      // slot 35 (+0x8C) pure here; CSoundManagerLH 0x00a3e830
    virtual void Virtual36_00a3e850() = 0;                      // slot 36 (+0x90) pure here; CSoundManagerLH 0x00a3e850
    virtual void Virtual37_00a3f140() = 0;                      // slot 37 (+0x94) pure here; CSoundManagerLH 0x00a3f140
    virtual void Virtual38_00a3ed90() = 0;                      // slot 38 (+0x98) pure here; CSoundManagerLH 0x00a3ed90
    virtual void Virtual39_00a3f250() = 0;                      // slot 39 (+0x9C) pure here; CSoundManagerLH 0x00a3f250
    virtual void Virtual40_00a3eed0() = 0;                      // slot 40 (+0xA0) pure here; CSoundManagerLH 0x00a3eed0
    virtual void Virtual41_00a3f340() = 0;                      // slot 41 (+0xA4) pure here; CSoundManagerLH 0x00a3f340
    virtual void Virtual42_00a3e920() = 0;                      // slot 42 (+0xA8) pure here; CSoundManagerLH 0x00a3e920
    virtual void Virtual43_00a3eae0() = 0;                      // slot 43 (+0xAC) pure here; CSoundManagerLH 0x00a3eae0
    virtual void Virtual44_00a3e540() = 0;                      // slot 44 (+0xB0) pure here; CSoundManagerLH 0x00a3e540
    virtual void Virtual45_00a3e980() = 0;                      // slot 45 (+0xB4) pure here; CSoundManagerLH 0x00a3e980
    virtual void SetGroupVolume(unsigned long,float,float) = 0; // slot 46 (+0xB8) pure here; CSoundManagerLH 0x00a3e9d0
    virtual void Virtual47_00a3ea20() = 0;                      // slot 47 (+0xBC) pure here; CSoundManagerLH 0x00a3ea20
    virtual void Virtual48_00a3ea80() = 0;                      // slot 48 (+0xC0) pure here; CSoundManagerLH 0x00a3ea80
    virtual void Virtual49_00a3e480() = 0;                      // slot 49 (+0xC4) pure here; CSoundManagerLH 0x00a3e480
    virtual void Virtual50_00a3e3c0() = 0;                      // slot 50 (+0xC8) pure here; CSoundManagerLH 0x00a3e3c0
    virtual void Virtual51_00a3e800() = 0;                      // slot 51 (+0xCC) pure here; CSoundManagerLH 0x00a3e800
    virtual void Virtual52_009936b0(long);                        // slot 52 (+0xD0) 0x009936b0; CSoundManagerLH 0x00a41800
    virtual void Virtual53_009936c0(long);                        // slot 53 (+0xD4) 0x009936c0; CSoundManagerLH 0x00a3e330
    void SetEnabled(bool);                                               // 0x00991600 +0x24; slot 16 on disable (landed binding: CSoundManagerLH::SetEnabled)
    void SetEnabled25_009915e0(bool);                                    // 0x009915E0 +0x25; slot 18 (0) on disable (descriptive)
    void SetEnabled26_00991620(bool);                                    // 0x00991620 +0x26; slot 2 (value) on change (descriptive)
private:
    unsigned char _pad_0x04[0x4c - 0x04];                                // +0x04 members (opaque, see above)
};
FABLE_STATIC_ASSERT(sizeof(CASoundManager) == 0x4c);

extern CASoundManager* PSoundManager;                                    // 0x013B8394 ?PSoundManager@@3PAVCASoundManager@@A (ESTABLISHED_SPELLINGS)

#endif // FABLE_ENGINE_CASOUNDMANAGER_H
