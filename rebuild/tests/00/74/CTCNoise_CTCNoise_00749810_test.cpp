// Behaviour test for 00749810: src/00749810.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCNoise::CTCNoise(CThing*)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCNoise::CTCNoise(CThing*) -- CTCBase(owner), vtable, an inline-constructed Definition (CTCNoiseDef), zero timer and range, an empty Creator pointer
// Shape: the inline CDefClassBase ctor is the landed out-of-line 0x0042D9E0 form (PDefManager in the init list, the three
// status bits cleared in the body, byte-packed def base): inlined here VC7.1 clears the bits with retail's split
// read-modify-write (`mov cl,[edi+24h] ; xor eax,eax ; and cl,0F8h ; mov [edi+24h],cl`) and stores PDefManager from eax.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00749810, length 79 bytes (vtable 0x01268A74).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, the def root constructor @ 0x009FBEC0 (labelled "CResource::CResource"),
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10.
// Retail CTCNoise (PDB): Definition (CTCNoiseDef, 0x34 bytes, vtable 0x01231E9C) +0x0c, LifeTimer +0x40,
// DistanceCanBeHeardFrom +0x44, Creator (CIntelligentPointer<const CThing>, vtable 0x01237F1C) +0x48.
// Shape: CDefClassBase's constructor (out of line @ 0x0044C0C0 elsewhere) inlined here: its bit-field clear
// (`and cl,0F8h` on +0x24: Setup/Template/DefaultValsApplied) and PDefManager = 0 (+0x1c); its own vtable store is dead
// under CTCNoiseDef's. Definition's address stays in edi; the CTCNoise scalars share eax's zero.
struct CThing;

struct CResourceRoot {                              // the def root (0x1c bytes)
    virtual void v00();
    unsigned char _pad04[0x18];
    CResourceRoot();                                // 0x009FBEC0
};

#pragma pack(push, 1)
struct CDefClassBase : CResourceRoot {
    void*         PDefManager;                      // +0x1c
    unsigned long GlobalIndex;                      // +0x20
    unsigned char Setup : 1;                        // +0x24
    unsigned char Template : 1;
    unsigned char DefaultValsApplied : 1;
    CDefClassBase()
        : PDefManager(0)
    {
        Setup = 0;
        Template = 0;
        DefaultValsApplied = 0;
    }
    virtual void v00();
};
#pragma pack(pop)

struct CTCNoiseDef : CDefClassBase {                // 0x34 bytes
    long  Type;                                     // +0x28 ENoiseType
    float LifeTime;                                 // +0x2c
    float DistanceCanBeHeardFrom;                   // +0x30
    CTCNoiseDef() {}
    virtual void v00();
};

struct CBaseIntelligentPointer {
    virtual void v00();
    const CThing* m_pItem;                          // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
};

template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() {}
    virtual void v00();
};

struct CTCBase_00749810 {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];
    CTCBase_00749810(CThing* owner);                // 0x00686800
};

struct CTCNoise : CTCBase_00749810 {
    CTCNoiseDef                       Definition;   // +0x0c
    long                              LifeTimer;    // +0x40
    float                             DistanceCanBeHeardFrom;   // +0x44
    CIntelligentPointer<const CThing> Creator;      // +0x48
    CTCNoise(CThing* owner);
    virtual void v00();
};

CTCNoise::CTCNoise(CThing* owner)
    : CTCBase_00749810(owner), LifeTimer(0), DistanceCanBeHeardFrom(0.0f)
{
}

// ---- stubs and driver ----
#include <new>
static int g_resourceCtors = 0;
CResourceRoot::CResourceRoot() { ++g_resourceCtors; }
void CResourceRoot::v00() { }
void CDefClassBase::v00() { }
void CTCNoiseDef::v00() { }
static int g_ipCtors = 0;
CBaseIntelligentPointer::CBaseIntelligentPointer() { ++g_ipCtors; m_pItem = 0; }
void CBaseIntelligentPointer::v00() { }
template<> void CIntelligentPointer<const CThing>::v00() { }
static CThing* g_owner = 0;
CTCBase_00749810::CTCBase_00749810(CThing* owner) { g_owner = owner; ParentThing = owner; }
void CTCBase_00749810::v00() { }
void CTCNoise::v00() { }
int main()
{
    static unsigned char raw[sizeof(CTCNoise)];
    memset(raw, 0xff, sizeof(raw));
    CTCNoise* n = new (raw) CTCNoise((CThing*)0x1234);
    CHECK(g_owner == (CThing*)0x1234); CHECK(n->ParentThing == (CThing*)0x1234);
    CHECK(g_resourceCtors == 1); CHECK(g_ipCtors == 1);
    CHECK(n->Definition.PDefManager == 0);
    CHECK(n->Definition.Setup == 0); CHECK(n->Definition.Template == 0); CHECK(n->Definition.DefaultValsApplied == 0);
    CHECK((raw[0xc + 0x24] & 0xf8) == 0xf8);            // only the three status bits are cleared
    CHECK(n->LifeTimer == 0); CHECK(n->DistanceCanBeHeardFrom == 0.0f); CHECK(n->Creator.m_pItem == 0);
    if (fails == 0) printf("00749810_TEST PASS\n"); else printf("00749810_TEST FAIL\n");
    return fails;
}
