#include <stdio.h>
#include <string.h>
// CTCMultiArrow::CreateAmmoEffect() -- count one ammo effect down; look "AMMO_EFFECT" up in the ability def's special-effects def, create that particle effect at the owner, attach a live effect's emitter to the owner and keep the effect in AmmoEffects
// Leaf: CreateAmmoEffect
// Shape: CalcAndLookup (0x00669D30) gets a same-TU out-value-only stand-in body (the 0x008D9740 idiom), so the effect
//   index does not escape and VC7.1 packs the intelligent pointer over the def/string words as retail (3-word frame).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CC2B0, 352 bytes (+ int3 padding), `ret`. Speed TU (esp frame).
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580, the special-effects def's
//   CalcAndLookup(const CCharString&, long*) @ 0x00669D30 (label "CCRC::CalcAndLookup" is propagated; as the landed
//   0x008C4170), CCharString(const char*, long = -1) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CThing::GetPosition
//   @ 0x004C73D0, CreateParticleEffect(long, const C3DVector&) @ 0x0072AB70 (`__fastcall`), CBaseIntelligentPointer
//   ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for CTCDParticleEmitter, type
//   0x4B = bit 11 of CThing+0x28), CTCDParticleEmitter::AttachToThing @ 0x006E0BE0,
//   vector<CIntelligentPointer<CThing> >::push_back @ 0x004AD520; the def's virtual slot 1 at the last release.
// Data: "AMMO_EFFECT" @ 0x0126EDEC, "" @ 0x0122D70E; CIntelligentPointer<CThing> vtable @ 0x01238C6C.
// Shape: the GetInterface result is not tested and the emitter is not pre-set, so the absent path reads the
//   uninitialised out pointer from the frame word shared with the effect index and the lookup key (as the landed
//   FinalCleanup 0x007CC410). The lookup's string is an implicit conversion temporary (the bool result survives its
//   destructor in bl).
// Layout: Ego_r.pdb CTCMultiArrow with the retail 12-byte vector (as the landed siblings 0x007CC230 / 0x007CC060):
//   PDef +0x0C, CreateAmmoEffectCount +0x24, AmmoEffects +0x30.
struct C3DVector { float X, Y, Z; };
class CThing;
class CBaseObject;
class CTCBase;

class CCharString {
public:
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    void* m_pStringData;
};

struct CDefPointeeBase {
    virtual void Destroy();                            // slot 0
    virtual void Delete();                             // slot 1 (+0x04)
    long m_refCount;                                   // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
struct CSpecialEffectsDef : CDefPointeeBase {
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_CalcAndLookup(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail): it writes the out value only, so VC7.1 knows the effect index's address does
// not escape the call (the 0x008D9740 GetEffectFromString idiom).
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
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);       // 0x00669580
};
struct CSpecialAbilitiesMultiArrowDef : CDefClassBase { };
void* FindDef_CDefClassBase_GetSubDef_0();             // stand-in callee of the stand-in body below
// Same-TU stand-in for the out-of-line GetSubDef<CSpecialEffectsDef> (the landed 0x008C4170 idiom): it writes the out
// slot only inline, so VC7.1 knows the def pointer's address does not escape and keeps the def in ebp.
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CInterfaceMap {                                 // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
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

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                    // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                           // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;              // 0x004C73D0
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
};
CThing* __fastcall CreateParticleEffect(long type, const C3DVector& position);   // 0x0072AB70

