// Behaviour test for 008eca50: src/008eca50.cpp copied verbatim; Create, the particle emitter and CCharString stubbed so
// the def-index gate, the creation at the parent's position and the attachment (bone 8, empty bone name, scale 0) are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCDegradableBase::ShowParticleEffects(long) -- create the stage's smash particle thing at the parent's position and attach its emitter to the parent (bone 8)
// Retry 10b (accessor sweep, FIXES B-4/B-5): the parent thing is read through an inline accessor; the plain member read gives the parked register difference.
// Leaf: ShowParticleEffects
//
// Retail 0x008ECA50, 100 bytes (`ret 4`). Ego_r.pdb `?ShowParticleEffects@CTCDegradableBase@@AAEXJ@Z`.
// Callees: CThing::GetPosition() const @ 0x004C73D0, the global Create(long, const C3DVector&, bool)
// @ 0x006E0880 (`__fastcall`, the bool on the stack, pushed first), CCharString::CCharString(const
// char*, long) @ 0x0099EBF0 (the empty string 0x0122D70E), CTCDParticleEmitter::AttachToThing(CThing*,
// long, const CCharString&, long, float) @ 0x006E0BE0, ~CCharString @ 0x0099EAE0.
// Layout: retail CThing interface bitset +0x20 (particle emitter = interface 0x4b, word 2 bit 11) and
// the cached particle-emitter component pointer at +0x5c.
// Shape: `T* e = 0; if (Has(k)) e = cached;` gives the preloaded `xor edi,edi`; the empty bone name is
// a named CCharString in its own block, built in the dead argument slot (the function has no other
// named locals and no frame).
struct C3DVector;

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    unsigned char _pad44[0x5c - 0x44];
    struct CTCDParticleEmitter* PParticleEmitter;           // +0x5c
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

CThing* __fastcall Create(long defIndex, const C3DVector& position, bool flag);   // 0x006E0880

struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long flags, float scale);   // 0x006E0BE0
};

struct CTCDegradableBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    CThing* GetParentThing() const { return ParentThing; }
    void ShowParticleEffects(long particleDefIndex);
};

void CTCDegradableBase::ShowParticleEffects(long particleDefIndex)
{
    if (particleDefIndex > 0)
    {
        CThing* particle = Create(particleDefIndex, GetParentThing()->GetPosition(), false);
        if (particle)
        {
            CTCDParticleEmitter* emitter = 0;
            if (particle->HasInterface(0x4b))
                emitter = particle->PParticleEmitter;
            {
                CCharString noBone("", -1);
                emitter->AttachToThing(ParentThing, 8, noBone, 0, 0.0f);
            }
        }
    }
}
// ---- end source ----
static int g_posStore; static const C3DVector* g_createPos = 0; static long g_createDef = 0; static int g_creates = 0;
const C3DVector& CThing::GetPosition() const { return *(const C3DVector*)&g_posStore; }
static CThing g_particle; static bool g_createOk = true;
CThing* __fastcall Create(long defIndex, const C3DVector& position, bool flag)
{ ++g_creates; g_createDef = defIndex; g_createPos = &position; return (g_createOk && !flag) ? &g_particle : 0; }
static int g_strings = 0, g_stringDtors = 0;
CCharString::CCharString(const char* text, long) : PStringData((void*)text) { ++g_strings; }
CCharString::~CCharString() { ++g_stringDtors; }
static int g_attaches = 0; static CThing* g_attachThing = 0; static long g_bone = -1; static long g_flags = -1; static float g_scale = -1; static CTCDParticleEmitter* g_pEmitterThis = 0;
static const char* g_boneName = 0;
void CTCDParticleEmitter::AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long flags, float scale)
{ ++g_attaches; g_pEmitterThis = this; g_attachThing = thing; g_bone = boneIndex; g_boneName = (const char*)boneName.PStringData; g_flags = flags; g_scale = scale; }
void CTCDegradableBase::v00() {}
int main()
{
    static CThing parent; static CTCDParticleEmitter emitter;
    memset(&g_particle, 0, sizeof(g_particle));
    g_particle.m_presentBits[0x4b >> 5] |= 1UL << (0x4b & 31); g_particle.PParticleEmitter = &emitter;
    CTCDegradableBase d; d.ParentThing = &parent;
    d.ShowParticleEffects(0); CHECK(g_creates == 0);
    d.ShowParticleEffects(12);
    CHECK(g_creates == 1 && g_createDef == 12 && g_createPos == (const C3DVector*)&g_posStore);
    CHECK(g_attaches == 1 && g_pEmitterThis == &emitter && g_attachThing == &parent && g_bone == 8 && g_flags == 0 && g_scale == 0.0f);
    CHECK(g_boneName && g_boneName[0] == 0 && g_strings == 1 && g_stringDtors == 1);
    g_createOk = false;
    d.ShowParticleEffects(12); CHECK(g_creates == 2 && g_attaches == 1);
    if (fails == 0) printf("008eca50_TEST PASS\n"); else printf("008eca50_TEST FAIL\n");
    return fails;
}