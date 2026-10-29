// NCreatureAI::Talk(CThingAICreature& creature, CThingCreatureBase& listener, const CCharString& sound_name, bool, bool tell_listener) -- play the creature's entity-sound for the name at the listener: needs the creature's sound player and entity-sound def and the listener's event component; with tell_listener, skip it while the listener already holds a type-0x12 event and post a CEventHeroHasBeenSpokenToByVillagers after playing
// Leaf: ?Talk@NCreatureAI@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008643A0, 320 bytes incl. int3 padding, /Gr `__fastcall` (creature in ecx,
//   listener in edx, the name, an unused bool and tell_listener on the stack), `ret 0Ch`.
// Identity: the catalog's "Talk" (_global) is the PDB name set's
//   `?Talk@NCreatureAI@@YIXAAVCThingAICreature@@AAVCThingCreatureBase@@ABVCCharString@@_N3@Z`; the first bool is never
//   read in retail.
// Callees: CCharString copy ctor 0x0099EC30 / dtor 0x0099EAE0, CVectorMap::LowerBound 0x0040F020 (non-const),
//   CDefClassBase::GetSubDef<CEntitySoundDef>(CDefPointer<const CEntitySoundDef>*) const 0x0065F604 (a same-TU noinline
//   stand-in, the GetSubDef<T> body of the landed AddWound 0x00757B30: external, the release reloads the def pointer
//   for the Destroy call), CSoundMap::GetPSoundPair(const CCharString&) const
//   0x0065FBA4, CTCEntityEvents::HasEventOfType(EEventType) const 0x00811340 (catalog "CPlayer::HasMode" is propagated),
//   CSoundInit::CSoundInit() 0x0041C930, CTCSoundPlayer::PlayAttachedSample(const CSoundPair&, const CSoundInit&, bool,
//   bool, bool) 0x0066B280, operator new 0x00BFEA1A, CTCEntityEvents::AddEvent(CEventBase*) 0x008113C0.
// Layout: CThing UsingComponents +0x20, Components +0x44, PBaseDef +0x70; CEntitySoundDef SoundMap +0x28 (Ego_r);
//   CSoundInit 32 bytes; NEntityEvents::CEventBase {vptr, long WorldFrame +4} (8 bytes); the event's vtable 0x01276E00
//   (RTTI NEntityEvents::CEventHeroHasBeenSpokenToByVillagers). TC interface types: 0x10 CTCSoundPlayer, 0x42
//   CTCEntityEvents.
// Shape: the name is copied into a local CCharString first, then the function-scope entity-sound CDefPointer (its
//   zero store lands between the HasTC(0x10) test and its branch, its storage the dead name-reference argument slot;
//   the release is skipped on the path that never called GetSubDef); the sound player and the listener's event
//   component through the direct-return non-const GetTC; the bool args to PlayAttachedSample are pushed before the CSoundInit temporary is constructed.
class CCharString {
public:
    void* PStringData;                                      // +0x00
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }

template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};

class CSoundPair;
class CSoundMap {                                           // CVectorMap<ulong, CSoundPair> + criteria map
public:
    unsigned char _pad0[0x28];
    const CSoundPair* GetPSoundPair(const CCharString& name) const;   // 0x0065FBA4
};

class CEntitySoundDef {
public:
    virtual void v00();
    virtual void Destroy() const;                           // slot 1
    mutable long  RefCount;                                 // +0x04
    unsigned char _pad08[0x28 - 0x08];
    CSoundMap     SoundMap;                                 // +0x28
};

class CDefinitionManager {
public:
    CDefPointer<const CEntitySoundDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};

class CDefClassBase {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual const long* GetSubDefIndex(const CCharString& name) const;   // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 4];
    CDefinitionManager* m_pManager;                         // +0x1c
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;
};

// Same-TU stand-in for GetSubDef<CEntitySoundDef> 0x0065F604 (the GetSubDef<T> body of the landed AddWound 0x00757B30).
#pragma auto_inline(off)
template <> bool CDefClassBase::GetSubDef<CEntitySoundDef>(CDefPointer<const CEntitySoundDef>* out) const
{
    const long* index;
    {
        CCharString name("CEntitySoundDef");
        index = GetSubDefIndex(name);
    }
    if (index) {
        CDefPointer<const CEntitySoundDef> result = m_pManager->GetPDefFromGlobalIndex(*index);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CThingBaseDef : public CDefClassBase { };

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                          // +0x00
    SEntry* last;                                           // +0x04
    SEntry* LowerBound(const ETCInterfaceType& type);                   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 UsingComponents bitset
    CInterfaceMap m_interfaces;                             // +0x44 Components
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<const CThingBaseDef> PBaseDef;              // +0x70

    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->first ? end : it)->second;
    }
};
class CThingCreatureBase : public CThing { };
class CThingAICreature : public CThingCreatureBase { };

class CSoundInit {                                          // 32 bytes
public:
    float         Volume;                                   // +0x00
    float         Pitch;                                    // +0x04
    float         Pan;                                      // +0x08
    long          NoPlays;                                  // +0x0C
    long          PlaybackType;                             // +0x10
    unsigned long GainFaderBits;                            // +0x14
    unsigned long PitchFaderBits;                           // +0x18
    long          OverrideID;                               // +0x1C
    CSoundInit();                                           // 0x0041C930
};

class CTCSoundPlayer {                                      // interface type 0x10
public:
    unsigned long PlayAttachedSample(const CSoundPair& pair, const CSoundInit& init, bool a, bool b, bool c);   // 0x0066B280
};

namespace NEntityEvents {
enum EEventType { EVENT_TYPE_0x12 = 0x12 };
class CEventBase {
public:
    CEventBase() : WorldFrame(0) { }
    virtual ~CEventBase();
    virtual EEventType GetType() const;
    long WorldFrame;                                        // +0x04
};
class CEventHeroHasBeenSpokenToByVillagers : public CEventBase {
public:
    virtual ~CEventHeroHasBeenSpokenToByVillagers();
    virtual EEventType GetType() const;
    virtual long GetLifeTime() const;
};
}

class CTCEntityEvents {                                     // interface type 0x42
public:
    bool HasEventOfType(NEntityEvents::EEventType type) const;    // 0x00811340
    void AddEvent(NEntityEvents::CEventBase* event);         // 0x008113C0
};

namespace NCreatureAI {

void __fastcall Talk(CThingAICreature& creature, CThingCreatureBase& listener, const CCharString& sound_name, bool,
                     bool tell_listener)
{
    CCharString name(sound_name);
    CDefPointer<const CEntitySoundDef> sound_def;
    if (creature.HasTC(0x10)) {
        CTCSoundPlayer* sound_player = (CTCSoundPlayer*)creature.GetTC(0x10);
        if (creature.PBaseDef->GetSubDef(&sound_def) && listener.HasTC(0x42)) {
            CTCEntityEvents* events = (CTCEntityEvents*)listener.GetTC(0x42);
            if (!tell_listener || !events->HasEventOfType(NEntityEvents::EVENT_TYPE_0x12)) {
                const CSoundPair* pair = sound_def->SoundMap.GetPSoundPair(name);
                if (pair) {
                    sound_player->PlayAttachedSample(*pair, CSoundInit(), true, false, false);
                    if (tell_listener)
                        events->AddEvent(new NEntityEvents::CEventHeroHasBeenSpokenToByVillagers);
                }
            }
        }
    }
}

}