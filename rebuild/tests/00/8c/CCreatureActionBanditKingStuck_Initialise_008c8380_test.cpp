#include <stdio.h>
#include <string.h>
// CCreatureAction_BanditKingStuck::Initialise() -- animation BK_STUCK; unless a live stuck effect exists, create the creature's STUCKLOOP special effect at its position, keep it in PStuckEffect and attach it to the creature through its particle emitter; then the base Initialise
// Leaf: ?Initialise@CCreatureAction_BanditKingStuck
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C8380, 455 bytes (464 with padding), virtual Initialise.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetAnimationName
//   0x008574D0, CBaseIntelligentPointer::GetPItem 0x00A01B50 / CBaseIntelligentPointer() 0x00A01B10 / SetPItem 0x00A01B90 /
//   ~CBaseIntelligentPointer 0x00A01C10, CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CCRC::CalcAndLookup
//   0x00669D30, CThing::GetPosition 0x004C73D0, the special-effect Create(long, const C3DVector&, bool) 0x006E0880
//   (__fastcall), CVectorMap::LowerBound 0x0040F020 (inline GetInterface 0x4B = CTCDParticleEmitter),
//   CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float) 0x006E0BE0,
//   CCombatActionBase::Initialise 0x00857520.
// Data: "BK_STUCK" 0x01281AA0, "STUCKLOOP" 0x01281A94, "" 0x0122D70E; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Layout (retail, local model): CCombatActionBase is 0x114 bytes (PCreature +0x08); PStuckEffect +0x118 (PDB +0x130,
//   the engine header carries PDB offsets); CThing def +0x70, dead flag +0x91 bit 0.
// Shape: the sub-def lookup and the name lookup are one `&&` (the name is a conditional temporary with its flag word);
//   the created thing goes through a CIntelligentPointer<CThing> temporary assigned with GetPItem/SetPItem, built
//   straight from the CreateSpecialEffect call (a named `created` local puts the temporary over the effect word);
//   CalcAndLookup / GetSubDef have same-TU out-only stand-ins, LowerBound a same-TU body, GetInterface the ternary pick.
struct C3DVector { float X, Y, Z; };
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

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;             // 0x004C73D0
    CDefClassBase* GetDef() const { return m_pDef; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
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

struct CCombatActionBase {                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
    void Initialise();                                // 0x00857520
};
struct CCreatureAction_BanditKingStuck : CCombatActionBase {
    long                     TimesToLoop;             // +0x114
    CIntelligentPointerThing PStuckEffect;            // +0x118
    void Initialise();
};

void CCreatureAction_BanditKingStuck::Initialise()
{
    {
        CCharString name("BK_STUCK");
        SetAnimationName(name);
    }
    if (PStuckEffect.GetPItem() == 0 || (PStuckEffect.GetPItem()->m_flags91 & 1)) {
        CDefPointer<CSpecialEffectsDef> effects;
        bool found;
        {
            long effect = 0;
            found = PCreature.GetPItem()->GetDef()->GetSubDef(&effects) && effects.Object->CalcAndLookup("STUCKLOOP", &effect);
            if (found) {
                PStuckEffect = CIntelligentPointerThing(CreateSpecialEffect(effect, PCreature.GetPItem()->GetPosition(), false));
            }
        }
        if (found) {
            if (PStuckEffect.GetPItem() && !(PStuckEffect.GetPItem()->m_flags91 & 1)) {
                CTCDParticleEmitter* emitter = 0;
                PStuckEffect.GetPItem()->GetInterface(&emitter, 0x4b);
                emitter->AttachToThing(PCreature.GetPItem(), 0x2a, "", 0, 0);
            }
        }
    }
    CCombatActionBase::Initialise();
}

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
static const char* g_lastText; static int g_strings, g_strDtors;
CCharString::CCharString(const char* text, long) { g_lastText = text; PStringData = (void*)text; g_strings++; }
CCharString::~CCharString() { g_strDtors++; }
static int g_destroys;
void CSpecialEffectsDef::v00() {}
void CSpecialEffectsDef::Destroy() { g_destroys++; }
static long g_lookup; static const char* g_lookupName; static const void* g_lookupDef;
long Lookup_CSpecialEffectsDef_CalcAndLookup(const void* def, const CCharString& name) { g_lookupDef = def; g_lookupName = (const char*)name.PStringData; return g_lookup; }
static void* g_subDef;
void* FindDef_CDefClassBase_GetSubDef_0() { return g_subDef; }
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
const C3DVector& CThing::GetPosition() const { return g_pos; }
static CThing* g_created; static long g_createdEffect; static C3DVector g_createdAt; static int g_createdAttached, g_creates;
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached)
{ g_createdEffect = effect; g_createdAt = position; g_createdAttached = attached; g_creates++; return g_created; }
void CBaseIntelligentPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() { m_pItem = 0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
void CIntelligentPointerThing::v00() {}
static void* g_attachThis; static CThing* g_attachThing; static long g_attachBone, g_attachA, g_attachB; static const char* g_attachName; static int g_attaches;
void CTCDParticleEmitter::AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long a, long b)
{ g_attachThis = this; g_attachThing = thing; g_attachBone = boneIndex; g_attachName = (const char*)boneName.PStringData; g_attachA = a; g_attachB = b; g_attaches++; }
static const char* g_anim; static int g_baseInits;
void CCombatActionBase::v00() {}
void CCombatActionBase::SetAnimationName(const CCharString& name) { g_anim = (const char*)name.PStringData; }
void CCombatActionBase::Initialise() { g_baseInits++; }

