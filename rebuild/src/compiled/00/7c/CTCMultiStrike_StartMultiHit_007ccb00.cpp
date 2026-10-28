// CTCMultiStrike::StartMultiHit(const CHitParameters& hit, CThing* target) -- remember the target, the hit location, weapon, point and direction of the hit, start the strike (first hit, delay from the def's per-level DelayTimeSecs), spawn the def's HIT_EFFECT at the target, clear the weapon's strike effects, rumble the main player's joystick and feed the experience multi-hit combo
// Leaf: StartMultiHit
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CCB00, 619 bytes, `ret 8`.
// Callees: CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10
//   (CIntelligentPointer<CThing> vtable 0x01238C6C), __ftol2 @ 0x00BFEA70, GetSubDef<CSpecialEffectsDef> @ 0x00669580,
//   CalcAndLookup @ 0x00669D30, CCharString @ 0x0099EBF0/0x0099EAE0, CThing::GetPos @ 0x004C73D0, CreateParticleEffect
//   @ 0x0072AB70 (__fastcall), const LowerBound @ 0x004365B0 (0x13) / LowerBound @ 0x0040F020 (0x68), CTCMultiStrike::
//   RemoveEffectOnWeapon @ 0x007CC9E0, CThing::GetPlayer @ 0x004C7A30, CPlayer::IsMainPlayer @ 0x00487520,
//   GFGetSystemManager @ 0x009A4EC0, CThing::GetJoystickDeviceNumber @ 0x004C7CA0, the joystick's Rumble (slot 17),
//   the interface-0x68 component's ProcessMultiHitCombo @ 0x006D8830; the effects def's slot 1.
// Data: "HIT_EFFECT" 0x0126EE7C, frames per second 0x01375550.
// Layout: CTCMultiStrike as Ego_r.pdb (landed FrameUpdate 0x007CCE20); retail CHitParameters is 8 bytes below Ego_r after
//   its 12-byte map/vector: Position +0x02, Direction +0x0E, PHitLocation +0x48, PWeapon +0x58; the multi-strike def's
//   DelayTimeSecs vector at +0x6C (retail).
class CTCBase;
class CThing;
class CBaseObject;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };

class CDefPointeeBase { public:
    virtual void Destroy();
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
class CSpecialEffectsDef : public CDefPointeeBase { public:
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_CalcAndLookup(const void* def, const CCharString& name);   // stand-in callee
#pragma auto_inline(off)
// Stand-in body (out of line in retail): writes the out value only, so the effect index's address does not escape.
__declspec(noinline) bool CSpecialEffectsDef::CalcAndLookup(const CCharString& name, long* value)
{
    long found = Lookup_CSpecialEffectsDef_CalcAndLookup(this, name);
    if (found < 0)
        return false;
    *value = found;
    return true;
}
#pragma auto_inline(on)
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
void* FindDef_CDefClassBase_GetSubDef_0();
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<CSpecialEffectsDef> (the landed 0x007CC2B0 idiom): writes the out slot only.
bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
template <class T> struct CStdVector {                 // STLport vector, 12 bytes in retail
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](long i) const { return _M_start[i]; }
};
class CSpecialAbilitiesMultiStrikeDef : public CDefClassBase { public:
    unsigned char     _pad08[0x6c - 0x08];
    CStdVector<float> DelayTimeSecs;                   // +0x6c
};
extern long g_FramesPerSecond;                         // 0x01375550

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
    SEntry* LowerBound(const long* type) const;        // 0x004365B0
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                   // +0x04
    CBaseIntelligentPointer();                         // 0x00A01B10
    ~CBaseIntelligentPointer();                        // 0x00A01C10
    CThing* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                  // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem((CBaseObject*)other.GetPItem());
        return *this;
    }
};
#pragma pack(pop)

class CPlayer {
public:
    bool IsMainPlayer() const;                         // 0x00487520
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
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
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPos() const;              // 0x004C73D0
    CPlayer* GetPlayer() const;                        // 0x004C7A30
    long GetJoystickDeviceNumber() const;              // 0x004C7CA0
};
CThing* __fastcall CreateParticleEffect(long type, const C3DVector& position);   // 0x0072AB70

