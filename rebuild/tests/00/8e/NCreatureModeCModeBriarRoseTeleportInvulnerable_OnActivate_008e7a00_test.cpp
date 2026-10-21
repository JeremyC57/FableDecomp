#include <stdio.h>
#include <string.h>
// NCreatureMode::CModeBriarRoseTeleportInvulnerable::OnActivate() -- Briar Rose charges a teleport: set the 1.25 s frame count, suspend a player creature's animation, advance the boss phase when ready, and spawn the TELEPORT_CHARGE effect at her position, keeping it in the mode's effect pointer
// Leaf: OnActivate
// Shape: GetEffectFromString (0x00669D30) gets a same-TU out-type-only stand-in body (the 0x008D9740 idiom), so the effect
//   type does not escape and the CIntelligentPointer temp overlays its word as retail; `out` declared before `effectType`
//   (zero-store order).
// Callees: CModeStandBase::OnActivate 0x008621A0, __ftol2 0x00BFEA70, SetSuspendAnimation 0x00833A10, LowerBound 0x0040F020, CTCBoss::IsReadyForPhaseChange 0x007AEE90 / IncrementBossPhase 0x007AED20, GetSubDef 0x00669580, CCharString 0x0099EBF0/~ 0x0099EAE0, GetEffectFromString 0x00669D30, GetPosition 0x004C73D0, CreateEffectThing 0x006E0880, CBaseIntelligentPointer 0x00A01B10/B90/B50/C10.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CTCBase;
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

