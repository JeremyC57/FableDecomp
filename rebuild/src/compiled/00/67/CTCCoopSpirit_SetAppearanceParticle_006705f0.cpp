// CTCCoopSpirit::SetAppearanceParticle(const CCharString& particleThingName) -- kill the current appearance particle when it is still alive, create the named particle thing at the spirit's position as the new one and, when it lives, attach its emitter to the spirit (bone 0x88, scale 1.1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006705F0, 282 bytes (+ int3 padding), `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x7) / ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 (x2) / dtor @
//   0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C), CThing::Kill(bool) @ 0x004C9B80, CThing::GetPosition @
//   0x004C73D0, the global Create(const CCharString&, const C3DVector&, bool) @ 0x006E0970 (`__fastcall`),
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (interface 0x4b), CCharString(const char*, long) @
//   0x0099EBF0 ("" @ 0x0122D70E), CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float) @
//   0x006E0BE0, ~CCharString @ 0x0099EAE0.
// Layout: Ego_r.pdb CTCCoopSpirit: ParentThing +0x04, the appearance particle (CIntelligentPointer<CThing>) +0x2c.
// Shape: the landed AddAppearanceParticle 0x00670560 attach block (the `__forceinline` GetParticleEmitter helper with an
//   unset out pointer, the key and the empty bone name sharing the dead argument slot); both liveness tests are the inline
//   IS_THING_ALIVE (two GetPItem calls); the new pointer is assigned through a CIntelligentPointer<CThing> temporary (inline
//   converting ctor = base ctor + vtable + SetPItem, inline operator= = SetPItem(other.GetPItem()), out-of-line base dtor).
class CTCBase;
class C3DVector;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                                // +0x91 bit 0: dead
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
    const C3DVector& GetPosition() const;                   // 0x004C73D0
    void Kill(bool immediately);                            // 0x004C9B80
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
CThing* __fastcall Create(const CCharString& defName, const C3DVector& position, bool flag);   // 0x006E0970
class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long flags, float scale);   // 0x006E0BE0
};
__forceinline CTCDParticleEmitter* GetParticleEmitter(CThing* thing)
{
    CTCDParticleEmitter* emitter;
    thing->GetInterface(&emitter, 0x4b);                    // TC_PARTICLE_EMITTER
    return emitter;
}
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                          // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    CThing* GetPItem() const;                               // 0x00A01B50
    void SetPItem(void* item);                              // 0x00A01B90
};
struct CThingPointer : CBaseIntelligentPointer {            // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CThingPointer(CThing* item) { SetPItem(item); }
    CThingPointer& operator=(const CThingPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CTCCoopSpirit { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    unsigned char _pad08[0x2c - 0x08];
    CThingPointer PAppearanceParticle;                      // +0x2c
    void SetAppearanceParticle(const CCharString& particleThingName);
};

void CTCCoopSpirit::SetAppearanceParticle(const CCharString& particleThingName)
{
    if (IS_THING_ALIVE(PAppearanceParticle))
        PAppearanceParticle.GetPItem()->Kill(true);
    PAppearanceParticle = CThingPointer(Create(particleThingName, ParentThing->GetPosition(), false));
    if (IS_THING_ALIVE(PAppearanceParticle)) {
        CTCDParticleEmitter* emitter = GetParticleEmitter(PAppearanceParticle.GetPItem());
        {
            CCharString noBone("", -1);
            emitter->AttachToThing(ParentThing, 0x88, noBone, 0, 1.1f);
        }
    }
}