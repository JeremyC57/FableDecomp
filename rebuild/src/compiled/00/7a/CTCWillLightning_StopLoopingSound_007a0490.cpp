// CTCWillLightning::StopLoopingSound() -- stop the lightning power's looping sound through the caster's sound player and forget its handle
// Leaf: ?StopLoopingSound@CTCWillLightning@@QAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A0490, 96 bytes, __fastcall, plain `ret`.
// Identity: CTCWillLightning code block (vtable 0x0126C724); the mirror of the landed 0x007A0400 (which restarts the sound when
//   the handle is invalid); Ego_r.pdb `?StopLoopingSound@CTCWillLightning@@QAEXXZ` / `?StartLoopingSound@...`. Callers
//   0x007A099B (the ctor block) and 0x007A29B8. The catalog's "CThing_OnLevelUnload" is descriptive.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inline GetInterface: 0x10 = the sound player,
//   bitset word +0x20 bit 16), CTCSoundPlayer::StopAttachedSound(unsigned long) 0x0066ADD0. g_InvalidSoundHandle 0x0129D25C.
// Layout: retail CTCWillLightning LoopingSound +0x98 (PDB +0xa4; the PDB list / intelligent pointers are 12 bytes larger).
// Shape: `sound = LoopingSound; if (sound != g_InvalidSoundHandle) { if (has-0x10) player->StopSample(sound); LoopingSound =
//   g_InvalidSoundHandle; }` -- the handle is a named local (edi from the compare to the call; reading the member twice
//   compares against memory); GetInterface stores through the ONE-ternary spelling (the landed CancelAimingWeapon
//   0x00789FF0 form), which loads the component straight into ecx (`mov ecx,[eax+4]`; the if/assign form goes through
//   eax); the global is read again for the store.
// Label correction: catalogued as `CThing_OnLevelUnload`; the body is CTCWillLightning::StopLoopingSound.
class CTCBase;

extern unsigned long g_InvalidSoundHandle;               // 0x0129D25C

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCSoundPlayer {                                   // interface 0x10
public:
public:
    void StopAttachedSound(unsigned long);   // 0x0066ADD0
};

class CTCWillLightning {
public:
    virtual ~CTCWillLightning();
    CThing*       ParentThing;                           // +0x04 (CTCBase)
    unsigned char _pad08[0x98 - 0x08];
    unsigned long LoopingSound;                          // +0x98

    void StopLoopingSound();
};

void CTCWillLightning::StopLoopingSound()
{
    unsigned long sound = LoopingSound;
    if (sound != g_InvalidSoundHandle) {
        CTCSoundPlayer* player;
        if (ParentThing->GetTC(TCI_SOUND_PLAYER, &player))
            player->StopAttachedSound(sound);
        LoopingSound = g_InvalidSoundHandle;
    }
}