struct CDefPointeeBase {
    virtual void Destroy();
    virtual void Delete();
    long m_refCount;
};
template <class T> struct CDefPointer {
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
template <class T> struct CDefCopy {
    T* Object;
    CDefCopy(T* object) : Object(object) { if (Object) ++Object->m_refCount; }
    ~CDefCopy()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
struct CSpecialEffectsDef : CDefPointeeBase {
    bool GetEffectFromString(const CCharString& name, long* type);   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail): it writes the out type only, so VC7.1 knows the effect type's address does
// not escape the call (the 0x008D9740 idiom).
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
struct CDefClassBase : CDefPointeeBase {
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
struct CModeBase;
struct CThing {
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                      // +0x10
    unsigned char  _pad11[0x0f];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x24];
    CDefClassBase* PDef;                        // +0x70
    unsigned char  _pad74[0x1d];
    unsigned char  m_flags91;                   // +0x91
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
    void SetSuspendAnimation(bool suspend, CModeBase* mode);   // 0x00833A10
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CTCBoss {
    bool IsReadyForPhaseChange();               // 0x007AEE90
    void IncrementBossPhase();                  // 0x007AED20
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
extern long g_GameFramesPerSecond;              // 0x01375550
struct CModeBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x14];
};
struct CModeStandBase : CModeBase {
    CThing* GetParent() const { return ParentThing; }
    unsigned char _body[0x4c];
    void OnActivate();                          // 0x008621A0
};
namespace NCreatureMode {
struct CModeBriarRoseTeleportInvulnerable : CModeStandBase {
    unsigned char _pad68[0x0c];
    long          FrameCount;                   // +0x74
    unsigned char _pad78[0x08];
    CIntelligentPointerThing PEffect;           // +0x80
    void OnActivate();
};
void CModeBriarRoseTeleportInvulnerable::OnActivate()
{
    CModeStandBase::OnActivate();
    FrameCount = (long)(g_GameFramesPerSecond * 1.25f);
    if (ParentThing->m_type == 1)
        ParentThing->SetSuspendAnimation(true, this);
    {
    CTCBoss* boss;
    if (ParentThing->GetInterface(&boss, 0xad) && boss->IsReadyForPhaseChange())
        boss->IncrementBossPhase();
    }
    {
    CDefPointer<CSpecialEffectsDef> out;
    long effectType = 0;
    bool found = CDefCopy<CDefClassBase>(ParentThing->PDef)->GetSubDef(&out)
        && out.Object->GetEffectFromString("TELEPORT_CHARGE", &effectType);
    CDefHolder<CSpecialEffectsDef> effects(out.Object);
    if (found) {
        CThing* effect = CreateEffectThing(effectType, &GetParent()->GetPosition(), false);
        if (effect && !(effect->m_flags91 & 1))
            PEffect = CIntelligentPointerThing(effect);
    }
    }
}
}

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
static char g_log[512];
static void L(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static const char* g_lastText; static int g_deletes;
CCharString::CCharString(const char* text, long) { g_lastText = text; m_pData = 0; }
CCharString::~CCharString() {}
void CDefPointeeBase::Destroy() {}
void CDefPointeeBase::Delete() { g_deletes++; }
static long g_lookup; static const char* g_lookupName;
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void*, const CCharString&) { g_lookupName = g_lastText; L("lookup"); return g_lookup; }
static void* g_def; static long g_classRefAtFind;
static CDefClassBase g_classDef;
void* FindDef_CDefClassBase_GetSubDef_0() { g_classRefAtFind = g_classDef.m_refCount; L("subdef"); return g_def; }
static int g_suspend = -1; static void* g_suspendMode;
void CThing::SetSuspendAnimation(bool s, CModeBase* mode) { g_suspend = s; g_suspendMode = mode; L("suspend"); }
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
const C3DVector& CThing::GetPosition() const { return g_pos; }
static int g_ready;
bool CTCBoss::IsReadyForPhaseChange() { L("ready?"); return g_ready != 0; }
void CTCBoss::IncrementBossPhase() { L("phase"); }
static CThing* g_created; static long g_createdType; static const C3DVector* g_createdPos; static int g_createdFlag;
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag) { g_createdType = type; g_createdPos = position; g_createdFlag = flag; L("create"); return g_created; }
void CBaseIntelligentPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() { PItem = 0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return PItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { PItem = item; }
void CIntelligentPointerThing::v00() {}
long g_GameFramesPerSecond = 30;
void CModeBase::v00() {}
void CModeStandBase::OnActivate() { L("base"); }
static unsigned char g_parentBuf[0x100], g_effectBuf[0x100];
static void Reset() { g_log[0] = 0; g_deletes = 0; g_suspend = -1; }
using namespace NCreatureMode;
int main()
{
    static CModeBriarRoseTeleportInvulnerable mode;
    CThing* parent = (CThing*)g_parentBuf; CThing* effect = (CThing*)g_effectBuf;
    mode.ParentThing = parent; parent->PDef = &g_classDef; g_classDef.m_refCount = 1;
    static CTCBoss boss; static CSpecialEffectsDef effects;
    CInterfaceMap::SEntry e[] = { { 0x10, 0 }, { 0xad, (CTCBase*)&boss } };
    parent->m_interfaces.first = e; parent->m_interfaces.last = e + 2;
    // player creature, boss ready, effect found and alive
    parent->m_type = 1; parent->m_presentBits[0xad >> 5] = 1UL << (0xad & 31); g_ready = 1;
    g_def = &effects; effects.m_refCount = 2; g_lookup = 12; g_created = effect; Reset();
    mode.OnActivate();
    CHECK(strcmp(g_log, "base;suspend;ready?;phase;subdef;lookup;create;") == 0);
    CHECK(mode.FrameCount == 37 && g_suspend == 1 && g_suspendMode == &mode && strcmp(g_lookupName, "TELEPORT_CHARGE") == 0);
    CHECK(g_classRefAtFind == 2 && g_classDef.m_refCount == 1 && g_createdType == 12 && g_createdPos == &g_pos && g_createdFlag == 0);
    CHECK(mode.PEffect.PItem == effect && effects.m_refCount == 1 && g_deletes == 0);
    // not a player, boss not ready, effect dead: nothing kept
    parent->m_type = 2; g_ready = 0; g_effectBuf[0x91] = 1; mode.PEffect.PItem = 0; effects.m_refCount = 2; Reset();
    mode.OnActivate();
    CHECK(strcmp(g_log, "base;ready?;subdef;lookup;create;") == 0 && mode.PEffect.PItem == 0);
    // no boss interface, lookup fails: no effect; last effects reference released
    parent->m_presentBits[0xad >> 5] = 0; g_lookup = -1; effects.m_refCount = 1; Reset();
    mode.OnActivate();
    CHECK(strcmp(g_log, "base;subdef;lookup;") == 0 && g_deletes == 1);
    // no sub-def: no lookup, no release
    g_def = 0; Reset();
    mode.OnActivate();
    CHECK(strcmp(g_log, "base;subdef;") == 0 && g_deletes == 0 && g_classDef.m_refCount == 1);
    // effect not created
    g_def = &effects; effects.m_refCount = 2; g_lookup = 3; g_created = 0; Reset();
    mode.OnActivate();
    CHECK(strcmp(g_log, "base;subdef;lookup;create;") == 0 && mode.PEffect.PItem == 0);
    if (fails == 0) printf("008e7a00_TEST PASS\n"); else printf("008e7a00_TEST FAIL\n");
    return fails;
}