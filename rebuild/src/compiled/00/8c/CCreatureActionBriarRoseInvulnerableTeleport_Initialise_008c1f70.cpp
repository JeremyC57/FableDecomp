// CCreatureAction_BriarRoseInvulnerableTeleport::Initialise() -- animation "INVULNERABLE_OUT_OF", base Initialise; push a teleport-intangible mode (living enemy); pop the TELEPORT_POP special effect at the creature and play its TELEPORT_AWAY_WANK sound
// Leaf: ?Initialise@CCreatureAction_BriarRoseInvulnerableTeleport
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C1F70, 712 bytes (+ int3 padding), virtual Initialise, plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetCombatAnimation
//   0x008574D0, CCombatActionBase::Initialise 0x00857520 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature
//   +8, PEnemy +0x114), CVectorMap::LowerBound 0x0040F020 (inlined GetInterface: 0x31 CTCCreatureModeManager bit 17 of
//   +0x24, 0x10 CTCSoundPlayer bit 16 of +0x20), operator new 0x00BFEA1A (0x80 bytes), CModeBriarRoseTeleportIntangible
//   (CThing* owner, CThingCreatureBase* enemy) 0x008C3700, CCountedPointer<CModeBase>(CModeBase*) 0x00718A00,
//   CTCCreatureModeManager::AddMode 0x0073ACC0, CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580 and
//   <CEntitySoundDef> 0x0065F604, CSpecialEffectsDef::CalcAndLookup 0x00669D30, CThing::GetPos 0x004C73D0,
//   CreateSpecialEffect 0x006E0880 (__fastcall), CSoundMap::GetPSound(Pair) 0x0065FBA4 (manifest
//   "CVectorMap_LowerBound_Faction" is propagated), CTCSoundPlayer::PlaySound(long, const CSoundInit&, long, long)
//   0x0066ACE0 (manifest "AI_PlaySoundAtCreatureWithStateGroup" is propagated). Strings "INVULNERABLE_OUT_OF" 0x01280374,
//   "TELEPORT_POP" 0x01280364, "TELEPORT_AWAY_WANK" 0x01280350.
// Layout: retail CCombatActionBase is 0x114 bytes (Ego_r.pdb 0x12c): PCreature +0x08; PEnemy +0x114 (PDB +0x12c), so `this`
//   is modelled locally (the engine header carries PDB offsets). CThing: interface bitset +0x20, map +0x44, def +0x70, dead
//   bit +0x91. CEntitySoundDef::SoundMap +0x28.
// Shape: the sound player out-slot is declared first and left uninitialised (its frame word is shared with the 0x31
//   lookup key); the two def lookups are `&&` conditions on CDefClassReference temporaries (AddRef/Release, flag bits in
//   ebx) with zeroed CDefPointer locals released by their inline dtors; GetSubDef / CalcAndLookup have same-TU out-only
//   stand-ins (non-template) so VC7.1 keeps the defs in registers. CSoundInit's default ctor sets PlaybackType in its
//   body after the list (retail stores OverrideID -1 before PlaybackType 1).
class CTCBase;
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
class CSpecialEffectsDef : public CDefBase { public:
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30
};
class CSoundMap { public:
    const long* GetPSound(const CCharString& name) const;       // 0x0065FBA4
};
class CEntitySoundDef : public CDefBase { public:
    unsigned char _pad08[0x28 - 0x08];
    CSoundMap     SoundMap;                             // +0x28
};
class CDefClassBase : public CDefBase { public:
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);       // 0x00669580
    bool GetSubDef(CDefPointer<CEntitySoundDef>* out);          // 0x0065F604
};
void* FindSubDef_CSpecialEffectsDef(CDefClassBase* defClass);  // stand-in callees of the stand-in bodies below
void* FindSubDef_CEntitySoundDef(CDefClassBase* defClass);
long Lookup_CSpecialEffectsDef(CSpecialEffectsDef* def, const CCharString& name);
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    void* def = FindSubDef_CSpecialEffectsDef(this);
    if (!def)
        return false;
    out->Object = (CSpecialEffectsDef*)def;
    return true;
}
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CEntitySoundDef>* out)
{
    void* def = FindSubDef_CEntitySoundDef(this);
    if (!def)
        return false;
    out->Object = (CEntitySoundDef*)def;
    return true;
}
__declspec(noinline) bool CSpecialEffectsDef::CalcAndLookup(const CCharString& name, long* value)
{
    long found = Lookup_CSpecialEffectsDef(this, name);
    if (found < 0)
        return false;
    *value = found;
    return true;
}
#pragma auto_inline(on)
struct CDefClassReference {                             // a counted reference to the thing's def class
    CDefClassBase* m_p;
    CDefClassReference(CDefClassBase* p) : m_p(p) { if (m_p) ++m_p->RefCount; }
    ~CDefClassReference() { if (m_p) { if (--m_p->RefCount == 0) m_p->Destroy(); } }
    CDefClassBase* operator->() const { return m_p; }
};
class CSoundInit {
public:
    float Volume, Pitch, Pan;
    long NoPlays, PlaybackType;
    unsigned long GainFaderBits, PitchFaderBits;
    long OverrideID;
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), GainFaderBits(0), PitchFaderBits(0), OverrideID(-1)
    {
        PlaybackType = 1;
    }
};
class CTCSoundPlayer { public:                                 // interface 0x10
    unsigned long PlaySound(long sound, const CSoundInit& init, long a, long b);   // 0x0066ACE0
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                              // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    const C3DVector& GetPos() const;               // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { public: };
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached);   // 0x006E0880