class CTCDParticleEmitter {
public:
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
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
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
#pragma pack(pop)
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

template <class T> struct CRetailVector {              // STLport vector<T> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void push_back(const T& x);                        // 0x004AD520
};

class CTCMultiArrow {
public:
    void*         __vftable;                           // +0x00
    CThing*       ParentThing;                         // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CSpecialAbilitiesMultiArrowDef* PDef;              // +0x0c CDefPointer<const CSpecialAbilitiesMultiArrowDef>
    long          NumberOfShots;                       // +0x10
    long          NumberInVolley;                      // +0x14
    long          NumOfVolleys;                        // +0x18
    long          SpellLevel;                          // +0x1c
    bool          RemovedEffect;                       // +0x20
    long          CreateAmmoEffectCount;               // +0x24
    long          CreateAmmoEffectDelay;               // +0x28
    long          CreateAmmoEffectTimer;               // +0x2c
    CRetailVector<CIntelligentPointer<CThing> > AmmoEffects;   // +0x30
    void CreateAmmoEffect();
};

void CTCMultiArrow::CreateAmmoEffect()
{
    --CreateAmmoEffectCount;
    CDefPointer<CSpecialEffectsDef> effects;
    long effectIndex = 0;
    if (PDef->GetSubDef(&effects)) {
        if (effects->CalcAndLookup("AMMO_EFFECT", &effectIndex)) {
            CIntelligentPointer<CThing> effect(CreateParticleEffect(effectIndex, ParentThing->GetPosition()));
            if (IS_THING_ALIVE(effect)) {
                CTCDParticleEmitter* emitter;
                effect.GetPItem()->GetInterface(&emitter, 0x4b);
                if (emitter)
                    emitter->AttachToThing(ParentThing, 8, "", 0, 0.0f);
            }
            AmmoEffects.push_back(effect);
        }
    }
}

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
static const char* g_lastText; static int g_strings, g_strDtors, g_deletes;
CCharString::CCharString(const char* text, long) { g_lastText = text; g_strings++; m_pStringData = 0; }
CCharString::~CCharString() { g_strDtors++; }
void CDefPointeeBase::Destroy() {}
void CDefPointeeBase::Delete() { g_deletes++; }
static long g_lookup; static const char* g_lookupName; static void* g_lookupDef;
long Lookup_CSpecialEffectsDef_CalcAndLookup(const void* def, const CCharString& name) { g_lookupDef = (void*)def; g_lookupName = g_lastText; return g_lookup; }
static void* g_def;
void* FindDef_CDefClassBase_GetSubDef_0() { return g_def; }
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
const C3DVector& CThing::GetPosition() const { return g_pos; }
static CThing* g_created; static long g_createdType; static const C3DVector* g_createdPos; static int g_creates;
CThing* __fastcall CreateParticleEffect(long type, const C3DVector& position) { g_createdType = type; g_createdPos = &position; g_creates++; return g_created; }
static void* g_attachThis; static CThing* g_attachThing; static long g_attachType, g_attachIndex; static float g_attachOff; static const char* g_attachName; static int g_attaches;
void CTCDParticleEmitter::AttachToThing(CThing* thing, long attachType, const CCharString&, long helperIndex, float offset)
{ g_attachThis = this; g_attachThing = thing; g_attachType = attachType; g_attachIndex = helperIndex; g_attachOff = offset; g_attachName = g_lastText; g_attaches++; }
void CBaseIntelligentPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() { m_pItem = 0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; }
template <class T> void CIntelligentPointer<T>::v00() {}
static CThing* g_pushed; static int g_pushes;
template <class T> void CRetailVector<T>::push_back(const T& x) { g_pushed = x.GetPItem(); g_pushes++; }
static void Reset() { g_strings = g_strDtors = g_deletes = g_creates = g_attaches = g_pushes = 0; g_pushed = 0; g_attachThis = 0; g_lookupName = 0; }
static unsigned char g_parentBuf[0x100], g_effectBuf[0x100];
int main()
{
    CTCMultiArrow arrow; memset(&arrow, 0, sizeof(arrow));
    CThing* parent = (CThing*)g_parentBuf; CThing* effect = (CThing*)g_effectBuf;
    static CSpecialAbilitiesMultiArrowDef classDef; arrow.PDef = &classDef; arrow.ParentThing = parent;
    static CSpecialEffectsDef effectsDef;
    static CTCDParticleEmitter emitter;
    CInterfaceMap::SEntry e[] = { { 0x10, 0 }, { 0x4b, (CTCBase*)&emitter }, { 0x60, 0 } };
    effect->m_interfaces.first = e; effect->m_interfaces.last = e + 3;
    effect->m_presentBits[0x4b >> 5] |= 1UL << (0x4b & 31);
    // no sub-def: count down only
    arrow.CreateAmmoEffectCount = 3; g_def = 0; Reset();
    arrow.CreateAmmoEffect();
    CHECK(arrow.CreateAmmoEffectCount == 2 && g_strings == 0 && g_creates == 0 && g_pushes == 0 && g_deletes == 0);
    // sub-def, lookup fails: string built and destroyed, def released (last reference -> Delete)
    g_def = &effectsDef; effectsDef.m_refCount = 1; g_lookup = -1; Reset();
    arrow.CreateAmmoEffect();
    CHECK(arrow.CreateAmmoEffectCount == 1 && g_strings == 1 && g_strDtors == 1 && strcmp(g_lookupName, "AMMO_EFFECT") == 0);
    CHECK(g_lookupDef == &effectsDef && g_creates == 0 && g_pushes == 0 && g_deletes == 1 && effectsDef.m_refCount == 0);
    // found, live effect with an emitter: attach to the parent and keep it
    effectsDef.m_refCount = 2; g_lookup = 7; g_created = effect; effect->_pad00[0] = 0; Reset();
    arrow.CreateAmmoEffect();
    CHECK(g_creates == 1 && g_createdType == 7 && g_createdPos == &g_pos);
    CHECK(g_attaches == 1 && g_attachThis == &emitter && g_attachThing == parent && g_attachType == 8 && g_attachIndex == 0 && g_attachOff == 0.0f && strcmp(g_attachName, "") == 0);
    CHECK(g_pushes == 1 && g_pushed == effect && g_deletes == 0 && effectsDef.m_refCount == 1 && g_strings == 2 && g_strDtors == 2);
    // dead effect: no attach, still kept
    effectsDef.m_refCount = 2; g_created = effect; g_effectBuf[0x91] = 1; Reset();
    arrow.CreateAmmoEffect();
    CHECK(g_attaches == 0 && g_pushes == 1 && g_pushed == effect);
    // no effect created: kept as null
    g_effectBuf[0x91] = 0; effectsDef.m_refCount = 2; g_created = 0; Reset();
    arrow.CreateAmmoEffect();
    CHECK(g_attaches == 0 && g_pushes == 1 && g_pushed == 0 && effectsDef.m_refCount == 1);
    if (fails == 0) printf("007cc2b0_TEST PASS\n"); else printf("007cc2b0_TEST FAIL\n");
    return fails;
}