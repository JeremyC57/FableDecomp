// CTCBulletTime::StopSound(unsigned long sound) -- if the parent thing has a sound player, stop the given attached sound on it
// Leaf: ?StopSound@CTCBulletTime@@AAEXK@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Grade: RELOCATION_MATCH -- the bool GetInterface(T**, long) out-parameter form (landed 0x007A0490 idiom): the component pointer goes through eax before mov ecx,eax
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079cee0, 71 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?StopSound@CTCBulletTime@@AAEXK@Z (private); its only callers are CTCBulletTime::Pause 0x0079CF30; it reaches CTCSoundPlayer::StopAttachedSound 0x0066ADD0 (bound) through the parent's interface 16.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020, CTCSoundPlayer::StopAttachedSound 0x0066ADD0.
// Shape: the inline CThing::GetInterface (UsingComponents bit 16 = `test [eax+20h],10000h`, the LowerBound of a key copy
//   homed in the `push ecx` slot, then end() when the argument is below the found entry), stored through the out pointer.
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };
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
class CThing {                                             // Ego_r.pdb layout, retail offsets
public:
    unsigned char _pad00[0x20];                           // (members not touched here)
    unsigned long UsingComponents[9];                     // +0x20 bitset<274>
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    template <class T> __forceinline bool GetInterface(T** out, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(type);
            std::pair<ETCInterfaceType, CTCBase*>* last = map->end();
            *out = static_cast<T*>(((it == last || interfaceType < it->first) ? last : it)->second);
            return true;
        }
        return false;
    }
};
class CTCBulletTime : public CTCBase {
private:
    void StopSound(unsigned long sound);
};
void CTCBulletTime::StopSound(unsigned long sound)
{
    CTCSoundPlayer* player;
    if (ParentThing->GetInterface(&player, 16))                       // the sound-player interface
        player->StopAttachedSound(sound);
}