namespace NHitLocations { class CHitLocation; }
#pragma pack(push, 1)
class CHitParameters {                                 // retail layout (Ego_r minus 8 after the 12-byte containers)
public:
    bool                         Localised;            // +0x00
    bool                         Directional;          // +0x01
    C3DVector                    Position;             // +0x02
    C3DVector                    Direction;            // +0x0e
    unsigned char                _pad1a[0x48 - 0x1a];
    NHitLocations::CHitLocation* PHitLocation;         // +0x48
    unsigned char                _pad4c[0x58 - 0x4c];
    CIntelligentPointer<CThing>  PWeapon;              // +0x58
};
#pragma pack(pop)

struct CRumbleParams {                                 // 20 bytes
    float Strength;
    float Duration;
    float Delay;
    float Falloff;
    bool  Looping;
    CRumbleParams(float strength, float duration, float delay, float falloff, bool looping)
        : Strength(strength), Duration(duration), Delay(delay), Falloff(falloff), Looping(looping) { }
};
struct CJoystickDevice {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16();
    virtual void Rumble(long type, const CRumbleParams& params, long flag);   // slot 17 (+0x44)
};
struct CJoystickEntry {                                // 8 bytes
    CJoystickDevice* Device;
    long             Id;
};
class CInputManager { public:
    unsigned char   _pad00[0x2c];
    CJoystickEntry* Joysticks;                         // +0x2c (vector begin)
    CJoystickDevice* GetJoystick(long n) const { return Joysticks[n].Device; }
};
class CSystemManager { public:
    unsigned char  _pad00[0x58];
    CInputManager* InputManager;                       // +0x58
};
CSystemManager* __fastcall GFGetSystemManager();       // 0x009A4EC0

#pragma pack(push, 1)
struct CTCWeaponHolder {                               // interface 0x13 (descriptive)
    unsigned char _pad00[0x134];
    CBaseIntelligentPointer PWeapon;                   // +0x134
};
#pragma pack(pop)
struct CTCExperienceCombo {                            // interface 0x68 (descriptive)
    void ProcessMultiHitCombo(const CHitParameters& hit);   // 0x006D8830
};

class CTCMultiStrike {
public:
    void*                        __vftable;            // +0x00
    CThing*                      ParentThing;          // +0x04 (CTCBase)
    unsigned char                _pad08[0x0c - 0x08];
    CDefPointer<const CSpecialAbilitiesMultiStrikeDef> PDef;   // +0x0c
    long                         SpellLevel;           // +0x10
    bool                         StrikeStarted;        // +0x14
    long                         DelayTimer;           // +0x18
    long                         NumHits;              // +0x1c
    NHitLocations::CHitLocation* PHitLocation;         // +0x20
    CIntelligentPointer<CThing>  PWeapon;              // +0x24
    C3DVector                    PointOfHit;           // +0x2c
    C3DVector                    HitDirection;         // +0x38
    CIntelligentPointer<CThing>  PTarget;              // +0x44
    void RemoveEffectOnWeapon(CThing* weapon);         // 0x007CC9E0
    void StartMultiHit(const CHitParameters& hit, CThing* target);
};

void CTCMultiStrike::StartMultiHit(const CHitParameters& hit, CThing* target)
{
    PTarget = CIntelligentPointer<CThing>(target);
    PHitLocation = hit.PHitLocation;
    PWeapon = CIntelligentPointer<CThing>(hit.PWeapon.GetPItem());
    PointOfHit = hit.Position;
    HitDirection = hit.Direction;
    StrikeStarted = true;
    NumHits = 1;
    DelayTimer = (long)(g_FramesPerSecond * PDef->DelayTimeSecs[SpellLevel]);
    CDefPointer<CSpecialEffectsDef> effects;
    long effect = 0;
    bool found = ((CDefClassBase*)PDef.Object)->GetSubDef(&effects) && effects->CalcAndLookup("HIT_EFFECT", &effect);
    if (found)
        CreateParticleEffect(effect, target->GetPos());
    const CThing* parentThing = ParentThing;
    if (parentThing->HasTC(0x13)) {
        CThing* weapon = ((CTCWeaponHolder*)parentThing->GetTC(0x13))->PWeapon.GetPItem();
        if (weapon)
            RemoveEffectOnWeapon(weapon);
    }
    if (ParentThing->GetPlayer()->IsMainPlayer()) {
        CThing* parent = ParentThing;
        CInputManager* input = GFGetSystemManager()->InputManager;
        input->GetJoystick(parent->GetJoystickDeviceNumber())->Rumble(4, CRumbleParams(1.0f, 0.5f, 0.0f, 0.4f, true), 0);
    }
    CTCExperienceCombo* combo;
    if (ParentThing->GetInterface(&combo, 0x68))
        combo->ProcessMultiHitCombo(hit);
}