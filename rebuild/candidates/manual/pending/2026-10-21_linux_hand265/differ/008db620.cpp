// CModeDragonFireBeam::StartBeamEffect() -- once: find the dragon's mouth dummy, look up the BEAM_ATTACK effect in the owner's special-effects def, create it at the mouth into PBeamWeaponEffect and attach its particle emitter to the mouth dummy; mark the beam active
// Grade: DIFFER(490v481) -- instruction stream matches; frame slots differ: retail packs effectType, the CIntelligentPointer temporary, GetInterface's key and the second DRAGON_MOUTH_DUMMY name into one slot (R-0x2c), ours gives effectType its own slot (R-0x28) and shifts the rest
// Retail 0x008DB620, 481 bytes, plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CTCGraphicAppearance::GetDummyObject
//   0x004C08C0, CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CSpecialEffectsDef::GetEffectFromString
//   0x00669D30 (label "CCRC::CalcAndLookup" is wrong), __fastcall effect-thing Create 0x006E0880, CBaseIntelligentPointer
//   ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (CIntelligentPointer vtable
//   0x01238C6C), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020, CTCDParticleEmitter::AttachToThing
//   0x006E0BE0.
// Strings: "DRAGON_MOUTH_DUMMY" 0x01285B4C, "BEAM_ATTACK" 0x0127203C.
// Layout: CModeDragonFireBeam (PDB): owner creature +0x04, PBeamWeaponEffect +0x84, BeamActive +0x90; CThing graphic
//   appearance +0x64, def +0x70 (counted: count +4, slot 1 Destroy), interface bitset +0x20, map +0x44, flags +0x91.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up, Forward; };
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
        CRightHandedSet* axes, float scale);          // 0x004C08C0
};
struct CTCBase;
struct CInterfaceMap {                                // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
struct CDefBase {
    virtual ~CDefBase();
    virtual void Destroy();                           // slot 1
    long m_refCount;                                  // +0x04
};
struct CSpecialEffectsDef : CDefBase {
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    T* operator->() const { return m_p; }
};
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580
};
struct CDefClassReference {
    CDefClassBase* m_p;
    CDefClassReference(CDefClassBase* p) : m_p(p) { if (m_p) ++m_p->m_refCount; }
    ~CDefClassReference() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    CDefClassBase* operator->() const { return m_p; }
};
class CThing;
class CTCDParticleEmitter {
public:
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                  // +0x20
    CInterfaceMap  m_interfaces;                      // +0x44
    unsigned char  _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* PGraphicAppearance;         // +0x64
    unsigned char  _pad68[0x70 - 0x68];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                         // +0x91 bit 0: dead
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
struct CBaseIntelligentPointer {
    CBaseIntelligentPointer();                        // 0x00A01B10
    virtual ~CBaseIntelligentPointer();               // 0x00A01C10
    void* m_pItem;
    void SetPItem(void* item);                        // 0x00A01B90
    void* GetPItem() const;                           // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct CModeDragonFireBeam {
    void*                       __vftable;
    CThing*                     Owner;                // +0x04
    unsigned char               _pad08[0x84 - 0x08];
    CIntelligentPointer<CThing> PBeamWeaponEffect;    // +0x84
    long                        LastExplodeFrame;     // +0x8C
    bool                        BeamActive;           // +0x90
    void StartBeamEffect();
};

void CModeDragonFireBeam::StartBeamEffect()
{
    if (BeamActive)
        return;
    CDefPointer<CSpecialEffectsDef> effects;
    long effectType = 0;
    C3DVector position;
    CRightHandedSet axes;
    {
        CCharString name("DRAGON_MOUTH_DUMMY");
        Owner->PGraphicAppearance->GetDummyObject(name, 0, 0, &position, &axes, 1.0f);
    }
    if (CDefClassReference(Owner->m_pDef)->GetSubDef(effects) && effects.m_p->GetEffectFromString("BEAM_ATTACK", effectType)) {
        PBeamWeaponEffect = CIntelligentPointer<CThing>(CreateEffectThing(effectType, &position, false));
        CTCDParticleEmitter* emitter;
        if (PBeamWeaponEffect.GetPItem() && !(PBeamWeaponEffect.GetPItem()->m_flags91 & 1)
            && PBeamWeaponEffect.GetPItem()->GetInterface(&emitter, 0x4b)) {
            CCharString name("DRAGON_MOUTH_DUMMY");
            emitter->AttachToThing(Owner, 0x2a, name, 0, 0.0f);
        }
        BeamActive = true;
    }
}
