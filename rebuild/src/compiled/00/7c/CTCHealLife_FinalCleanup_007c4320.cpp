// CTCHealLife::FinalCleanup() -- end the timeline of every live healing effect's particle emitter, then stop the parent's looping heal sound
// Leaf: ?FinalCleanup@CTCHealLife@@AAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C4320, 226 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb ?FinalCleanup@CTCHealLife@@AAEXXZ (private void()); its only references are the CTCHealLife
//   vtable thunks (vtable 0x0126E36C) 0x007C4410 (slot 14, OnKill) and 0x007C4420 (slot 2, OnRemove) that tail-jump here
//   (the ability-TC pattern of the landed CTCWillLightning::FinalCleanup 0x007A0950). Label correction: the landed
//   0x007C4420 is catalogued as FinalCleanup (it is OnRemove), 0x007C4410 as vfunc_14 (OnKill).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (inlined IS_THING_ALIVE, then the effect), CVectorMap LowerBound
//   0x0040F020 (inline GetInterface: 0x4B CTCDParticleEmitter, 0x10 CTCSoundPlayer), CTCDParticleEmitter::EnterTimelineEnd
//   0x006E05F0, CTCSoundPlayer::StopAttachedSound(unsigned long) 0x0066ADD0.
// Layout (retail): CTCHealLife LoopingSound +0x14, HealingEffects +0x28 (STLport vector<CIntelligentPointer<CThing> >, 8-byte
//   elements; Ego_r +0x30 less the 4-byte retail ThingsToHeal list).
// Shape: `for (long i = 0; i < (long)HealingEffects.size(); ++i)` (the `test eax,0FFFFFFF8h` guard); the emitter is one
//   zero-initialised local declared before the loop (its frame slot doubles as the lookup key) and is used without a
//   presence test; the sound lookup is tested.
class CThing;
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    void Kill(bool immediately);                    // 0x004C9B80
    void RemoveTC(long interfaceType);              // 0x004C9840
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCDParticleEmitter {                         // interface 0x4B
public:
    void EnterTimelineEnd();                        // 0x006E05F0
};

class CTCSoundPlayer {                              // interface 0x10
public:
    void StopAttachedSound(unsigned long sound);    // 0x0066ADD0
};


class CTCHealLife {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x14 - 0x08];
    unsigned long LoopingSound;                     // +0x14
    unsigned char _pad18[0x28 - 0x18];
    std::vector<CIntelligentPointer<CThing> > HealingEffects;   // +0x28
private:
    void FinalCleanup();
};

void CTCHealLife::FinalCleanup()
{
    CTCDParticleEmitter* emitter = 0;
    for (long i = 0; i < (long)HealingEffects.size(); ++i) {
        if (IS_THING_ALIVE(HealingEffects[i])) {
            HealingEffects[i].GetPItem()->GetInterface(&emitter, 0x4b);
            emitter->EnterTimelineEnd();
        }
    }
    CTCSoundPlayer* sound;
    if (ParentThing->GetInterface(&sound, 0x10))
        sound->StopAttachedSound(LoopingSound);
}