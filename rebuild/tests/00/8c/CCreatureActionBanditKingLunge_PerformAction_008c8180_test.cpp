#include <stdio.h>
#include <string.h>
// CCreatureAction_BanditKingLunge::PerformAction() -- spawn THING_EXPLOSION_TWINBLADE_POUND 1.5 units ahead of the creature (owner = creature), then create its STUCK special effect at the creature and attach it through the particle emitter
// Leaf: ?PerformAction@CCreatureAction_BanditKingLunge
// Shape: CalcAndLookup (0x00669D30) gets a same-TU out-value-only stand-in body (the 0x008D9740 idiom): the effect index
//   stops escaping, so the GetInterface(0x4B) key shares the dead effect word (+0x1c) as retail.
// Layout: CThing bitset +0x20, map +0x44, PTCDriver +0x5c, physics +0x60, def +0x70, dead flag +0x91 bit 0 (as 0x8C8380/0x8F5060).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
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

struct CPhysicsBody {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing() const;              // slot 70 (+0x118)
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x5c - 0x4c];
    CTCBase* PTCDriver;                               // +0x5c
    CPhysicsBody* PhysicsBody;                        // +0x60
    unsigned char _pad64[0x70 - 0x64];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;             // 0x004C73D0
    CDefClassBase* GetDef() const { return m_pDef; }
    CPhysicsBody* GetPhysics() const { return PhysicsBody; }
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
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
};
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached);   // 0x006E0880

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CBaseIntelligentPointer();                        // 0x00A01B10
    ~CBaseIntelligentPointer();                       // 0x00A01C10
    CThing* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CThing* item);                      // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};

struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long a, long b);   // 0x006E0BE0
};

struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CTCDExplosion {
    static CThing* __fastcall Create(long defIndex, const C3DVector& position);   // 0x0080C460
};
struct CTCDriver41 {
    void SetOwner(CThing* owner);                     // 0x0080CBF0
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    CThing* GetCreature() const { return PCreature.GetPItem(); }
    CDefinitionManager* GetPDefinitionManager() const;   // 0x00692910
};
struct CCreatureAction_BanditKingLunge : CCreatureActionBase {
    void PerformAction();
};

