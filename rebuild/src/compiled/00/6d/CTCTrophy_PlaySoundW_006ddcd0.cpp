// CTCTrophy::PlaySoundW(const CCharString&) -- plays a sound through the owner's sound component (interface 16) and returns its handle, or the invalid handle when the owner has no sound component
// Leaf: PlaySoundW
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DDCD0, length 95 bytes. PDB `?PlaySoundW@CTCTrophy@@AAEKABVCCharString@@@Z`
// (the W is windows.h's PlaySound macro applied to the original source).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CSoundInfo::PlaySound(name, 1, 0, 0, -1)
// @ 0x0066BD00 (on the component). The invalid-handle global @ 0x0129D25C is returned on the absent path.
// Shape: the presence word (+0x20) is loaded once and tested twice with the mask kept in ecx: an explicit HasTC
// test `&&` the inline out-parameter GetInterface template (landed 0x007504E0 / 0x00849250 shape: key 16 in the
// `push ecx` frame word, the map pointer a named local, end-or-larger-key -> the end entry).
class CCharString;

class CTCSoundPlayer { public:
public:
    unsigned long PlayAttachedCriteria(const CCharString&, bool, bool, bool, long);   // 0x0066BD00
};

class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

extern unsigned long g_InvalidSoundHandle;  // 0x0129D25C

class CTCTrophy { public:
    void*   __vftable;                      // +0x00
    CThing* Thing;                          // +0x04
    unsigned long PlaySoundW(const CCharString& name);
};

unsigned long CTCTrophy::PlaySoundW(const CCharString& name)
{
    CTCSoundPlayer* sound;
    if (Thing->HasTC(TCI_SOUND_PLAYER) && Thing->GetTC(TCI_SOUND_PLAYER, &sound))
        return sound->PlayAttachedCriteria(name, true, 0, 0, -1);
    return g_InvalidSoundHandle;
}