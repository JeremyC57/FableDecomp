#include <stdio.h>
#include <string.h>
// NCreatureMode::CModeThunderLightningStorm::CreateHandGlow() -- spawn Thunder's LIGHTNING_HAND_EFFECT at his position, keep it in the mode's hand-glow pointer and attach its particle emitter to his WEAPON_FOCUS_01 bone
// Leaf: CreateHandGlow
// Shape: GetSubDef and GetEffectFromString have same-TU out-slot-only stand-in bodies, so the effect type does not escape:
//   VC7.1 overlays its word with the intelligent-pointer temporary, the GetInterface key and the bone-name string (as
//   retail); GetInterface out-store as one ternary.
// Callees: GetSubDef 0x00669580 (same-TU stand-in body), CCharString 0x0099EBF0/~ 0x0099EAE0, GetEffectFromString 0x00669D30, GetPosition 0x004C73D0, CreateEffectThing 0x006E0880, CBaseIntelligentPointer 0x00A01B10/B90/B50/C10, LowerBound 0x0040F020, CTCDParticleEmitter::AttachToThing 0x006E0BE0.
class CTCBase;
class CBaseObject;
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
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

class CDefPointeeBase { public:
    virtual void Destroy();
    virtual void Delete();
    long m_refCount;
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
class CSpecialEffectsDef : public CDefPointeeBase { public:
    bool GetEffectFromString(const CCharString& name, long* type);   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail): it writes the out type only, so VC7.1 knows the effect type's address does
// not escape the call.
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long* type)
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    *type = found;
    return true;
}
#pragma auto_inline(on)
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
void* FindDef_CDefClassBase_GetSubDef_0();
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
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x45];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
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
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long flags, float scale);   // 0x006E0BE0
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;
    CBaseIntelligentPointer();             // 0x00A01B10
    ~CBaseIntelligentPointer();            // 0x00A01C10
    CThing* GetPItem() const;              // 0x00A01B50
    void SetPItem(CThing* item);           // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CModeBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x14];
};
struct CModeStandBase : CModeBase {
    unsigned char _body[0x4c];
};
namespace NCreatureMode {
struct CModeThunderLightningStorm : CModeStandBase {
    unsigned char _pad68[0x34];
    CIntelligentPointerThing PHandGlow;         // +0x9c
    CDefClassBase* PDef;                        // +0xa4
    void CreateHandGlow();
};
void CModeThunderLightningStorm::CreateHandGlow()
{
    CDefPointer<CSpecialEffectsDef> out;
    long effectType = 0;
    PDef->GetSubDef(&out);
    CDefHolder<CSpecialEffectsDef> effects(out.Object);
    if (effects->GetEffectFromString("LIGHTNING_HAND_EFFECT", &effectType)) {
        C3DVector position = ParentThing->GetPosition();
        CThing* glow = CreateEffectThing(effectType, &position, false);
        if (glow && !glow->IsDead()) {
            PHandGlow = CIntelligentPointerThing(glow);
            CTCDParticleEmitter* emitter;
            if (glow->GetInterface(&emitter, 0x4b) && emitter)
                emitter->AttachToThing(ParentThing, 10, "WEAPON_FOCUS_01", 0, 0.0f);
        }
    }
}
}

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
static const char* g_lastText; static int g_strings, g_strDtors, g_deletes;
CCharString::CCharString(const char* text, long) { g_lastText = text; g_strings++; m_pData = 0; }
CCharString::~CCharString() { g_strDtors++; }
void CDefPointeeBase::Destroy() {}
void CDefPointeeBase::Delete() { g_deletes++; }
static long g_lookup; static const char* g_lookupName; static void* g_lookupDef;
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString&) { g_lookupDef = (void*)def; g_lookupName = g_lastText; return g_lookup; }
static void* g_def;
void* FindDef_CDefClassBase_GetSubDef_0() { return g_def; }
static C3DVector g_pos = { 4.0f, 5.0f, 6.0f };
const C3DVector& CThing::GetPosition() const { return g_pos; }
static CThing* g_created; static long g_createdType; static C3DVector g_createdPos; static int g_createdFlag, g_creates;
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag) { g_createdType = type; g_createdPos = *position; g_createdFlag = flag; g_creates++; return g_created; }
static void* g_attachThis; static CThing* g_attachThing; static long g_attachBone, g_attachFlags; static float g_attachScale; static const char* g_attachName; static int g_attaches;
void CTCDParticleEmitter::AttachToThing(CThing* thing, long boneIndex, const CCharString&, long flags, float scale)
{ g_attachThis = this; g_attachThing = thing; g_attachBone = boneIndex; g_attachFlags = flags; g_attachScale = scale; g_attachName = g_lastText; g_attaches++; }
void CBaseIntelligentPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() { PItem = 0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return PItem; }
static int g_sets;
void CBaseIntelligentPointer::SetPItem(CThing* item) { PItem = item; g_sets++; }
void CIntelligentPointerThing::v00() {}
void CModeBase::v00() {}
static void Reset() { g_strings = g_strDtors = g_deletes = g_creates = g_attaches = g_sets = 0; g_attachThis = 0; g_lookupName = 0; g_attachName = 0; }
static unsigned char g_parentBuf[0x100], g_glowBuf[0x100];
using namespace NCreatureMode;
int main()
{
    static CModeThunderLightningStorm mode;
    CThing* parent = (CThing*)g_parentBuf; CThing* glow = (CThing*)g_glowBuf;
    static CDefClassBase classDef; static CSpecialEffectsDef effectsDef; static CTCDParticleEmitter emitter;
    mode.PDef = &classDef; mode.ParentThing = parent; g_def = &effectsDef;
    CInterfaceMap::SEntry e[] = { { 0x10, 0 }, { 0x4b, (CTCBase*)&emitter }, { 0x60, 0 } };
    glow->m_interfaces.first = e; glow->m_interfaces.last = e + 3;
    // lookup fails: string built/destroyed, effects reference released
    effectsDef.m_refCount = 1; g_lookup = -1; Reset();
    mode.CreateHandGlow();
    CHECK(g_strings == 1 && g_strDtors == 1 && strcmp(g_lookupName, "LIGHTNING_HAND_EFFECT") == 0 && g_lookupDef == &effectsDef);
    CHECK(g_creates == 0 && g_deletes == 1 && effectsDef.m_refCount == 0);
    // found, nothing created
    effectsDef.m_refCount = 2; g_lookup = 9; g_created = 0; Reset();
    mode.CreateHandGlow();
    CHECK(g_creates == 1 && g_createdType == 9 && g_createdPos.X == 4.0f && g_createdPos.Y == 5.0f && g_createdPos.Z == 6.0f && g_createdFlag == 0);
    CHECK(g_sets == 0 && g_attaches == 0 && g_deletes == 0 && effectsDef.m_refCount == 1);
    // dead glow: nothing kept
    effectsDef.m_refCount = 2; g_created = glow; glow->m_flags91 = 1; Reset();
    mode.CreateHandGlow();
    CHECK(g_sets == 0 && g_attaches == 0 && mode.PHandGlow.PItem == 0);
    // live glow with the emitter: kept and attached to WEAPON_FOCUS_01
    glow->m_flags91 = 0; glow->m_presentBits[0x4b >> 5] = 1UL << (0x4b & 31); Reset();
    mode.CreateHandGlow();
    CHECK(mode.PHandGlow.PItem == glow && g_attaches == 1 && g_attachThis == &emitter && g_attachThing == parent);
    CHECK(g_attachBone == 10 && g_attachFlags == 0 && g_attachScale == 0.0f && strcmp(g_attachName, "WEAPON_FOCUS_01") == 0 && g_strings == 2 && g_strDtors == 2);
    // live glow without the interface: kept, not attached
    glow->m_presentBits[0x4b >> 5] = 0; mode.PHandGlow.PItem = 0; Reset();
    mode.CreateHandGlow();
    CHECK(mode.PHandGlow.PItem == glow && g_attaches == 0);
    // interface present but null component: not attached
    glow->m_presentBits[0x4b >> 5] = 1UL << (0x4b & 31); e[1].component = 0; Reset();
    mode.CreateHandGlow();
    CHECK(g_attaches == 0);
    if (fails == 0) printf("008dacb0_TEST PASS\n"); else printf("008dacb0_TEST FAIL\n");
    return fails;
}