// CTCMeshAutomaticEntityCreator::CreateParticle(const CCharString& dummyName, const CCharString& particleName, CTCGraphicAppearance* appearance) -- for every dummy object of that name on the appearance, create the particle effect at the owner, attach its emitter to the owner's dummy, mark it created-by-owner, give it a CTCCreatedEntity whose creator is the owner, and an owned-entity component
// Leaf: CreateParticle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072E130, 352 bytes (+ int3 padding), `ret 0Ch`. Speed TU (esp frame,
//   `lea ecx,[ecx]` loop alignment); the 8-byte intelligent-pointer temporary built inside the loop gives the
//   `and esp,-8` aligned frame (GOTCHAS chunk 232).
// Callees: CTCGraphicAppearance::GetNoDummyObjects(const CCharString&, float) @ 0x004BE000 (re-evaluated per pass),
//   CThing::GetPosition @ 0x004C73D0 (label "CVertexBufferWin32::DoSizeof" is propagated), CTCDParticleEmitter::Create
//   (`__fastcall`) @ 0x006E0970, CTCDParticleEmitter::AttachToThing @ 0x006E0BE0, CCharString(const char*, long = -1)
//   @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CThing::AddTC @ 0x004C9D60, CBaseIntelligentPointer ctor @ 0x00A01B10 /
//   GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10, CTCOwnedEntity::SetPOwner @ 0x007E8590 (through
//   the inlined same-TU AddOwnedEntityTC, landed 0x0072DD60).
// Data: "CTCCreatedEntity" @ 0x0123AA60, "CTCOwnedEntity" @ 0x0123AA74; CIntelligentPointer<const CThing> vtable
//   @ 0x01237F1C.
// Layout: as the landed CTCChest::Open (0x00750540): retail CThing bitset +0x20 (interface 0x4B = bit 11 of +0x28), the
//   cached driver +0x5C, flags +0x91 (bit 0 dead, bit 1 created by owner thing creation); CTCCreatedEntity PCreator +0x0C
//   (Ego_r.pdb); the creator's owning thing +0x04.
class CThing;
class CBaseObject;
class CTCParamsBase;

class CCharString {
public:
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    void* m_pStringData;
};
class C3DVector { public: float X, Y, Z; };

class CTCDParticleEmitter {
public:
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
    static CThing* __fastcall Create(const CCharString& name, const C3DVector& position, bool flag);   // 0x006E0970
};

class CTCBase {
public:
    void SetPOwner(CThing* owner);                     // 0x007E8590 (CTCOwnedEntity::SetPOwner)
};

class CThing {
public:
    unsigned char        _pad00[0x20];
    unsigned long        m_presentBits[9];             // +0x20
    unsigned char        _pad44[0x5c - 0x44];
    CTCDParticleEmitter* PTCDriver;                    // +0x5c (the cached interface-0x4B component)
    unsigned char        _pad60[0x91 - 0x60];
    unsigned char        m_flags91;                    // +0x91 bit 0 dead, bit 1 created by owner thing creation
    const C3DVector& GetPosition() const;              // 0x004C73D0
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // CIntelligentPointer<const CThing> vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
#pragma pack(pop)

class CTCCreatedEntity : public CTCBase {
public:
    unsigned char _pad00[0x0c];
    CIntelligentPointer<const CThing> PCreator;        // +0x0C
};

class CTCGraphicAppearance {
public:
    long GetNoDummyObjects(const CCharString& name, float scale);   // 0x004BE000
};

class CTCMeshAutomaticEntityCreator {
public:
    void*   m_vtable;                                  // +0x00
    CThing* m_pThing;                                  // +0x04 the creator's owning thing
    void AddOwnedEntityTC(CThing* created)             // 0x0072DD60 (same TU, inlined here)
    {
        CTCBase* owned = created->AddTC("CTCOwnedEntity", false, 0);
        owned->SetPOwner(m_pThing);
    }
    void CreateParticle(const CCharString& dummyName, const CCharString& particleName, CTCGraphicAppearance* appearance);
};

void CTCMeshAutomaticEntityCreator::CreateParticle(const CCharString& dummyName, const CCharString& particleName,
                                                   CTCGraphicAppearance* appearance)
{
    for (long i = 0; i < appearance->GetNoDummyObjects(dummyName, 1.0f); ++i) {
        CThing* particle = CTCDParticleEmitter::Create(particleName, m_pThing->GetPosition(), false);
        if (particle && !(particle->m_flags91 & 1)) {
            CTCDParticleEmitter* emitter = 0;
            if (particle->HasTC(0x4b))
                emitter = particle->PTCDriver;
            emitter->AttachToThing(m_pThing, 10, dummyName, i, 0.0f);
            particle->m_flags91 |= 2;
            CTCCreatedEntity* created = (CTCCreatedEntity*)particle->AddTC("CTCCreatedEntity", false, 0);
            created->PCreator = CIntelligentPointer<const CThing>(m_pThing);
            AddOwnedEntityTC(particle);
        }
    }
}