void CCreatureAction_BanditKingLunge::PerformAction()
{
    long index = GetPDefinitionManager()->GetDefGlobalIndexFromName("THING_EXPLOSION_TWINBLADE_POUND");
    if (index > 0) {
        C3DVector offset = GetCreature()->GetPhysics()->GetFacing() * 1.5f;
        C3DVector position = GetCreature()->GetPosition() + offset;
        CThing* explosion = CTCDExplosion::Create(index, position);
        if (explosion) {
            CTCDriver41* driver;
            if (explosion->HasTC(0x41))
                driver = (CTCDriver41*)explosion->PTCDriver;
            driver->SetOwner(GetCreature());
        }
    }
    CDefPointer<CSpecialEffectsDef> effects;
    bool found;
    CThing* created;
    {
        long effect = 0;
        found = GetCreature()->GetDef()->GetSubDef(&effects) && effects.Object->CalcAndLookup("STUCK", &effect);
        if (found)
            created = CreateSpecialEffect(effect, GetCreature()->GetPosition(), false);
    }
    if (found) {
        if (created && !(created->m_flags91 & 1)) {
            CTCDParticleEmitter* emitter = 0;
            created->GetInterface(&emitter, 0x4b);
            emitter->AttachToThing(GetCreature(), 0x2a, "", 0, 0);
        }
    }
}

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
static char g_log[512];
static void L(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static const char* g_lastText; static int g_destroys;
CCharString::CCharString(const char* text, long) { g_lastText = text; PStringData = (void*)text; }
CCharString::~CCharString() {}
void CSpecialEffectsDef::v00() {}
void CSpecialEffectsDef::Destroy() { g_destroys++; }
static long g_lookup; static const char* g_lookupName;
long Lookup_CSpecialEffectsDef_CalcAndLookup(const void*, const CCharString& name) { g_lookupName = (const char*)name.PStringData; L("lookup"); return g_lookup; }
static void* g_subDef;
void* FindDef_CDefClassBase_GetSubDef_0() { L("subdef"); return g_subDef; }
#define VD(n) void CPhysicsBody::v##n() {}
#define VD10(p) VD(p##0) VD(p##1) VD(p##2) VD(p##3) VD(p##4) VD(p##5) VD(p##6) VD(p##7) VD(p##8) VD(p##9)
VD10(0) VD10(1) VD10(2) VD10(3) VD10(4) VD10(5) VD10(6)
C3DVector CPhysicsBody::GetFacing() const { return C3DVector(0.0f, 2.0f, 0.0f); }
static C3DVector g_pos(10.0f, 20.0f, 30.0f);
const C3DVector& CThing::GetPosition() const { return g_pos; }
static CThing* g_created; static long g_createdEffect; static C3DVector g_createdPos; static int g_createdAttached;
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached) { g_createdEffect = effect; g_createdPos = position; g_createdAttached = attached; L("effect"); return g_created; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
static void* g_attachThis; static CThing* g_attachThing; static long g_attachBone; static const char* g_attachName;
void CTCDParticleEmitter::AttachToThing(CThing* thing, long boneIndex, const CCharString& name, long, long) { g_attachThis = this; g_attachThing = thing; g_attachBone = boneIndex; g_attachName = (const char*)name.PStringData; L("attach"); }
static long g_defIndex; static const char* g_defName;
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) { g_defName = (const char*)name.PStringData; L("defindex"); return g_defIndex; }
static CDefinitionManager g_defMgr;
CDefinitionManager* CCreatureActionBase::GetPDefinitionManager() const { return &g_defMgr; }
static CThing* g_explosion; static long g_explIndex; static C3DVector g_explPos;
CThing* __fastcall CTCDExplosion::Create(long defIndex, const C3DVector& position) { g_explIndex = defIndex; g_explPos = position; L("explode"); return g_explosion; }
static void* g_driverThis; static CThing* g_owner;
void CTCDriver41::SetOwner(CThing* owner) { g_driverThis = this; g_owner = owner; L("owner"); }
static unsigned char g_actBuf[0x40], g_creatureBuf[0x100], g_explBuf[0x100], g_effBuf[0x100];
static void Reset() { g_log[0] = 0; g_destroys = 0; g_attachThis = 0; g_owner = 0; }
int main()
{
    CCreatureAction_BanditKingLunge* act = (CCreatureAction_BanditKingLunge*)g_actBuf;
    CThing* creature = (CThing*)g_creatureBuf; CThing* expl = (CThing*)g_explBuf; CThing* eff = (CThing*)g_effBuf;
    act->PCreature.m_pItem = creature;
    static CPhysicsBody body; creature->PhysicsBody = &body;
    static CDefClassBase cdef; creature->m_pDef = &cdef;
    static CTCDriver41 driver; expl->PTCDriver = (CTCBase*)&driver; expl->m_presentBits[0x41 >> 5] = 1UL << (0x41 & 31);
    static CTCDParticleEmitter emitter;
    CInterfaceMap::SEntry e[] = { { 0x10, 0 }, { 0x4b, (CTCBase*)&emitter } };
    eff->m_interfaces.first = e; eff->m_interfaces.last = e + 2; eff->m_presentBits[0x4b >> 5] = 1UL << (0x4b & 31);
    static CSpecialEffectsDef sdef;
    // full path
    g_defIndex = 5; g_explosion = expl; g_subDef = &sdef; sdef.RefCount = 2; g_lookup = 8; g_created = eff; Reset();
    act->PerformAction();
    CHECK(strcmp(g_log, "defindex;explode;owner;subdef;lookup;effect;attach;") == 0);
    CHECK(strcmp(g_defName, "THING_EXPLOSION_TWINBLADE_POUND") == 0 && g_explIndex == 5 && g_explPos.X == 10.0f && g_explPos.Y == 23.0f && g_explPos.Z == 30.0f);
    CHECK(g_driverThis == &driver && g_owner == creature && strcmp(g_lookupName, "STUCK") == 0);
    CHECK(g_createdEffect == 8 && g_createdPos.X == 10.0f && g_createdPos.Y == 20.0f && g_createdAttached == 0);
    CHECK(g_attachThis == &emitter && g_attachThing == creature && g_attachBone == 0x2a && strcmp(g_attachName, "") == 0);
    CHECK(sdef.RefCount == 1 && g_destroys == 0);
    // index 0, no explosion; dead effect: no attach
    g_defIndex = 0; g_effBuf[0x91] = 1; Reset();
    act->PerformAction();
    CHECK(strcmp(g_log, "defindex;subdef;lookup;effect;") == 0);
    // explosion not created; effect not created
    g_defIndex = 3; g_explosion = 0; g_created = 0; g_effBuf[0x91] = 0; Reset();
    act->PerformAction();
    CHECK(strcmp(g_log, "defindex;explode;subdef;lookup;effect;") == 0);
    // lookup fails: no effect; last reference destroyed
    g_defIndex = 0; g_lookup = -1; sdef.RefCount = 1; Reset();
    act->PerformAction();
    CHECK(strcmp(g_log, "defindex;subdef;lookup;") == 0 && g_destroys == 1);
    // no sub-def
    g_subDef = 0; Reset();
    act->PerformAction();
    CHECK(strcmp(g_log, "defindex;subdef;") == 0 && g_destroys == 0);
    if (fails == 0) printf("008c8180_TEST PASS\n"); else printf("008c8180_TEST FAIL\n");
    return fails;
}