// CTCDoor::PlaySoundW(const CCharString& soundName) -- play the named sound of the door's CEntitySoundDef through the owner's sound player (interface 0x10)
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00730A00, length 210 bytes (+ int3 padding). __fastcall, `ret 4`.
//         Ego_r.pdb: `?PlaySoundW@CTCDoor@@IAEXABVCCharString@@@Z` (protected).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (interface 0x10, bitset word 0 bit 16),
//          CDefClassBase::GetSubDef<CEntitySoundDef>(CDefPointer<const CEntitySoundDef>*) const @ 0x0065F604,
//          CSoundMap::GetPSoundPair(const CCharString&) const @ 0x0065FBA4,
//          CTCSoundPlayer::PlayAttachedSample(const CSoundPair&, const CSoundInit&, bool, bool, bool) @ 0x0066B280,
//          the def's virtual slot 1 (+0x04, destroy on the last release).
// Shape: the sound player is the entry select of HasTC + GetTC (component kept in esi); the out def pointer shares the
//        lookup key's frame word and is zeroed after its address is pushed; the def is read once from the out slot into a
//        holder whose destructor releases it (`dec [edi+4] ; jne`, no re-read after the calls that saw the address).
//        CSoundInit is default-constructed (1.0, 1.0, 0, 1, 1, 0, 0, -1) with GainFaderBits set to 0x8010.
// Layout: engine/CTCDoor.h (its +0x04 `m4` is CTCBase::ParentThing); retail CThing PBaseDef.Object +0x70 (PDB +0x80);
//         CEntitySoundDef::SoundMap +0x28 (PDB).
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
#include "engine/CTCDoor.h"

class CTCBase;
class CCharString;
class CSoundPair;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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

class CTCSoundPlayer {                              // interface 0x10
public:
    unsigned long PlayAttachedSample(const CSoundPair& sound, const CSoundInit& init, bool attach, bool a, bool b);   // 0x0066B280
};

class CSoundMap {
public:
    const CSoundPair* GetPSoundPair(const CCharString& name) const;   // 0x0065FBA4
};

class CEntitySoundDef {
public:
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long RefCount;                                  // +0x04
    unsigned char _pad08[0x28 - 0x08];
    CSoundMap SoundMap;                             // +0x28
};

template<class T> class CDefPointer {               // the out slot: the def object only
public:
    T* Object;
    CDefPointer() : Object(0) {}
};

class CDefClassBase {
public:
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x0065F604 for T = CEntitySoundDef
};
void* FindDef_s7r_0();
long TmplSink_s7_1(const void* self, long seen);   // stand-in: the work FindDef_s7r_0 does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindDef_s7r_0 instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void* FindDef_s7r_0()
{
    long r = TmplSink_s7_1(0, 0);
    return (void*)r;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template<class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)

struct CSoundDefHolder {                            // releases the sub-def read once from the out slot
    CEntitySoundDef* Def;
    CSoundDefHolder(const CEntitySoundDef* def) : Def((CEntitySoundDef*)def) {}
    ~CSoundDefHolder()
    {
        if (Def && --Def->RefCount == 0)
            Def->Destroy();
    }
};

struct CBaseDefPointer {                           // CDefPointer<const CThingBaseDef>
    const CDefClassBase* Object;
    const CDefClassBase* operator->() const { return Object; }
};

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad54[0x1c];
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
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)key);
        CInterfaceMap::value_type* end = map->end();
        return (it == end || type < it->first ? end : it)->second;
    }
};

struct CTCDoor_Methods : CTCDoor {
    CThing* GetParentThing() const { return (CThing*)m4; }
    void PlaySoundW(const CCharString& soundName);
};

void CTCDoor_Methods::PlaySoundW(const CCharString& soundName)
{
    CThing* parent = GetParentThing();
    if (parent->HasTC(0x10)) {
        CTCSoundPlayer* player = (CTCSoundPlayer*)parent->GetTC(0x10);
        {
            const CDefClassBase* baseDef = parent->PBaseDef.operator->();
            CDefPointer<const CEntitySoundDef> out;
            bool found = baseDef->GetSubDef(&out);
            CSoundDefHolder soundDef(out.Object);
            if (found) {
                const CSoundPair* sound = soundDef.Def->SoundMap.GetPSoundPair(soundName);
                if (sound) {
                    CSoundInit init;
                    init.GainFaderBits = 0x8010;
                    player->PlayAttachedSample(*sound, init, true, false, false);
                }
            }
        }
    }
}