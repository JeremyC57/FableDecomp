// CTCBetting::PlaySoundW(const CCharString& name) -- play `name` on the parent thing's sound player; the invalid handle when it has none
// Leaf: ?PlaySoundW@CTCBetting@@QAEKABVCCharString@@@Z
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E51D0, 88 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?PlaySoundW@CTCBetting@@QAEKABVCCharString@@@Z (public; PlaySoundW is windows.h's
//   PlaySound); its callers are CTCBetting::ProcessInput 0x007E5230 (twice). The manifest's "CTCBetting::PlaySoundW" is propagated across byte-identical bodies.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inline CThing::GetInterface, interface 16),
//   CTCSoundPlayer::PlayAttachedCriteria(const CCharString&, bool, bool, bool, long) 0x0066BD00; g_InvalidSoundHandle 0x0129D25C.
// Shape (family M163, representative 0x005EA950): `CTCSoundPlayer* sound; if (ParentThing->GetInterface(&sound, 16)) return
//   sound->PlayAttachedCriteria(name, true, false, false, -1); return g_InvalidSoundHandle;` -- the landed
//   CTCWillLightning::StartLoopingSound 0x007A0400 GetInterface; the interface key in the `push ecx` word. Speed TU.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
extern unsigned long g_InvalidSoundHandle;                // 0x0129D25C
class CTCBase;
class CTCSoundPlayer {
public:
    unsigned long PlayAttachedCriteria(const CCharString& name, bool a, bool b, bool c, long d);   // 0x0066BD00
};

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCBetting {
public:
    void*   __vftable;                                    // +0x00 (CTCBase)
    CThing* ParentThing;                                  // +0x04
public:
    unsigned long PlaySoundW(const CCharString& name);
};
unsigned long CTCBetting::PlaySoundW(const CCharString& name)
{
    CTCSoundPlayer* sound;
    if (ParentThing->GetTC(TCI_SOUND_PLAYER, &sound))
        return sound->PlayAttachedCriteria(name, true, false, false, -1);
    return g_InvalidSoundHandle;
}