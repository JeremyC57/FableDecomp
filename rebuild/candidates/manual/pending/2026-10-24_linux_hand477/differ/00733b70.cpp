// Grade: DIFFER(383v380) -- the conditional temporary's flag sits in ebx (mov ebx,1 / test bl,1) and the condition result in a frame byte, where retail keeps the flag word in the frame ([esp+18h], set by mov [..],esi=1) and the condition in bl; ours also reads the hero-combat def through the returned pointer (mov eax,[eax]) where retail re-reads the temporary's home slot. Frame (0x38), the spilled sound player, the sound def in edi, both lookups, the release blocks and the sound call match. Tried: one if-condition (388v380), named bool + Release-style local def pointer (385v380), same-TU GetSubDef<CEntitySoundDef> body (383v380, this file).
// CTCCombat::PlayCombatStrikeSounds() -- on a melee strike by a hero whose combat def allows it, play the hero-experience on-hit sound from the creature's entity sound def through its sound player
// Leaf: ?PlayCombatStrikeSounds@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00733B70, 380 bytes, __thiscall, plain `ret`.
// PDB: ?PlayCombatStrikeSounds@CTCCombat@@QAEXXZ (name set); catalog "CTCCreature_PlayDeathFlourish" was descriptive.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (non-const, interface 0x10) / 0x004365B0 (const,
//   interface 0x68), CDefClassBase::GetSubDef<CEntitySoundDef> 0x0065F604, CThingCreatureBase::GetHeroCombatDef 0x00663340
//   (CDefPointer by hidden pointer), CTCHeroExperience::GetOnHitSound 0x006D8BF0 (CCharString by hidden pointer),
//   CSoundMap::GetPSoundPair 0x0065FBA4, CTCSoundPlayer::PlayAttachedSample 0x0066B280, ~CCharString 0x0099EAE0, the
//   defs' slot 1 (Destroy) on the last release.
// Shape: one condition: has the sound player (0x10) && the thing def's CEntitySoundDef && has the hero experience (0x68)
//   && the creature's hero-combat def flag at +0xE2; the hero-combat def is a conditional temporary (frame flag word, bit
//   0) released after the condition through the ref-count base (`add eax,4`). The sound player and the experience
//   component are assigned inside the condition; the on-hit sound string reuses the experience pointer's frame word.
// Layout: CTCCombat (engine/CTCCombat.h): ParentThing +0x04 (`obj`), ParentCreature +0x24; retail CThing present bits
//   +0x20, interface map +0x44, PBaseDef +0x70; CEntitySoundDef SoundMap +0x28; CHeroCombatDef flag +0xE2 (retail).
#include "engine/CTCCombat.h"

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

struct CRefCounted {
    mutable long m_count;                           // +0x04 behind the vptr
    void DecRef() const { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
};
struct CDefObject : CDefVirtual, CRefCounted {
    void ReleaseTemporary() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
class CDefClassBase;
template <class T> struct CDefPointerTemp;
template <class T> struct CDefPointer {          // a def pointer held by the caller
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointerTemp<const CDefClassBase>& other);
    ~CDefPointer()
    {
        if (Object && --Object->m_count == 0)
            Object->Destroy();
    }
    __forceinline CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->m_count == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
    T* operator->() const { return Object; }
};
template <class T> struct CDefPointerTemp {      // a getter's by-value result
    T* Object;
    ~CDefPointerTemp()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};

class CSoundPair;
class CSoundInit {
public:
    float Volume;                                   // +0x00
    float Pitch;                                    // +0x04
    float Pan;                                      // +0x08
    long NoPlays;                                   // +0x0C
    long PlaybackType;                              // +0x10 ESoundPlaybackType
    unsigned long GainFaderBits;                    // +0x14
    unsigned long PitchFaderBits;                   // +0x18
    long OverrideID;                                // +0x1C
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), PlaybackType(1), GainFaderBits(0),
                   PitchFaderBits(0), OverrideID(-1) {}
};
class CSoundMap {
public:
    const CSoundPair* GetPSoundPair(const CCharString& name) const;   // 0x0065FBA4
};
struct CEntitySoundDef : CDefObject {
    unsigned char _pad08[0x28 - 0x08];
    CSoundMap SoundMap;                             // +0x28
};
struct CHeroCombatDef : CDefObject {
    unsigned char _pad08[0xe2 - 0x08];
    bool PlayStrikeSounds;                          // +0xE2 (retail; Ego_r.pdb field not resolved)
};

class CTCBase;
class CTCSoundPlayer {                              // interface 0x10
public:
    unsigned long PlayAttachedSample(const CSoundPair& sound, const CSoundInit& init, bool attach, bool a, bool b);   // 0x0066B280
};
class CTCHeroExperience {                           // interface 0x68
public:
    CCharString GetOnHitSound() const;              // 0x006D8BF0
};

template <class T> CDefPointer<T>::CDefPointer(const CDefPointerTemp<const CDefClassBase>& other)
    : Object((T*)other.Object) { if (Object) ++Object->m_count; }

class CDefinitionManager {
public:
    CDefPointerTemp<const CDefClassBase> GetPDefFromGlobalIndex(long index) const;   // 0x009ADA10
};

class CDefClassBase : public CDefObject {
public:
    virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual const long* GetSubDefIndex(const CCharString& name) const;   // slot 14 (+0x38)
    unsigned char _pad08[0x1c - 0x08];
    CDefinitionManager* m_pManager;                 // +0x1C
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x0065F604 for T = CEntitySoundDef
};

#pragma auto_inline(off)
// Same-TU body of the GetSubDef<CEntitySoundDef> instantiation (the template is compiled in every TU that uses it; the
// linker kept 0x0065F604): with it visible VC7.1 sees that the out pointer's address does not escape, so the sound def
// stays in edi across the later calls.
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    const long* pIndex;
    {
        CCharString name("CEntitySoundDef", -1);
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<const T> result(m_pManager->GetPDefFromGlobalIndex(*pIndex));
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU bodies of the two CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiations (template members compiled
// in the caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
__declspec(noinline) const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CBaseDefPointer {                            // CDefPointer<const CThingBaseDef>
    const CDefClassBase* Object;
    const CDefClassBase* operator->() const { return Object; }
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CBaseDefPointer PBaseDef;                       // +0x70 CDefPointer<const CThingBaseDef>
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

struct CThingCreatureBase : CThing {
    CDefPointerTemp<const CHeroCombatDef> GetHeroCombatDef() const;   // 0x00663340
};

struct CTCCombat_Methods : CTCCombat {
    CThing* GetParentThing() const { return (CThing*)obj; }
    void PlayCombatStrikeSounds();
};

void CTCCombat_Methods::PlayCombatStrikeSounds()
{
    CThing* thing = GetParentThing();
    CTCSoundPlayer* soundPlayer;
    CDefPointer<const CEntitySoundDef> soundDef;
    const CTCHeroExperience* heroExperience = 0;
    bool play = thing->HasTC(0x10)
        && (soundPlayer = (CTCSoundPlayer*)thing->GetTC(0x10), thing->PBaseDef->GetSubDef(&soundDef))
        && GetParentThing()->HasTC(0x68)
        && (heroExperience = (const CTCHeroExperience*)((const CThing*)GetParentThing())->GetTC(0x68),
            ParentCreature->GetHeroCombatDef()->PlayStrikeSounds);
    if (play) {
        CCharString sound = heroExperience->GetOnHitSound();
        const CSoundPair* pair = soundDef->SoundMap.GetPSoundPair(sound);
        if (pair)
            soundPlayer->PlayAttachedSample(*pair, CSoundInit(), true, false, false);
    }
}
