// Behaviour test for 007dfe20: src/007dfe20.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCTeleporter::~CTCTeleporter()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCTeleporter::~CTCTeleporter() -- destructor: kill a live particle effect and clear the pointer through an empty temporary, then the members (PParticle, PDef) and CTCBase
// Leaf: ??1CTCTeleporter
// Retail: Fable.exe @ 0x007DFE20, 141 bytes (vtable 0x0126FDA4 = CTCTeleporter, see the landed ctor 0x007DFEE0).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3), CThing::Kill(bool) 0x004C9B80, CBaseIntelligentPointer::
//          CBaseIntelligentPointer 0x00A01B10 (temporary; then the CIntelligentPointer<CThing> vtable 0x01238C6C),
//          ::SetPItem 0x00A01B90 (x2), ::~CBaseIntelligentPointer 0x00A01C10 (temporary, then the PParticle member);
//          the def's slot 1 on release; CTCBase::~CTCBase 0x00686830.
// Layout: Ego_r.pdb CTCTeleporter (packed, as the landed ctor): PDef +0x0c, Active +0x10, Activated +0x11,
//         HeroInsideRadius +0x12, PParticle +0x13 (unaligned 8-byte intelligent pointer), WaitingForTutorialToFinish +0x1b.
// Shape: `PParticle = CIntelligentPointer<CThing>()` is the clearing temporary in its own block (SetPItem(0) on the
//        temporary, then PParticle.SetPItem(temp.GetPItem())); PDef's dtor releases and nulls the pointer.
//        The temporary's GetPItem() result goes through a named local before PParticle.SetPItem. That gives retail's
//        registers (`this` in edi, &PParticle in esi). Passing it straight through, or the operator= form, swaps them
//        with otherwise identical instructions.
class CThing {
public:
    void Kill(bool immediately);                        // 0x004C9B80
};

struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_count;                                       // +0x04
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
class CTeleporterDef : public CDefPointee { public:};
struct CDefPointer {                                    // CDefPointer<const CTeleporterDef>
    CTeleporterDef* Object;
    ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                        // 8 bytes: vptr + item
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem();                                 // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {  // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() {}
};

class CTCBase { public:
    virtual ~CTCBase();                                 // 0x00686830
    CThing* ParentThing;                                // +0x04
    unsigned char _pad_0x08[0x4];
};

class CTCTeleporter : public CTCBase { public:
    CDefPointer PDef;                                   // +0x0c
    bool Active;                                        // +0x10
    bool Activated;                                     // +0x11
    bool HeroInsideRadius;                              // +0x12
    CIntelligentPointer PParticle;                      // +0x13
    bool WaitingForTutorialToFinish;                    // +0x1b
    virtual ~CTCTeleporter();
};
#pragma pack(pop)

CTCTeleporter::~CTCTeleporter()
{
    if (PParticle.GetPItem()) {
        PParticle.GetPItem()->Kill(true);
        {
            CIntelligentPointer empty;
            empty.SetPItem(0);
            CThing* item = empty.GetPItem();
            PParticle.SetPItem(item);
        }
    }
}

// ---- stubs and driver ----
static int g_kills = 0, g_destroys = 0, g_ctors = 0, g_dtors = 0, g_baseDtors = 0, g_sets = 0;
static bool g_killArg = false;
static CThing* g_killed = 0;
void CThing::Kill(bool immediately) { ++g_kills; g_killArg = immediately; g_killed = this; }
void CDefPointee::v00() { }
void CDefPointee::Destroy() { ++g_destroys; }
void CBaseIntelligentPointer::v00() { }
void CIntelligentPointer::v00() { }
CBaseIntelligentPointer::CBaseIntelligentPointer() { ++g_ctors; m_pItem = (CThing*)0x1; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtors; }
CThing* CBaseIntelligentPointer::GetPItem() { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { ++g_sets; m_pItem = item; }
CTCBase::~CTCBase() { ++g_baseDtors; }
int main()
{
    CThing particle;
    CTeleporterDef def; def.m_count = 1;
    static unsigned char raw[sizeof(CTCTeleporter)];
    CTCTeleporter* t = (CTCTeleporter*)raw;
    t->PDef.Object = &def;
    t->PParticle.m_pItem = &particle;
    t->CTCTeleporter::~CTCTeleporter();
    CHECK(g_kills == 1 && g_killed == &particle && g_killArg);
    CHECK(g_ctors == 1 && g_sets == 2 && t->PParticle.m_pItem == 0);
    CHECK(g_dtors == 2 && g_destroys == 1 && def.m_count == 0 && t->PDef.Object == 0 && g_baseDtors == 1);
    // no particle: no kill, no temporary; the member pointer and def still destroyed
    CTeleporterDef def2; def2.m_count = 2;
    t->PDef.Object = &def2; t->PParticle.m_pItem = 0;
    t->CTCTeleporter::~CTCTeleporter();
    CHECK(g_kills == 1 && g_ctors == 1 && g_sets == 2 && g_dtors == 3 && def2.m_count == 1 && g_destroys == 1 && t->PDef.Object == 0 && g_baseDtors == 2);
    if (fails == 0) printf("007dfe20_TEST PASS\n");
    return fails;
}
