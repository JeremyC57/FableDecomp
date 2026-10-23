// CTCAssassinRush::CreateTrailingEffect() -- start the rush's effects from the special-effects sub-def: the startup trail (scaled by the spell level over the ability's maximum) and the displacement trail, both attached to the rusher and kept updating, then fade in the radial blur centred above the rusher
// Leaf: CreateTrailingEffect
// Retail 0x007A9FE0, 912 bytes, `ret`. Callees: CDefClassBase::GetSubDef 0x00669580, CCharString 0x0099EBF0 / dtor
// 0x0099EAE0, CCRC::CalcAndLookup 0x00669D30, LowerBound const 0x004365B0 (interface 0x6F) and 0x0040F020 (0x4B),
// CTCInventoryAbilities::GetAbilityMaximumLevel 0x005EDEA3, CThing::GetPosition 0x004C73D0, the scaled effect
// Create 0x006E11F0 (__fastcall), CreateSpecialEffect 0x006E0880 (__fastcall), CBaseIntelligentPointer ctor 0x00A01B10 /
// SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10, CTCDParticleEmitter::SetAlwaysUpdate 0x006E0590 and
// AttachToThing 0x006E0BE0, CTCBase::GetWorld 0x00686A70, RadialBlurFadeTo 0x006B8730, RadialBlurSetCenterWorldPos
// 0x006B8310. Literals 0x0126CFB0, 0x0126BC64, 0x0126CF9C, 0x0122D70E (""); CIntelligentPointer<CThing> vtable 0x01238C6C.
// Layout: Ego_r.pdb CTCAssassinRush less 4 past the 12-byte vector (PDef +0x14, AssassinTrail +0x18,
// AssassinDisplacementTrail +0x20, SpellLevel +0x8c, RadialBlurFadeKey +0x98); retail def blur block +0x44..+0x64.
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CSpecialEffectsDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
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
// does not escape the call (the landed 0x008C8380 idiom).
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
void* FindDef_CDefClassBase_GetSubDef_0();             // stand-in callee
struct CAssassinRushDef {
    unsigned char _pad00[0x44];
    float         BlurHeight;                           // +0x44
    float         BlurStrength;                         // +0x48
    float         BlurFadeTime;                         // +0x4c
    float         Blur[6];                              // +0x50..+0x64
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580 (CDefClassBase)
};
// Stand-in body (the real one @ 0x00669580 is out of line): it writes the out slot only (the landed 0x008C8380 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CAssassinRushDef::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
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
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_stateFlags;                         // +0x91 (bit 0: dead)
    const C3DVector& GetPosition() const;               // 0x004C73D0
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
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
CThing* __fastcall CreateScaledEffect(long lowEffect, long effect, float scale, const C3DVector& pos, bool attached);   // 0x006E11F0
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached);   // 0x006E0880
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_stateFlags & 1))
            return true;
    }
    return false;
}
struct CTCInventoryAbilities { long GetAbilityMaximumLevel(long ability); };   // 0x005EDEA3 (interface 0x6F)
struct CTCDParticleEmitter {                            // interface 0x4B
    void SetAlwaysUpdate(bool on);                      // 0x006E0590
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CEnvironmentSpellEffects {
    void* RadialBlurFadeTo(float strength, long mode, float a, float b, float c, float d, float e, float f);   // 0x006B8730
    void RadialBlurSetCenterWorldPos(void* key, const C3DVector& pos);   // 0x006B8310
};
struct CEnvironment {
    unsigned char _pad00[0x1964];
    CEnvironmentSpellEffects* SpellEffects;             // +0x1964
};
struct CWorld {
    unsigned char _pad00[0x18];
    CEnvironment* PEnvironment;                         // +0x18
};
struct CTCAssassinRush {
    void*                    __vftable;                 // +0x00
    CThing*                  ParentThing;               // +0x04
    unsigned char            _pad08[0x14 - 0x08];
    CAssassinRushDef*        PDef;                      // +0x14
    CIntelligentPointerThing AssassinTrail;             // +0x18
    CIntelligentPointerThing AssassinDisplacementTrail; // +0x20
    unsigned char            _pad28[0x8c - 0x28];
    long                     SpellLevel;                // +0x8c
    unsigned char            _pad90[0x98 - 0x90];
    void*                    RadialBlurFadeKey;         // +0x98
    CWorld* GetWorld();                                 // 0x00686A70
    void CreateTrailingEffect();
};

void CTCAssassinRush::CreateTrailingEffect()
{
    CDefPointer<CSpecialEffectsDef> effects;
    long lowEffect = 0;
    CThing* parent = ParentThing;
    if (PDef->GetSubDef(&effects)) {
        if (effects->CalcAndLookup("STARTUP_EFFECT_LOW", &lowEffect)) {
            long startEffect = 0;
            effects->CalcAndLookup("STARTUP_EFFECT", &startEffect);
            float scale = 0.0f;
            const CThing* constParent = parent;
            CTCInventoryAbilities* abilities;
            if (constParent->GetInterface(&abilities, 0x6f)) {
                long maxLevel = abilities->GetAbilityMaximumLevel(13);
                scale = (float)SpellLevel / maxLevel;
            }
            AssassinTrail = CIntelligentPointerThing(CreateScaledEffect(lowEffect, startEffect, scale, ParentThing->GetPosition(), false));
            if (IS_THING_ALIVE(AssassinTrail)) {
                CTCDParticleEmitter* emitter = 0;
                AssassinTrail.GetPItem()->GetInterface(&emitter, 0x4b);
                emitter->SetAlwaysUpdate(true);
                emitter->AttachToThing(ParentThing, 8, "", 0, 0.0f);
            }
        }
        long displacement = 0;
        if (effects->CalcAndLookup("DISPLACEMENT_EFFECT", &displacement)) {
            AssassinDisplacementTrail = CIntelligentPointerThing(CreateSpecialEffect(displacement, ParentThing->GetPosition(), false));
            if (IS_THING_ALIVE(AssassinDisplacementTrail)) {
                CTCDParticleEmitter* emitter = 0;
                AssassinDisplacementTrail.GetPItem()->GetInterface(&emitter, 0x4b);
                emitter->AttachToThing(ParentThing, 8, "", 0, 0.0f);
                emitter->SetAlwaysUpdate(true);
            }
        }
    }
    CEnvironmentSpellEffects* spellEffects = GetWorld()->PEnvironment->SpellEffects;
    RadialBlurFadeKey = spellEffects->RadialBlurFadeTo(PDef->BlurStrength, 2, PDef->Blur[0], PDef->Blur[1], PDef->Blur[2],
                                                      PDef->Blur[3], PDef->Blur[4], PDef->Blur[5]);
    C3DVector pos = parent->GetPosition();
    spellEffects->RadialBlurSetCenterWorldPos(RadialBlurFadeKey, C3DVector(pos.X, pos.Y, pos.Z + PDef->BlurHeight));
}