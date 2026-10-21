#include <cstdio>
#include <cstring>
// Behaviour test for CTCMeshAutomaticEntityCreator::CreateParticle (0x0072E130): same body as src/, with the dummy
// count, the emitter creation / attach, AddTC and the intelligent-pointer primitives stubbed and recorded.
class CThing;
class CBaseObject;
class CTCParamsBase;

class CCharString {
public:
    CCharString(const char* text, long length = -1) : m_text(text) { (void)length; }
    const char* m_text;
};
struct C3DVector { float X, Y, Z; };

static int g_attachCalls = 0;
static CThing* g_attachThing = 0;
static long g_attachType = -1, g_lastHelperIndex = -1;
static const CCharString* g_attachHelper = 0;
class CTCDParticleEmitter {
public:
    int attached;
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset)
    {
        ++attached; ++g_attachCalls; g_attachThing = thing; g_attachType = attachType; g_attachHelper = &helperName;
        g_lastHelperIndex = helperIndex; (void)offset;
    }
    static CThing* Create(const CCharString& name, const C3DVector& position, bool flag);
};

static CThing* g_owner = 0;
class CTCBase {
public:
    CThing* owner;
    void SetPOwner(CThing* o) { owner = o; }
};

struct CCreatedEntityStub;
class CThing {
public:
    unsigned long        m_presentBits[9];
    CTCDParticleEmitter* PTCDriver;
    unsigned char        m_flags91;
    C3DVector            pos;
    CTCBase*             createdTC;
    CTCBase*             ownedTC;
    const C3DVector& GetPosition() const { return pos; }
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params)
    {
        (void)activate; (void)params;
        if (!std::strcmp(name.m_text, "CTCCreatedEntity")) return createdTC;
        if (!std::strcmp(name.m_text, "CTCOwnedEntity")) return ownedTC;
        return 0;
    }
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};

class CTCCreatedEntity : public CTCBase {
public:
    CIntelligentPointer<const CThing> PCreator;
};

static CThing* g_created[8];
static int g_createIndex = 0, g_createCalls = 0;
static const C3DVector* g_createPos = 0;
static const CCharString* g_createName = 0;
CThing* CTCDParticleEmitter::Create(const CCharString& name, const C3DVector& position, bool flag)
{
    ++g_createCalls; g_createPos = &position; g_createName = &name;
    if (flag) return 0;
    return g_created[g_createIndex++];
}

static int g_countCalls = 0;
class CTCGraphicAppearance {
public:
    long count;
    long GetNoDummyObjects(const CCharString& name, float scale) { ++g_countCalls; (void)name; return scale == 1.0f ? count : 0; }
};

class CTCMeshAutomaticEntityCreator {
public:
    void*   m_vtable;
    CThing* m_pThing;
    void AddOwnedEntityTC(CThing* created)
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

int main()
{
    int fails = 0;
    CThing owner; std::memset(&owner, 0, sizeof owner);
    CTCDParticleEmitter em[3]; std::memset(em, 0, sizeof em);
    CThing p[3]; std::memset(p, 0, sizeof p);
    CTCCreatedEntity ce[3]; CTCBase oe[3];
    for (int k = 0; k < 3; ++k) {
        p[k].m_presentBits[0x4b >> 5] = 1UL << (0x4b & 31);
        p[k].PTCDriver = &em[k];
        p[k].createdTC = &ce[k]; p[k].ownedTC = &oe[k];
        ce[k].owner = 0; oe[k].owner = 0;
    }
    p[1].m_flags91 = 1;                                 // created dead: skipped
    g_created[0] = &p[0]; g_created[1] = &p[1]; g_created[2] = &p[2];
    CTCMeshAutomaticEntityCreator creator; creator.m_pThing = &owner;
    CTCGraphicAppearance app; app.count = 3;
    CCharString dummy("HDL_FX"), particle("FX_SMOKE");
    creator.CreateParticle(dummy, particle, &app);
    if (g_createCalls != 3 || g_countCalls != 4 || g_createPos != &owner.pos || g_createName != &particle) ++fails;
    if (em[0].attached != 1 || em[1].attached != 0 || em[2].attached != 1) ++fails;
    if (g_attachThing != &owner || g_attachType != 10 || g_attachHelper != &dummy || g_lastHelperIndex != 2) ++fails;
    if (p[0].m_flags91 != 2 || p[1].m_flags91 != 1 || p[2].m_flags91 != 2) ++fails;
    if (ce[0].PCreator.GetPItem() != &owner || ce[1].PCreator.GetPItem() != 0 || ce[2].PCreator.GetPItem() != &owner) ++fails;
    if (oe[0].owner != &owner || oe[1].owner != 0 || oe[2].owner != &owner) ++fails;
    // No dummies: nothing created.
    app.count = 0;
    creator.CreateParticle(dummy, particle, &app);
    if (g_createCalls != 3 || g_countCalls != 5) ++fails;
    if (fails) { std::printf("0072e130_TEST FAIL %d\n", fails); return 1; }
    std::printf("0072e130_TEST PASS\n");
    return 0;
}
