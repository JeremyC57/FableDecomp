// CCreatureAction_BanditKingUnstuck::Initialise() -- clear Knockdown, animation BK_UNSTUCK, drop creature mode 0x53, close the twin-blade "stuck" hit location and reopen the twin-blade one, create the UNSTUCK special effect at the creature and attach it through its particle emitter, then the base Initialise
// Leaf: ?Initialise@CCreatureAction_BanditKingUnstuck
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C8550, 586 bytes (592 with padding), virtual Initialise, plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetAnimationName
//   0x008574D0, CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (inline interface lookups
//   0x31 CTCCreatureModeManager, 0x7F CTCHitLocations, 0x4B CTCDParticleEmitter), CTCCreatureModeManager::RemoveMode
//   0x0073ABA0, CTCHitLocations::GetPHitLocationFromName 0x00815820, CHitLocation::SetEnabled 0x00816390,
//   CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CCRC::CalcAndLookup 0x00669D30, CThing::GetPosition 0x004C73D0,
//   the special-effect Create(long, const C3DVector&, bool) 0x006E0880 (__fastcall),
//   CTCDParticleEmitter::AttachToThing 0x006E0BE0, CCombatActionBase::Initialise 0x00857520.
// Data: "BK_UNSTUCK" 0x01281AEC, "HIT_LOCATION_TWINBLADE_STUCK" 0x01281ACC, "HIT_LOCATION_TWINBLADE" 0x01281AB4,
//   "UNSTUCK" 0x01281AAC, "" 0x0122D70E.
// Layout (retail, local model): CCombatActionBase is 0x114 bytes (PCreature +0x08); Knockdown +0x114 (PDB +0x12c, the
//   engine header carries PDB offsets); CThing def +0x70, dead flag +0x91 bit 0.
// Shape: the landed BanditKingStuck::Initialise (0x008C8380) model. The mode-manager and hit-location components use the
//   out-parameter GetInterface with no null test, so the miss path reads the never-written out slot (retail reads the
//   spilled `this` word); the emitter is the `T* c = 0; if (HasTC) c = GetTC(ternary)` accessor (`xor esi,esi` before the
//   bit test); the created effect is used directly (no intelligent-pointer member); the "" bone name is the implicit
//   const CCharString& conversion, built before the other AttachToThing arguments.
struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct CThing;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};

struct CSpecialEffectsDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
long Lookup_CSpecialEffectsDef_CalcAndLookup(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail @ 0x00669D30): it writes the out value only, so VC7.1 knows the local's address
// does not escape the call (the 0x008D9740 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::CalcAndLookup(const CCharString& name, long* value)
{
    long found = Lookup_CSpecialEffectsDef_CalcAndLookup(this, name);
    if (found < 0)
        return false;
    *value = found;
    return true;
}
#pragma auto_inline(on)
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
void* FindDef_CDefClassBase_GetSubDef_0();             // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x008C4170 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
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
#pragma auto_inline(on)

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;             // 0x004C73D0
    CDefClassBase* GetDef() const { return m_pDef; }
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline T* GetTCIfPresent(long interfaceType)
    {
        T* component = 0;
        if (HasTC(interfaceType))
            component = GetTC<T>(interfaceType);
        return component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached);   // 0x006E0880

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CThing* GetPItem() const;                         // 0x00A01B50
};
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long a, long b);   // 0x006E0BE0
};
struct CTCCreatureModeManager {                       // interface type 0x31
    void RemoveMode(unsigned long mode);              // 0x0073ABA0
};
struct CHitLocation {
    void SetEnabled(bool enabled);                    // 0x00816390
};
struct CTCHitLocations {                              // interface type 0x7F
    CHitLocation* GetPHitLocationFromName(const CCharString& name);   // 0x00815820
};
struct CCombatActionBase {                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
    void Initialise();                                // 0x00857520
};
struct CCreatureAction_BanditKingUnstuck : CCombatActionBase {
    bool Knockdown;                                   // +0x114 (PDB +0x12c)
    bool NoTarget;                                    // +0x115
    void Initialise();
};

void CCreatureAction_BanditKingUnstuck::Initialise()
{
    Knockdown = false;
    {
        CCharString name("BK_UNSTUCK");
        SetAnimationName(name);
    }
    CTCCreatureModeManager* modes;
    PCreature.GetPItem()->GetInterface(&modes, 0x31);
    modes->RemoveMode(0x53);
    CTCHitLocations* hitLocations;
    PCreature.GetPItem()->GetInterface(&hitLocations, 0x7f);
    CHitLocation* stuck = hitLocations->GetPHitLocationFromName("HIT_LOCATION_TWINBLADE_STUCK");
    CHitLocation* blade = hitLocations->GetPHitLocationFromName("HIT_LOCATION_TWINBLADE");
    if (stuck)
        stuck->SetEnabled(false);
    if (blade)
        blade->SetEnabled(true);
    CDefPointer<CSpecialEffectsDef> effects;
    long effect = 0;
    if (PCreature.GetPItem()->GetDef()->GetSubDef(&effects) && effects->CalcAndLookup("UNSTUCK", &effect)) {
        CThing* created = CreateSpecialEffect(effect, PCreature.GetPItem()->GetPosition(), false);
        if (created && !(created->m_flags91 & 1)) {
            CTCDParticleEmitter* emitter = created->GetTCIfPresent<CTCDParticleEmitter>(0x4b);
            emitter->AttachToThing(PCreature.GetPItem(), 0x2a, "", 0, 0);
        }
    }
    CCombatActionBase::Initialise();
}