class CBaseObject;
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                                    // +0x04
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    operator CThing*() const { return ((CThing*)GetPItem()); }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {
    CThingCreatureBase* GetPItem() const { return (CThingCreatureBase*)CBaseIntelligentPointer::GetPItem(); }
};
struct CModeBase { };
template <class T> class CCountedPointer { public:
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer(T* object);                         // 0x00718A00
    CCountedPointer(const CCountedPointer& other);
};
struct CModeBriarRoseTeleportIntangible : CModeBase {
    unsigned char _pad[0x80];
    CModeBriarRoseTeleportIntangible(CThing* owner, CThingCreatureBase* enemy);   // 0x008C3700
};
class CTCCreatureModeManager { public:
    bool AddMode(CCountedPointer<CModeBase> mode);      // 0x0073ACC0
};
class CCombatActionBase {
public:
    virtual void Initialise();                          // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                  // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    void SetCombatAnimation(const CCharString& name);     // 0x008574D0
};
class CCreatureAction_BriarRoseInvulnerableTeleport : public CCombatActionBase {
public:
    CIntelligentPointerCreature PEnemy;                 // +0x114 (PDB +0x12c)
    virtual void Initialise();
};

void CCreatureAction_BriarRoseInvulnerableTeleport::Initialise()
{
    CTCSoundPlayer* soundPlayer;
    {
        CCharString name("INVULNERABLE_OUT_OF");
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modeManager;
    if (((CThing*)PCreature.GetPItem())->GetInterface(&modeManager, 0x31) && PEnemy.GetPItem() && !(PEnemy.GetPItem()->m_flags91 & 1))
        modeManager->AddMode(CCountedPointer<CModeBase>(new CModeBriarRoseTeleportIntangible(PCreature, PEnemy.GetPItem())));
    CDefPointer<CSpecialEffectsDef> effects;
    long effect = 0;
    if (CDefClassReference(((CThing*)PCreature.GetPItem())->m_pDef)->GetSubDef(&effects) && effects->CalcAndLookup("TELEPORT_POP", &effect))
        CreateSpecialEffect(effect, ((CThing*)PCreature.GetPItem())->GetPos(), false);
    CDefPointer<CEntitySoundDef> sounds;
    if (((CThing*)PCreature.GetPItem())->GetInterface(&soundPlayer, 0x10) && CDefClassReference(((CThing*)PCreature.GetPItem())->m_pDef)->GetSubDef(&sounds)) {
        const long* sound = sounds->SoundMap.GetPSound("TELEPORT_AWAY_WANK");
        if (sound)
            soundPlayer->PlaySound(*sound, CSoundInit(), 0, 0);
    }
}