static unsigned char g_creatureBuf[0x100], g_effectBuf[0x100], g_oldBuf[0x100];
static CSpecialEffectsDef g_fx; static CDefClassBase g_classDef; static CTCDParticleEmitter g_particles;
static void Reset() { g_strings = g_strDtors = g_destroys = g_creates = g_attaches = g_baseInits = 0; g_anim = 0; g_lookupName = 0; g_attachThis = 0; }
int main()
{
    CThing* creature = (CThing*)g_creatureBuf; CThing* effect = (CThing*)g_effectBuf; CThing* old = (CThing*)g_oldBuf;
    creature->m_pDef = &g_classDef;
    CInterfaceMap::SEntry e[] = { { 0x10, 0 }, { 0x4b, (CTCBase*)&g_particles }, { 0x60, 0 } };
    effect->m_interfaces.first = e; effect->m_interfaces.last = e + 3;
    effect->m_presentBits[0x4b >> 5] = 1UL << (0x4b & 31);
    static unsigned char actionBuf[0x200]; CCreatureAction_BanditKingStuck* a = (CCreatureAction_BanditKingStuck*)actionBuf;
    a->PCreature.m_pItem = creature;
    // a live stuck effect already exists: only the animation and the base Initialise
    old->m_flags91 = 0; a->PStuckEffect.m_pItem = old; Reset();
    a->Initialise();
    CHECK(strcmp(g_anim, "BK_STUCK") == 0 && g_baseInits == 1 && g_creates == 0 && g_lookupName == 0 && a->PStuckEffect.m_pItem == old);
    CHECK(g_strings == 1 && g_strDtors == 1);
    // the old effect is dead, but there is no special-effects sub-def
    old->m_flags91 = 1; g_subDef = 0; Reset();
    a->Initialise();
    CHECK(g_baseInits == 1 && g_creates == 0 && g_lookupName == 0 && g_destroys == 0 && a->PStuckEffect.m_pItem == old);
    // no stuck effect; sub-def found but STUCKLOOP unknown: def released, nothing created
    a->PStuckEffect.m_pItem = 0; g_subDef = &g_fx; g_fx.RefCount = 1; g_lookup = -1; Reset();
    a->Initialise();
    CHECK(strcmp(g_lookupName, "STUCKLOOP") == 0 && g_lookupDef == &g_fx && g_creates == 0 && g_destroys == 1 && g_fx.RefCount == 0 && g_baseInits == 1);
    CHECK(g_strings == 2 && g_strDtors == 2);
    // found, effect creation fails: pointer set to null, nothing attached, def kept (count 2 -> 1)
    g_fx.RefCount = 2; g_lookup = 33; g_created = 0; Reset();
    a->Initialise();
    CHECK(g_creates == 1 && g_createdEffect == 33 && g_createdAt.X == 1.0f && g_createdAt.Y == 2.0f && g_createdAt.Z == 3.0f && g_createdAttached == 0);
    CHECK(a->PStuckEffect.m_pItem == 0 && g_attaches == 0 && g_destroys == 0 && g_fx.RefCount == 1 && g_baseInits == 1);
    // created effect is dead: kept, not attached
    g_fx.RefCount = 2; g_created = effect; effect->m_flags91 = 1; Reset();
    a->Initialise();
    CHECK(a->PStuckEffect.m_pItem == effect && g_attaches == 0 && g_baseInits == 1);
    // created live effect with its particle emitter: attached to the creature at bone 0x2a with "" name
    effect->m_flags91 = 0; a->PStuckEffect.m_pItem = 0; g_fx.RefCount = 2; Reset();
    a->Initialise();
    CHECK(a->PStuckEffect.m_pItem == effect && g_attaches == 1 && g_attachThis == &g_particles && g_attachThing == creature);
    CHECK(g_attachBone == 0x2a && strcmp(g_attachName, "") == 0 && g_attachA == 0 && g_attachB == 0 && g_baseInits == 1);
    CHECK(g_strings == 3 && g_strDtors == 3);
    if (fails == 0) printf("008c8380_TEST PASS\n"); else printf("008c8380_TEST FAIL\n");
    return fails;
}