// CCreatureAction_NymphTeleportLimboBase::DoEffect() -- spawn the creature's TELEPORT_POS special effect at its position and attach its particle emitter to the creature's DUMMY_CENTRE
// Leaf: DoEffect
// Grade: DIFFER(294v296) -- retail caches the sub-def pointer in ebp right after GetSubDef (extra push ebp, frame offsets +4); ours re-reads it from its frame word for CalcAndLookup and the release; everything else matches
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C9FE0, 296 bytes (304 with padding).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +0x08), CDefClassBase::GetSubDef<CSpecialEffectsDef>
//   0x00669580, CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("TELEPORT_POS" 0x012821D0, "DUMMY_CENTRE" 0x0126F110),
//   CCRC::CalcAndLookup 0x00669D30, CThing::GetPosition 0x004C73D0, the special-effect Create(long, const C3DVector*, bool)
//   0x006E0880 (__fastcall, returns the effect thing), CVectorMap::LowerBound 0x0040F020 (inlined lookup of 0x4B =
//   CTCDParticleEmitter, bit 11 of +0x28), CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float)
//   0x006E0BE0.
// Shape: the JackDeathInto 0x008C0EA0 model: `found = GetSubDef(&def) && def->CalcAndLookup("TELEPORT_POS", &effect)` (the
//   conditionally built string tracked by a flag in ebx, the bool kept in a frame byte); the emitter pointer is a
//   zero-initialised local filled by an unchecked lookup; the GetSubDef same-TU stand-in carries retail's LTCG knowledge.
struct C3DVector { float x, y, z; };
struct CTCBase;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CSpecialEffectsDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30 (CCRC, at the def)
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* object) : Object(object) { if (Object) ++Object->RefCount; }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
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
struct CDefinitionManager {
    CDefPointer<CSpecialEffectsDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
struct CDefClassBase {
    virtual void v00(); virtual void Destroy(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    long RefCount;                                    // +0x04
    unsigned char _pad08[0x1c - 8];
    CDefinitionManager* m_pManager;                   // +0x1c
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<CSpecialEffectsDef> 0x00669580 (its landed body): VC7.1 then knows the sub-def pointer's
// address does not escape, so the def is held in esi and the name string reuses its frame word, as in retail.
bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    long* pIndex;
    {
        CCharString name("CSpecialEffectsDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CSpecialEffectsDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;             // 0x004C73D0
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
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
    CDefPointer<CDefClassBase> GetDef() const { return CDefPointer<CDefClassBase>(m_pDef); }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CThing* GetPItem() const;                         // 0x00A01B50
};
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector* position, bool attached);   // 0x006E0880

struct CTCDParticleEmitter {                          // interface type 0x4B
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CCombatActionBase {                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
};
struct CCreatureAction_NymphTeleportLimboBase : CCombatActionBase {
    void DoEffect();
};

void CCreatureAction_NymphTeleportLimboBase::DoEffect()
{
    CTCDParticleEmitter* emitter = 0;
    long effect = 0;
    CDefPointer<CSpecialEffectsDef> effects;
    CDefClassBase* thingDef = PCreature.GetPItem()->m_pDef;
    bool found = thingDef->GetSubDef(&effects) && effects->CalcAndLookup("TELEPORT_POS", &effect);
    if (found) {
        CThing* fx = CreateSpecialEffect(effect, &PCreature.GetPItem()->GetPosition(), false);
        if (fx && !(fx->m_flags91 & 1)) {
            fx->GetInterface(&emitter, 0x4B);
            emitter->AttachToThing(PCreature.GetPItem(), 10, "DUMMY_CENTRE", 0, 0);
        }
    }
}
