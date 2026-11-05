// CTCUnholyPower::StopSound(unsigned long sound) -- if the parent thing has a sound player, stop the given attached sound on it
// Leaf: ?StopSound@CTCUnholyPower@@AAEXK@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Grade: RELOCATION_MATCH -- the bool GetInterface(T**, long) out-parameter form (landed 0x007A0490 idiom): the component pointer goes through eax before mov ecx,eax
// Retail: Fable.exe (TLC, VC7.1) @ 0x00794180, 71 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?StopSound@CTCUnholyPower@@AAEXK@Z (private); its only callers are CTCUnholyPower::FrameUpdate 0x00795300 (vtable slot 9 of CTCUnholyPower; catalogued as CTCDivineWrath::FrameUpdate); it reaches CTCSoundPlayer::StopAttachedSound 0x0066ADD0 (bound) through the parent's interface 16.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020, CTCSoundPlayer::StopAttachedSound 0x0066ADD0.
// Shape: the inline CThing::GetInterface (UsingComponents bit 16 = `test [eax+20h],10000h`, the LowerBound of a key copy
//   homed in the `push ecx` slot, then end() when the argument is below the found entry), stored through the out pointer.
#include "engine/CVectorMap.h"
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CThing;
class CTCBase {                                            // CTCBase head: vptr, ParentThing
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                  // +0x04
};
class CTCSoundPlayer : public CTCBase {
public:
    void StopAttachedSound(unsigned long sound);          // 0x0066ADD0
};

class CTCUnholyPower : public CTCBase {
private:
    void StopSound(unsigned long sound);
};
void CTCUnholyPower::StopSound(unsigned long sound)
{
    CTCSoundPlayer* player;
    if (ParentThing->GetTC(TCI_SOUND_PLAYER, &player))                       // the sound-player interface
        player->StopAttachedSound(sound);
}