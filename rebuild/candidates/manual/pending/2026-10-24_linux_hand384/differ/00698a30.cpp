// Grade: DIFFER(239v230) -- only the closing CDefPointer<const CEntitySoundDef> release differs: retail `mov ecx,[esp+4] ; test ecx,ecx ; je ; dec dword [ecx+4] ; jne ; mov edx,[ecx] ; call [edx+4]` (flags of the dec, pointer kept in ecx); ours `mov eax,[esp+4] ; test ; je ; dec [eax+4] ; mov ecx,[esp+4] ; mov eax,[ecx+4] ; test eax,eax ; jne` -- the escaped holder (its address went to GetSubDef) is re-read after the dec. Every call, argument, temporary slot and branch above it matches. Tried: cast-in-dtor `--((CDefBase*)Object)->RefCount`, CDefBase* member + plain `--Object->RefCount == 0`, register local `object = Object` (load/dec/store, worse), mutable count + inline const Release (worse).
// PlayCameraSoundThroughObservedThing(const CThing* thing, const CCameraManagerDef& def, const CCharString& sound) -- play a camera sound on the observed thing's sound player: the camera def's entity-sound sub-def maps the name to a sample pair (played as a sample) or else to a criteria string (played as criteria)
// Leaf: ?PlayCameraSoundThroughObservedThing@?A0x
// (Leaf is a prefix: the anonymous-namespace hash varies with the file path; it excludes the Call... wrapper.)
//
// Retail 0x00698A30, 230 bytes (+ int3 padding to 240), __fastcall `ret 4` (thing in ecx, def in edx, the sound name
//   on the stack).
// Callees: CDefClassBase::GetSubDef<CEntitySoundDef>(CDefPointer<const CEntitySoundDef>*) const @ 0x0065F604 (landed),
//   the non-const CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inline GetTC of interface 0x10),
//   CSoundMap::GetPSoundPair @ 0x0065FBA4 (landed), CSoundInit::CSoundInit() @ 0x0041C930,
//   CTCSoundPlayer::PlayAttachedSample(const CSoundPair&, const CSoundInit&, bool, bool, bool) @ 0x0066B280 (landed),
//   CSoundMap::GetPCriteriaString @ 0x0069D090 (this chunk), CDefString::operator CCharString() const @ 0x00415DD0,
//   CTCSoundPlayer::PlayAttachedCriteria(const CCharString&, bool, bool, bool, long) @ 0x0066BD00 (landed),
//   ~CCharString @ 0x0099EAE0; the sub-def's virtual slot 1 (destroy) on the last release.
// Callers: nine CGameCameraManager members (0x00698B3B .. 0x00699AB0), e.g. PlayCameraSoundThroughObservedThing(const
//   CCharString&) const 0x00698B20 = `::PlayCameraSoundThroughObservedThing(ObservedThing.GetPItem(), *PDef, sound)`.
// Layout: retail CThing interface bitset +0x20 (0x10 = bit 16), interface map +0x44; Ego_r.pdb CEntitySoundDef
//   SoundMap +0x28 (CSoundMap: SoundMap +0, CriteriaMap +0x10); the def's refcount +4 behind its vptr.
// Shape: `CDefPointer<const CEntitySoundDef> soundDef;` (null, set before the thing test so the null path skips the
//   release) `if (thing && def.GetSubDef<CEntitySoundDef>(&soundDef) && thing->HasTC(0x10)) { player = GetTC(0x10); if (pair =
//   map.GetPSoundPair(sound)) player->PlayAttachedSample(*pair, CSoundInit(), true, false, false); else if (criteria =
//   map.GetPCriteriaString(sound)) player->PlayAttachedCriteria(*criteria, true, false, false, -1); }` -- the criteria's
//   CCharString temporary is homed in the dead `sound` argument slot; the CSoundInit temporary is the 0x20-byte frame
//   block.  The interface map is reached through the non-const LowerBound copy although the thing is const (modelled
//   with a const_cast in the inline GetTC).
// Name: Ego_r.pdb `?PlayCameraSoundThroughObservedThing@?A0xa59f4854@@YIXPBVCThing@@ABVCCameraManagerDef@@ABVCCharString@@@Z`.
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};

class CDefString {
public:
    long Index;
    operator CCharString() const;                           // 0x00415DD0
};

class CSoundPair {
public:
    long FirstSound;
    long LastSound;
};

class CSoundInit {                                          // 0x20 bytes
public:
    unsigned char Data[0x20];
    CSoundInit();                                           // 0x0041C930
};

class CSoundMap {
public:
    unsigned char Maps[0x28];                               // SoundMap +0, CriteriaMap +0x10 (two 0x10 CVectorMaps) + tail
    const CSoundPair* GetPSoundPair(const CCharString& name) const;        // 0x0065FBA4
    const CDefString* GetPCriteriaString(const CCharString& name) const;  // 0x0069D090
};

class CDefBase {
public:
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long RefCount;                                          // +0x04
};

class CEntitySoundDef : public CDefBase {
public:
    unsigned char _pad08[0x28 - 0x08];
    CSoundMap     SoundMap;                                 // +0x28
};

template <class T> class CDefPointer {                    // the pointee held through its CDefBase (refcount) view
public:
    CDefBase* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object) {
            if (--Object->RefCount == 0)
                Object->Destroy();
        }
    }
    T* operator->() const { return static_cast<T*>(Object); }
};

class CDefClassBase {
public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // <CEntitySoundDef> 0x0065F604
};

class CCameraManagerDef : public CDefClassBase { };

class CTCSoundPlayer {
public:
    unsigned long PlayAttachedSample(const CSoundPair& sound, const CSoundInit& init, bool flag, bool loop,
                                     bool attached);        // 0x0066B280
    unsigned long PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long d);   // 0x0066BD00
};

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020 (the non-const copy)
};

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                        // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                            // +0x44
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline void* GetTC(long type) const
    {
        long key = type;
        CInterfaceMap* map = const_cast<CInterfaceMap*>(&m_interfaces);
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

namespace {
void __fastcall PlayCameraSoundThroughObservedThing(const CThing* thing, const CCameraManagerDef& def,
                                                    const CCharString& sound)
{
    CDefPointer<const CEntitySoundDef> soundDef;
    if (thing && def.GetSubDef<CEntitySoundDef>(&soundDef) && thing->HasTC(0x10)) {
        CTCSoundPlayer* player = (CTCSoundPlayer*)thing->GetTC(0x10);
        const CSoundMap& map = soundDef->SoundMap;
        const CSoundPair* pair = map.GetPSoundPair(sound);
        if (pair)
            player->PlayAttachedSample(*pair, CSoundInit(), true, false, false);
        else {
            const CDefString* criteria = map.GetPCriteriaString(sound);
            if (criteria)
                player->PlayAttachedCriteria(*criteria, true, false, false, -1);
        }
    }
}
}

void __fastcall CallPlayCameraSoundThroughObservedThing(const CThing* thing, const CCameraManagerDef& def,
                                                        const CCharString& sound)
{
    PlayCameraSoundThroughObservedThing(thing, def, sound);
}
