// Grade: DIFFER(537v537) -- register allocation only: retail keeps this in edi, the promoted zero and the found-bool in ebx
//        and the weapon in a late-pushed ebp; ours puts this in ebp and the zero in edi (bool alone in a shrink-wrapped ebx).
//        Without the __forceinline ~CDefPointer the registers match but the def release is an out-of-line call (520v537).
// CTCAICreatureWillPowerIndicator::FrameUpdate() -- while the creature is in melee and not yet powered up, count the
// power-up timer down; once it is within the frame-rate window (and no indicator is alive) spawn the def's
// "POWER_UP_INDICATOR" effect at the creature, keep it as PEmitter and attach its particle emitter to the weapon in the
// primary carrying slot ("weapon_pos_a")
// Retail 0x007E0F20, 0x219 bytes. Callees: CVectorMap::LowerBound 0x0040F020 (inlined GetTC 0x30 and 0x46),
// CTCCreatureMelee?::IsInMelee 0x00733190, IS_THING_ALIVE<CIntelligentPointer<CThing>> 0x004CC340 and
// IS_THING_ALIVE<CThing*> 0x0040DCE0 (__fastcall), CDefClassBase::GetSubDef<CAICreatureWillPowerIndicatorDef> 0x00669580,
// CCharString(const char*, int) 0x0099EBF0 / ~CCharString 0x0099EAE0, GetEffectFromString 0x00669D30 (on the def),
// CThing::GetPosition 0x004C73D0, CTCCarrying::GetThingInPrimarySlot 0x006A1440, CreateEffectThing 0x006E0880
// (__fastcall), CIntelligentPointer<CThing>(CThing*) 0x004A96F0 / operator= 0x004A9760 / ~CBaseIntelligentPointer
// 0x00A01C10, CThing::GetTCDriver 0x004ACB50 (label <CTCDExperienceOrb>, bit 75), CTCDParticleEmitter::AttachToThing
// 0x006E0BE0. Global frame rate 0x01375550.
// Layout: Ego_r.pdb CTCAICreatureWillPowerIndicator (packed: PEmitter at +0x19).
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, int length = -1);   // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct CAICreatureWillPowerIndicatorDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
template <class T> struct CDefPointer {
    T* Data;
    CDefPointer() : Data(0) {}
    __forceinline ~CDefPointer() { if (Data) { if (--Data->RefCount == 0) Data->Destroy(); } }
    T* operator->() const { return Data; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CAICreatureWillPowerIndicatorDef>* out);   // 0x00669580
};
class CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    ~CBaseIntelligentPointer();                       // 0x00A01C10
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing(CThing* thing);          // 0x004A96F0
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other);   // 0x004A9760
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // <CIntelligentPointer<CThing>> 0x004CC340, <CThing*> 0x0040DCE0

struct CTCBase;
struct CTCMelee { bool IsInMelee() const; };          // 0x00733190
struct CTCCarrying { CThing* GetThingInPrimarySlot(); };   // 0x006A1440
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    CDefClassBase* GetDefClass() const { return m_pDef; }
    const C3DVector& GetPosition() const;             // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> bool GetTCDriver(T** out);     // 0x004ACB50
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
extern long g_FramesPerSecond;                        // 0x01375550

// Same-TU read-only stand-ins for the out-of-line callees that receive a local's address (retail's LTCG view: the
// addresses do not escape).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return *(void* const*)&thing != 0;
}
bool CDefClassBase::GetSubDef(CDefPointer<CAICreatureWillPowerIndicatorDef>* out)
{
    CAICreatureWillPowerIndicatorDef* def = *(CAICreatureWillPowerIndicatorDef**)this;
    if (out->Data == def)
        return true;
    out->Data = def;
    return def != 0;
}
bool CAICreatureWillPowerIndicatorDef::GetEffectFromString(const CCharString& name, long& type) const
{
    if (name.PStringData == 0)
        return false;
    type = RefCount;
    return true;
}
template <class T> __declspec(noinline) bool CThing::GetTCDriver(T** out)
{
    if (m_presentBits[2] & 0x800) {
        *out = (T*)m_pDef;
        return true;
    }
    return false;
}
#pragma auto_inline(on)


#pragma pack(push, 1)
class CTCAICreatureWillPowerIndicator {
public:
    void* __vftable;                                  // +0x00
    CThing* ParentThing;                              // +0x04
    bool ToBeRemoved;                                 // +0x08
    bool IsInGlobalUpdateTCs;                         // +0x09
    unsigned char _pad0a[2];
    bool PoweredUp;                                   // +0x0c
    unsigned char _pad0d[3];
    long Timer;                                       // +0x10
    long ResetCount;                                  // +0x14
    bool ShowEffect;                                  // +0x18
    CIntelligentPointerThing PEmitter;                // +0x19
    void FrameUpdate();
};
#pragma pack(pop)

void CTCAICreatureWillPowerIndicator::FrameUpdate()
{
    if (!ParentThing->HasTC(0x30))
        return;
    if (!((CTCMelee*)ParentThing->GetTC(0x30))->IsInMelee())
        return;
    if (PoweredUp)
        return;
    if ((float)Timer <= (float)g_FramesPerSecond && !IS_THING_ALIVE(PEmitter) && ShowEffect) {
        CDefPointer<CAICreatureWillPowerIndicatorDef> def;
        long type = 0;
        if (ParentThing->m_pDef->GetSubDef(&def) && def->GetEffectFromString("POWER_UP_INDICATOR", type)) {
            C3DVector position = ParentThing->GetPosition();
            if (ParentThing->HasTC(0x46)) {
                CThing* weapon = ((CTCCarrying*)ParentThing->GetTC(0x46))->GetThingInPrimarySlot();
                if (IS_THING_ALIVE<const CThing*>(weapon)) {
                    CThing* effect = CreateEffectThing(type, &position, false);
                    if (effect) {
                        {
                            CIntelligentPointerThing pointer(effect);
                            PEmitter = pointer;
                        }
                        CTCDParticleEmitter* emitter = 0;
                        effect->GetTCDriver(&emitter);
                        if (weapon)
                            emitter->AttachToThing(weapon, 0xc, "weapon_pos_a", 0, 0.0f);
                    }
                }
            }
        }
    }
    if (Timer <= 0)
        PoweredUp = true;
    else
        --Timer;
}
